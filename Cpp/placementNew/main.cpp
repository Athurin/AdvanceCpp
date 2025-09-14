/*
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
    std::cout << "sizeof(MyClass): " << sizeof(MyClass) << std::endl; //4

    // 分配一块足够大的内存
    char buffer[sizeof(MyClass)];

    // 使用 placement new 在 buffer 上构造对象
    MyClass* obj = new (buffer) MyClass(42);

    // 使用对象
    std::cout << "Object value: " << obj->value << std::endl; //42

    // 手动调用析构函数
    obj->~MyClass();

    //内存可能没释放？
    std::cout << "调用析构函数后，sizeof(buffer):" << sizeof(buffer) << std::endl; //4
    std::cout << "调用析构函数后，obj指向的值:"     << obj->value     << std::endl; //42
    std::cout << "调用析构函数后，sizeof(obj):"    << sizeof(obj)    << std::endl; //8
    std::cout << "调用析构函数后，sizeof(*obj):"   << sizeof(*obj);                //4

    return 0;
}
*/