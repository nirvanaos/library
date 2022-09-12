/// \file
/// \brief std::basic_string template specialization.
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
#ifndef NIRVANA_BASIC_STRING_H_
#define NIRVANA_BASIC_STRING_H_
#pragma once

#ifdef _XSTRING_
#error "basic_string.h must be included first"
#endif

#include <CORBA/StringView.h>
#include "stl_utils.h"
#include "real_copy.h"
#include <errno.h>

namespace std {
#ifdef NIRVANA_C17
template <class C, class T> class basic_string_view;
#endif


// MS extension
struct _String_constructor_concat_tag;

}

namespace Nirvana {

class StdString :
	public StdContainer
{
protected:
	NIRVANA_NORETURN static void xout_of_range ()
	{
		StdContainer::xout_of_range ("invalid string position");
	}
	NIRVANA_NORETURN static void xlength_error ()
	{
		StdContainer::xlength_error ("string too long");
	}
};

}

namespace std {

template <typename C, class T>
class basic_string <C, T, allocator <C> > :
	protected CORBA::Internal::ABI <CORBA::Internal::StringT <C> >,
	public Nirvana::StdString
{
	typedef basic_string <C, T, allocator <C> > MyType;
	typedef CORBA::Internal::ABI <CORBA::Internal::StringT <C> > ABI;

public:
	template <uint32_t bound>
	operator const CORBA::Internal::StringView <C, bound>& () const NIRVANA_NOEXCEPT
	{
		return reinterpret_cast <const CORBA::Internal::StringView <C, bound>&> (*this);
	}

	using const_iterator = Nirvana::StdConstIterator <MyType>;
	using iterator = Nirvana::StdIterator <MyType>;

	typedef std::reverse_iterator <const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator <iterator> reverse_iterator;

	typedef C value_type;
	typedef T traits_type;
	typedef allocator <C> allocator_type;

	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef value_type& reference;

	typedef ptrdiff_t difference_type;
	typedef size_t size_type;

	static const size_type npos = -1;

#ifdef NIRVANA_C17
	// A helper type for avoiding boiler-plate.
	typedef basic_string_view <C, T> __sv_type;

	template<typename _Tp, typename _Res>
	using _If_sv = enable_if_t <
		is_convertible <const _Tp&, __sv_type>::value
		&& !is_convertible <const _Tp*, const basic_string*>::value
		&& !is_convertible <const _Tp&, const C*>::value,
		_Res>;

#endif

	~basic_string ()
	{
		try {
			release_memory ();
		} catch (...) {
			assert (false);
		}
	}

	// Constructors

	basic_string () NIRVANA_NOEXCEPT
	{
		ABI::reset ();
	}

	explicit basic_string (const allocator_type&) :
		basic_string ()
	{}

	basic_string (const basic_string& src)
	{
		if (src.ABI::is_large ()) {
			ABI::reset ();
			assign (src.large_pointer (), src.large_size ());
		} else
			ABI::operator = (src);
	}

	basic_string (basic_string&& src) NIRVANA_NOEXCEPT
	{
		ABI::operator = (src);
		src.reset ();
	}

	basic_string (const basic_string& src, size_type off, size_type cnt = npos)
	{
		ABI::reset ();
		assign (src, off, cnt);
	}

	basic_string (const basic_string& src, size_type off, size_type cnt, const allocator_type&) :
		basic_string (src, off, cnt)
	{}

	basic_string (const value_type* ptr, size_type cnt)
	{
		ABI::reset ();
		assign (ptr, cnt);
	}

	basic_string (const value_type* ptr, size_type cnt, const allocator_type&) :
		basic_string (ptr, cnt)
	{}

	basic_string (const value_type* ptr)
	{
		ABI::reset ();
		assign (ptr);
	}

	basic_string (const value_type* ptr, const allocator_type&) :
		basic_string (ptr)
	{}

	basic_string (size_type cnt, value_type c)
	{
		ABI::reset ();
		assign (cnt, c);
	}

	basic_string (size_type cnt, value_type c, const allocator_type&) :
		basic_string (cnt, c)
	{}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	basic_string (InputIterator b, InputIterator e)
	{
		ABI::reset ();
		assign (b, e);
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	basic_string (InputIterator b, InputIterator e, const allocator_type&) :
		basic_string (b, e)
	{}

	basic_string (const_pointer b, const_pointer e)
	{
		ABI::reset ();
		assign (b, e);
	}

	basic_string (const_iterator b, const_iterator e)
	{
		ABI::reset ();
		assign (b, e);
	}

#ifdef NIRVANA_C11

	basic_string (initializer_list <value_type> ilist)
	{
		ABI::reset ();
		assign (ilist);
	}

#endif

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	basic_string (const V& v, const allocator_type& = allocator_type ())
	{
		ABI::reset ();
		assign (v);
	}

	template <class V, class = _If_sv <V, void> >
	basic_string (const V& v, size_type pos, size_type count, const allocator_type& = allocator_type ())
	{
		ABI::reset ();
		assign (v, pos, count);
	}

	operator basic_string_view <value_type, traits_type> () const NIRVANA_NOEXCEPT
	{
		return basic_string_view <value_type, traits_type> (data (), length ());
	}

#endif

	// MS extensions

	basic_string (const _String_constructor_concat_tag&, const basic_string&, const value_type* const _Left_ptr,
		const size_type _Left_size, const value_type* const _Right_ptr, const size_type _Right_size)
	{
		ABI::reset ();
		reserve (_Left_size + _Right_size);
		assign (_Left_ptr, _Left_size);
		append (_Right_ptr, _Right_size);
	}

	basic_string (const _String_constructor_concat_tag&, basic_string& _Left, basic_string& _Right)
	{
		ABI::operator = (_Left);
		_Left.reset ();
		append (_Right);
	}

	// Assignments

	basic_string& operator = (value_type c)
	{
		return assign (1, c);
	}

	basic_string& operator = (const value_type* s)
	{
		return assign (s);
	}

	basic_string& operator = (const basic_string& src)
	{
		return assign (src);
	}

	basic_string& operator = (basic_string&& src) NIRVANA_NOEXCEPT
	{
		if (this != &src) {
			release_memory ();
			ABI::operator = (src);
			src.reset ();
		}
		return *this;
	}

#ifdef NIRVANA_C11

	basic_string& operator = (initializer_list <value_type> ilist)
	{
		return assign (ilist);
	}

#endif

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	basic_string& operator = (const V& v)
	{
		return assign (v);
	}

#endif

	basic_string& assign (const value_type* ptr)
	{
		return assign (ptr, traits_type::length (ptr));
	}

	basic_string& assign (const value_type* ptr, size_type count)
	{
		assign_internal (count, ptr);
		return *this;
	}

	basic_string& assign (const basic_string& str, size_type off, size_type count = npos)
	{
		const_pointer p = str.get_range (off, count);
		assign (p, count);
	}

	basic_string& assign (const basic_string& src)
	{
		if (this != &src) {
			if (!src.is_large ()) {
				if (!ABI::is_large ())
					ABI::operator = (src);
				else
					assign (src.small_pointer (), src.small_size ());
			} else
				assign (src.large_pointer (), src.large_size ());
		}
		return *this;
	}

	basic_string& assign (size_type count, value_type c)
	{
		traits_type::assign (assign_internal (count), count, c);
		return *this;
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	basic_string& assign (InputIterator b, InputIterator e);

	basic_string& assign (const_pointer b, const_pointer e)
	{
		return assign (b, e - b);
	}

	basic_string& assign (const_iterator b, const_iterator e)
	{
		return assign (&*b, e - b);
	}

#ifdef NIRVANA_C11

	basic_string& assign (initializer_list <value_type> ilist)
	{
		return assign (ilist.begin (), ilist.size ());
	}

#endif

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	basic_string& assign (const V& v)
	{
		__sv_type sv (v);
		return assign (sv.data (), sv.length ());
	}

	template <class V, class = _If_sv <V, void> >
	basic_string& assign (const V& v, size_type pos, size_type count = npos)
	{
		__sv_type sv = __sv_type (v).substr (pos, count);
		return assign (sv.data (), sv.length ());
	}

#endif

	// append

	basic_string& append (const value_type* ptr)
	{
		return append (ptr, traits_type::length (ptr));
	}

	basic_string& append (const value_type* ptr, size_type count)
	{
		return insert (length (), ptr, count);
	}

	basic_string& append (const basic_string& str, size_type off, size_type count)
	{
		const_pointer p = str.get_range (off, count);
		append (p, count);
	}

	basic_string& append (const basic_string& src)
	{
		return append (src.data (), src.size ());
	}

	basic_string& append (size_type count, value_type c)
	{
		return insert (length (), count, c);
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	basic_string& append (InputIterator b, InputIterator e)
	{
		insert (end (), b, e);
		return *this;
	}

	basic_string& append (const_pointer b, const_pointer e)
	{
		return append (b, e - b);
	}

	basic_string& append (const_iterator b, const_iterator e)
	{
		if (b != e)
			append (&*b, e - b);
		return *this;
	}

#ifdef NIRVANA_C11

	basic_string& append (initializer_list <value_type> ilist)
	{
		return append (ilist.begin (), ilist.size ());
	}

#endif

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	basic_string& append (const V& v)
	{
		__sv_type sv (v);
		return append (sv.data (), sv.length ());
	}

	template <class V, class = _If_sv <V, void> >
	basic_string& append (const V& v, size_type pos, size_type count = npos)
	{
		__sv_type sv = __sv_type (v).substr (pos, count);
		return append (sv.data (), sv.length ());
	}

#endif

	basic_string& operator += (value_type c)
	{
		return append (1, c);
	}

	basic_string& operator += (const value_type* s)
	{
		return append (s);
	}

	basic_string& operator += (const basic_string& s)
	{
		return append (s);
	}

#ifdef NIRVANA_C11

	basic_string& operator += (initializer_list <value_type> ilist)
	{
		return append (ilist);
	}

#endif

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	basic_string& operator += (const V& v)
	{
		return append (v);
	}

#endif

	// insert

	basic_string& insert (size_type pos, const basic_string& s)
	{
		return insert (pos, s.c_str (), s.length ());
	}

	basic_string& insert (size_type pos, const basic_string& s, size_type off, size_type count)
	{
		const_pointer p = s.get_range (off, count);
		return insert (pos, p, count);
	}

	basic_string& insert (size_type pos, const value_type* s)
	{
		return insert (pos, s, traits_type::length (s));
	}

	basic_string& insert (size_type pos, const value_type* s, size_type count)
	{
		assert (s);
		insert_internal (pos, count, s);
		return *this;
	}

	basic_string& insert (size_type pos, size_type count, value_type c)
	{
		traits_type::assign (insert_internal (pos, count), count, c);
		return *this;
	}

	void insert (iterator pos, size_type count, value_type c)
	{
		return insert (get_offset (pos), count, c);
	}

	iterator insert (iterator pos, value_type c)
	{
		insert (pos, 1, c);
		return begin () + pos;
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	void insert (iterator pos, InputIterator b, InputIterator e);

	void insert (iterator pos, const_pointer b, const_pointer e)
	{
		insert (get_offset (pos), b, e - b);
	}

	void insert (iterator pos, const_iterator b, const_iterator e)
	{
		if (b != e)
			insert (get_offset (pos), &*b, e - b);
	}

#ifdef NIRVANA_C11

	iterator insert (const_iterator pos, initializer_list <value_type> ilist)
	{
		return iterator (insert_internal (get_offset (pos), ilist.size (), ilist.begin ()), *this);
	}

#endif

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	basic_string& insert (size_type pos, const V& v)
	{
		__sv_type sv (v);
		return insert (pos, sv.data (), sv.length ());
	}

	template <class V, class = _If_sv <V, void> >
	basic_string& insert (size_type idx, const V& v, size_type pos, size_type count = npos)
	{
		__sv_type sv = __sv_type (v).substr (pos, count);
		return insert (idx, sv.data (), sv.length ());
	}

#endif

	// replace
	
	basic_string& replace (size_type pos, size_type count, const basic_string& s)
	{
		return replace (pos, count, s.data (), s.size ());
	}

	basic_string& replace (const_iterator b, const_iterator e, const basic_string& s)
	{
		size_type pos = get_offset (b);
		return replace (pos, get_offset (e) - pos, s.data (), s.size ());
	}

	basic_string& replace (size_type pos, size_type count, const basic_string& s, size_type pos2, size_type count2 = npos)
	{
		const_pointer p = s.get_range (pos2, count2);
		return replace (pos, count, p, count2);
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	basic_string& replace (const_iterator b, const_iterator e, InputIterator sb, InputIterator se);

	basic_string& replace (size_type pos, size_type count, const value_type* s, size_type count2)
	{
		replace_internal (pos, count, count2, s);
		return *this;
	}

	basic_string& replace (const_iterator b, const_iterator e, const value_type* s, size_type count2)
	{
		size_type pos = get_offset (b);
		return replace (pos, get_offset (e) - pos, s, count2);
	}

	basic_string& replace (size_type pos, size_type count, const value_type* s)
	{
		return replace (pos, count, s, traits_type::length (s));
	}

	basic_string& replace (const_iterator b, const_iterator e, const value_type* s)
	{
		return replace (b, e, s, traits_type::length (s));
	}

	basic_string& replace (size_type pos, size_type count, size_type count2, value_type c)
	{
		traits_type::assign (replace_internal (pos, count, count2), count2, c);
		return *this;
	}

	basic_string& replace (const_iterator b, const_iterator e, size_type count2, value_type c)
	{
		size_type pos = get_offset (b);
		return replace (pos, get_offset (e) - pos, count2, c);
	}

#ifdef NIRVANA_C11

	basic_string& replace (const_iterator b, const_iterator e, initializer_list <value_type> ilist)
	{
		size_type pos = get_offset (b);
		return replace (pos, get_offset (e) - pos, ilist.data (), ilist.size ());
	}

#endif

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	basic_string& replace (size_type pos, size_type count, const V& v)
	{
		__sv_type sv (v);
		return replace (pos, count, sv.data (), sv.length ());
	}

	template <class V, class = _If_sv <V, void> >
	basic_string& replace (const_iterator b, const_iterator e, const V& v)
	{
		__sv_type sv (v);
		return replace (b, e, sv.data (), sv.length ());
	}

	template <class V, class = _If_sv <V, void> >
	basic_string& replace (size_type pos, size_type count, const V& v, size_type pos2, size_type count2 = npos)
	{
		__sv_type sv = __sv_type (v).substr (pos2, count2);
		return replace (pos, count, sv.data (), sv.length ());
	}

#endif

	// compare

	int compare (const basic_string& s) const
	{
		return compare (c_str (), length (), s.c_str (), s.length ());
	}

	// For MSVC compatibility
	bool _Equal (const basic_string& s) const
	{
		return compare (s) == 0;
	}

	int compare (size_type pos, size_type cnt, const basic_string& s) const
	{
		const_pointer p = get_range (pos, cnt);
		return compare (p, cnt, s, s.length ());
	}

	int compare (size_type pos, size_type cnt, const basic_string& s, size_type off, size_type cnt2 = npos) const
	{
		const_pointer p = get_range (pos, cnt);
		const_pointer ps = s.get_range (off, cnt2);
		return compare (p, cnt, ps, cnt2);
	}

	int compare (const value_type* s) const
	{
		return compare (c_str (), length (), s, traits_type::length (s));
	}

	// For MSVC compatibility
	bool _Equal (const value_type* s) const
	{
		return compare (s) == 0;
	}

	int compare (size_type pos, size_type cnt, const value_type* s) const
	{
		const_pointer p = get_range (pos, cnt);
		return compare (p, cnt, s, traits_type::length (s));
	}

	int compare (size_type pos, size_type cnt, const value_type* s, size_type cnt2) const
	{
		const_pointer p = get_range (pos, cnt);
		return compare (p, cnt, s, cnt2);
	}

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	int compare (const V& v) const
	{
		__sv_type sv (v);
		return compare (sv.data (), sv.length ());
	}

	template <class V, class = _If_sv <V, void> >
	int compare (size_type pos, size_type cnt, const V& v) const
	{
		__sv_type sv (v);
		return compare (pos, cnt, sv.data (), sv.length ());
	}

	template <class V, class = _If_sv <V, void> >
	int compare (size_type pos, size_type cnt, const V& v, size_type pos2, size_type cnt2 = npos) const
	{
		__sv_type sv = __sv_type (v).substr (pos2, cnt2);
		return compare (pos, cnt, sv.data (), sv.length ());
	}

#endif

	// find

	size_type find (const basic_string& s, size_type pos = 0) const NIRVANA_NOEXCEPT
	{
		return find (s.data (), pos, s.length ());
	}

	size_type find (const value_type* s, size_type pos = 0) const
	{
		return find (s, pos, traits_type::length (s));
	}

	size_type find (const value_type* s, size_type pos, size_type len) const;

	size_type find (const value_type c, size_type pos = 0) const NIRVANA_NOEXCEPT;

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	size_type find (const V& v, size_type pos = 0) const
	{
		__sv_type sv (v);
		return find (sv.data (), pos, sv.length ());
	}

#endif

	size_type rfind (const basic_string& s, size_type pos = npos) const
	{
		return rfind (s.data (), pos, s.length ());
	}

	size_type rfind (const value_type* s, size_type pos = npos) const NIRVANA_NOEXCEPT
	{
		return rfind (s, pos, traits_type::length (s));
	}

	size_type rfind (const value_type* s, size_type pos, size_type len) const;

	size_type rfind (const value_type c, size_type pos = npos) const NIRVANA_NOEXCEPT;

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	size_type rfind (const V& v, size_type pos = npos) const
	{
		__sv_type sv (v);
		return rfind (sv.data (), pos, sv.length ());
	}

#endif

	size_type find_first_not_of (const basic_string& s, size_type pos = 0) const NIRVANA_NOEXCEPT
	{
		return find_first_not_of (s.data (), pos, s.length ());
	}

	size_type find_first_not_of (const value_type* s, size_type pos = 0) const
	{
		return find_first_not_of (s, pos, traits_type::length (s));
	}

	size_type find_first_not_of (const value_type* s, size_type pos, size_type len) const;

	size_type find_first_not_of (const value_type c, size_type pos = 0) const
	{
		const_pointer f, e;
		get_range (pos, f, e);
		for (; f != e; ++f) {
			if (!traits_type::eq (*f, c))
				break;
		}
		if (f == e)
			return npos;
		else
			return f - ABI::_ptr ();
	}

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	size_type find_first_not_of (const V& v, size_type pos = 0) const NIRVANA_NOEXCEPT
	{
		__sv_type sv (v);
		return find_first_not_of (sv.data (), pos, sv.length ());
	}

#endif

	size_type find_first_of (const basic_string& s, size_type pos = 0) const
	{
		return find_first_of (s.data (), pos, s.length ());
	}

	size_type find_first_of (const value_type* s, size_type pos = 0) const
	{
		return find_first_of (s, pos, traits_type::length (s));
	}

	size_type find_first_of (const value_type* s, size_type pos, size_type len) const;

	size_type find_first_of (const value_type c, size_type pos = 0) const
	{
		return find (c, pos);
	}

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	size_type find_first_of (const V& v, size_type pos = 0) const NIRVANA_NOEXCEPT
	{
		__sv_type sv (v);
		return find_first_of (sv.data (), pos, sv.length ());
	}

#endif

	size_type find_last_not_of (const basic_string& s, size_type pos = npos) const
	{
		return find_last_not_of (s.data (), pos, s.length ());
	}

	size_type find_last_not_of (const value_type* s, size_type pos = npos) const
	{
		return find_last_not_of (s, pos, traits_type::length (s));
	}

	size_type find_last_not_of (const value_type* s, size_type pos, size_type len) const;

	size_type find_last_not_of (const value_type c, size_type pos = npos) const
	{
		const_pointer b, f;
		get_range_rev (pos, b, f);
		--b; --f;
		for (; b != f; --f) {
			if (!traits_type::eq (*f, c))
				break;
		}
		if (f == b)
			return npos;
		else
			return f - ABI::_ptr ();
	}

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	size_type find_last_not_of (const V& v, size_type pos = 0) const NIRVANA_NOEXCEPT
	{
		__sv_type sv (v);
		return find_last_not_of (sv.data (), pos, sv.length ());
	}

#endif

	size_type find_last_of (const basic_string& s, size_type pos = npos) const
	{
		return find_last_of (s.data (), pos, s.length ());
	}

	size_type find_last_of (const value_type* s, size_type pos = npos) const
	{
		return find_last_of (s, pos, traits_type::length (s));
	}

	size_type find_last_of (const value_type* s, size_type pos, size_type len) const;

	size_type find_last_of (const value_type c, size_type pos = npos) const
	{
		return rfind (c, pos);
	}

#ifdef NIRVANA_C17

	template <class V, class = _If_sv <V, void> >
	size_type find_last_of (const V& v, size_type pos = 0) const NIRVANA_NOEXCEPT
	{
		__sv_type sv (v);
		return find_last_of (sv.data (), pos, sv.length ());
	}

#endif

	// Misc. operations

	const_reference at (size_type off) const
	{
		if (ABI::is_large ()) {
			if (off >= ABI::large_size ())
				xout_of_range ();
			return ABI::large_pointer () [off];
		} else {
			if (off >= ABI::small_size ())
				xout_of_range ();
			return ABI::small_pointer () [off];
		}
	}

	reference at (size_type off)
	{
		return const_cast <reference> (const_cast <basic_string*> (this)->at (off));
	}

	const_reference operator [] (size_type off) const
	{
		return ABI::_ptr () [off];
	}

	reference operator [] (size_type off)
	{
		return ABI::_ptr () [off];
	}

	const value_type* data () const
	{
		return ABI::_ptr ();
	}

#ifdef NIRVANA_C17
	value_type* data ()
	{
		return ABI::_ptr ();
	}
#endif

	const value_type* c_str () const
	{
		return data ();
	}

	size_type length () const
	{
		return size ();
	}

	size_type size () const
	{
		return ABI::size ();
	}

	size_type capacity () const
	{
		return ABI::capacity ();
	}

	void clear ();

	bool empty () const
	{
		return ABI::empty ();
	}

	static size_type max_size ()
	{
		return ABI::max_size ();
	}

	size_type copy (value_type* ptr, size_type count, size_type off = 0) const
	{
		const_pointer p = get_range (off, count);
		memory ()->copy (ptr, p, count * sizeof (value_type), 0);
	}

	iterator erase (iterator b, iterator e)
	{
		size_type pos = get_offset (b);
		erase (pos, get_offset (e) - pos);
		return b;
	}

	iterator erase (iterator it)
	{
		erase (get_offset (it), 1);
		return it;
	}

	basic_string& erase (size_type pos = 0, size_type count = npos);

	void pop_back ()
	{
		erase (length () - 1, 1);
	}

	void push_back (value_type c)
	{
		append (1, c);
	}

	void reserve (size_type cap = 0);

	void resize (size_type new_size, value_type c)
	{
		size_type size = ABI::size ();
		if (new_size > size) {
			size_type count = new_size - size;
			traits_type::assign (insert_internal (size, count), count, c);
		} else
			erase (new_size, size - new_size);
	}

	void resize (size_type new_size)
	{
		size_type size = ABI::size ();
		if (new_size > size)
			insert_internal (size, new_size - size);
		else
			erase (new_size, size - new_size);
	}

	void shrink_to_fit ();

	basic_string substr (size_type pos = 0, size_type len = npos) const
	{
		const_pointer p = get_range (pos, len);
		return basic_string (p, len);
	}

	void swap (basic_string& rhs)
	{
		ABI tmp = *this;
		ABI::operator = (rhs);
		static_cast <ABI&> (rhs) = tmp;
	}

	NIRVANA_NODISCARD allocator_type get_allocator () const
	{
		return allocator_type ();
	}

	// Iterators

	NIRVANA_NODISCARD const_iterator cbegin () const NIRVANA_NOEXCEPT
	{
		return const_iterator (ABI::_ptr (), *this);
	}

	NIRVANA_NODISCARD iterator begin () NIRVANA_NOEXCEPT
	{
		return iterator (ABI::_ptr (), *this);
	}

	NIRVANA_NODISCARD const_iterator begin () const NIRVANA_NOEXCEPT
	{
		return cbegin ();
	}

	NIRVANA_NODISCARD const_iterator cend () const NIRVANA_NOEXCEPT
	{
		return const_iterator (ABI::_end_ptr (), *this);
	}

	NIRVANA_NODISCARD iterator end () NIRVANA_NOEXCEPT
	{
		return iterator (ABI::_end_ptr (), *this);
	}

	NIRVANA_NODISCARD const_iterator end () const NIRVANA_NOEXCEPT
	{
		return cend ();
	}

	NIRVANA_NODISCARD const_reverse_iterator crbegin () const NIRVANA_NOEXCEPT
	{
		return const_reverse_iterator (cend ());
	}

	NIRVANA_NODISCARD const_reverse_iterator rbegin () const NIRVANA_NOEXCEPT
	{
		return const_reverse_iterator (end ());
	}

	NIRVANA_NODISCARD reverse_iterator rbegin () NIRVANA_NOEXCEPT
	{
		return reverse_iterator (end ());
	}

	NIRVANA_NODISCARD const_reverse_iterator crend () const NIRVANA_NOEXCEPT
	{
		return const_reverse_iterator (cbegin ());
	}

	NIRVANA_NODISCARD const_reverse_iterator rend () const NIRVANA_NOEXCEPT
	{
		return const_reverse_iterator (begin ());
	}

	NIRVANA_NODISCARD reverse_iterator rend () NIRVANA_NOEXCEPT
	{
		return reverse_iterator (begin ());
	}

	const_reference front () const NIRVANA_NOEXCEPT
	{
		return ABI::_ptr () [0];
	}

	reference front () NIRVANA_NOEXCEPT
	{
		return ABI::_ptr () [0];
	}

	const_reference back () const NIRVANA_NOEXCEPT
	{
		assert (length ());
		return *(ABI::_end_ptr () - 1);
	}

	reference back () NIRVANA_NOEXCEPT
	{
		assert (length ());
		return *(ABI::_end_ptr () - 1);
	}

	// MSVC specific
#ifdef _MSC_BUILD

	const_pointer _Unchecked_begin () const NIRVANA_NOEXCEPT
	{
		return ABI::_ptr ();
	}

	const_pointer _Unchecked_end () const NIRVANA_NOEXCEPT
	{
		return ABI::_end_ptr ();
	}

	pointer _Unchecked_begin () NIRVANA_NOEXCEPT
	{
		return ABI::_ptr ();
	}

	pointer _Unchecked_end () NIRVANA_NOEXCEPT
	{
		return ABI::_end_ptr ();
	}

#endif

private:
	void release_memory ()
	{
		size_t cb = ABI::allocated ();
		if (cb)
			memory ()->release (ABI::large_pointer (), cb);
	}

	static size_t byte_size (size_type char_cnt) NIRVANA_NOEXCEPT
	{
		return (char_cnt + 1) * sizeof (value_type);
	}

	static size_type char_cnt (size_t byte_size) NIRVANA_NOEXCEPT
	{
		return byte_size / sizeof (value_type) - 1;
	}

	static size_type add_size (size_type s1, size_type s2)
	{
		if (ABI::max_size () - s1 < s2)
			xlength_error ();
		return s1 + s2;
	}

	size_t get_offset (const_iterator it) const NIRVANA_NOEXCEPT
	{
		const_pointer p = it.ptr_;
		const_pointer b = data ();
		assert (b <= p && p <= (b + length ()));
		return p - b;
	}

	const_pointer get_range (size_type off, size_type& count) const;
	void get_range (size_type off, const_pointer& b, const_pointer& e) const NIRVANA_NOEXCEPT;
	void get_range_rev (size_type off, const_pointer& b, const_pointer& e) const NIRVANA_NOEXCEPT;

	static int compare (const value_type* s0, size_type len0, const value_type* s1, size_type len1)
		NIRVANA_NOEXCEPT
	{
		size_type len = len0 < len1 ? len0 : len1;
		int ret = traits_type::compare (s0, s1, len);
		if (!ret) {
			if (len0 < len1)
				ret = -1;
			else if (len0 > len1)
				ret = 1;
		}
		return ret;
	}

	//! \fn pointer assign_internal (size_type count, const value_type* s = nullptr)
	//!
	//! \brief Assign helper.
	//!
	//! \param count New number of characters.
	//! \param s     A source string. If NULL, then no copy, just commit the block.
	//!
	//! \return A pointer to begin of data.

	pointer assign_internal (size_type count, const value_type* s = nullptr)
	{
		return replace_internal (0, size (), count, s);
	}

	//! \fn pointer insert_internal (size_type pos, const value_type* s, size_type count);
	//!
	//! \brief Inserts block of characters.
	//!
	//! \param pos   The position where to insert.
	//! \param count Number of characters to insert.
	//! \param s     A source string. If NULL, then no copy, just commit the block.
	//!
	//! \return A pointer to begin of inserted block.

	pointer insert_internal (size_type pos, size_type count, const value_type* s = nullptr)
	{
		return replace_internal (pos, 0, count, s);
	}

	//! \fn pointer replace_internal (size_type pos, size_type size, const value_type* s, size_type count);
	//!
	//! \brief Replace the block of characters.
	//!
	//! \param pos   The position of the block of character.
	//! \param size  Size of the block to replace.
	//! \param count New size of the block.
	//! \param s     A source string. If NULL, then no copy, just commit the block.
	//!
	//! \return A pointer to begin of the replaced block.

	pointer replace_internal (size_type pos, size_type size, size_type count, const value_type* s = nullptr);

	template <class InputIterator>
	static void traits_copy (InputIterator b, InputIterator e, pointer dst)
	{
		for (InputIterator p = b; p != e; ++p, ++dst)
			traits_type::assign (*dst, *p);
	}

	static pointer real_copy (const_pointer src, size_type count, pointer dst)
	{
		if (&traits_type::copy == &char_traits <value_type>::copy)
			return ::Nirvana::real_copy (src, src + count, dst);
		else {
			traits_type::copy (dst, src, count);
			return dst + count;
		}
	}

	friend struct CORBA::Internal::Type <MyType>;
};

template <typename C, class T>
void basic_string <C, T, allocator <C> >::clear ()
{
	if (ABI::is_large ()) {
		pointer p = ABI::large_pointer ();
		p [0] = 0;
		size_t cc = ABI::large_size ();
		if (cc) {
			ABI::large_size (0);
			memory ()->decommit (p + 1, cc * sizeof (value_type));
		}
	} else {
		ABI::small_pointer () [0] = 0;
		ABI::small_size (0);
	}
}

template <typename C, class T>
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::erase (size_type pos, size_type count)
{
	pointer dst = const_cast <pointer> (get_range (pos, count));
	if (count) {
		if (ABI::is_large ()) {
			size_type size = ABI::large_size ();
			MemoryHelper::erase (ABI::large_pointer (), byte_size (size),
				pos * sizeof (value_type), count * sizeof (value_type));
			ABI::large_size (size - count);
		} else {
			size_type size = ABI::small_size ();
			pointer src = dst + count;
			::Nirvana::real_copy (src, ABI::small_pointer () + ABI::small_size () + 1, dst);
			ABI::small_size (size - count);
		}
	}
	return *this;
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::reserve (size_type cap)
{
	if (!cap)
		shrink_to_fit ();
	else if (cap > ABI::max_size ())
		xlength_error ();
	if (cap > ABI::capacity ()) {
		pointer p;
		size_type cc;
		size_t space = 0;
		if (ABI::is_large ()) {
			p = ABI::large_pointer ();
			cc = ABI::large_size ();
			space = ABI::allocated ();
		} else {
			p = ABI::small_pointer ();
			cc = ABI::small_size ();
		}
		ABI::large_pointer ((pointer)MemoryHelper::reserve (p, space, byte_size (cc), byte_size (cap)));
		ABI::large_size (cc);
		ABI::allocated (space);
	}
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::shrink_to_fit ()
{
	if (ABI::is_large ()) {
		size_t cc = ABI::large_size ();
		if (cc <= ABI::SMALL_CAPACITY) {
			C* p = ABI::large_pointer ();
			size_t space = ABI::allocated ();
			::Nirvana::real_copy (p, p + cc + 1, ABI::small_pointer ());
			ABI::small_size (cc);
			if (space)
				memory ()->release (p, space);
		} else {
			size_t space = ABI::allocated ();
			if (space) {
				MemoryHelper::shrink_to_fit (ABI::large_pointer (), space, byte_size (cc));
				ABI::allocated (space);
			}
		}
	}
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::pointer
basic_string <C, T, allocator <C> >::replace_internal (size_type pos, size_type size, size_type count, const value_type* s)
{
	size_type old_size = ABI::size ();
	if (add_size (pos, size) > old_size)
		xout_of_range ();
	size_type new_size;
	if (count > size)
		new_size = add_size (old_size, count - size);
	else if (count < size)
		new_size = old_size + count - size;
	else {
		pointer p = ABI::_ptr ();
		if (!count || !s || (pos == 0 && s == p))
			return p + pos;
		else
			new_size = old_size;
	}

	pointer p;
	if (!ABI::is_large ()) {
		if (new_size <= ABI::SMALL_CAPACITY) {
			p = ABI::small_pointer ();
			pointer dst = p + pos;
			pointer tail = dst + size;
			pointer end = p + old_size;
			if (tail == end) {
				if (s)
					*real_copy (s, count, dst) = 0;
				else
					p [new_size] = 0;
			} else {
				if (size != count)
					::Nirvana::real_move (tail, end + 1, dst + count);
				if (s)
					real_copy (s, count, dst);
			}
			ABI::small_size (new_size);
			return dst;
		}
		p = ABI::small_pointer ();
	} else
		p = ABI::large_pointer ();

	size_t space = ABI::allocated ();

	size_t old_bytes = size * sizeof (value_type);
	size_t new_bytes = count * sizeof (value_type);
	// If at end, copy one character more to commit space for zero terminator
	if (pos + size == old_size) {
		old_bytes += sizeof (value_type);
		new_bytes += sizeof (value_type);
	}

	p = (pointer)MemoryHelper::replace (p, space, byte_size (old_size),
		pos * sizeof (value_type), old_bytes, new_bytes, (&traits_type::copy == &char_traits <value_type>::copy) ? s : nullptr);

	if (&traits_type::copy != &char_traits <value_type>::copy && s)
		traits_type::copy (p + pos, s, count);

	p [new_size] = 0; // on append, ptr may be not zero-terminated

	ABI::large_pointer (p);
	ABI::large_size (new_size);
	ABI::allocated (space);

	return p + pos;
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::const_pointer basic_string <C, T, allocator <C> >
::get_range (size_type off, size_type& count) const
{
	const_pointer p;
	size_type l;
	if (ABI::is_large ()) {
		p = ABI::large_pointer ();
		l = ABI::large_size ();
	} else {
		p = ABI::small_pointer ();
		l = ABI::small_size ();
	}
	if (off > l)
		xout_of_range ();
	if (npos == count)
		count = l - off;
	else if (count > l - off)
		xout_of_range ();
	return p + off;
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::get_range (size_type off,
	const_pointer& b, const_pointer& e) const NIRVANA_NOEXCEPT
{
	const_pointer p;
	size_type l;
	if (ABI::is_large ()) {
		p = ABI::large_pointer ();
		l = ABI::large_size ();
	} else {
		p = ABI::small_pointer ();
		l = ABI::small_size ();
	}
	if (off > l)
		off = l;
	b = p + off;
	e = p + l;
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::get_range_rev (size_type off,
	const_pointer& b, const_pointer& e) const NIRVANA_NOEXCEPT
{
	const_pointer p;
	size_type l;
	if (ABI::is_large ()) {
		p = ABI::large_pointer ();
		l = ABI::large_size ();
	} else {
		p = ABI::small_pointer ();
		l = ABI::small_size ();
	}
	if (off > l)
		off = l;
	else
		++off;
	b = p;
	e = p + off;
}

}

#include <string>
#include <algorithm>

namespace std {

static_assert (sizeof (std::basic_string <char>) == sizeof (CORBA::Internal::ABI <CORBA::Internal::StringT <char> >),
	"sizeof (basic_string <char>) != sizeof (ABI <StringT <char>>)");
static_assert (sizeof (std::basic_string <CORBA::WChar>) == sizeof (CORBA::Internal::ABI <CORBA::Internal::StringT <CORBA::WChar> >),
	"sizeof (basic_string <CORBA::WChar>) != sizeof (ABI <StringT <CORBA::WChar>>)");
static_assert (is_nothrow_move_constructible <string> (), "!is_nothrow_move_constructible <string>");
static_assert (is_nothrow_move_assignable <string> (), "!is_nothrow_move_assignable <string>");

template <typename C, class T>
template <class InputIterator
#ifdef NIRVANA_C11
	, typename
#endif
>
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::assign (InputIterator b, InputIterator e)
{
	traits_copy (b, e, assign_internal (distance (b, e)));
	return *this;
}

template <typename C, class T>
template <class InputIterator
#ifdef NIRVANA_C11
	, typename
#endif
>
void basic_string <C, T, allocator <C> >::insert (iterator it, InputIterator b, InputIterator e)
{
	traits_copy (b, e, insert_internal (get_offset (it), nullptr, distance (b, e)));
}

template <typename C, class T>
template <class InputIterator
#ifdef NIRVANA_C11
	, typename
#endif
>
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::replace (const_iterator b, const_iterator e, InputIterator sb, InputIterator se)
{
	traits_copy (sb, se, replace_internal (b - begin (), e - b, nullptr, distance (b, e)));
	return *this;
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::find (const value_type* s, size_type pos, size_type len) const
{
	const_pointer f, e;
	get_range (pos, f, e);
	f = std::search (f, e, s, s + len, traits_type::eq);
	if (f == e)
		return npos;
	else
		return f - ABI::_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::find (const value_type c, size_type pos) const NIRVANA_NOEXCEPT
{
	const_pointer f, e;
	get_range (pos, f, e);
	f = traits_type::find (f, e - f, c);
	if (f)
		return f - ABI::_ptr ();
	else
		return npos;
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::rfind (const value_type* s, size_type pos, size_type len) const
{
	const_pointer f, e;
	get_range_rev (pos, f, e);
	f = std::find_end (f, e, s, s + len, traits_type::eq);
	if (f == e)
		return npos;
	else
		return f - ABI::_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::rfind (const value_type c, size_type pos) const NIRVANA_NOEXCEPT
{
	const_pointer b, f;
	get_range_rev (pos, b, f);
	--b; --f;
	for (; b != f; --f) {
		if (traits_type::eq (*f, c))
			break;
	}
	if (f == b)
		return npos;
	else
		return f - ABI::_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::find_first_not_of (const value_type* s, size_type pos, size_type len) const
{
	const_pointer f, e;
	get_range (pos, f, e);
	for (; f != e; ++f) {
		if (!traits_type::find (s, len, *f))
			break;
	}
	if (f == e)
		return npos;
	else
		return f - ABI::_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::find_first_of (const value_type* s, size_type pos, size_type len) const
{
	const_pointer f, e;
	get_range (pos, f, e);
	f = std::find_first_of (f, e, s, s + len, traits_type::eq);
	if (f == e)
		return npos;
	else
		return f - ABI::_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::find_last_not_of (const value_type* s, size_type pos, size_type len) const
{
	const_pointer b, f;
	get_range_rev (pos, b, f);
	--b, --f;
	for (; f != b; --f) {
		if (!traits_type::find (s, len, *f))
			break;
	}
	if (f == b)
		return npos;
	else
		return f - ABI::_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::find_last_of (const value_type* s, size_type pos, size_type len) const
{
	const_pointer b, f;
	get_range_rev (pos, b, f);
	--b, --f;
	for (; f != b; --f) {
		if (traits_type::find (s, len, *f))
			break;
	}
	if (f == b)
		return npos;
	else
		return f - ABI::_ptr ();
}

}

#ifdef NIRVANA_C11
#include <initializer_list>
#endif

#ifdef NIRVANA_C17
# include <string_view>
#endif

namespace CORBA {
namespace Internal {

template <typename C, ULong bound>
template <class A, typename>
inline StringView <C, bound>::StringView (const std::basic_string <C, std::char_traits <C>, A>& s)
{
	size_t size = s.size ();
	if (bound && size > bound)
		Nirvana::throw_BAD_PARAM ();
	ABI::large_pointer (const_cast <C*> (s.data ()));
	ABI::large_size (size);
	ABI::allocated (0);
}

}
}

#endif
