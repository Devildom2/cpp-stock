 #include <memory>
  
 namespace ringbuffer
 {  
 template <typename T>
 class RingBuffer
 {
 private:
     T *buffer;
     int buffer_size, head, tail, data_length;
     int GetDataLength(); //データサイズを計算する関数
 public:
     RingBuffer(unsigned int size); //コンストラクタ(バッファの初期化)
     ~RingBuffer(); //デストラクタ
     void Push(const T *push_data, const int push_size); //書き込み関数
     std::shared_ptr<T[]> Pop(const int pop_size); //読み出し関数
 };
 
 template <typename T>
 RingBuffer<T>::RingBuffer(unsigned int size) //コンストラクタ(バッファの初期化)
 {
     buffer = new T[size];
     head = tail = data_length = 0;
     buffer_size = size;
 }
 
 template <typename T>
 RingBuffer<T>::~RingBuffer() //デストラクタ
 {
     delete buffer;
 }
 
 template <typename T>
 int RingBuffer<T>::GetDataLength() //データサイズを計算する関数
 {
     if (tail >= head)
         return tail - head;
     else
         return tail - head + buffer_size;
 }
 
 template <typename T>
 void RingBuffer<T>::Push(const T *push_data, const int push_size) //書き込み関数
 {
Logger::Info("push_size:%d", push_size);

     if (buffer_size - data_length > push_size) //書き込むデータサイズが空き容量より小さければ書き込む
     {
         for (int i = 0; i < push_size; i++)
         {
Logger::Info("head:%d tail:%d", head, tail );

//             *(buffer + ((tail + i) % buffer_size)) = *(push_data++); //データの書き込み
             buffer[tail] = *(push_data++);
             tail = (tail + 1) % buffer_size; //インデックスをずらす
         }
         data_length = GetDataLength(); //データサイズ更新
     }

/// デバッグ start
Logger::Info("head:%d tail:%d", head, tail );
for (int i = 0; i < buffer_size; i++)
{
    Logger::Info("i:%d data:%d", i, *(buffer + i) );
}
/// デバッグ end
 }
 
 template <typename T>
 std::shared_ptr<T[]> RingBuffer<T>::Pop(const int pop_size) //読み出し関数
 {
     //std::shared_ptr<T> pop_data(new T[pop_size], std::default_delete<T[]>()); //読み出したデータを格納する配列
     std::shared_ptr<T[]> pop_data(new T[pop_size]); //読み出したデータを格納する配列

     if (data_length >= pop_size) //読み出すデータサイズがバッファ内のデータサイズ以下なら読み出す
     { 
         for (int i = 0; i < pop_size; i++)
         {
Logger::Info("head:%d buffer[head]:%d", head, buffer[head] );
              //pop_data.get()[i] = buffer[head]; //データの読み出し
              pop_data[i] = buffer[head]; //データの読み出し
              head = (head + 1) % buffer_size; //インデックスをずらす
         }
         data_length = GetDataLength(); //データサイズ更新
     }

/// デバッグ start
Logger::Info("pop_size:%d head:%d tail:%d", pop_size, head, tail );
for (int i = 0; i < buffer_size; i++)
{
    Logger::Info("i:%d data:%d", i, *(buffer + i) );
}
/// デバッグ end

     return pop_data;
 }
} // namespace ringbuffer