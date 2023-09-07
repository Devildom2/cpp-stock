namespace ringbuffer {

template <typename T>
class RingBuffer {
private:
    T *buffer;
    unsigned int buffer_size;
    unsigned int data_length, head, tail;

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

    for(int i = 0; i < push_size; i++) {

        buffer[tail] = *(push_data++);
        tail = (tail + 1) % buffer_size;

        if( buffer_size <= data_length ) {
            head = (head + 1) % buffer_size;
        }else{
            data_length++;
        }
    }
Logger::Info("Push - head:%d tail:%d data_length:%d", head, tail, data_length);
}

template <typename T>
std::shared_ptr<T[]> RingBuffer<T>::Pop(const int pop_size) {

    std::shared_ptr<T[]> pop_data(new T[pop_size]);

    if (data_length >= pop_size) {

        for (int i = 0; i < pop_size; i++) {
            pop_data[i] = buffer[head];
            head = (head + 1) % buffer_size;
            data_length--;
         }
Logger::Info("Pop - head:%d tail:%d data_length:%d", head, tail, data_length);
     }
     return pop_data;
}
} // namespase ringbuffer 