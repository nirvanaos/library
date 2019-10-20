#ifndef NIRVANA_ORB_STLUTILS_H_
#define NIRVANA_ORB_STLUTILS_H_

#include "MemoryHelper.h"
#include "RuntimeSupport_c.h"

namespace std {
template <typename C> class allocator;
template <typename A> struct allocator_traits;
struct random_access_iterator_tag;
template <class I> class reverse_iterator;
#if __cplusplus >= 201103L
template <class _Elem> class initializer_list;
#endif
}

#ifdef _DEBUG
#	if defined (_ITERATOR_DEBUG_LEVEL)
#		if (_ITERATOR_DEBUG_LEVEL > 1)
#			define NIRVANA_DEBUG_ITERATORS
#		endif
#	elif defined (_GLIBCXX_DEBUG)
#		if (_GLIBCXX_DEBUG != 0)
#			define NIRVANA_DEBUG_ITERATORS
#		endif
#	elif defined (_LIBCPP_DEBUG)
#		if (_LIBCPP_DEBUG != 0)
#			define NIRVANA_DEBUG_ITERATORS
#		endif
# else
#			define NIRVANA_DEBUG_ITERATORS
#	endif
#endif

namespace Nirvana {

class StdExceptions
{
public:
	NIRVANA_NORETURN static void xout_of_range (const char* msg);
	NIRVANA_NORETURN static void xlength_error (const char* msg);
};

class StdContainer :
	public StdExceptions
{
protected:
	~StdContainer ()
	{
#ifdef NIRVANA_DEBUG_ITERATORS
		runtime_support ()->object_set_remove (this);
#endif
	}
};

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
#ifdef _DEBUG
		container_ (nullptr),
#endif
		ptr_ (nullptr)
	{}

	StdConstIterator (pointer p, const Cont& c) :
#ifdef _DEBUG
		container_ (&c),
#endif
		ptr_ (p)
	{
#ifdef _DEBUG
#ifdef NIRVANA_DEBUG_ITERATORS
		runtime_support ()->object_set_add (&static_cast <const StdContainer&> (c));
#endif
#endif
	}

	NIRVANA_NODISCARD reference operator * () const
	{
		_check_deref ();
		return *ptr_;
	}

	NIRVANA_NODISCARD pointer operator -> () const
	{
		_check_deref ();
		return ptr_;
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

	NIRVANA_NODISCARD StdConstIterator <Cont> operator + (difference_type off) const
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

	NIRVANA_NODISCARD StdConstIterator <Cont> operator - (difference_type off) const
	{
		StdConstIterator <Cont> tmp = *this;
		return tmp -= off;
	}

	NIRVANA_NODISCARD difference_type operator - (const StdConstIterator <Cont>& rhs) const
	{	// return difference of iterators
		_check_compat (rhs);
		return ptr_ - rhs.ptr_;
	}

	NIRVANA_NODISCARD reference operator [] (difference_type off) const
	{	// subscript
		return *(*this + off);
	}

	NIRVANA_NODISCARD bool operator == (const StdConstIterator <Cont>& rhs) const
	{	// test for iterator equality
		return ptr_ == rhs.ptr_;
	}

	NIRVANA_NODISCARD bool operator != (const StdConstIterator <Cont>& rhs) const
	{	// test for iterator inequality
		return !(*this == rhs);
	}

	NIRVANA_NODISCARD bool operator < (const StdConstIterator <Cont>& rhs) const
	{	// test if this < rhs
		_check_compat (rhs);
		return (ptr_ < rhs.ptr_);
	}

	NIRVANA_NODISCARD bool operator > (const StdConstIterator <Cont>& rhs) const
	{	// test if this > rhs
		return (rhs < *this);
	}

	NIRVANA_NODISCARD bool operator <= (const StdConstIterator <Cont>& rhs) const
	{	// test if this <= rhs
		return (!(rhs < *this));
	}

	NIRVANA_NODISCARD bool operator >= (const StdConstIterator <Cont>& rhs) const
	{	// test if this >= rhs
		return (!(*this < rhs));
	}

private:
	// Iterator debugging

#ifdef _DEBUG
	const Cont* container () const
	{
		assert (container_);
#ifdef NIRVANA_DEBUG_ITERATORS
		assert (runtime_support ()->object_set_check (&static_cast <const StdContainer&> (*container_)));
#endif
		return container_;
	}

	struct Margins
	{
		Margins (const Cont* cont)
		{
			assert (cont);
			begin = cont->data ();
			end = begin + cont->size ();
		}

		Margins (const StdConstIterator& it) :
			Margins (it.container ())
		{}

		pointer begin, end;
	};
#endif

	void _check_deref () const
	{
#ifdef _DEBUG
		Margins m (*this);
		assert (m.begin <= ptr_ && ptr_ < m.end);
#endif
	}

	void _check_offset (difference_type off) const
	{
#ifdef _DEBUG
		Margins m (*this);
		pointer p = ptr_;
		assert (m.begin <= p && p <= m.end);
		p += off;
		assert (m.begin <= p && p <= m.end);
#endif
	}

	void _check_compat (const StdConstIterator <Cont>& rhs) const
	{
#ifdef _DEBUG
		const Cont* cont = container ();
		assert (cont == rhs.container_);
		Margins m (cont);
		assert (m.begin <= ptr_ && ptr_ <= m.end);
		assert (m.begin <= rhs.ptr_ && ptr_ <= rhs.m.end);
#endif
	}

private:
	pointer ptr_;

	friend Cont;

#ifdef _DEBUG
	const Cont* container_;
#endif
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

	NIRVANA_NODISCARD reference operator * () const
	{
		return const_cast <reference> (StdConstIterator <Cont>::operator * ());
	}

	NIRVANA_NODISCARD pointer operator -> () const
	{
		return const_cast <pointer> (StdConstIterator <Cont>::operator -> ());
	}

	NIRVANA_NODISCARD reference operator [] (difference_type off) const
	{	// subscript
		return const_cast <reference> (StdConstIterator <Cont>::operator [] (off));
	}
};

}

#endif
