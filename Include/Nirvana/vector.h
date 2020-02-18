#ifndef NIRVANA_VECTOR_H_
#define NIRVANA_VECTOR_H_

#include "StlUtils.h"
#include <CORBA/SequenceABI.h>
#include <CORBA/exceptions.h>
#include <CORBA/Type_forward.h>
#include <vector>
#include <iterator>
#ifdef NIRVANA_C11
#include <initializer_list>
#endif

namespace std {
template <class T, class A> class vector;
}

namespace Nirvana {

class StdVector :
	public StdContainer
{
public:
	NIRVANA_NORETURN static void xout_of_range ()
	{
		StdContainer::xout_of_range ("invalid vector position");
	}
	NIRVANA_NORETURN static void xlength_error ()
	{
		StdContainer::xlength_error ("vector too long");
	}
};

}

namespace std {

template <class T>
class vector <T, allocator <T> > :
	public ::Nirvana::StdVector,
	private CORBA::Nirvana::SequenceABI <T>
{
	typedef CORBA::Nirvana::SequenceABI <T> ABI;
	typedef vector <T, allocator <T> > MyType;

public:
	using const_iterator = ::Nirvana::StdConstIterator <MyType>;
	using iterator = ::Nirvana::StdIterator <MyType>;

	typedef std::reverse_iterator <const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator <iterator> reverse_iterator;

	typedef T value_type;
	typedef allocator <T> allocator_type;

	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef value_type& reference;

	typedef typename allocator_traits <allocator_type>::difference_type difference_type;
	typedef typename allocator_traits <allocator_type>::size_type size_type;

	// Destructor
	~vector ()
	{
		clear ();
		release_memory ();
	}

	// Constructors
	vector () NIRVANA_NOEXCEPT
	{
		ABI::reset ();
	}

	explicit vector (const allocator_type&) NIRVANA_NOEXCEPT :
		vector ()
	{}

	explicit vector (size_type count)
	{
		if (count) {
			if (count > max_size ())
				xlength_error ();
			ABI::allocated = 0;
			pointer p = ABI::ptr = (pointer)MemoryHelper::assign (nullptr, ABI::allocated, 0, count * sizeof (value_type));
			if (is_nothrow_default_constructible <value_type> ())
				construct (p, p + count);
			else {
				try {
					construct (p, p + count);
				} catch (...) {
					memory ()->release (p, ABI::allocated);
					throw;
				}
			}
			ABI::size = count;
		} else
			ABI::reset ();
	}

#ifndef NIRVANA_C11
	explicit
#endif
	vector (size_type count, const value_type& v)
	{
		if (count) {
			if (count > max_size ())
				xlength_error ();
			ABI::allocated = 0;
			pointer p = (pointer)MemoryHelper::assign (nullptr, ABI::allocated, 0, count * sizeof (value_type));
			if (is_nothrow_copy_constructible <value_type> ())
				construct (p, p + count, v);
			else {
				try {
					construct (p, p + count, v);
				} catch (...) {
					memory ()->release (p, ABI::allocated);
					throw;
				}
			}
			ABI::ptr = p;
			ABI::size = count;
		} else
			ABI::reset ();
	}

#ifndef NIRVANA_C11
	explicit
#endif
	vector (size_type count, const value_type& v, const allocator_type&) :
		vector (count, v)
	{}

	vector (const vector& src)
	{
		copy_constructor (src);
	}

	vector (vector&& src) NIRVANA_NOEXCEPT
	{
		static_cast <ABI&> (*this) = src;
		src.reset ();
	}

	vector (vector&& src, const allocator_type&) NIRVANA_NOEXCEPT :
		vector (std::move (src))
	{}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	vector (InputIterator b, InputIterator e)
	{
		construct_it (b, e);
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	vector (InputIterator b, InputIterator e, const allocator_type&)
	{
		construct_it (b, e);
	}

#ifdef NIRVANA_C11
	vector (initializer_list <value_type> ilist)
	{
		construct_it (ilist.begin (), ilist.end ());
	}
#endif

	// Assignments

	vector& operator = (const vector& src)
	{
		copy (src);
		return *this;
	}

	vector& operator = (vector&& src) NIRVANA_NOEXCEPT
	{
		if (this != &src) {
			destruct (data (), data () + size ());
			release_memory ();
			static_cast <ABI&> (*this) = src;
			src.reset ();
		}
		return *this;
	}

	void assign (size_type count, const value_type& val)
	{
		clear ();
		pointer p = ABI::ptr;
		if (count > capacity ())
			ABI::ptr = p = (pointer)MemoryHelper::assign (p, ABI::allocated, 0, count * sizeof (value_type));
		construct (p, p + count, val);
		ABI::size = count;
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	void assign (InputIterator b, InputIterator e)
	{
		assign_it (b, e);
	}

#ifdef NIRVANA_C11
	void assign (initializer_list <value_type> ilist)
	{
		assign_it (ilist.begin (), ilist.end ());
	}
#endif

	// erase

	iterator erase (const_iterator pos)
	{
		pointer p = get_ptr (pos);
		if (p < data () + size ()) {
			erase_internal (p, p + 1);
			return iterator (p, *this);
		} else
			return end ();
	}

	iterator erase (const_iterator b, const_iterator e)
	{
		pointer pb = get_ptr (b), pe = get_ptr (e);
		if (pb < pe && data () <= pb && pe <= (data () + size ())) {
			erase_internal (pb, pe);
			return iterator (pb, *this);
		} else
			return end ();
	}

	// insert

	iterator insert (const_iterator pos, const value_type& val)
	{
		return iterator (insert_one (get_ptr (pos), val), *this);
	}

	iterator insert (const_iterator pos, value_type&& val)
	{
		return iterator (insert_one (get_ptr (pos), std::move (val)), *this);
	}

	iterator insert (const_iterator pos, size_type count, const value_type& val);

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	iterator insert (const_iterator pos, InputIterator b, InputIterator e)
	{
		return insert_it (pos, b, e);
	}

#ifdef NIRVANA_C11
	iterator insert (const_iterator pos, initializer_list <value_type> ilist)
	{
		return insert_it (pos, ilist.begin (), ilist.end ());
	}

	template <class ... Args>
	iterator emplace (const_iterator pos, Args&&... args)
	{
		return iterator (emplace_internal (get_ptr (pos), std::forward <Args> (args)...), *this);
	}
#endif

	// Misc. operations

	reference at (size_type pos)
	{
		if (pos >= size ())
			xout_of_range ();
		return data () [pos];
	}

	const_reference at (size_type pos) const
	{
		if (pos >= size ())
			xout_of_range ();
		return data () [pos];
	}

	reference operator [] (size_type pos)
	{
		return data () [pos];
	}

	const_reference operator [] (size_type pos) const
	{
		return data () [pos];
	}

	size_type capacity () const
	{
		size_t space = ABI::allocated;
		if (space)
			return space / sizeof (value_type);
		else
			return size ();
	}

	void clear ()
	{
		pointer p = ABI::ptr;
		destruct (p, p + ABI::size);
		ABI::size = 0;
	}

	static size_type max_size ()
	{
		return SIZE_MAX / sizeof (value_type);
	}

	void push_back (const value_type& v)
	{
		insert_one (data () + size (), v);
	}

	void push_back (value_type&& v)
	{
		insert_one (data () + size (), std::move (v));
	}

#ifdef NIRVANA_C11
	template <class ... Args>
	void emplace_back (Args&&... args)
	{
		emplace_internal (data () + size (), std::forward <Args> (args)...);
	}
#endif

	void pop_back ()
	{
		size_type s = size ();
		if (s) {
			pointer p = data () + s;
			erase_internal (p - 1, p);
		}
	}

	void reserve (size_type count);

	const_pointer data () const
	{
		return ABI::ptr;
	}

	pointer data ()
	{
		return ABI::ptr;
	}

	size_type size () const
	{
		return ABI::size;
	}

	bool empty () const
	{
		return !ABI::size;
	}

	void swap (vector& rhs)
	{
		ABI tmp = *this;
		ABI::operator = (rhs);
		static_cast <ABI&> (rhs) = tmp;
	}

	void shrink_to_fit ()
	{
		if (empty ()) {
			release_memory ();
			ABI::reset ();
		} else
			Nirvana::MemoryHelper::shrink_to_fit (ABI::ptr, ABI::allocated, size () * sizeof (value_type));
	}

	NIRVANA_NODISCARD allocator_type get_allocator () const
	{
		return allocator_type ();
	}

	// Iterators

	NIRVANA_NODISCARD const_iterator cbegin () const
	{
		return const_iterator (ABI::ptr, *this);
	}

	NIRVANA_NODISCARD iterator begin ()
	{
		return iterator (ABI::ptr, *this);
	}

	NIRVANA_NODISCARD const_iterator begin () const
	{
		return cbegin ();
	}

	NIRVANA_NODISCARD const_iterator cend () const
	{
		return const_iterator (ABI::ptr + ABI::size, *this);
	}

	NIRVANA_NODISCARD iterator end ()
	{
		return iterator (ABI::ptr + ABI::size, *this);
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
		return ABI::ptr [0];
	}

	reference front ()
	{
		return ABI::ptr [0];
	}

	const_reference back () const
	{
		assert (size ());
		return ABI::ptr [size () - 1];
	}

	reference back ()
	{
		assert (size ());
		return ABI::ptr [size () - 1];
	}

	// MSVC specific
	const_pointer _Unchecked_begin () const
	{
		return data ();
	}

	const_pointer _Unchecked_end () const
	{
		return data () + size ();
	}

	// Implementation
protected:
	template <class InputIterator>
	void construct_it (InputIterator b, InputIterator e);

	template <class InputIterator>
	void assign_it (InputIterator b, InputIterator e);

	template <class InputIterator>
	iterator insert_it (const_iterator pos, InputIterator b, InputIterator e);

private:
	void release_memory ()
	{
		size_t cb = ABI::allocated;
		if (cb)
			memory ()->release (ABI::ptr, cb);
	}

	pointer get_ptr (const_iterator it) const
	{
		const_pointer p = it.ptr_;
		assert (data () <= p && p <= (data () + size ()));
		return const_cast <pointer> (p);
	}

	void erase_internal (pointer pb, pointer pe);

	void copy_constructor (const vector& src);

	void construct (pointer b, pointer e)
	{
		if (is_nothrow_default_constructible <value_type> ()) {
			for (; b < e; ++b) {
				new (b) value_type ();
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new (p) value_type ();
				}
			} catch (...) {
				while (p > b) {
					(--p)->~value_type ();
				}
				throw;
			}
		}
	}

	void construct (pointer b, pointer e, const value_type& v)
	{
		if (is_nothrow_copy_constructible <value_type> ()) {
			for (; b < e; ++b) {
				new (b)value_type (v);
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new (p)value_type (v);
				}
			} catch (...) {
				while (p > b) {
					(--p)->~value_type ();
				}
				throw;
			}
		}
	}

	template <class InputIterator>
	void construct (pointer b, pointer e, InputIterator src)
	{
		if (is_nothrow_copy_constructible <value_type> ()) {
			for (; b < e; ++b) {
				new (b)value_type (*(src++));
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new (p)value_type (*(src++));
				}
			} catch (...) {
				while (p > b) {
					(--p)->~value_type ();
				}
				throw;
			}
		}
	}

