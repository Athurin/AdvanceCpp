#include <thread>
#include <iostream>

void do_something(int& i)
{
    ++i;
}

struct func
{
    int& i;

    func(int& i_):i(i_){} //i 与 i_ 都是some_local_state 的引用

    void operator()()
    {
        for(unsigned j=0;j<100;++j)
        {
            do_something(i);  //oops可能提前结束，但是却仍在引用局部变量some_local_state
            std::cout << i <<'\n';
        }
    }
};


void oops()
{
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    //my_thread.detach();
    my_thread.join();
}

int main()
{
    oops();

    return 0;
}
