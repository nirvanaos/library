/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
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

#if defined _XMEMORY_ || defined _IOSFWD_ || defined _XSTRING_
#error "stl_utils.h must be included first"
#endif

#define allocator StdAllocator

#ifdef _MSC_BUILD
#define __declspec(x)
#endif

#include <xmemory>
#include <iosfwd>

#ifdef _MSC_BUILD
#undef __declspec
#endif

#undef allocator

namespace std {

template <typename T>
using allocator = StdAllocator <T>;

struct random_access_iterator_tag;
template <class I> class reverse_iterator;
#ifdef NIRVANA_C11
template <class _Elem> class initializer_list;
template <class _Iter> struct iterator_traits;
template <class _Ty> struct iterator_traits<_Ty*>;
struct input_iterator_tag;
#endif
}

#include <type_traits>
#include "MemoryHelper.h"

/// \def NIRVANA_DEBUG_ITERATORS
/// Controls the iterator debugging.
/// Possible values:
/// 0 - No iterator debugging.
/// 1 - Check container margins on iterator dereference.
/// > 1 - Check container margins on aall cases.

// Set NIRVANA_DEBUG_ITERATORS to default
#if !defined (NIRVANA_DEBUG_ITERATORS)
#	if defined (_ITERATOR_DEBUG_LEVEL)
#		ifdef _DEBUG
#			define NIRVANA_DEBUG_ITERATORS _ITERATOR_DEBUG_LEVEL
#		elif _ITERATOR_DEBUG_LEVEL != 0
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

	static Memory::_ptr_type memory ()
	{
		return MemoryHelper::memory ();
	}

	StdContainer () NIRVANA_NOEXCEPT
	{}

	~StdContainer () NIRVANA_NOEXCEPT
	{
#if (NIRVANA_DEBUG_ITERATORS != 0)
		remove_proxy ();
#endif
	}

private:
	template <class Cont> friend class StdConstIterator;

	void remove_proxy () const NIRVANA_NOEXCEPT;
};

class RuntimeProxy;

class StdDebugIterator
{
protected:
	StdDebugIterator () NIRVANA_NOEXCEPT;
	StdDebugIterator (const void* cont);
	~StdDebugIterator () NIRVANA_NOEXCEPT;

	StdDebugIterator (const StdDebugIterator& src);

	StdDebugIterator& operator = (const StdDebugIterator& src);

	const void* container () const NIRVANA_NOEXCEPT;

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
	typedef std::random_access_iterator_tag iterator_category;
	typedef typename Cont::value_type value_type;
	typedef ptrdiff_t difference_type;
	typedef const value_type* pointer;
	typedef const value_type& reference;

	StdConstIterator () NIRVANA_NOEXCEPT
	{}

	StdConstIterator (pointer p, const Cont& c) NIRVANA_NOEXCEPT :
#if (NIRVANA_DEBUG_ITERATORS != 0)
		StdDebugIterator (&c),
#endif
		ptr_ (p)
	{}

	NIRVANA_NODISCARD reference operator * () const NIRVANA_NOEXCEPT
	{
		_check_deref ();
		return *ptr_;
	}

	NIRVANA_NODISCARD pointer operator -> () const NIRVANA_NOEXCEPT
	{
		_check_deref ();
		return ptr_;
	}

	NIRVANA_NODISCARD reference operator [] (difference_type off) const NIRVANA_NOEXCEPT
	{	// subscript
		return *(*this + off); // _check_deref() called here
	}

	StdConstIterator <Cont>& operator ++ () NIRVANA_NOEXCEPT
	{
		_check_offset (1);
		++ptr_;
		return *this;
	}

	StdConstIterator <Cont> operator ++ (int) NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont> tmp = *this;
		operator ++ ();
		return tmp;
	}

	StdConstIterator <Cont>& operator -- () NIRVANA_NOEXCEPT
	{
		_check_offset (-1);
		--ptr_;
		return *this;
	}

	StdConstIterator <Cont> operator -- (int) NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont> tmp = *this;
		operator -- ();
		return tmp;
	}

	StdConstIterator <Cont>& operator += (difference_type off) NIRVANA_NOEXCEPT
	{
		_check_offset (off);
		ptr_ += off;
		return *this;
	}

	NIRVANA_NODISCARD StdConstIterator <Cont> operator + (difference_type off) const NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont> tmp = *this;
		tmp += off;
		return tmp;
	}

	StdConstIterator <Cont>& operator -= (difference_type off) NIRVANA_NOEXCEPT
	{
		_check_offset (-off);
		ptr_ -= off;
		return *this;
	}

	NIRVANA_NODISCARD StdConstIterator <Cont> operator - (difference_type off) const NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont> tmp = *this;
		tmp -= off;
		return tmp;
	}

	NIRVANA_NODISCARD difference_type operator - (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// return difference of iterators
		_check_compat (rhs);
		return ptr_ - rhs.ptr_;
	}

	NIRVANA_NODISCARD bool operator == (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// test for iterator equality
		_check_compat (rhs);
		return ptr_ == rhs.ptr_;
	}

	NIRVANA_NODISCARD bool operator != (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// test for iterator inequality
		return !(*this == rhs);
	}

	NIRVANA_NODISCARD bool operator < (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// test if this < rhs
		_check_compat (rhs);
		return (ptr_ < rhs.ptr_);
	}

	NIRVANA_NODISCARD bool operator > (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// test if this > rhs
		return (rhs < *this);
	}

	NIRVANA_NODISCARD bool operator <= (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// test if this <= rhs
		return (!(rhs < *this));
	}

	NIRVANA_NODISCARD bool operator >= (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// test if this >= rhs
		return (!(*this < rhs));
	}

