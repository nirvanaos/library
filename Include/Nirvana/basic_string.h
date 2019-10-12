#ifndef NIRVANA_BASIC_STRING_C_H_
#define NIRVANA_BASIC_STRING_C_H_

#include <CORBA/StringABI.h>
#include "StlUtils.h"
#include "real_copy.h"

namespace std {
template <class C, class T, class A> class basic_string;
template <class C> struct char_traits;
template <typename C, class T> class basic_string <C, T, allocator <C> >;
#if __cplusplus >= 201103L
template <class _Elem> class initializer_list;
#endif
#if __cplusplus >= 201703L
template <class C, class T> class basic_string_view;
#endif
}

namespace Nirvana {

class StdString :
	public StdContainer
{
public:
	NIRVANA_NORETURN static void xout_of_range ()
	{
		StdContainer::xout_of_range ("invalid string position");
	}
	NIRVANA_NORETURN static void xlength_error ()
	{
		StdContainer::xlength_error ("string too long");
	}

	static Memory_ptr heap ()
	{
		return default_heap ();
	}

	class MemoryHelper :
		public ::Nirvana::MemoryHelper
	{
	public:
		MemoryHelper () :
			::Nirvana::MemoryHelper (heap ())
		{}
	};
};

}

namespace std {

template <typename C, class T>
class basic_string <C, T, allocator <C> > :
	public CORBA::Nirvana::StringABI <C>,
	private Nirvana::StdString
{
	typedef CORBA::Nirvana::StringABI <C> ABI;
	typedef basic_string <C, char_traits <C>, allocator <C> > MyType;
public:
	using const_iterator = Nirvana::StdConstIterator <MyType>;
	using iterator = Nirvana::StdIterator <MyType>;

	typedef std::reverse_iterator <const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator <iterator> reverse_iterator;

	typedef std::allocator <C> allocator_type;
	typedef typename allocator_type::const_pointer const_pointer;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::difference_type difference_type;
	typedef typename allocator_type::pointer pointer;
	typedef typename allocator_type::reference reference;
	typedef typename allocator_type::size_type size_type;
	typedef T traits_type;
	typedef C value_type;

	static const size_type npos = -1;

	~basic_string ()
	{
		release_memory ();
	}

	// Constructors

	basic_string ()
	{
		this->reset ();
	}

	basic_string (const allocator_type&) :
		basic_string ()
	{}

	basic_string (const basic_string& src)
	{
		if (src.is_large ()) {
			ABI::reset ();
			assign (src.large_pointer (), src.large_size ());
		} else
			this->data_ = src.data_;
	}

	basic_string (basic_string&& src)
	{
		this->data_ = src.data_;
		src.reset ();
	}

	basic_string (const basic_string& src, size_type off, size_type cnt = npos)
	{
		this->reset ();
		assign (src, off, npos);
	}

	basic_string (const basic_string& src, size_type off, size_type cnt, const allocator_type&) :
		basic_string (src, off, cnt)
	{}

	basic_string (const value_type* ptr, size_type cnt)
	{
		this->reset ();
		assign (ptr, cnt);
	}

	basic_string (const value_type* ptr, size_type cnt, const allocator_type&) :
		basic_string (ptr, cnt)
	{}

	basic_string (const value_type* ptr)
	{
		this->reset ();
		assign (ptr);
	}

	basic_string (const value_type* ptr, const allocator_type&) :
		basic_string (ptr)
	{}

	basic_string (size_type cnt, value_type c)
	{
		this->reset ();
		assign (cnt, c);
	}

	basic_string (size_type cnt, value_type c, const allocator_type&) :
		basic_string (cnt, c)
	{}

	template <class InputIterator>
	basic_string (InputIterator b, InputIterator e)
	{
		this->reset ();
		assign (b, e);
	}

	template <class InputIterator>
	basic_string (InputIterator b, InputIterator e, const allocator_type&) :
		basic_string (b, e)
	{}

	basic_string (const_pointer b, const_pointer e)
	{
		this->reset ();
		assign (b, e);
	}

	basic_string (const_iterator b, const_iterator e)
	{
		this->reset ();
		assign (b, e);
	}

#if __cplusplus >= 201103L
	basic_string (initializer_list <value_type> ilist);
#endif

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

	basic_string& operator = (basic_string&& src)
	{
		release_memory ();
		this->data_ = src.data_;
		src.reset ();
		return *this;
	}

#if __cplusplus >= 201103L
	basic_string& operator = (initializer_list <value_type> ilist);
#endif

	basic_string& assign (const value_type* ptr)
	{
		return assign (ptr, traits_type::length (ptr));
	}

	basic_string& assign (const value_type* ptr, size_type count);

	basic_string& assign (const basic_string& str, size_type off, size_type count = npos)
	{
		const_pointer p = str.get_range (off, count);
		assign (p, count);
	}

	basic_string& assign (const basic_string& src)
	{
		if (!src.is_large ()) {
			if (!this->is_large ())
				this->data_ = src.data_;
			else
				assign (src.small_pointer (), src.small_size ());
		} else
			assign (src.large_pointer (), src.large_size ());
		return *this;
	}

	basic_string& assign (size_type count, value_type c)
	{
		traits_type::assign (commit (count), count, c);
		return *this;
	}

	template <class InputIterator>
	basic_string& assign (InputIterator b, InputIterator e)
	{
		traits_copy (b, e, commit (e - b));
		return *this;
	}

	basic_string& assign (const_pointer b, const_pointer e)
	{
		return assign (b, e - b);
	}

	basic_string& assign (const_iterator b, const_iterator e)
	{
		return assign (&*b, e - b);
	}

#if __cplusplus >= 201103L
	basic_string& assign (std::initializer_list <value_type> ilist);
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

	template <class InputIterator>
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
		return append (&*b, e - b);
	}

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
		insert_internal (pos, s, count);
		return *this;
	}

	basic_string& insert (size_type pos, size_type count, value_type c)
	{
		traits_type::assign (insert_internal (pos, nullptr, count) + pos, count, c);
		return *this;
	}

	void insert (iterator pos, size_type count, value_type c)
	{
		return insert (pos - begin (), count, c);
	}

	iterator insert (iterator pos, value_type c)
	{
		insert (pos, 1, c);
		return begin () + pos;
	}

	template <class InputIterator>
	void insert (iterator it, InputIterator b, InputIterator e)
	{
		size_t pos = it - begin ();
		traits_copy (b, e, insert_internal (pos, nullptr, e - b) + pos);
	}

	void insert (iterator it, const_pointer b, const_pointer e)
	{
		insert_internal (it - begin (), b, e - b);
	}

	void insert (iterator it, const_iterator b, const_iterator e)
	{
		insert_internal (it - begin (), b, e - b);
	}

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

	// find

	size_type find (const basic_string& s, size_type pos = 0) const
	{
		return find (s.data (), pos, s.length ());
	}

	size_type find (const value_type* s, size_type pos = 0) const
	{
		return find (s, pos, traits_type::length (s));
	}

	size_type find (const value_type* s, size_type pos, size_type len) const;

	size_type find (const value_type c, size_type pos = 0) const;

	size_type rfind (const basic_string& s, size_type pos = npos) const
	{
		return rfind (s.data (), pos, s.length ());
	}

	size_type rfind (const value_type* s, size_type pos = npos) const
	{
		return rfind (s, pos, traits_type::length (s));
	}

	size_type rfind (const value_type* s, size_type pos, size_type len) const;

	size_type rfind (const value_type c, size_type pos = npos) const;

	size_type find_first_not_of (const basic_string& s, size_type pos = 0) const
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
			return f - this->_ptr ();
	}

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
			return f - this->_ptr ();
	}

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

	// Misc. operations

	const_reference at (size_type off) const
	{
		if (this->is_large ()) {
			if (off >= this->large_size ())
				xout_of_range ();
			return this->large_pointer () [off];
		} else {
			if (off >= this->small_size ())
				xout_of_range ();
			return this->small_pointer () [off];
		}
	}

	reference at (size_type off)
	{
		return const_cast <reference> (const_cast <basic_string*> (this)->at (off));
	}

	const_reference operator [] (size_type off) const
	{
		return this->_ptr () [off];
	}

	reference operator [] (size_type off)
	{
		return this->_ptr () [off];
	}

	const value_type* data () const
	{
		return this->_ptr ();
	}