	void construct_move (pointer b, pointer e, pointer src)
	{
		if (is_nothrow_move_constructible <value_type> () || (!is_move_constructible <value_type> () && is_nothrow_copy_constructible <value_type> ())) {
			for (; b < e; ++b) {
				new (b)value_type (std::move (*(src++)));
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new (p)value_type (std::move (*(src++)));
				}
			} catch (...) {
				while (p > b) {
					(--p)->~value_type ();
				}
				throw;
			}
		}
	}

	void destruct (pointer b, pointer e)
	{
		if (is_destructible <value_type> ()) {
			for (; b < e; ++b) {
				b->~value_type ();
			}
		}
	}

	void copy (const vector& src)
	{
		if (this != &src) {
			clear ();
			copy_to_empty (src);
		}
	}

	void copy_to_empty (const vector& src);

	pointer insert_one (pointer p, const value_type& val)
	{
		insert_internal (p, 1);
		if (is_nothrow_copy_constructible <value_type> ())
			new (p) value_type (val);
		else {
			try {
				new (p) value_type (val);
			} catch (...) {
				close_hole (p, 1);
				throw;
			}
		}
		return p;
	}

	pointer insert_one (pointer p, value_type&& val)
	{
		insert_internal (p, 1);
		if (is_nothrow_move_constructible <value_type> ())
			new (p) value_type (std::move (val));
		else {
			try {
				new (p) value_type (std::move (val));
			} catch (...) {
				close_hole (p, 1);
				throw;
			}
		}
		return p;
	}

