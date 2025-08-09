
template<typename T>
AthAllocator<T>::AthAllocator()
{

}

template<typename T>
AthAllocator<T>::~AthAllocator()
{

}
 

template<typename T>
typename AthAllocator<T>::pointer
AthAllocator<T>::allocate(size_type n)
{
    return static_cast<pointer>(::operator new(n * sizeof(value_type)));
}


template<typename T>
void AthAllocator<T>::deallocate(pointer p, size_type n) noexcept
{
    ::operator delete(p);
}

template<typename T>
typename AthAllocator<T>::size_type 
AthAllocator<T>::max_size() const noexcept
{
    return SIZE_MAX / sizeof(value_type);
}


template<typename T>
void AthAllocator<T>::construct(AthAllocator<T>::pointer p, AthAllocator<T>::const_reference val)
{
    :: new ((void *)p) T(val); //把 p 强制转换为 void* 类型
}

template<typename T>
void AthAllocator<T>::destroy(AthAllocator<T>::pointer p)
{
    p->~T();
}