#if __cplusplus >= 201703L
	value_type* data ()
	{
		return this->_ptr ();
	}
#endif

	const value_type* c_str () const
	{
		return data ();
	}

	size_type length () const
	{
		return this->size ();
	}

	size_type capacity () const
	{
		if (this->is_large ()) {
			size_t cb = this->large_allocated ();
			if (cb)
				return char_cnt (cb);
			else
				return this->large_size ();
		} else
			return ABI::SMALL_CAPACITY;
	}

	void clear ();

	size_type copy (value_type* ptr, size_type count, size_type off = 0) const
	{
		const_pointer p = get_range (off, count);
		heap ()->copy (ptr, p, count * sizeof (value_type), 0);
	}

	iterator erase (iterator b, iterator e)
	{
		erase (&*b - c_str (), &*e - &*b);
		return b;
	}

	iterator erase (iterator it)
	{
		erase (&*it - c_str (), 1);
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

	void resize (size_type new_size)
	{
		resize (new_size, 0);
	}

	void resize (size_type new_size, value_type c);

	void shrink_to_fit ();

	basic_string substr (size_type pos = 0, size_type len = npos) const
	{
		const_pointer p = get_range (pos, len);
		return basic_string (p, len);
	}

	void swap (basic_string& rhs)
	{
		typename ABI::Data tmp = this->data_;
		this->data_ = rhs.data_;
		rhs.data_ = tmp;
	}

	// Iterators

	NIRVANA_NODISCARD const_iterator cbegin () const
	{
		return const_iterator (this->_ptr (), *this);
	}

	NIRVANA_NODISCARD iterator begin ()
	{
		return iterator (this->_ptr (), *this);
	}

	NIRVANA_NODISCARD const_iterator begin () const
	{
		return cbegin ();
	}

	NIRVANA_NODISCARD const_iterator cend () const
	{
		return const_iterator (this->_ptr () + this->size (), *this);
	}

	NIRVANA_NODISCARD iterator end ()
	{
		return iterator (this->_ptr () + this->size (), *this);
	}

	NIRVANA_NODISCARD const_iterator end () const
	{
		return cend ();
	}

	NIRVANA_NODISCARD const_reverse_iterator crbegin () const
	{
		return const_reverse_iterator (cend ());
	}

	NIRVANA_NODISCARD const_reverse_iterator rbegin () const
	{
		return const_reverse_iterator (end ());
	}

	NIRVANA_NODISCARD reverse_iterator rbegin ()
	{
		return reverse_iterator (end ());
	}

	NIRVANA_NODISCARD const_reverse_iterator crend () const
	{
		return const_reverse_iterator (cbegin ());
	}

	NIRVANA_NODISCARD const_reverse_iterator rend () const
	{
		return const_reverse_iterator (begin ());
	}

	NIRVANA_NODISCARD reverse_iterator rend ()
	{
		return reverse_iterator (begin ());
	}

	const_reference front () const
	{
		return this->_ptr () [0];
	}

	reference front ()
	{
		return this->_ptr () [0];
	}

	const_reference back () const
	{
		assert (length ());
		return this->_ptr () [length () - 1];
	}

	reference back ()
	{
		assert (length ());
		return this->_ptr () [length () - 1];
	}

	NIRVANA_NODISCARD allocator_type get_allocator () const
	{
		return allocator_type ();
	}

	static basic_string& unmarshal (ABI* abi);
	static const basic_string& unmarshal (const ABI* abi);

private:
	void release_memory ()
	{
		if (this->is_large ()) {
			size_t cb = this->large_allocated ();
			if (cb)
				heap ()->release (this->large_pointer (), cb);
		}
	}

	pointer commit (size_type size);

	static size_t byte_size (size_type char_cnt)
	{
		return (char_cnt + 1) * sizeof (value_type);
	}

	static size_type char_cnt (size_t byte_size)
	{
		return byte_size / sizeof (value_type) - 1;
	}

	static size_type add_size (size_type s1, size_type s2)
	{
		if (ABI::max_size () - s1 < s2)
			xlength_error ();
		return s1 + s2;
	}

	const_pointer get_range (size_type off, size_type& count) const;
	void get_range (size_type off, const_pointer& b, const_pointer& e) const;
	void get_range_rev (size_type off, const_pointer& b, const_pointer& e) const;

	static int compare (const value_type* s0, size_type len0, const value_type* s1, size_type len1)
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

	pointer insert_internal (size_type pos, const value_type* s, size_type count);

	template <class InputIterator>
	static void traits_copy (InputIterator b, InputIterator e, pointer dst)
	{
		for (InputIterator p = b; p != e; ++p, ++dst)
			traits_type::assign (*dst, *p);
	}

	static pointer real_copy (const_pointer src, size_type count, pointer dst)
	{
		if (traits_type::copy == char_traits <value_type>::copy)
			return ::Nirvana::real_copy (src, src + count, dst);
		else {
			traits_type::copy (dst, src, count);
			return dst + count;
		}
	}
};

