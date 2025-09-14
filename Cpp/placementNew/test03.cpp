#include <cstdio>
#include <cstdlib>
#include <new>

// no inline, required by [replacement.functions]/3
void* operator new(std::size_t sz) //确保定义全局唯一，使用inline
{
    std::printf("1) new(size_t), size = %zu\n", sz);
    if (sz == 0)
        ++sz; // avoid std::malloc(0) which may return nullptr on success
    //避免malloc(0)分配成功时返回nullptr

    if (void* ptr = std::malloc(sz))
        return ptr;

    throw std::bad_alloc{}; // required by [new.delete.single]/3
    //无论内存分配成功与否，都不会返回nullptr。
    //创建一个 默认构造的临时对象（相当于 std::bad_alloc()）
}

// no inline, required by [replacement.functions]/3
void* operator new[](std::size_t sz)
{
    std::printf("2) new[](size_t), size = %zu\n", sz);
    if (sz == 0)
        ++sz; // avoid std::malloc(0) which may return nullptr on success

    if (void* ptr = std::malloc(sz))
        return ptr;

    throw std::bad_alloc{}; // required by [new.delete.single]/3
}

void operator delete(void* ptr) noexcept
{
    std::puts("3) delete(void*)");
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t size) noexcept
{
    std::printf("4) delete(void*, size_t), size = %zu\n", size);
    std::free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    std::puts("5) delete[](void* ptr)");
    std::free(ptr);
}

void operator delete[](void* ptr, std::size_t size) noexcept
{
    std::printf("6) delete[](void*, size_t), size = %zu\n", size);
    std::free(ptr);
}

int main()
{
    int* p1 = new int;
    delete p1;

    int* p2 = new int[10]; // guaranteed to call the replacement in C++11
    delete[] p2;
}

/*
1) new(size_t), size = 4
4) delete(void*, size_t), size = 4
2) new[](size_t), size = 40
5) delete[](void* ptr)
*/