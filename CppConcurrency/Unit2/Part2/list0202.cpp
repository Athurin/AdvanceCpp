#include <thread>
#include <iostream>

void do_something(int& i)
{
    ++i;
    std::cout << i << " ";
}

struct func
{
    int& i;

    func(int& i_):i(i_){}

    void operator()()
    {
        for(unsigned j=0;j<100;++j)
        {
            do_something(i);
        }
    }
};

void do_something_in_current_thread()
{}

void f()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread t(my_func);
    try
    {
        do_something_in_current_thread();
    }
    catch(...) //捕获所有类型的异常
    {
        //t.join();
        if (t.joinable()) 
        {
            t.join(); // 确保线程被 join
        }
        throw; //抛出异常，后面的所有操作都不会执行
    }
    //t.join();
    if (t.joinable()) 
    {
        t.join();
    }
}

int main()
{
    f();
}