template <typename C, class T>
void basic_string <C, T, allocator <C> >::clear ()
{
	if (this->is_large ()) {
		pointer p = this->large_pointer ();
		p [0] = 0;
		size_t cc = this->large_size ();
		if (cc) {
			this->large_size (0);
			heap ()->decommit (p + 1, cc * sizeof (value_type));
		}
	} else {
		this->small_pointer () [0] = 0;
		this->small_size (0);
	}
}

template <typename C, class T>
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::assign (const value_type* ptr, size_type count)
{
	if (count <= ABI::SMALL_CAPACITY && !this->is_large ()) {
		*real_copy (ptr, count, this->small_pointer ()) = 0;
		this->small_size (count);
	} else if (count > ABI::max_size ())
		xlength_error ();
	else {
		pointer p;
		size_t space;
		size_t size;
		if (!this->is_large ()) {
			p = nullptr;
			space = 0;
			size = 0;
		} else {
			p = this->large_pointer ();
			space = this->large_allocated ();
			size = byte_size (this->large_size ());
		}
		if (traits_type::copy == char_traits <value_type>::copy)
			p = (pointer)MemoryHelper ().assign (p, space, size, ptr, byte_size (count));
		else
			traits_type::copy (p = commit (count), ptr, count);
		p [count] = 0;
		this->large_pointer (p);
		this->large_size (count);
		this->large_allocated (space);
	}
	return *this;
}

