#ifndef NIRVANA_ORB_STLUTILS_H_
#define NIRVANA_ORB_STLUTILS_H_

#include "MemoryHelper.h"

#if __cplusplus >= 201103L
#define ORB_STL_NORETURN [[noreturn]]
#else
#define ORB_STL_NORETURN
#endif

#if __cplusplus >= 201703L
#define ORB_STL_NODISCARD [[nodiscard]]
#else
#define ORB_STL_NODISCARD
#endif

namespace std {
template <typename C> class allocator;
struct random_access_iterator_tag;
template <class I> class reverse_iterator;
}

namespace Nirvana {

class StdExceptions
{
public:
	ORB_STL_NORETURN static void xout_of_range (const char* msg);
	ORB_STL_NORETURN static void xlength_error (const char* msg);
};

class StdContainer :
	public StdExceptions
{};

template <class Cont>
class StdConstIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename Cont::value_type;
	using difference_type = ptrdiff_t;
	using pointer = const typename Cont::value_type*;
	using reference = const typename Cont::value_type&;

	StdConstIterator () :
		ptr_ (nullptr)
	{}

	StdConstIterator (pointer p, const Cont& c) :
		ptr_ (p)
	{}

	ORB_STL_NODISCARD reference operator * () const
	{
		_check_deref ();
		return *ptr_;
	}

	ORB_STL_NODISCARD pointer operator -> () const
	{
		return (std::pointer_traits <pointer>::pointer_to (**this));
	}

	StdConstIterator <Cont>& operator ++ ()
	{
		_check_offset (1);
		++ptr_;
		return (*this);
	}

	StdConstIterator <Cont> operator ++ (int)
	{
		_check_offset (1);
		StdConstIterator <Cont> tmp = *this;
		++ptr_;
		return tmp;
	}

	StdConstIterator <Cont>& operator -- ()
	{
		_check_offset (-1);
		--ptr_;
		return (*this);
	}

	StdConstIterator <Cont> operator -- (int)
	{
		_check_offset (-1);
		StdConstIterator <Cont> tmp = *this;
		--ptr_;
		return tmp;
	}

	StdConstIterator <Cont>& operator += (difference_type off)
	{
		_check_offset (off);
		ptr_ += off;
		return (*this);
	}

	ORB_STL_NODISCARD StdConstIterator <Cont> operator + (difference_type off) const
	{
		StdConstIterator <Cont> tmp = *this;
		return tmp += off;
	}

	StdConstIterator <Cont>& operator -= (difference_type off)
	{
		_check_offset (-off);
		ptr_ -= off;
		return (*this);
	}

	ORB_STL_NODISCARD StdConstIterator <Cont> operator - (difference_type off) const
	{
		StdConstIterator <Cont> tmp = *this;
		return tmp -= off;
	}

	ORB_STL_NODISCARD difference_type operator - (const StdConstIterator <Cont>& rhs) const
	{	// return difference of iterators
		_check_compat (rhs);
		return ptr_ - rhs.ptr_;
	}

	ORB_STL_NODISCARD reference operator [] (difference_type off) const
	{	// subscript
		return *(*this + off);
	}

	ORB_STL_NODISCARD bool operator == (const StdConstIterator <Cont>& rhs) const
	{	// test for iterator equality
		return ptr_ == rhs.ptr_;
	}

	ORB_STL_NODISCARD bool operator != (const StdConstIterator <Cont>& rhs) const
	{	// test for iterator inequality
		return !(*this == rhs);
	}

	ORB_STL_NODISCARD bool operator < (const StdConstIterator <Cont>& rhs) const
	{	// test if this < rhs
		_check_compat (rhs);
		return (ptr_ < rhs.ptr_);
	}

	ORB_STL_NODISCARD bool operator > (const StdConstIterator <Cont>& rhs) const
	{	// test if this > rhs
		return (rhs < *this);
	}

	ORB_STL_NODISCARD bool operator <= (const StdConstIterator <Cont>& rhs) const
	{	// test if this <= rhs
		return (!(rhs < *this));
	}

	ORB_STL_NODISCARD bool operator >= (const StdConstIterator <Cont>& rhs) const
	{	// test if this >= rhs
		return (!(*this < rhs));
	}

private:
	// TBD: Iterator debugging

	void _check_deref () const
	{
		assert (ptr_);
	}

	void _check_offset (difference_type off) const
	{
		assert (ptr_);
	}

	void _check_compat (const StdConstIterator <Cont>) const
	{
		assert (ptr_);
	}

protected:
	pointer ptr_;
};

template <class Cont>
class StdIterator : public StdConstIterator <Cont>
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename Cont::value_type;
	using difference_type = ptrdiff_t;
	using pointer = typename Cont::value_type*;
	using reference = typename Cont::value_type&;

	StdIterator ()
	{}

	StdIterator (pointer p, const Cont& c) :
		StdConstIterator <Cont> (p, c)
	{}

	ORB_STL_NODISCARD reference operator * () const
	{
		return const_cast <reference> (StdConstIterator <Cont>::operator * ());
	}

	ORB_STL_NODISCARD pointer operator -> () const
	{
		return const_cast <pointer> (StdConstIterator <Cont>::operator -> ());
	}

	ORB_STL_NODISCARD reference operator [] (difference_type off) const
	{	// subscript
		return const_cast <reference> (StdConstIterator <Cont>::operator [] (off));
	}
};

}

#endif