private:
	// Iterator debugging

#if (NIRVANA_DEBUG_ITERATORS != 0)

	const Cont* container () const NIRVANA_NOEXCEPT
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

	void _check_deref () const NIRVANA_NOEXCEPT
	{
#if (NIRVANA_DEBUG_ITERATORS != 0)
		const Cont* cont = container ();
		if (!cont)
			return;
		Margins m (cont);
		assert (m.begin <= ptr_ && ptr_ < m.end);
#endif
	}

	void _check_offset (difference_type off) const NIRVANA_NOEXCEPT
	{
#if (NIRVANA_DEBUG_ITERATORS > 1)
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

	void _check_compat (const StdConstIterator <Cont>& rhs) const
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

template <class Cont>
class StdIterator : public StdConstIterator <Cont>
{
public:
	typedef typename StdConstIterator <Cont>::iterator_category iterator_category;
	typedef typename Cont::value_type value_type;
	typedef typename StdConstIterator <Cont>::difference_type difference_type;
	typedef value_type* pointer;
	typedef value_type& reference;

	StdIterator () NIRVANA_NOEXCEPT
	{}

	StdIterator (pointer p, const Cont& c) NIRVANA_NOEXCEPT :
		StdConstIterator <Cont> (p, c)
	{}

	NIRVANA_NODISCARD reference operator * () const NIRVANA_NOEXCEPT
	{
		return const_cast <reference> (StdConstIterator <Cont>::operator * ());
	}

	NIRVANA_NODISCARD pointer operator -> () const NIRVANA_NOEXCEPT
	{
		return const_cast <pointer> (StdConstIterator <Cont>::operator -> ());
	}

	NIRVANA_NODISCARD reference operator [] (difference_type off) const NIRVANA_NOEXCEPT
	{	// subscript
		return const_cast <reference> (StdConstIterator <Cont>::operator [] (off));
	}

	StdIterator <Cont>& operator ++ () NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont>::operator ++ ();
		return *this;
	}

	StdIterator <Cont> operator ++ (int) NIRVANA_NOEXCEPT
	{
		StdIterator <Cont> tmp = *this;
		operator ++ ();
		return tmp;
	}

	StdIterator <Cont>& operator -- () NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont>::operator -- ();
		return *this;
	}

	StdIterator <Cont> operator -- (int) NIRVANA_NOEXCEPT
	{
		StdIterator <Cont> tmp = *this;
		operator -- ();
		return tmp;
	}

	StdIterator <Cont>& operator += (difference_type off) NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont>::operator += (off);
		return *this;
	}

	NIRVANA_NODISCARD StdIterator <Cont> operator + (difference_type off) const NIRVANA_NOEXCEPT
	{
		StdIterator <Cont> tmp = *this;
		tmp += off;
		return tmp;
	}

	StdIterator <Cont>& operator -= (difference_type off) NIRVANA_NOEXCEPT
	{
		StdConstIterator <Cont>::operator -= (off);
		return *this;
	}

	NIRVANA_NODISCARD StdIterator <Cont> operator - (difference_type off) const NIRVANA_NOEXCEPT
	{
		StdIterator <Cont> tmp = *this;
		tmp -= off;
		return tmp;
	}

	NIRVANA_NODISCARD difference_type operator - (const StdConstIterator <Cont>& rhs) const NIRVANA_NOEXCEPT
	{	// return difference of iterators
		return StdConstIterator <Cont>::operator - (rhs);
	}
};

#ifdef NIRVANA_C11
template <typename _InIter>
using _RequireInputIter = typename
std::enable_if <std::is_convertible <typename
	std::iterator_traits <_InIter>::iterator_category,
	std::input_iterator_tag>::value>::type;
#endif

}

#endif
