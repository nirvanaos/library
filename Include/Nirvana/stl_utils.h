/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef NIRVANA_STL_UTILS_H_
#define NIRVANA_STL_UTILS_H_
#pragma once

#include "NirvanaBase.h"
#include <type_traits>
#include "MemoryHelper.h"

NIRVANA_STD_BEGIN

struct random_access_iterator_tag;
template <class Itf> class reverse_iterator;
template <class _Iter> struct iterator_traits;
template <class _Ty> struct iterator_traits<_Ty*>;
struct input_iterator_tag;

#ifdef NIRVANA_C20
struct contiguous_iterator_tag;
template <class _Iter> struct pointer_traits;
#endif

NIRVANA_STD_END

namespace std {
template <class _Elem> class initializer_list;
}

/// \def NIRVANA_DEBUG_ITERATORS
/// Controls the iterator debugging.
/// Possible values:
/// 0 - No iterator debugging.
/// 1 - Check container margins on iterator dereference.
/// > 1 - Check container margins in all cases.

// Set NIRVANA_DEBUG_ITERATORS to default
#if !defined (NIRVANA_DEBUG_ITERATORS)
#	if defined (_ITERATOR_DEBUG_LEVEL)
#		ifndef NDEBUG
#			define NIRVANA_DEBUG_ITERATORS _ITERATOR_DEBUG_LEVEL
#		elif _ITERATOR_DEBUG_LEVEL != 0
#			define NIRVANA_DEBUG_ITERATORS 1
#		endif
#	elif defined (_GLIBCXX_DEBUG) && (_GLIBCXX_DEBUG != 0)
#		define NIRVANA_DEBUG_ITERATORS 1
#	elif defined (_LIBCPP_DEBUG) && (_LIBCPP_DEBUG != 0)
#		define NIRVANA_DEBUG_ITERATORS 1
# elif defined (NDEBUG)
#		define NIRVANA_DEBUG_ITERATORS 0
# else
#		define NIRVANA_DEBUG_ITERATORS 1
#	endif
#endif

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
	typedef Nirvana::MemoryHelper MemoryHelper;

	NIRVANA_CONSTEXPR20
	StdContainer () noexcept
	{}

	NIRVANA_CONSTEXPR20
	~StdContainer () noexcept
	{
#if (NIRVANA_DEBUG_ITERATORS != 0)
#ifdef NIRVANA_C20
		if (!std::is_constant_evaluated ())
#endif
			remove_proxy ();
#endif
	}

private:
	template <class Cont> friend class StdConstIterator;

	void remove_proxy () const noexcept;
};

class RuntimeProxy;

class StdDebugIterator
{
protected:
	StdDebugIterator () noexcept;

	StdDebugIterator (const void* cont)
#if (NIRVANA_DEBUG_ITERATORS != 0)
		: proxy_ (
#ifdef NIRVANA_C20
			std::is_constant_evaluated () ? nullptr :
#endif
			get_proxy (cont))
#endif
	{}

	~StdDebugIterator () noexcept;

	StdDebugIterator (const StdDebugIterator& src);

	StdDebugIterator& operator = (const StdDebugIterator& src);

	const void* container () const noexcept;

private:
	static CORBA::Internal::I_ref <RuntimeProxy> get_proxy (const void* cont);

protected:
	CORBA::Internal::I_ref <RuntimeProxy> proxy_;
};

template <class Cont>
class StdConstIterator
#if (NIRVANA_DEBUG_ITERATORS != 0)
	: StdDebugIterator
