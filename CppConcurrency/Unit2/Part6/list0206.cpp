#include <thread>
#include <utility>
#include <iostream>
//#include <mutex>

//std::mutex io_mutex; // 用于同步输出的互斥锁

class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_):
        t(std::move(t_))
    {
        if(!t.joinable()) //线程不可汇入时
            throw std::logic_error("No thread");
        std::cout << "Thread created\n";
    }
    ~scoped_thread()
    {
        t.join();
        std::cout << "joinEND\n";
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};

void do_something(int& i)
{
    ++i;
    //std::lock_guard<std::mutex> lock(io_mutex); // 使用互斥锁保护输出
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
    //std::lock_guard<std::mutex> lock(io_mutex); // 使用互斥锁保护输出
    std::cout<<"do_something_in_current_thread\n";
}

void f()
{
    int some_local_state = 0;
    scoped_thread t{ std::thread(func(some_local_state)) };
    
    do_something_in_current_thread();
}

int main()
{
    f();
    std::cout.flush();
}
