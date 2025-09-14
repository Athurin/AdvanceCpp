/*
* 普通new 的内存管理是怎样的呢
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
        std::cout << "Constructor called with value: " << value << std::endl; //55
    }
    ~MyClass()
    {
        std::cout << "Destructor called with value: " << value << std::endl; //55
    }
};

int main()
{
    MyClass* pobj = new MyClass(55);
    std::cout << "sizeof(MyClass): " << sizeof(MyClass) << std::endl; //4
    std::cout << "sizeof(pobj): " << sizeof(pobj) << std::endl; //8
    std::cout << "sizeof(*pobj): " << sizeof(*pobj) << std::endl; //4
    std::cout << "pobj指向的地址: " << pobj << std::endl; // 00000254E0C48BA0
    std::cout << "pobj->value: " << pobj->value << std::endl << std::endl; //55

    delete(pobj);

    std::cout << "delete后：" << std::endl;
    std::cout << "sizeof(pobj): " << sizeof(pobj) << std::endl; //8 sizeof只看静态类型 MyClass* ，所以不变
    std::cout << "sizeof(*pobj): " << sizeof(*pobj) << std::endl; //4 sizeof只看静态类型 MyClass，所以不变
    std::cout << "pobj指向的地址: " << pobj << std::endl; // 00000254E0C48BA0 pobj如果不置空，就依然指向原来的地址
    //std::cout << "pobj->value: " << pobj->value << std::endl; //输出无法预知，pobj已经变为野指针

    pobj = nullptr;
    std::cout << "pobj = nullptr 后：" << std::endl;
    std::cout << "pobj指向的地址: " << pobj << std::endl; // 0000000000000000

    //delete实际上做了两件事，
    //1. 调用析构函数
    //2. 归还堆区内存

    return 0;
}
*/

/*
Constructor called with value: 55
sizeof(MyClass): 4
sizeof(pobj): 8
sizeof(*pobj): 4
pobj指向的地址: 00000271751F8820
pobj->value: 55

Destructor called with value: 55
delete后：
sizeof(pobj): 8
sizeof(*pobj): 4
pobj指向的地址: 00000271751F8820
pobj = nullptr 后：
pobj指向的地址: 0000000000000000
*/