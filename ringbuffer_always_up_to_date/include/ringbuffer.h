#include <memory>
#include <mutex>
#include <condition_variable>

namespace ringbuffer {

template <typename T>
class RingBuffer {
private:
    T *buffer;
    unsigned int buffer_size;
    unsigned int data_length, head, tail;
    std::mutex mtx_push, mtx_pop, mtx;
    std::condition_variable cv_push, cv_pop;

public:
    explicit RingBuffer(unsigned int size);
    ~RingBuffer();
    void Push(const T *push_data, const int push_size);
    std::shared_ptr<T[]> Pop(const int pop_size); 
};

template <typename T>
RingBuffer<T>::RingBuffer(unsigned int size) {
    buffer = new T[size];
    head = tail = data_length = 0;
    buffer_size = size;
}

template <typename T>
RingBuffer<T>::~RingBuffer() {
    delete [] buffer;
}

template <typename T>
void RingBuffer<T>::Push(const T *push_data, const int push_size) {

    std::unique_lock<std::mutex> lk_push(mtx_push);
    
    for(int i = 0; i < push_size; i++) {

        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒もあれば寝るはず
        std::unique_lock<std::mutex> lk(mtx);

        buffer[tail] = *(push_data++);
        tail = (tail + 1) % buffer_size;

        if( buffer_size <= data_length ) {
            head = (head + 1) % buffer_size;
        }else{
            data_length++;
        }
        cv_pop.notify_all();
Logger::Info("Push - head:%d tail:%d data_length:%d push_data:%d", head, tail, data_length, *(push_data - 1));
    }
}

template <typename T>
std::shared_ptr<T[]> RingBuffer<T>::Pop(const int pop_size) {

    std::unique_lock<std::mutex> lk_pop(mtx_pop);
    std::unique_lock<std::mutex> lk_empty(mtx);

    std::shared_ptr<T[]> pop_data(new T[pop_size]);

    for (int i = 0; i < pop_size; i++) {
        
        if( 0 >= data_length ){

Logger::Info("Pop wait");  

            cv_pop.wait(lk_empty, [&] { return data_length > 0; });

Logger::Info("Pop wake");  

        }
        pop_data[i] = buffer[head];
        head = (head + 1) % buffer_size;
        data_length--;

Logger::Info("Pop - head:%d tail:%d data_length:%d pop_data:%d", head, tail, data_length, pop_data[i]);
    }    
    return pop_data;
}
} // namespase ringbuffer 