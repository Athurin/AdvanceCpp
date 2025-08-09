#include <iostream>
#include <memory>


int main()
{
    std::allocator<std::string> alloc;
    std::string* p = alloc.allocate(5); // 分配 5 个 int 的内存

    for (int i = 0; i < 5; ++i)
        std::construct_at(p + i, std::string("zzz")); // 构造int()的默认实例
    

    for (int i = 0; i < 5; ++i) 
        std::cout << p[i] << " "; // 输出: zzz zzz zzz zzz zzz
    

    for (int i = 0; i < 5; ++i) 
        std::destroy_at(p + i); // 析构string对象
    
    alloc.deallocate(p, 5); // 释放内存

	return 0;
}