#include <mutex>
#include <condition_variable>
#include <memory>

namespace ringbuffer
{

template <typename T>
class RingBuffer
{
private:
    T *buffer;
    int buffer_size;
    int data_length, head, tail;
    std::mutex mtx_push, mtx_pop, mtx;
    std::condition_variable cv_full, cv_empty;
    int GetDataLength();

public:
    explicit RingBuffer(unsigned int size);
    ~RingBuffer();
    void Push(const T *push_data, const int push_size);
    std::shared_ptr<T[]> Pop(const int pop_size);
};

template <typename T>
RingBuffer<T>::RingBuffer(unsigned int size)
{
    buffer = new T[size];
    head = tail = data_length = 0;
    buffer_size = size;
}

template <typename T>
RingBuffer<T>::~RingBuffer()
{
    delete[] buffer;
}

template <typename T>
int RingBuffer<T>::GetDataLength()
{
    if (tail >= head)
        return tail - head;
    else
        return tail - head + buffer_size;
}

template <typename T>
void RingBuffer<T>::Push(const T *push_data, const int push_size)
{
Logger::Info("PUSH CALL push_size:%d", push_size);
    std::unique_lock<std::mutex> lk_push(mtx_push); //まず Producer の共通ミューテックス mtx_push をロック
    std::unique_lock<std::mutex> lk_full(mtx); ////つづいて同時に読み出しされないように mtx をロック
    
Logger::Info("PUSH START push_size:%d", push_size);
    int tmp_size = buffer_size - data_length - 1; //空き容量の計算
    for (int i = 0; i < push_size; i++) //push_size 分 push 開始
    {
Logger::Info("PUSH LOOP push_size:%d counter:%d dataLength:%d", push_size, i, data_length);
        if (tmp_size-- == 0) //満杯かどうかの判定
        {
            data_length = GetDataLength(); //データ長の更新
            cv_empty.notify_all(); //満杯である(空ではない)ことを待機中の Consumer スレッドに通知
Logger::Info("PUSH WAIT push_size:%d counter:%d dataLength:%d", push_size, i, data_length);           
            cv_full.wait(lk_full, [&] { return 1 < (buffer_size - data_length); }); //空きができるまで待機
Logger::Info("PUSH RE-START push_size:%d counter:%d dataLength:%d", push_size, i, data_length); 
            tmp_size = buffer_size - data_length - 2;//空き容量の計算(直後に1つ書き込むので先に -1 している点に注意)
        }
        buffer[tail] = *(push_data++);
        tail = (tail + 1) % buffer_size;
    }
    data_length = GetDataLength();
    cv_empty.notify_all(); //書き込み終了を待機中の Consumer スレッドに通知
}

template <typename T>
std::shared_ptr<T[]> RingBuffer<T>::Pop(const int pop_size)
{
Logger::Info("POP CALL pop_size:%d", pop_size);
    //std::shared_ptr<T> pop_data(new T[pop_size], std::default_delete<T[]>());
    std::shared_ptr<T[]> pop_data(new T[pop_size]); //読み出したデータを格納する配列
    std::unique_lock<std::mutex> lk_pop(mtx_pop); //まず Consumer の共通ミューテックス mtx_pop をロック
    std::unique_lock<std::mutex> lk_empty(mtx); //つづいて同時に書き込みされないように mtx をロック

Logger::Info("POP START pop_size:%d", pop_size);
    int tmp_size = data_length; //現在のデータ長をコピー
    for (int i = 0; i < pop_size; i++) //pop_size 分 pop 開始
    {
Logger::Info("POP LOOP pop_size:%d counter:%d dataLength:%d", pop_size, i, data_length);
        if (tmp_size-- == 0) //空かどうかの判定
        {
            data_length = GetDataLength(); //データ長の更新
            cv_full.notify_all(); //空である(満杯ではない)ことを待機中の Consumer スレッドに通知
Logger::Info("POP WAIT pop_size:%d counter:%d dataLength:%d", pop_size, i, data_length);
            cv_empty.wait(lk_empty, [&] { return data_length > 0; }); //データが追加されるまで待機
Logger::Info("POP RESTRT pop_size:%d counter:%d dataLength:%d", pop_size, i, data_length);
            tmp_size = data_length - 1; //データ長をコピー(直後に1つ読み出すので先に -1 している点に注意)
        }
        //pop_data.get()[i] = buffer[head];
        pop_data[i] = buffer[head];
        head = (head + 1) % buffer_size;
    }
    data_length = GetDataLength();
    cv_full.notify_all(); //読み出し終了を Producer スレッドに通知
    return pop_data;
}
} // namespace ringbuffer
