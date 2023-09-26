#include <iostream>
#include <functional>
#include "Logger.h"

class Test {
public:
	int print1(int a) {
		std::cout << "print1 " << a * 1 << std::endl;
        return a * 1;
	}

	int print2(int a) {
		std::cout << "print2 " << a * 2 << std::endl;
        return a * 2;
	}
	int print3(int a) {
		std::cout << "print3 " << a * 3 << std::endl;
        return a * 3;
	}
};

typedef int(Test::*MEMFUNC)(int a);

static const MEMFUNC mem[] = 
{
    &Test::print1,
    &Test::print2,
    &Test::print3
};

Test t;
std::function<float (int a)> mem2[] = 
{
    std::bind( &Test::print1, &t, std::placeholders::_1 ),
    std::bind( &Test::print2, &t, std::placeholders::_1 ),
    std::bind( &Test::print3, &t, std::placeholders::_1 ),
};

int main() {

    Logger::Info("C言語型の関数テーブル");
	
    int a1 = 0;
    a1 = (t.*mem[0])(2); Logger::Info("C言語型:%d", a1);
	a1 = (t.*mem[1])(2); Logger::Info("C言語型:%d", a1);
	a1 = (t.*mem[2])(2); Logger::Info("C言語型:%d", a1);

    Logger::Info("C+11型の関数テーブル");

    float a2 = 0;
    a2 = mem2[0](3); Logger::Info("C+11型:%f", a2);
	a2 = mem2[1](3); Logger::Info("C+11型:%f", a2);
	a2 = mem2[2](3); Logger::Info("C+11型:%f", a2);

    return 0;
}