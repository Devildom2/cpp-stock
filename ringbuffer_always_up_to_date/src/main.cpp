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

    RingBuffer<int> ringbuffer(3);
        
    //int array_int1[10] = {1,2,3,4,5,6,7,9,10};
    int array_int2[4] = {11,12,13,14};
        
    //ringbuffer.Push(array_int1, _countof(array_int1));
    ringbuffer.Push(array_int2, _countof(array_int2));

     // サブスレッド
    std::thread th([&]() {

        int pop_size = 6;
        std::shared_ptr<int[]> array1 = ringbuffer.Pop(pop_size);
        for( int i = 0 ; i < pop_size ; i++ ) {
            Logger::Info("ringbuffer pop data:%d", array1[i]);
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒もあれば寝るはず
    int array_int3[4] = {15,16,17,18};
    ringbuffer.Push(array_int3, _countof(array_int3));

    th.join();
}
