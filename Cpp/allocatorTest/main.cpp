
#include "Allocator.h"

//Test
int main()
{
	AthAllocator<std::string> all;

    std::string* p = all.allocate(5); // 分配 5 个 string 的内存

    for (int i = 0; i < 5; ++i)
        all.construct(p + i, std::string("zzz")); // 构造string的实例


    for (int i = 0; i < 5; ++i)
        std::cout << p[i] << " "; // 输出: zzz zzz zzz zzz zzz


    for (int i = 0; i < 5; ++i)
        all.destroy(p + i); // 析构string对象

    all.deallocate(p, 5); // 释放内存


	return 0;
}