#ifndef NIRVANA_VECTOR_H_
#define NIRVANA_VECTOR_H_

#include <CORBA/SequenceABI.h>
#include "StlUtils.h"
#include <type_traits>

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

#define NO_BOOL_ALLOCATOR enable_if <!is_same <T, bool>::value, allocator <T> >

namespace std {

template <class T>
class vector <T, NO_BOOL_ALLOCATOR > :
	public Nirvana::StdVector,
	public CORBA::Nirvana::SequenceABI <T>
{
	typedef CORBA::Nirvana::SequenceABI <T> ABI;
	typedef vector <T, NO_BOOL_ALLOCATOR > MyType;
public:
	using const_iterator = Nirvana::StdConstIterator <MyType>;
	using iterator = Nirvana::StdIterator <MyType>;

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
	vector ()
	{
		this->reset ();
	}

	explicit vector (const allocator_type&) :
		vector ()
	{}

	explicit vector (size_type count)
	{
		if (count) {
			if (count > max_size ())
				xlength_error ();
			this->data_.allocated = 0;
			pointer p = this->data_.ptr = Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
			if (is_nothrow_default_constructible <T>)
				construct (p, p + count);
			else {
				try {
					construct (p, p + count);
				} catch (...) {
					Nirvana::default_heap ()->release (p, this->data_.allocated);
					throw;
				}
			}
			this->data_.size = count;
		} else
			this->reset ();
	}

	vector (size_type count, const value_type& v)
	{
		if (count) {
			if (count > max_size ())
				xlength_error ();
			this->data_.allocated = 0;
			pointer p = Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
			if (is_nothrow_copy_constructible <T>)
				construct (p, p + count, v);
			else {
				try {
					construct (p, p + count, v);
				} catch (...) {
					Nirvana::default_heap ()->release (p, this->data_.allocated);
					throw;
				}
			}
			this->data_.ptr = p;
			this->data_.size = count;
		} else
			this->reset ();
	}

	vector (size_type count, const value_type& v, const allocator_type&) :
		vector (count, v)
	{}

	vector (const vector& src)
	{
		copy_constructor (src);
	}

	vector (vector&& src)
	{
		this->data_ (src.data_);
		src.reset ();
	}

	template <class InputIterator>
	vector (InputIterator b, InputIterator e);

	template <class InputIterator>
	vector (InputIterator b, InputIterator e, const allocator_type&) :
		vector (b, e)
	{}

#if __cplusplus >= 201103L
	vector (initializer_list <T> ilist) :
		vector (ilist.begin (), ilist.end ())
	{}
#endif

	// Assignments

	vector& operator = (const vector& src)
	{
		copy (src);
		return *this;
	}

	vector& operator = (const vector&& src)
	{
		destruct (data (), data () + size ());
		release_memory ();
		this->data_ = src.data_;
		src.reset ();
	}

	void assign (size_type count, const value_type& val)
	{
		clear ();
		pointer p = this->ptr_;
		if (count > capacity ())
			this->ptr_ = p = Nirvana::MemoryHelper ().assign (p, this->allocated_, 0, count * sizeof (value_type));
		construct (p, p + count, val);
		this->size_ = count;
	}

	template <class InputIterator>
	void assign (InputIterator b, InputIterator e);

#if __cplusplus >= 201103L
	void assign (initializer_list <T> ilist)
	{
		assign (ilist.begin (), ilist.end ());
	}
#endif

	// erase

	iterator erase (const_iterator pos)
	{
		pointer p = get_ptr (pos);
		if (p < this->data_.ptr + this->data_.size)
			erase_internal (p, p + 1);
		return pos;
	}

	iterator erase (const_iterator b, const_iterator e)
	{
		pointer pb = get_ptr (b), pe = get_ptr (e);
		if (pb < pe)
			erase_internal (pb, pe);
		return e;
	}

	// insert

	iterator insert (const_iterator pos, const value_type& val);
	iterator insert (const_iterator pos, value_type&& val);
	iterator insert (const_iterator pos, size_type count, const value_type& val);

	template <class InputIterator>
	void insert (const_iterator pos, InputIterator b, InputIterator e);

	// Misc. operations

	reference at (size_type pos);
	const_reference at (size_type pos) const;

	reference operator [] (size_type pos);
	const_reference operator [] (size_type pos) const;

	size_type capacity () const
	{
		size_t space = this->data_.allocated;
		if (space)
			return space / sizeof (value_type);
		else
			return size ();
	}

	void clear ()
	{
		pointer p = this->data_.ptr;
		destruct (p, p + this->data_.size);
		this->data_.size = 0;
	}

	static size_type max_size ()
	{
		return SIZE_MAX / sizeof (value_type);
	}

	void push_back (const value_type& v);
	void push_back (value_type&& v);

	void pop_back ();

	void reserve (size_type count)
	{
		if (count > capacity ()) {
			size_t new_space = count * sizeof (value_type);
			size_type size = this->data_.size;
			if (is_trivially_copyable <value_type> || !size)
				this->data_.ptr = (pointer)Nirvana::MemoryHelper ().reserve (this->data_.ptr, this->data_.allocated, size * sizeof (value_type), new_space);
			else {
				size_t space = this->data_.allocated;
				size_t add = new_space - space;
				if (!Nirvana::default_heap ()->allocate ((uint8_t*)(this->data_.ptr) + space, add, Memory::EXACTLY)) {
					pointer new_ptr = (pointer)Nirvana::default_heap ()->allocate (nullptr, new_space, Memory::RESERVED);
					size_t au = Nirvana::default_heap ()->query (new_ptr, Memory::ALLOCATION_UNIT);
					new_space = round_up (new_space, au);
					pointer old_ptr = this->data_.ptr;
					try {
						Nirvana::default_heap ()->commit (new_ptr, (size + count) * sizeof (value_type));
						construct_move (new_ptr, new_ptr + size, old_ptr);
					} catch (...) {
						Nirvana::default_heap ()->release (new_ptr, new_space);
						throw;
					}
					destruct (old_ptr, old_ptr + size);
					this->data_.ptr = new_ptr;
					Nirvana::default_heap ()->release (old_ptr, space);
				}
				this->data_.allocated = new_space;
			}
		}
	}
	/*
		bool insert_internal (pointer pos, size_type count)
		{
			if (is_trivially_copyable <value_type> || !size) {
				this->data_.ptr = (pointer)MemoryHelper ().insert (p, this->data_.allocated,
					size * sizeof (value_type), (pos - p) * sizeof (value_type), count * sizeof (value_type), nullptr);
				return false; // Inserted memory is not initialized
			} else {
				size_type size = this->data_.size;
				size_type new_size = size + count;
				if (new_size > capacity ()) {
					size_t new_space = new_size * sizeof (value_type);
					pointer ptr = this->data_.ptr;
					size_t space = this->data_.allocated;
					size_t add = new_space - space;
					if (default_heap ()->allocate ((uint8_t*)ptr + space, add, Memory::EXACTLY)) {
						pointer end = ptr + size;

						pointer tail = end - count;
						construct_move (end, end + count, tail);
					} else {
						pointer new_ptr = (pointer)default_heap ()->allocate (nullptr, new_space, Memory::RESERVED);
						size_t au = default_heap ()->query (new_ptr, Memory::ALLOCATION_UNIT);
						new_space = round_up (new_space, au);
						try {
							default_heap ()->commit (new_ptr, (size + count) * sizeof (value_type));
							construct_move (new_ptr, new_ptr + (ptr - pos), ptr);
						} catch (...) {
							default_heap ()->release (new_ptr, new_space);
							throw;
						}
						destruct (ptr, pos);
						this->data_.ptr = new_ptr;
						default_heap ()->release (old_ptr, space);
					}
					this->data_.allocated = new_space;
				}
			}
		}
	*/
	const_pointer data () const
	{
		return this->data_.ptr;
	}

	pointer data ()
	{
		return this->data_.ptr;
	}

	size_type size () const
	{
		return this->data_.size;
	}

	bool empty () const
	{
		return !this->data_.size;
	}

	// Iterators

	NIRVANA_NODISCARD const_iterator cbegin () const
	{
		return const_iterator (this->data_.ptr, *this);
	}

	NIRVANA_NODISCARD iterator begin ()
	{
		return iterator (this->data_.ptr, *this);
	}

	NIRVANA_NODISCARD const_iterator begin () const
	{
		return cbegin ();
	}

	NIRVANA_NODISCARD const_iterator cend () const
	{
		return const_iterator (this->data_.ptr + this->data_.size, *this);
	}

	NIRVANA_NODISCARD iterator end ()
	{
		return iterator (this->data_.ptr + this->data_.size, *this);
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
		return this->data_.ptr [0];
	}

	reference front ()
	{
		return this->data_.ptr [0];
	}

	const_reference back () const
	{
		assert (size ());
		return this->data_.ptr [size () - 1];
	}

	reference back ()
	{
		assert (size ());
		return this->data_.ptr [size () - 1];
	}

	NIRVANA_NODISCARD allocator_type get_allocator () const
	{
		return allocator_type ();
	}

private:
	void release_memory ()
	{
		size_t cb = this->data_.allocated;
		if (cb)
			Nirvana::default_heap ()->release (this->data_.ptr, cb);
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
		if (is_nothrow_default_constructible <T>) {
			for (; b < e; ++b) {
				new ((void*)b)T ();
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new ((void*)p)T ();
				}
			} catch (...) {
				while (p > b) {
					(--p)->~T ();
				}
				throw;
			}
		}
	}

