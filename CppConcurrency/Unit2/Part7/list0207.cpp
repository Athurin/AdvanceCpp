#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <iostream>

void do_work(unsigned id)
{
    std::cout<<id<<"\n";
}

void f()
{
    std::vector<std::thread> threads;
    for(unsigned i=0;i<20;++i)
    {
        threads.push_back(std::thread(do_work,i));
    }
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join)); //用于获取 std::thread 类的 join 成员函数的指针
}

int main()
{
    f();
}
