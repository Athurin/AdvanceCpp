是的，**C 语言本身不支持“函数重载（overload）”**——这是 C++ 的语法特性，不是 C 的。  
根本原因：C 编译器在符号表里只记录**函数名**，不会把参数类型、个数编码进最终的符号名；而 C++ 会把参数信息编码进去（所谓 name mangling），所以链接器能区分 `int foo(char)` 和 `int foo(int)`。

在纯 C 里，如果你写：

```c
int add(int a, int b)     { return a + b; }
double add(double a, double b) { return a + b; }  /* 编译错误：重复定义 */
```

第二行会报 **“redefinition of ‘add’”**；链接阶段还会看到两个同名全局符号，直接冲突。  
因此：

1. 函数名必须唯一。  
2. 想支持“同一语义、不同参数”，只能靠**不同的函数名**：

```c
int    add_i(int a, int b);
double add_d(double a, double b);
```

3. 想支持“同一名字、不同数量”也只有用变参，但类型检查极弱：

```c
int sum_int(int count, ...);   /* <stdarg.h> 手工解析 */
```

4. 宏可以玩“伪重载”，但只是文本替换，毫无类型安全：

```c
#define ADD(a, b)  (_Generic((a)+ (b), \
                    int:    add_i, \
                    double: add_d)(a, b))
```

（`_Generic` 是 C11 关键字，编译期按类型挑表达式，不是真正的重载，只是语法糖。）

内核、第三方库常见的做法就是加**后缀**：

```c
void gpio_set(int pin);
void gpio_set_mask(uint32_t mask);
```

**结论**  
- 严格来说，**C 没有函数重载**；  
- 想区分不同参数，只能手工换名字或用宏/`_Generic` 做编译期分发；  
- 如果你确实需要重载、模板、多态，请用 C++ 或其他语言。

