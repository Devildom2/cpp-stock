#include <iostream>
#include <string>
#include <thread>
#include "Logger.h"
//#include "ringbuffer_single.h"
#include "ringbuffer.h"

using namespace ringbuffer;
using namespace std;

int main() {

    Logger::Info("ringbuffer TEST");

    RingBuffer<int> ringbuffer(8);

    // サブスレッド
    std::thread th([&]() {
        
        int array_int1[3] = {1 ,2, 3};
        int array_int2[4] = {4, 5, 6, 7};
        int array_int3[1] = {8};
        int array_int4[2] = {9, 10};
        int array_int5[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

        ringbuffer.Push(array_int1, _countof(array_int1));
        ringbuffer.Push(array_int2, _countof(array_int2));
        ringbuffer.Push(array_int3, _countof(array_int3));
        ringbuffer.Push(array_int4, _countof(array_int4));
        ringbuffer.Push(array_int5, _countof(array_int5));
    });

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒もあれば寝るはず
    int pop_size = 3;
    std::shared_ptr<int[]> array1 = ringbuffer.Pop(pop_size);
    for( int i = 0 ; i < pop_size ; i++ ) {
        Logger::Info("ringbuffer pop data:%d", array1[i]);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒もあれば寝るはず
    pop_size = 2;
    std::shared_ptr<int[]> array2 = ringbuffer.Pop(pop_size);
    for( int i = 0 ; i < pop_size ; i++ ) {
        Logger::Info("ringbuffer pop data:%d", array2[i]);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒もあれば寝るはず
    pop_size = 4;
    std::shared_ptr<int[]> array3 = ringbuffer.Pop(pop_size);
    for( int i = 0 ; i < pop_size ; i++ ) {
        Logger::Info("ringbuffer pop data:%d", array3[i]);
    }
    pop_size = 8;
    std::shared_ptr<int[]> array4 = ringbuffer.Pop(pop_size);
    for( int i = 0 ; i < pop_size ; i++ ) {
        Logger::Info("ringbuffer pop data:%d", array4[i]);
    }

    th.join();
}