#endif
{
public:
#ifdef NIRVANA_C20
	typedef std::contiguous_iterator_tag iterator_category;
	typedef const typename Cont::value_type element_type;
#else
	typedef std::random_access_iterator_tag iterator_category;
#endif

	typedef typename Cont::value_type value_type;
	typedef ptrdiff_t difference_type;
	typedef const value_type* pointer;
	typedef const value_type& reference;

	StdConstIterator () noexcept :
		ptr_ (nullptr)
	{}

	StdConstIterator (pointer p, const Cont& c) noexcept :
#if (NIRVANA_DEBUG_ITERATORS != 0)
		StdDebugIterator (&c),
#endif
		ptr_ (p)
	{}

	NIRVANA_NODISCARD const value_type& operator * () const noexcept
	{
		_check_deref ();
		return *ptr_;
	}

	NIRVANA_NODISCARD const value_type* operator -> () const noexcept
	{
		_check_deref ();
		return ptr_;
	}

	NIRVANA_NODISCARD const value_type& operator [] (difference_type off) const noexcept
	{	// subscript
		_check_offset (off);
		return ptr_ [off];
	}

	StdConstIterator& operator ++ () noexcept
	{
		_check_offset (1);
		++ptr_;
		return *this;
	}

	StdConstIterator operator ++ (int) noexcept
	{
		StdConstIterator tmp = *this;
		operator ++ ();
		return tmp;
	}

	StdConstIterator& operator -- () noexcept
	{
		_check_offset (-1);
		--ptr_;
		return *this;
	}

	StdConstIterator operator -- (int) noexcept
	{
		StdConstIterator tmp = *this;
		operator -- ();
		return tmp;
	}

	StdConstIterator& operator += (difference_type off) noexcept
	{
		_check_offset (off);
		ptr_ += off;
		return *this;
	}

	StdConstIterator& operator -= (difference_type off) noexcept
	{
		_check_offset (-off);
		ptr_ -= off;
		return *this;
	}

	NIRVANA_NODISCARD bool operator == (const StdConstIterator& rhs) const noexcept
	{	// test for iterator equality
		_check_compat (rhs);
		return ptr_ == rhs.ptr_;
	}

	NIRVANA_NODISCARD bool operator != (const StdConstIterator& rhs) const noexcept
	{	// test for iterator inequality
		return !(*this == rhs);
	}

	NIRVANA_NODISCARD bool operator < (const StdConstIterator& rhs) const noexcept
	{	// test if this < rhs
		_check_compat (rhs);
		return (ptr_ < rhs.ptr_);
	}

	NIRVANA_NODISCARD bool operator > (const StdConstIterator& rhs) const noexcept
	{	// test if this > rhs
		return (rhs < *this);
	}

	NIRVANA_NODISCARD bool operator <= (const StdConstIterator& rhs) const noexcept
	{	// test if this <= rhs
		return (!(rhs < *this));
	}

	NIRVANA_NODISCARD bool operator >= (const StdConstIterator& rhs) const noexcept
	{	// test if this >= rhs
		return (!(*this < rhs));
	}

#ifdef NIRVANA_C20
	friend auto operator <=> (StdConstIterator, StdConstIterator) = default;
	template <class T> friend struct std::pointer_traits;
#endif

	template <class Cnt>
  friend ptrdiff_t operator - (const StdConstIterator <Cnt>&, const StdConstIterator <Cnt>&) noexcept;

private:
	// Iterator debugging

#if (NIRVANA_DEBUG_ITERATORS != 0)

	const Cont* container () const noexcept
	{
		return reinterpret_cast <const Cont*> (StdDebugIterator::container ());
	}

	struct Margins
	{
		Margins (const Cont* cont)
		{
			assert (cont);
			begin = cont->data ();
			end = begin + cont->size ();
		}

		pointer begin, end;
	};

#endif

	void _check_deref () const noexcept
	{
#if (NIRVANA_DEBUG_ITERATORS != 0)
		assert (ptr_);
		const Cont* cont = container ();
		if (!cont)
			return;
		Margins m (cont);
		assert (m.begin <= ptr_ && ptr_ < m.end);
#endif
	}

	void _check_offset (difference_type off) const noexcept
	{
#if (NIRVANA_DEBUG_ITERATORS > 1)
		assert (ptr_);
		const Cont* cont = container ();
		if (!cont)
			return;
		Margins m (cont);
		pointer p = ptr_;
		assert (m.begin <= p && p <= m.end);
		p += off;
		assert (m.begin <= p && p <= m.end);
#endif
	}

	void _check_compat (const StdConstIterator& rhs) const
	{
#if (NIRVANA_DEBUG_ITERATORS != 0)
		const Cont* cont = container ();
		if (!cont)
			return;
		assert (cont == rhs.container ());

#if (NIRVANA_DEBUG_ITERATORS > 1)
		Margins m (cont);
		assert (m.begin <= ptr_ && ptr_ <= m.end);
		assert (m.begin <= rhs.ptr_ && rhs.ptr_ <= m.end);
#endif
#endif
	}

private:
	pointer ptr_;

	friend Cont;
};

template <class Cont> NIRVANA_NODISCARD inline
StdConstIterator <Cont> operator + (StdConstIterator <Cont> it, ptrdiff_t off) noexcept
{
	return it += off;
}