template <typename C, class T>
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::erase (size_type pos, size_type count)
{
	const_pointer p = get_range (pos, count);
	if (count) {
		if (this->is_large ()) {
			size_t size = this->large_size ();
			MemoryHelper ().erase (this->large_pointer (), byte_size (size),
				pos * sizeof (value_type), count * sizeof (value_type));
			this->large_size (size - count);
		} else {
			pointer dst = this->small_pointer () + pos;
			pointer src = dst + count;
			::Nirvana::real_copy (src, this->small_pointer () + this->small_size () + 1, dst);
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
	if (this->is_large ()) {
		size_t space = byte_size (cap);
		size_t cur_space = this->large_allocated ();
		if (cur_space < space) {
			this->large_pointer ((pointer)MemoryHelper ().reserve (this->large_pointer (), byte_size (this->large_size ()), cur_space, space));
			this->large_allocated (space);
		}
	} else if (cap > ABI::SMALL_CAPACITY) {
		size_t space = byte_size (cap);
		size_t cc = this->small_size ();
		MemoryHelper mh;
		pointer p = (pointer)mh.assign (mh.reserve (space), space, 0, this->small_pointer (), byte_size (cc));
		this->large_pointer (p);
		this->large_size (cc);
		this->large_allocated (space);
	}
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::resize (size_type new_size, value_type c)
{
	size_t size = this->size ();
	if (new_size > size)
		append (new_size - size, c);
	else
		erase (new_size, size - new_size);
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::shrink_to_fit ()
{
	if (this->is_large ()) {
		size_t cc = this->large_size ();
		if (cc <= ABI::SMALL_CAPACITY) {
			C* p = this->large_pointer ();
			size_t space = this->large_allocated ();
			::Nirvana::real_copy (p, p + cc + 1, this->small_pointer ());
			this->small_size (cc);
			heap ()->release (p, space);
		} else {
			size_t space = this->large_allocated ();
			MemoryHelper ().shrink_to_fit (this->large_pointer (), space, byte_size (cc));
			this->large_allocated (space);
		}
	}
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::pointer
basic_string <C, T, allocator <C> >::insert_internal (size_type pos, const value_type* ptr, size_type count)
{
	size_type old_size = this->size ();
	if (pos > old_size)
		xout_of_range ();
	size_type new_size = add_size (old_size, count);
	if (!this->is_large ()) {
		if (new_size <= ABI::SMALL_CAPACITY) {
			pointer p = this->small_pointer ();
			if (pos == old_size) {
				if (ptr)
					*real_copy (ptr, count, p + pos) = 0;
				else
					p [new_size] = 0;
			} else {
				pointer dst = p + pos;
				::Nirvana::real_move (dst, p + old_size - pos + 1, dst + count);
				if (ptr)
					real_copy (ptr, count, dst);
			}
			this->small_size (new_size);
			return p;
		} else
			reserve (new_size);
	}
	size_t space = this->large_allocated ();
	size_t ins_bytes = count * sizeof (value_type);
	// On append, copy one character more
	if (pos == old_size)
		ins_bytes += sizeof (value_type);
	pointer p = (pointer)MemoryHelper ().insert (this->large_pointer (), space,
		old_size * sizeof (value_type), pos * sizeof (value_type),
		(traits_type::copy == char_traits <value_type>::copy) ? ptr : nullptr, ins_bytes);
	if (traits_type::copy != char_traits <value_type>::copy)
		traits_type::copy (p + pos, ptr, count);
	p [new_size] = 0; // on append, ptr may be not zero-terminated
	this->large_pointer (p);
	this->large_size (new_size);
	this->large_allocated (space);
	return p;
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::pointer basic_string <C, T, allocator <C> >::commit (size_type size)
{
	if (!this->is_large () && ABI::SMALL_CAPACITY >= size) {
		this->small_size (size);
		this->small_pointer () [size] = 0;
		return this->small_pointer ();
	} else {
		size_t space = this->large_allocated ();
		pointer p = (pointer)MemoryHelper ().commit (this->large_pointer (), space,
			byte_size (this->large_size ()), byte_size (size));
		p [size] = 0;
		this->large_pointer (p);
		this->large_size (size);
		this->large_allocated (space);
		return p;
	}
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::const_pointer basic_string <C, T, allocator <C> >
::get_range (size_type off, size_type& count) const
{
	const_pointer p;
	size_type l;
	if (this->is_large ()) {
		p = this->large_pointer ();
		l = this->large_size ();
	} else {
		p = this->small_pointer ();
		l = this->small_size ();
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
void basic_string <C, T, allocator <C> >::get_range (size_type off, const_pointer& b, const_pointer& e) const
{
	const_pointer p;
	size_type l;
	if (this->is_large ()) {
		p = this->large_pointer ();
		l = this->large_size ();
	} else {
		p = this->small_pointer ();
		l = this->small_size ();
	}
	if (off > l)
		off = l;
	b = p + off;
	e = p + l;
}

template <typename C, class T>
void basic_string <C, T, allocator <C> >::get_range_rev (size_type off, const_pointer& b, const_pointer& e) const
{
	const_pointer p;
	size_type l;
	if (this->is_large ()) {
		p = this->large_pointer ();
		l = this->large_size ();
	} else {
		p = this->small_pointer ();
		l = this->small_size ();
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
		return f - this->_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::find (const value_type c, size_type pos) const
{
	const_pointer f, e;
	get_range (pos, f, e);
	f = traits_type::find (f, e - f, c);
	if (f)
		return f - this->_ptr ();
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
		return f - this->_ptr ();
}

template <typename C, class T>
typename basic_string <C, T, allocator <C> >::size_type basic_string <C, T, allocator <C> >
::rfind (const value_type c, size_type pos) const
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
		return f - this->_ptr ();
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
		return f - this->_ptr ();
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
		return f - this->_ptr ();
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
		return f - this->_ptr ();
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
		return f - this->_ptr ();
}

}

#if __cplusplus >= 201103L

#include <initializer_list>

namespace std {

template <typename C, class T> inline
basic_string <C, T, allocator <C> >::basic_string (initializer_list <value_type> ilist)
{
	this->reset ();
	assign (ilist.begin (), ilist.end ());
}

template <typename C, class T> inline
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::operator = (std::initializer_list <value_type> ilist)
{
	return assign (ilist.begin (), ilist.end ());
}

template <typename C, class T> inline
basic_string <C, T, allocator <C> >& basic_string <C, T, allocator <C> >::assign (std::initializer_list <value_type> ilist)
{
	return assign (ilist.begin (), ilist.end ());
}

}

#endif

#if __cplusplus >= 201703L
# include <string_view>
// TODO: implement basic_string_view operations
#endif

#endif
