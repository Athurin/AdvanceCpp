#include <iostream>
#include <new>

class MyClass 
{
public:
    int value;
    MyClass(int v) : value(v) 
    {
        std::cout << "Constructor called with value: " << value << std::endl;
    }
    ~MyClass() 
    {
        std::cout << "Destructor called with value: " << value << std::endl;
    }
};

int main() 
{
    // 分配一块足够大的内存
    char buffer[sizeof(MyClass)];

    // 使用 placement new 在 buffer 上构造对象
    MyClass* obj = new (buffer) MyClass(42);

    // 使用对象
    std::cout << "Object value: " << obj->value << std::endl;

    // 手动调用析构函数
    obj->~MyClass();

    return 0;
}