#ifdef NIRVANA_C11
	template <class ... Args>
	pointer emplace_internal (pointer p, Args&&... args)
	{
		insert_internal (p, 1);
		if (is_nothrow_constructible <value_type, Args...> ())
			new (p) value_type (std::forward <Args> (args)...);
		else {
			try {
				new (p) value_type (std::forward <Args> (args)...);
			} catch (...) {
				close_hole (p, 1);
				throw;
			}
		}
		return p;
	}
#endif

	void insert_internal (pointer& pos, size_type count);
	void close_hole (pointer pos, size_type count);

	friend struct CORBA::Nirvana::Type <MyType>;
	friend struct CORBA::Nirvana::MarshalTraits <MyType>;
};

template <class T>
template <class InputIterator>
void vector <T, allocator <T> >::construct_it (InputIterator b, InputIterator e)
{
	size_t count = distance (b, e);
	if (count) {
		ABI::allocated = 0;
		pointer p = (pointer)MemoryHelper::assign (nullptr, ABI::allocated, 0, count * sizeof (value_type));
		ABI::ptr = p;
		if (is_nothrow_copy_constructible <value_type> ())
			construct (p, p + count, b);
		else {
			try {
				construct (p, p + count, b);
			} catch (...) {
				memory ()->release (p, ABI::allocated);
				throw;
			}
		}
		ABI::size = count;
	} else
		ABI::reset ();
}

