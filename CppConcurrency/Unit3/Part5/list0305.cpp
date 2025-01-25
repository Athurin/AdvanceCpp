#include <exception>
#include <stack>
#include <mutex>
#include <memory>
#include <iostream>

struct empty_stack: std::exception
{
    const char* what() const throw()
    {
        return "empty stack";
    }
    
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data; //受保护的数据结构
    mutable std::mutex m;  //互斥锁
public:
    threadsafe_stack(){}
    threadsafe_stack(const threadsafe_stack& other) //拷贝构造
    {
        std::lock_guard<std::mutex> lock(other.m);
        data=other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack&) = delete; //禁用复制赋值

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m);
        data.push(new_value);
    }
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
        data.pop();
        return res;
       
    }
    void pop(T& value)
    {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value=data.top();
        data.pop();
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

int main()
{
    threadsafe_stack<int> si;
    si.push(5);
    si.push(7);
    si.pop();
    if(!si.empty())
    {
        int x;
        si.pop(x);
        std::cout << x << std::endl;
    }

    try
    {
        int y;
        si.pop(y);
        std::cout << y << std::endl;
    }
    catch (const empty_stack& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
