[C++中placement new操作符（经典）_placement new 例子-CSDN博客](https://blog.csdn.net/zhangxinrun/article/details/5940019)

new和delete操作符我们应该都用过，它们是对堆中的内存进行申请和释放，而这两个都是不能被重载的。

要实现不同的内存分配行为，需要重载`operator new` 和 `operator delete`，而不是new和delete。

new实际上是执行如下3个过程：
	1调用operator new分配内存；
	2调用构造函数生成类对象；
	3返回相应指针。

operator new就像operator+一样，是可以重载的，
但是不能在全局对原型为void operator new(size_t size)这个原型进行重载，
一般只能在类中进行重载。

如果类中没有重载`operator new`，那么调用的就是全局的`::operator new`来完成堆的分配。
同理，`operator new[]、operator delete、operator delete[]`也是可以重载的，一般你重载了其中一个，那么最好把其余三个都重载一遍。

**`placement new`是`operator new`的一个重载版本**，只是我们很少用到它。
如果你想在==已经分配的内存==中创建一个对象，使用new是不行的。
也就是说`placement new`允许你在一个已经分配好的内存中（栈或堆中）构造一个新的对象。
	原型中`void*p`实际上就是指向一个==已经分配好==的内存缓冲区的的首地址。以下C++ 标准中定义的 `placement new` 的标准形式：
```cpp
inline void* __CRTDECL operator new(size_t _Size,
    _Writable_bytes_(_Size) void* _Where) noexcept
{
    (void)_Size;
    return _Where;
}
```

> [!NOTE]  AI解析
> ### 代码解析
> 
> ```cpp
> inline void* __CRTDECL operator new(size_t _Size, _Writable_bytes_(_Size) void* `_`Where) noexcept
> {
>     (void)_Size; // 忽略 _Size 参数
>     return _Where; // 直接返回传入的指针
> }
> ```
> 
> 1. **`inline`**:
>    - 表示这个函数可以被内联展开，以减少函数调用的开销。
> 
> 2. **`__CRTDECL`**:
>    - 这是一个宏，通常用于指定函数的调用约定。具体的定义取决于编译器和平台。在 Windows 平台上，它通常定义为 `__cdecl`，表示使用 C 的调用约定。
> 
> 3. **`operator new`**:
>    - 这是 `placement new` 的函数名。它是一个特殊的 `new` 操作符重载。
> 
> 4. **参数**:
>    - `size_t _Size`: 表示要分配的内存大小。对于 `placement new`，这个参数通常被忽略，因为内存已经由用户指定。
>    - `_Writable_bytes_(_Size) void* _Where`: 指向目标内存的指针。`_Writable_bytes_(_Size)` 是一个注释宏，用于标记 `_Where` 指向的内存区域是可写的，并且大小为 `_Size` 字节。
> 
> 5. **`noexcept`**:
>    - 表示这个函数不会抛出异常。`placement new` 不会分配内存，因此不会失败，也不会抛出异常。
> 
> 6. **函数体**:
>    - `(void)_Size;`: 这是一个空操作，用于忽略 `_Size` 参数。虽然 `_Size` 参数在 `placement new` 中通常被忽略，但它的存在是为了保持与普通 `new` 的一致性。将 `_Size` 的值转换为 `void` 类型，从而避免编译器对 `_Size` 的未使用警告。它并不涉及赋值或执行任何有意义的操作。
>    - `return _Where;`: 直接返回传入的指针 `_Where`。
> 
> ### 使用场景
> 
> `placement new` 用于在已分配的内存中构造对象。这种形式的 `new` 不会分配新的内存，而是直接在指定的内存位置构造对象。这在以下场景中非常有用：
> 
> 7. **对象池**:
>    - 在对象池中，预先分配一块内存，然后使用 `placement new` 在这块内存中构造对象，以避免频繁的内存分配和释放。
> 
> 8. **内存管理器**:
>    - 在自定义的内存管理器中，可以使用 `placement new` 在特定的内存块中构造对象。
> 
> 9. **性能优化**:
>    - 当需要快速构造和销毁对象时，使用 `placement new` 可以避免标准内存分配器的开销。
> 
> ### 注意事项
> 
> **手动调用析构函数**:
>    - 当使用 `placement new` 构造对象时，必须手动调用析构函数来销毁对象。因为 `placement new` 不会自动管理内存，也不会自动调用析构函数。
> 
> **内存对齐**:
>    - 确保目标内存地址 `_Where` 是正确对齐的。如果内存地址不对齐，可能会导致未定义行为。
> 
>**内存大小**:
>    - 确保目标内存足够大，能够容纳对象的大小。如果内存不足，可能会导致未定义行为。


我们知道使用new操作符分配内存需要在堆中**查找足够大的剩余空间**，这个操作速度是很**慢**的，而且有可能出现无法分配内存的异常（空间不够）。
placement new就可以解决这个问题。我们构造对象都是在一个**预先准备好了的**内存缓冲区中进行，不需要查找内存，**内存分配的时间是常数**；而且不会出现在程序运行中途出现内存不足的异常。
所以，placement new非常适合那些对时间要求比较高，长时间运行不希望被打断的应用程序。

使用方法如下：

1. 缓冲区提前分配

	可以使用堆的空间，也可以使用栈的空间，所以分配方式有如下两种：
	
```cpp
	class MyClass {…};  
	char *buf=new char[N*sizeof(MyClass)+ sizeof(int) ] ; 
	//或者char buf[N*sizeof(MyClass)+ sizeof(int) ];
```

2. 对象的构造

```cpp
MyClass * pClass=new(buf) MyClass;
```
3. 对象的销毁

	一旦这个对象使用完毕，你必须显式的调用类的析构函数进行销毁对象。但此时内存空间不会被释放，以便其他的对象的构造。

```cpp
pClass->~MyClass();
```
4. 内存的释放

	如果缓冲区在堆中，那么调用`delete[] buf`;进行内存的释放；如果在栈中，那么在其作用域内有效，跳出作用域，内存自动释放。


> [!NOTE] 
> 在C++标准中，对于`placement operator new []`有如下的说明：` placement operator new[] needs implementation-defined amount of additional storage to save a size of array.` 所以我们必须申请比原始对象大小多出`sizeof(int)`个字节来存放对象的个数，或者说数组的大小。
> 使用方法第二步中的`new`才是`placement new`，其实是没有申请内存的，只是调用了构造函数，返回一个指向已经分配好的内存的一个指针，所以对象销毁的时候不需要调用`delete`释放空间，但必须调用析构函数销毁对象。

==placement new怎么使用？是需要用户自己在重载吗?==

> [!NOTE] AI回答
> `placement new` 是 C++ 中一种特殊的 `new` 表达式，用于在指定的内存位置构造对象。它不需要用户自己重载，而是直接使用 C++ 标准提供的形式。`placement new` 的核心用途是在已经分配的内存中构造对象，而不是分配新的内存。
> 
> ### 标准形式
> `placement new` 的标准形式如下：
> ```cpp
> void* operator new(std::size_t size, void* ptr) noexcept;
> ```
> 这个版本的 `new` 不会分配内存，而是直接返回传入的指针 `ptr`，并在该位置构造对象。
> 
> ### 使用方法
> 以下是一个简单的例子，展示如何使用 `placement new`：
> 
> ```cpp
> `#include<iostrea>`
> `#include <new>`
>
> class MyClass {
> public:
>     int value;
>     MyClass(int v) : value(v) {
>         std::cout << "Constructor called with value: " << value << std::endl;
>     }
>     ~MyClass() {
>         std::cout << "Destructor called with value: " << value << std::endl;
>     }
> };
> 
> int main() {
>     // 分配一块足够大的内存
>     char buffer[sizeof(MyClass)];
> 
>     // 使用 placement new 在 buffer 上构造对象
>     MyClass* obj = new (buffer) MyClass(42);
> 
>     // 使用对象
>     std::cout << "Object value: " << obj->value << std::endl;
> 
>     // 手动调用析构函数
>     obj->~MyClass();
> 
>     return 0;
> }
> ```
> 运行结果：
> ```cpp
> Constructor called with value: 42
> Object value: 42
> Destructor called with value: 42
> ```
> 
> ### 关键点
> 1. **内存分配**：`placement new` 不会分配新的内存，而是使用用户提供的内存地址。在上面的例子中，`buffer` 是一块足够大的内存，用于存放 `MyClass` 对象。
> 2. **构造对象**：`new (buffer) MyClass(42)` 在 `buffer` 指定的内存位置构造了一个 `MyClass` 对象。
> 3. **析构对象**：由于 `placement new` 不会自动管理内存，用户需要手动调用析构函数来销毁对象。在上面的例子中，使用了 `obj->~MyClass()`。
> 
> ### 为什么不需要用户重载
> `placement new` 是 C++ 标准的一部分，编译器已经提供了默认的实现。用户通常不需要重载它，除非有特殊的内存管理需求。例如，如果需要在特定的内存分配器中构造对象，可以重载 `operator new`，但这与 `placement new` 的用途不同。
> 
> ### 总结
> `placement new` 是一种非常有用的工具，尤其是在需要精确控制对象内存布局的场景中（例如对象池、内存池等）。它不需要用户重载，而是直接使用标准提供的形式。
> 