template <class T>
template <class InputIterator>
void vector <T, allocator <T> >::assign_it (InputIterator b, InputIterator e)
{
	pointer p = ABI::ptr;
	destruct (p, p + ABI::size);
	ABI::size = 0;
	size_type count = distance (b, e);
	if (capacity () < count)
		ABI::ptr = p = (pointer)MemoryHelper::assign (p, ABI::allocated, 0, count * sizeof (T));
	construct (p, p + count, b);
	ABI::size = count;
}

template <class T>
typename vector <T, allocator <T> >::iterator
vector <T, allocator <T> >::insert (const_iterator pos, size_type count, const value_type& val)
{
	pointer p = get_ptr (pos);
	if (count) {
		insert_internal (p, count);
		if (is_nothrow_copy_constructible <value_type> ())
			construct (p, p + count, val);
		else {
			try {
				construct (p, p + count, val);
			} catch (...) {
				close_hole (p, count);
				throw;
			}
		}
	}
	return iterator (p, *this);
}

template <class T>
template <class InputIterator>
typename vector <T, allocator <T> >::iterator
vector <T, allocator <T> >::insert_it (const_iterator pos, InputIterator b, InputIterator e)
{
	pointer p = get_ptr (pos);
	size_type count = distance (b, e);
	if (count) {
		insert_internal (p, count);
		if (is_nothrow_constructible <value_type, typename InputIterator::value_type> ())
			construct (p, p + count, b);
		else {
			try {
				construct (p, p + count, b);
			} catch (...) {
				close_hole (p, count);
				throw;
			}
		}
	}
	return iterator (p, *this);
}

template <class T>
void vector <T, allocator <T> >::copy_constructor (const vector& src)
{
	ABI::reset ();
	if (is_trivially_copyable <value_type> () || is_nothrow_copy_constructible <value_type> ())
		copy_to_empty (src);
	else {
		try {
			copy_to_empty (src);
		} catch (...) {
			memory ()->release (ABI::ptr, ABI::allocated);
			throw;
		}
	}
}

