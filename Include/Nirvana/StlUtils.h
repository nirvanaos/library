#ifndef NIRVANA_ORB_STLUTILS_H_
#define NIRVANA_ORB_STLUTILS_H_

#include "MemoryHelper.h"
#include "RuntimeSupport_c.h"
#include <type_traits>

#if !defined (NIRVANA_DEBUG_ITERATORS) && defined (_DEBUG)
#	if defined (_ITERATOR_DEBUG_LEVEL)
#		if (_ITERATOR_DEBUG_LEVEL > 1)
#			define NIRVANA_DEBUG_ITERATORS 1
#		endif
#	elif defined (_GLIBCXX_DEBUG)
#		if (_GLIBCXX_DEBUG != 0)
#			define NIRVANA_DEBUG_ITERATORS 1
#		endif
#	elif defined (_LIBCPP_DEBUG)
#		if (_LIBCPP_DEBUG != 0)
#			define NIRVANA_DEBUG_ITERATORS 1
#		endif
# else
#			define NIRVANA_DEBUG_ITERATORS 1
#	endif
#endif

#if (__cplusplus <= 1)
#undef __cplusplus
#define __cplusplus 201103L // C++ 11 by default
#endif

namespace std {
template <typename C> class allocator;
template <typename A> struct allocator_traits;
struct random_access_iterator_tag;
template <class I> class reverse_iterator;
#if __cplusplus >= 201103L
template <class _Elem> class initializer_list;
template <class _Iter> struct iterator_traits;
template <class _Ty> struct iterator_traits<_Ty *>;
struct input_iterator_tag;
#endif
}

namespace Nirvana {

class StdExceptions
{
public:
	NIRVANA_NORETURN static void xout_of_range (const char* msg);
	NIRVANA_NORETURN static void xlength_error (const char* msg);
};

template <class Cont> class StdConstIterator;

class StdContainer :
	public StdExceptions
{
protected:
	StdContainer ()
	{}

	~StdContainer ();

private:
	template <class Cont> friend class StdConstIterator;

	RuntimeProxy_ptr get_proxy () const
	{
		return runtime_support ()->runtime_proxy_get (this);
	}
};

template <class Cont>
class StdConstIterator
{
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef typename Cont::value_type value_type;
	typedef ptrdiff_t difference_type;
	typedef const value_type* pointer;
	typedef const value_type& reference;

	StdConstIterator () :
		ptr_ (nullptr)
	{}

	StdConstIterator (pointer p, const Cont& c) :
#ifdef _DEBUG
		proxy_ (c.get_proxy ()),
#endif
		ptr_ (p)
	{}

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

	NIRVANA_NODISCARD reference operator [] (difference_type off) const
	{	// subscript
		return *(*this + off);
	}

	StdConstIterator <Cont>& operator ++ ()
	{
		_check_offset (1);
		++ptr_;
		return *this;
	}

	StdConstIterator <Cont> operator ++ (int)
	{
		StdConstIterator <Cont> tmp = *this;
		operator ++ ();
		return tmp;
	}

	StdConstIterator <Cont>& operator -- ()
	{
		_check_offset (-1);
		--ptr_;
		return *this;
	}

	StdConstIterator <Cont> operator -- (int)
	{
		StdConstIterator <Cont> tmp = *this;
		operator -- ();
		return tmp;
	}

	StdConstIterator <Cont>& operator += (difference_type off)
	{
		_check_offset (off);
		ptr_ += off;
		return *this;
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
		return *this;
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

#if defined (_DEBUG) && (NIRVANA_DEBUG_ITERATORS != 0)

	const Cont* container () const
	{
		assert (proxy_);
		const void* obj = proxy_->object ();
		assert (obj);
		return (const Cont*)obj;
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
		assert (cont == rhs.container ());
		Margins m (cont);
		assert (m.begin <= ptr_ && ptr_ <= m.end);
		assert (m.begin <= rhs.ptr_ && rhs.ptr_ <= m.end);
#endif
	}

private:
	pointer ptr_;

	friend Cont;

#if defined (_DEBUG) && (NIRVANA_DEBUG_ITERATORS != 0)
	RuntimeProxy_var proxy_;
#endif
};

template <class Cont>
class StdIterator : public StdConstIterator <Cont>
{
public:
	typedef typename StdConstIterator <Cont>::iterator_category iterator_category;
	typedef typename Cont::value_type value_type;
	typedef typename StdConstIterator <Cont>::difference_type difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

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

	StdIterator <Cont>& operator ++ ()
	{
		StdConstIterator <Cont>::operator ++ ();
		return *this;
	}

	StdIterator <Cont> operator ++ (int)
	{
		StdIterator <Cont> tmp = *this;
		operator ++ ();
		return tmp;
	}

	StdIterator <Cont>& operator -- ()
	{
		StdConstIterator <Cont>::operator -- ();
		return *this;
	}

	StdIterator <Cont> operator -- (int)
	{
		StdIterator <Cont> tmp = *this;
		operator -- ();
		return tmp;
	}

	StdIterator <Cont>& operator += (difference_type off)
	{
		StdConstIterator <Cont>::operator += (off);
		return *this;
	}

	NIRVANA_NODISCARD StdIterator <Cont> operator + (difference_type off) const
	{
		StdIterator <Cont> tmp = *this;
		return tmp += off;
	}

	StdIterator <Cont>& operator -= (difference_type off)
	{
		StdConstIterator <Cont>::operator -= (off);
		return *this;
	}

	NIRVANA_NODISCARD StdIterator <Cont> operator - (difference_type off) const
	{
		StdIterator <Cont> tmp = *this;
		return tmp -= off;
	}

	NIRVANA_NODISCARD difference_type operator - (const StdConstIterator <Cont>& rhs) const
	{	// return difference of iterators
		return StdConstIterator <Cont>::operator - (rhs);
	}
};

#if __cplusplus >= 201103L
template<typename _InIter>
using _RequireInputIter = typename
std::enable_if<std::is_convertible<typename
	std::iterator_traits<_InIter>::iterator_category,
	std::input_iterator_tag>::value>::type;
#endif

}

namespace CORBA {
namespace Nirvana {

template <class T> struct MarshalTraits;

template <typename Cont>
void _check_bound (const Cont& cont, size_t max_length)
{
	assert (max_length);
	if (cont.size () > max_length)
		throw BAD_PARAM ();
}

}
}

#endif
