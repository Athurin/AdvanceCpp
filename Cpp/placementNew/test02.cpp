/*
* 普通局部定义的对象 的内存管理是怎样的呢
*/

/*
#include <iostream>
#include <new>

class MyClass
{
public:
    int value;
    MyClass(int v) : value(v)
    {
        std::cout << "Constructor called with value: " << value << std::endl; //66
    }
    ~MyClass()
    {
        std::cout << "Destructor called with value: " << value << std::endl; //66
    }
};

int main()
{
    MyClass obj(66);
    std::cout << "sizeof(MyClass): " << sizeof(MyClass) << std::endl; //4
    std::cout << "sizeof(obj): " << sizeof(obj) << std::endl; //4
    std::cout << "obj地址: " << &obj << std::endl; // 000000EDCF3DFAB4
    std::cout << "obj.value: " << obj.value << std::endl << std::endl; //66

    obj.~MyClass(); //手动调用析构函数，观察生命周期和内存管理

    std::cout << "析构后：" << std::endl;
    std::cout << "sizeof(obj): " << sizeof(obj) << std::endl; //4
    std::cout << "obj地址: " << &obj << std::endl; // 000000EDCF3DFAB4
    std::cout << "obj.value: " << obj.value << std::endl << std::endl; //66


    return 0;
}
*/

/*
Constructor called with value: 66
sizeof(MyClass): 4
sizeof(obj): 4
obj地址: 000000EDCF3DFAB4
obj.value: 66

Destructor called with value: 66
析构后：
sizeof(obj): 4
obj地址: 000000EDCF3DFAB4
obj.value: 66
*/