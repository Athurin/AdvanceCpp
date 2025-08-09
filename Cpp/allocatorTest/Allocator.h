#ifndef __ATHALLOCATOR_H__
#define __ATHALLOCATOR_H__

#include <cstddef>
#include <limits.h>
#include <stdexcept>
#include <string>
#include <iostream>

template<typename T>
class AthAllocator
{
public:
	AthAllocator();
	~AthAllocator();

	using value_type		=   T;
	using pointer			=	value_type*;
	using reference			=	value_type&;
	using const_pointer		=	const value_type*;
	using const_reference	=	const value_type&;
	using size_type			=	size_t;
	using difference_type	=	ptrdiff_t;

	// rebind...
	//template <typename _other> struct rebind { typedef CHxAllocator<_other> other; };


	pointer allocate(size_type n);
	void deallocate(pointer p, size_type n = 0) noexcept;
	size_type max_size() const noexcept;

	void construct(pointer p, const_reference val);
	void destroy(pointer p);

	// 比较运算符
	friend bool operator==(const AthAllocator&, const AthAllocator&) noexcept
	{
		return true;
	}
	friend bool operator!=(const AthAllocator& a, const AthAllocator& b) noexcept
	{
		//return !(a == b);错误
		return false;
	}

};

#endif // __ATHALLOCATOR_H__

#include "Allocator.tcc"
