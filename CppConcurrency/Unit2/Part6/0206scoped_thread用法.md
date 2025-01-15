当某个对象转移了线程的所有权，就不能再对线程进行汇入和分离。
为了保证线程在程序退出前完成，定义了scoped_thread类。

scoped_thread类：
```cpp
class scoped_thread
{
    std::thread t;
public:
    explicit scoped_thread(std::thread t_):
        t(std::move(t_))
    {
        if(!t.joinable())  //检查是否可汇入
            throw std::logic_error("No thread");
    }
    ~scoped_thread()
    {
        t.join();
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};
```

下面的代码：
```cpp
scoped_thread t(std::thread(func(some_local_state)));
```
`func(some_local_state)`是可调用对象，构造了一个临时的`std::thread`对象。
临时的对象出了作用域就会被销毁，所以构造的时候要把资源转移过来。