template <class T>
void vector <T, allocator <T> >::copy_to_empty (const vector& src)
{
	size_type count = src.size ();
	if (count) {
		if (is_trivially_copyable <value_type> ())
			ABI::ptr = (pointer)MemoryHelper::assign (ABI::ptr, ABI::allocated, 0, count * sizeof (value_type), src.ptr);
		else {
			pointer p = ABI::ptr;
			if (count > capacity ())
				ABI::ptr = p = (pointer)MemoryHelper::assign (p, ABI::allocated, 0, count * sizeof (value_type));
			construct (p, p + count, src.ptr);
		}
		ABI::size = count;
	}
}

template <class T>
void vector <T, allocator <T> >::erase_internal (pointer pb, pointer pe)
{
	pointer p = ABI::ptr;
	size_type cnt = pe - pb;
	if (is_trivially_copyable <value_type> ())
		MemoryHelper::erase (p, size () * sizeof (value_type), (pb - p) * sizeof (value_type), cnt * sizeof (value_type));
	else {
		pointer end = p + size ();
		if (pe < end) {
			std::move (pe, end, pb);
			pb = end - (pe - pb);
		}
		destruct (pb, end);
		memory ()->decommit (pb, (end - pb) * sizeof (value_type));
	}
	ABI::size -= cnt;
}

template <class T>
void vector <T, allocator <T> >::reserve (size_type count)
{
	if (count > capacity ()) {
		size_t new_space = count * sizeof (value_type);
		size_type size = ABI::size;
		if (is_trivially_copyable <value_type> () || !size)
			ABI::ptr = (pointer)MemoryHelper::reserve (ABI::ptr, ABI::allocated, size * sizeof (value_type), new_space);
		else {
			size_t space = ABI::allocated;
			size_t add = new_space - space;
			if (!MemoryHelper::expand ((uint8_t*)(ABI::ptr) + space, add, ::Nirvana::Memory::RESERVED)) {
				try {
					pointer new_ptr = (pointer)memory ()->allocate (nullptr, new_space, ::Nirvana::Memory::RESERVED);
					size_t au = memory ()->query (new_ptr, ::Nirvana::MemQuery::ALLOCATION_UNIT);
					new_space = ::Nirvana::round_up (new_space, au);
					pointer old_ptr = ABI::ptr;
					try {
						memory ()->commit (new_ptr, size * sizeof (value_type));
						construct_move (new_ptr, new_ptr + size, old_ptr);
					} catch (...) {
						memory ()->release (new_ptr, new_space);
						throw;
					}
					destruct (old_ptr, old_ptr + size);
					ABI::ptr = new_ptr;
					memory ()->release (old_ptr, space);
				} catch (const CORBA::NO_MEMORY&) {
					throw std::bad_alloc ();
				}
			}
			ABI::allocated = new_space;
		}
	}
}

template <class T>
void vector <T, allocator <T> >::insert_internal (pointer& pos, size_type count)
{
	pointer ptr = ABI::ptr;
	size_type size = ABI::size;
	size_type new_size = size + count;
	if (new_size > max_size ())
		xlength_error ();

	if (pos < ptr)
		xout_of_range ();

	if (is_trivially_copyable <value_type> () || !size) {
		pointer new_ptr = ABI::ptr = (pointer)MemoryHelper::insert (ptr, ABI::allocated,
			size * sizeof (value_type), (pos - ptr) * sizeof (value_type), count * sizeof (value_type), nullptr);
		pos = new_ptr + (pos - ptr);
		ABI::size += count;
	} else if (pos == (ptr + size)) {
		reserve (new_size);
		pos = ABI::ptr + size;
		memory ()->commit (pos, count * sizeof (value_type));
		ABI::size += count;
	} else {
		if (new_size > capacity ()) {
			size_t new_space = new_size * sizeof (value_type);
			size_t space = ABI::allocated;
			size_t add = new_space - space;
			if (MemoryHelper::expand ((uint8_t*)ptr + space, add, 0))
				ABI::allocated = new_space;
			else {
				try {
					pointer new_ptr = (pointer)memory ()->allocate (nullptr, new_space, 0);
					size_t au = memory ()->query (new_ptr, ::Nirvana::MemQuery::ALLOCATION_UNIT);
					new_space = ::Nirvana::round_up (new_space, au);
					if (is_nothrow_move_constructible <value_type> ()) {
						pointer head_end = new_ptr + (ptr - pos);
						construct_move (new_ptr, head_end, ptr);
					} else {
						try {
							pointer head_end = new_ptr + (ptr - pos);
							construct_move (new_ptr, head_end, ptr);
							pointer tail = head_end + count;
							try {
								construct_move (tail, new_ptr + new_size, pos);
							} catch (...) {
								destruct (new_ptr, head_end);
								throw;
							}
						} catch (...) {
							memory ()->release (new_ptr, new_space);
							throw;
						}
					}
					destruct (ptr, ptr + size);
					ABI::ptr = new_ptr;
					ABI::size = new_size;
					memory ()->release (ptr, space);
					ABI::allocated = new_space;
					pos = new_ptr + (pos - ptr);
					return;
				} catch (const CORBA::NO_MEMORY&) {
					throw std::bad_alloc ();
				}
			}
		}

		// In-place insert
		pointer end = ptr + size;
		memory ()->commit (end, count * sizeof (value_type));
		pointer new_end = end + count;
		size_type move_count = end - pos;
		if (move_count < count) {
			construct_move (new_end - move_count, new_end, end - move_count);
		} else {
			construct_move (end, new_end, end - count);
			move_backward (pos, end - count, new_end);
		}
		destruct (pos, end);
		ABI::size = new_size;
	}
}