	void construct (pointer b, pointer e, const value_type& v)
	{
		if (is_nothrow_copy_constructible <T>) {
			for (; b < e; ++b) {
				new ((void*)b)T (v);
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new ((void*)p)T (v);
				}
			} catch (...) {
				while (p > b) {
					(--p)->~T ();
				}
				throw;
			}
		}
	}

	template <class InputIterator>
	void construct (pointer b, pointer e, InputIterator src)
	{
		if (is_nothrow_copy_constructible <T>) {
			for (; b < e; ++b) {
				new ((void*)b)T (*(src++));
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new ((void*)p)T (*(src++));
				}
			} catch (...) {
				while (p > b) {
					(--p)->~T ();
				}
				throw;
			}
		}
	}

	void construct_move (pointer b, pointer e, pointer src)
	{
		if (is_nothrow_move_constructible <T>) {
			for (; b < e; ++b, ++src) {
				new ((void*)b)T (std::move (*src));
			}
		} else
			construct (b, e, src);
	}

	void destruct (pointer b, pointer e)
	{
		if (is_destructible <value_type> ()) {
			for (; b < e; ++b) {
				b->~T ();
			}
		}
	}

	void copy (const vector& src)
	{
		clear ();
		copy_to_empty (src);
	}

	void copy_to_empty (const vector& src);
};