template <class Cont> NIRVANA_NODISCARD inline
StdConstIterator <Cont> operator + (ptrdiff_t off, StdConstIterator <Cont> it) noexcept
{
	return it += off;
}

template <class Cont> NIRVANA_NODISCARD inline
StdConstIterator <Cont> operator - (StdConstIterator <Cont> it, ptrdiff_t off) noexcept
{
	return it -= off;
}

template <class Cont> NIRVANA_NODISCARD inline
ptrdiff_t operator - (const StdConstIterator <Cont>& lhs, const StdConstIterator <Cont>& rhs) noexcept
{	// return difference of iterators
	lhs._check_compat (rhs);
	return lhs.ptr_ - rhs.ptr_;
}

template <class Cont>
class StdIterator : public StdConstIterator <Cont>
{
	typedef StdConstIterator <Cont> Base;

public:
	typedef typename Base::iterator_category iterator_category;
	typedef typename Cont::value_type value_type;
	typedef typename Base::difference_type difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

#ifdef NIRVANA_C20
	typedef typename Cont::value_type element_type;
#endif

	StdIterator () noexcept
	{}

	StdIterator (pointer p, const Cont& c) noexcept :
		Base (p, c)
	{}

	NIRVANA_NODISCARD value_type& operator * () const noexcept
	{
		return const_cast <value_type&> (Base::operator * ());
	}

	NIRVANA_NODISCARD value_type* operator -> () const noexcept
	{
		return const_cast <value_type*> (Base::operator -> ());
	}

	NIRVANA_NODISCARD value_type& operator [] (difference_type off) const noexcept
	{	// subscript
		return const_cast <value_type&> (Base::operator [] (off));
	}

	StdIterator& operator ++ () noexcept
	{
		Base::operator ++ ();
		return *this;
	}

	StdIterator operator ++ (int) noexcept
	{
		StdIterator tmp = *this;
		operator ++ ();
		return tmp;
	}

	StdIterator& operator -- () noexcept
	{
		Base::operator -- ();
		return *this;
	}

	StdIterator operator -- (int) noexcept
	{
		StdIterator tmp = *this;
		operator -- ();
		return tmp;
	}

	StdIterator& operator += (difference_type off) noexcept
	{
		Base::operator += (off);
		return *this;
	}

	StdIterator& operator -= (difference_type off) noexcept
	{
		Base::operator -= (off);
		return *this;
	}

#ifdef NIRVANA_C20
	template <class T> friend struct std::pointer_traits;
#endif

};

template <class Cont> NIRVANA_NODISCARD inline
StdIterator <Cont> operator + (StdIterator <Cont> it, ptrdiff_t off) noexcept
{
	return it += off;
}

template <class Cont> NIRVANA_NODISCARD inline
StdIterator <Cont> operator + (ptrdiff_t off, StdIterator <Cont> it) noexcept
{
	return it += off;
}

template <class Cont> NIRVANA_NODISCARD inline
StdIterator <Cont> operator - (StdIterator <Cont> it, ptrdiff_t off) noexcept
{
	return it -= off;
}

template <class Cont> NIRVANA_NODISCARD inline
ptrdiff_t operator - (const StdIterator <Cont>& lhs, const StdIterator <Cont>& rhs) noexcept
{	// return difference of iterators
	return operator - (static_cast <const StdConstIterator <Cont>&> (lhs),
		static_cast <const StdConstIterator <Cont>&> (rhs));
}

template <typename _InIter>
using _RequireInputIter = typename
std::enable_if <std::is_convertible <typename
	std::iterator_traits <_InIter>::iterator_category,
	std::input_iterator_tag>::value>::type;

}

#ifdef NIRVANA_C20

namespace std {

template <class Cont>
struct pointer_traits <Nirvana::StdConstIterator <Cont> > {
    static Nirvana::StdConstIterator <Cont>::element_type* to_address (const Nirvana::StdConstIterator <Cont>& i) {
      return i.ptr_;
    }
};

template <class Cont>
struct pointer_traits <Nirvana::StdIterator <Cont> > {
    static Nirvana::StdIterator <Cont>::element_type* to_address (const Nirvana::StdIterator <Cont>& i) {
      return const_cast <Nirvana::StdIterator <Cont>::element_type*> (i.ptr_);
    }
};

}

#endif

#endif
