#include <mutex>
#include <iostream>

class some_data
{
    int a = 99;
    std::string b = "hello";
public:
    void do_something()
    {
        std::cout << a << std::endl;
        std::cout << b << std::endl;
    }
};

class data_wrapper
{
private:
    some_data data;
    std::mutex m;
public:
    template<typename Function>
    void process_data(Function func)
    {
        std::lock_guard<std::mutex> l(m);
        func(data);  //1 传递保护数据给用户函数
    }
};

some_data* unprotected;

void malicious_function(some_data& protected_data)
{
    unprotected=&protected_data;
}

data_wrapper x;

void foo()
{
    x.process_data(malicious_function); //2 传递一个恶意函数
    std::cout << "unprotected 在无互斥保护的情况下访问保护数据" << std::endl;
    unprotected->do_something(); //在无权保护的情况下访问保护数据
}

int main()
{
    foo();
}