template <class T>
void vector <T, allocator <T> >::close_hole (pointer pos, size_type count)
{
	pointer ptr = ABI::ptr;
	size_type size = ABI::size;
	if (pos == ptr + size)
		ABI::size -= count;
	else if (is_trivially_copyable <value_type> ()) {
		MemoryHelper::erase (ptr, size * sizeof (value_type), (pos - ptr) * sizeof (value_type), count * sizeof (value_type));
		ABI::size -= count;
	} else {
		pointer end = ptr + size;
		pointer src = pos + count;
		if (is_nothrow_move_constructible <value_type> () || (!is_move_constructible <value_type> () && is_nothrow_copy_constructible <value_type> ())) {
			for (; src != end; ++pos, ++src) {
				new (pos) value_type (std::move (*src));
				src->~value_type ();
			}
		} else {
			try {
				for (; src != end; ++pos, ++src) {
					new (pos) value_type (std::move (*src));
					src->~value_type ();
				}
			} catch (...) {
				destruct (src, end);
			}
		}
		ABI::size = pos - ptr;
		memory ()->decommit (pos, count * sizeof (value_type));
	}
}

// vector <bool>

template <>
class vector <bool, allocator <bool> > :
	public vector <CORBA::Boolean, allocator <CORBA::Boolean> >
{
	typedef vector <CORBA::Boolean, allocator <CORBA::Boolean> > BaseVector;
	typedef CORBA::Boolean BooleanType;
public:
	typedef bool value_type;
	typedef allocator <bool> allocator_type;
	typedef const bool* const_pointer;
	typedef BaseVector::pointer pointer;
	typedef bool const_reference;

	class iterator;

	class reference
	{
		friend class vector <bool, allocator <bool> >;
		friend class iterator;

	public:
		operator bool () const
		{
			return ref_ != 0;
		}

		operator bool ()
		{
			return ref_ != 0;
		}

		reference& operator = (bool v)
		{
			ref_ = v;
			return *this;
		}

		reference& operator = (const reference& src)
		{
			ref_ = src.ref_;
			return *this;
		}

		void flip ()
		{
			ref_ = !ref_;
		}

		pointer operator & ()
		{
			return &ref_;
		}

	private:
		reference (BooleanType& ref) :
			ref_ (ref)
		{}

		BooleanType& ref_;
	};

	class const_iterator : public BaseVector::const_iterator
	{
	public:
		typedef BaseVector::const_iterator::iterator_category iterator_category;
		typedef BaseVector::const_iterator::difference_type difference_type;
		typedef bool value_type;
		typedef bool reference;
		typedef BaseVector::const_iterator::pointer pointer;

		const_iterator ()
		{}

		const_iterator (const BaseVector::const_iterator& base) :
			BaseVector::const_iterator (base)
		{}

		NIRVANA_NODISCARD reference operator * () const
		{
			return BaseVector::const_iterator::operator* () != 0;
		}

		NIRVANA_NODISCARD reference operator [] (difference_type off) const
		{	// subscript
			return BaseVector::const_iterator::operator [] (off);
		}

		const_iterator& operator ++ ()
		{
			BaseVector::const_iterator::operator ++ ();
			return *this;
		}

		const_iterator operator ++ (int)
		{
			const_iterator tmp = *this;
			operator ++ ();
			return tmp;
		}

		const_iterator& operator -- ()
		{
			BaseVector::const_iterator::operator -- ();
			return *this;
		}

		const_iterator operator -- (int)
		{
			const_iterator tmp = *this;
			operator -- ();
			return tmp;
		}

		const_iterator& operator += (difference_type off)
		{
			BaseVector::const_iterator::operator += (off);
			return *this;
		}

		NIRVANA_NODISCARD const_iterator operator + (difference_type off) const
		{
			const_iterator tmp = *this;
			return tmp += off;
		}

		const_iterator& operator -= (difference_type off)
		{
			BaseVector::const_iterator::operator -= (off);
			return *this;
		}

		NIRVANA_NODISCARD const_iterator operator - (difference_type off) const
		{
			const_iterator tmp = *this;
			return tmp -= off;
		}

		NIRVANA_NODISCARD difference_type operator - (const_iterator& rhs) const
		{	// return difference of iterators
			return BaseVector::const_iterator::operator - (rhs);
		}
	};

	class iterator : public BaseVector::iterator
	{
	public:
		typedef BaseVector::iterator::iterator_category iterator_category;
		typedef BaseVector::iterator::difference_type difference_type;
		typedef bool value_type;
		typedef vector <bool, allocator <bool> >::reference reference;
		typedef BaseVector::iterator::pointer pointer;

		iterator ()
		{}

		iterator (const BaseVector::iterator& base) :
			BaseVector::iterator (base)
		{}

		NIRVANA_NODISCARD reference operator * () const
		{
			return BaseVector::iterator::operator* ();
		}

		NIRVANA_NODISCARD reference operator [] (difference_type off) const
		{	// subscript
			return BaseVector::iterator::operator [] (off);
		}

		iterator& operator ++ ()
		{
			BaseVector::iterator::operator ++ ();
			return *this;
		}

		iterator operator ++ (int)
		{
			iterator tmp = *this;
			operator ++ ();
			return tmp;
		}

		iterator& operator -- ()
		{
			BaseVector::iterator::operator -- ();
			return *this;
		}

		iterator operator -- (int)
		{
			iterator tmp = *this;
			operator -- ();
			return tmp;
		}

		iterator& operator += (difference_type off)
		{
			BaseVector::const_iterator::operator += (off);
			return *this;
		}

		NIRVANA_NODISCARD iterator operator + (difference_type off) const
		{
			iterator tmp = *this;
			return tmp += off;
		}

		iterator& operator -= (difference_type off)
		{
			BaseVector::iterator::operator -= (off);
			return *this;
		}

		NIRVANA_NODISCARD iterator operator - (difference_type off) const
		{
			iterator tmp = *this;
			return tmp -= off;
		}

		NIRVANA_NODISCARD difference_type operator - (const_iterator& rhs) const
		{	// return difference of iterators
			return BaseVector::iterator::operator - (rhs);
		}
	};

	typedef std::reverse_iterator <const_iterator> const_reverse_iterator;
	typedef std::reverse_iterator <iterator> reverse_iterator;

	// Constructors
	vector () NIRVANA_NOEXCEPT
	{}

	explicit vector (const allocator_type&) NIRVANA_NOEXCEPT
	{}

	explicit vector (size_type count) :
		BaseVector (count)
	{}

#ifndef NIRVANA_C11
	explicit
#endif
	vector (size_type count, const value_type& v) :
		BaseVector (count, v)
	{}

#ifndef NIRVANA_C11
	explicit
#endif
	vector (size_type count, const value_type& v, const allocator_type&) :
		vector (count, v)
	{}

	vector (const vector& src) :
		BaseVector (src)
	{}

	vector (vector&& src) NIRVANA_NOEXCEPT :
		BaseVector (std::move (src))
	{}

	vector (vector&& src, const allocator_type&) NIRVANA_NOEXCEPT :
		vector (std::move (src))
	{}

#ifdef NIRVANA_C11
	vector (initializer_list <value_type> ilist)
	{
		construct_it (ilist.begin (), ilist.end ());
	}
#endif

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	vector (InputIterator b, InputIterator e)
	{
		construct_it (b, e);
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	vector (InputIterator b, InputIterator e, const allocator_type&)
	{
		construct_it (b, e);
	}

	// assign
	
	void assign (size_type count, const value_type& val)
	{
		BaseVector::assign (count, val);
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	void assign (InputIterator b, InputIterator e)
	{
		assign_it (b, e);
	}

#ifdef NIRVANA_C11
	void assign (initializer_list <value_type> ilist)
	{
		assign_it (ilist.begin (), ilist.end ());
	}
#endif

	// erase

	iterator erase (const_iterator pos)
	{
		return BaseVector::erase (pos);
	}

	iterator erase (const_iterator b, const_iterator e)
	{
		return BaseVector::erase (b, e);
	}

	// insert

	iterator insert (const_iterator pos, const value_type& val)
	{
		return BaseVector::insert (pos, val);
	}

	iterator insert (const_iterator pos, value_type&& val)
	{
		return BaseVector::insert (pos, val);
	}

	iterator insert (const_iterator pos, size_type count, const value_type& val)
	{
		return BaseVector::insert (pos, count, val);
	}

	template <class InputIterator
#ifdef NIRVANA_C11
		, typename = ::Nirvana::_RequireInputIter <InputIterator>
#endif
	>
	iterator insert (const_iterator pos, InputIterator b, InputIterator e)
	{
		return insert_it (pos, b, e);
	}

#ifdef NIRVANA_C11
	template <class ... Args>
	iterator emplace (const_iterator pos, Args&&... args)
	{
		return BaseVector::insert (pos, std::forward <Args> (args)...);
	}
#endif

	// Misc. operations

	reference at (size_type pos)
	{
		return BaseVector::at (pos);
	}

	const_reference at (size_type pos) const
	{
		return BaseVector::at (pos) != 0;
	}

	reference operator [] (size_type pos)
	{
		return BaseVector::operator [] (pos);
	}

	const_reference operator [] (size_type pos) const
	{
		return BaseVector::operator [] (pos);
	}

	void swap (vector& rhs)
	{
		BaseVector::swap (rhs);
	}

	void swap (reference ref1, reference ref2)
	{
		reference tmp = ref1;
		ref1 = ref2;
		ref2 = tmp;
	}

	void flip ()
	{
		for (BaseVector::pointer p = data (), end = p + size (); p != end; ++p) {
			*p = !*p;
		}
	}

	NIRVANA_NODISCARD allocator_type get_allocator () const
	{
		return allocator_type ();
	}

	// Iterators

	NIRVANA_NODISCARD const_iterator cbegin () const
	{
		return BaseVector::cbegin ();
	}

	NIRVANA_NODISCARD iterator begin ()
	{
		return BaseVector::begin ();
	}

	NIRVANA_NODISCARD const_iterator begin () const
	{
		return cbegin ();
	}

	NIRVANA_NODISCARD const_iterator cend () const
	{
		return BaseVector::cend ();
	}

	NIRVANA_NODISCARD iterator end ()
	{
		return BaseVector::end ();
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
		assert (size ());
		return *data ();
	}

	reference front ()
	{
		assert (size ());
		return reference (*data ());
	}

	const_reference back () const
	{
		assert (size ());
		return data () [size () - 1];
	}

	reference back ()
	{
		assert (size ());
		return reference (data () [size () - 1]);
	}
};

static_assert (is_nothrow_move_constructible <vector <int, allocator <int> > > (), "!is_nothrow_move_constructible <vector>");
static_assert (is_nothrow_move_assignable < vector <int, allocator <int> > > (), "!is_nothrow_move_assignable <vector>");

}

#endif
