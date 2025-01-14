#include <thread>
#include <iostream>

class thread_guard
{
    std::thread& t;

public:
    explicit thread_guard(std::thread& t_):
        t(t_)
    {} 

    ~thread_guard()
    {
        if(t.joinable()) {t.join();}
    }
    thread_guard(thread_guard const&) = delete; //禁用拷贝构造
    thread_guard& operator=(thread_guard const&) = delete; //禁用复制赋值
};

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
{
    std::cout<<"这是主线程\n";
}


void f()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);

    //把线程放到类里面，当出作用域时调用析构函数
    //析构函数里面又等待线程的完成，很好地解决了代码0202中线程周期的问题
    thread_guard g(t);
        
    do_something_in_current_thread();
}//4

int main()
{
    f();
}