template <class T>
void vector <T, NO_BOOL_ALLOCATOR >::copy_constructor (const vector& src)
{
	this->reset ();
	if (is_trivially_copyable <T> || is_nothrow_copy_constructible <T>)
		copy_to_empty (src);
	else {
		try {
			copy_to_empty (src);
		} catch (...) {
			Nirvana::default_heap ()->release (this->data_.begin, this->data_.allocated);
			throw;
		}
	}
}

template <class T>
void vector <T, NO_BOOL_ALLOCATOR >::copy_to_empty (const vector& src)
{
	size_type count = src.data_.size;
	if (count) {
		if (is_trivially_copyable <value_type>)
			this->data_.ptr = Nirvana::MemoryHelper ().assign (this->data_.ptr, this->data_.allocated, 0, count * sizeof (value_type), src.data_.ptr);
		else {
			pointer p = this->data_.ptr;
			if (count > capacity ())
				this->data_.ptr = p = Nirvana::MemoryHelper ().assign (p, this->data_.allocated, 0, count * sizeof (value_type));
			construct (p, p + count, src.data_.ptr);
		}
		this->data_.size = count;
	}
}

template <class T>
void vector <T, NO_BOOL_ALLOCATOR >::erase_internal (pointer pb, pointer pe)
{
	pointer p = this->data_.ptr;
	if (is_trivially_copyable <value_type>)
		Nirvana::MemoryHelper ().erase (p, size () * sizeof (value_type), (pb - p) * sizeof (value_type), (pe - pb) * sizeof (value_type));
	else {
		pointer end = p + size ();
		if (pe < end) {
			if (is_move_assignable <value_type>)
				std::move (pe, end, pb);
			else
				std::copy (pe, end, pb);
			pb = end - (pe - pb);
		}
		destruct (pb, end);
		Nirvana::default_heap ()->decommit (pb, (end - pb) * sizeof (value_type));
	}
}

}

#include <vector>

namespace std {

template <class T>
template <class InputIterator>
vector <T, NO_BOOL_ALLOCATOR >::vector (InputIterator b, InputIterator e)
{
	size_t count = distance (b, e);
	if (count) {
		this->data_.allocated = 0;
		pointer p = Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
		this->data_.begin = p;
		if (is_nothrow_copy_constructible <T>)
			construct (p, p + count, b);
		else {
			try {
				construct (p, p + count, b);
			} catch (...) {
				Nirvana::default_heap ()->release (p, this->data_.allocated);
				throw;
			}
		}
		this->data_.size = count;
	} else
		this->reset ();
}

template <class T>
template <class InputIterator>
void vector <T, NO_BOOL_ALLOCATOR >::assign (InputIterator b, InputIterator e)
{
	pointer p = this->data_.ptr;
	destruct (p, p + this->data_.size);
	this->data_.size = 0;
	size_t count = distance (b, e);
	if (capacity () < count)
		this->data_.ptr = p = Nirvana::MemoryHelper::assign (p, this->data_.allocated, 0, count * sizeof (T));
	construct (p, p + count, b);
	this->data_.size = count;
}

}

#endif
