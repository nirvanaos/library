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

namespace std {

template <class T>
class vector <T, allocator <T> > :
	public Nirvana::StdVector,
	public CORBA::Nirvana::SequenceABI <T>
{
	typedef CORBA::Nirvana::SequenceABI <T> ABI;
	typedef vector <T, allocator <T> > MyType;
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
		size_t cb = allocated ();
		if (cb)
			Nirvana::default_heap ()->release (this->data_.begin, cb);
	}

	// Constructors
	explicit vector (const allocator_type&)
	{
		this->reset ();
	}

	explicit vector (size_type count)
	{
		if (count) {
			if (count > max_size ())
				xlength_error ();
			this->data_.allocated = 0;
			pointer p = Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
			this->data_.begin = p;
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

	vector (size_type count, const value_type& v, const allocator_type&)
	{
		if (count) {
			if (count > max_size ())
				xlength_error ();
			this->data_.allocated = 0;
			pointer p = Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
			this->data_.begin = p;
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
			this->data_.size = count;
		} else
			this->reset ();
	}

	vector (const vector& src)
	{
		copy_constructor (src);
	}

	vector (vector&& src)
	{
		if (src.is_constant_allocated ())
			copy_constructor (src);
		else {
			this->data_ = src.data_;
			src.reset ();
		}
	}

	template <class InputIterator>
	vector (InputIterator b, InputIterator e, const allocator_type&);

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
		if (src.is_constant_allocated ())
			copy (src);
		else {
			this->data_ = src.data_;
			src.reset ();
		}
	}

	void assign (size_type count, const value_type& val)
	{
		clear ();
		pointer p = this->data_.begin;
		if (count > capacity ()) {
			size_t space = allocated ();
			p = Nirvana::MemoryHelper ().assign (p, space, 0, count * sizeof (value_type));
			this->data_.begin = p;
			this->data_.allocated = space;
		}
		construct (p, p + count, val);
		this->data_.size = count;
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
	
	iterator erase (const_iterator pos);
	iterator erase (const_iterator b, const_iterator e);

	// insert
	
	iterator insert (const_iterator pos, const value_type& val);
	iterator insert (const_iterator pos, value_type&& val);
	iterator insert (const_iterator pos, size_type count, const value_type& val);

	template <class InIt>
	void insert (const_iterator pos, InIt b, InIt e);

	// Misc. operations
	
	reference at (size_type pos);
	const_reference at (size_type pos) const;

	reference front ();
	const_reference front () const;
	reference back ();
	const_reference back () const;

	size_type capacity () const
	{
		return (this->allocated_ & ~1) / sizeof (value_type);
	}

	void clear ()
	{
		pointer p = this->data_.begin;
		destruct (p, p + this->data_.size);
		this->data_.size = 0;
	}

	static size_type max_size ()
	{
		return SIZE_MAX / sizeof (T);
	}

	void reserve (size_type count)
	{
		if (count > capacity ()) {
			size_t space = allocated ();
			size_type size = this->data_.size;
			if (is_trivially_copyable (T) || !size)
				this->data_.begin = (pointer)MemoryHelper ().reserve (this->data_.begin, space, size * sizeof (T), count * sizeof (T));
			else if (space) {
				size_t add = count * sizeof (T) - space;
				if (default_heap ()->allocate ((uint8_t*)(this->data_.begin) + space, add, Memory::EXACTLY));
			}
			allocated (space);
		}
	}

	size_type size () const
	{
		return this->size_;
	}

	bool empty () const
	{
		return !this->size_;
	}

private:
	pointer get_ptr (const_iterator it) const
	{
		const_pointer p = it.ptr_;
		assert (data () <= p && p <= (data () + size ()));
		return const_cast <pointer> (p);
	}

	void allocated (size_t size)
	{
		this->allocated_ = size;
	}

	size_t allocated () const
	{
		size_t size = this->allocated_;
		return (size & 1) ? 0 : size;
	}

	size_t is_constant_allocated () const
	{
		return this->allocated_ & 1;
	}

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
				new ((void*)b)T (src++);
			}
		} else {
			pointer p = b;
			try {
				for (; p < e; ++p) {
					new ((void*)p)T (src++);
				}
			} catch (...) {
				while (p > b) {
					(--p)->~T ();
				}
				throw;
			}
		}
	}

	void destruct (pointer b, pointer e)
	{
		if (is_destructible <T>) {
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

	void copy_to_empty (const vector& src)
	{
		pointer p = this->data_.begin;
		size_type count = src.data_.size;
		if (count) {
			if (is_trivially_copyable (T)) {
				size_t space = allocated ();
				this->data_.begin = Nirvana::MemoryHelper ().assign (p, space, 0, count * sizeof (value_type), src.data_.begin);
				this->data_.allocated = space;
			} else {
				if (count > capacity ()) {
					size_t space = allocated ();
					p = Nirvana::MemoryHelper ().assign (p, space, 0, count * sizeof (value_type));
					this->data_.begin = p;
					this->data_.allocated = space;
				}
				construct (p, p + count, src.data_.begin);
			}
			this->data_.size = count;
		}
	}
};

template <class T>
void vector <T, allocator <T> >::copy_constructor (const vector <T, allocator <T> >& src)
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

}

#include <vector>

namespace std {

template <class T>
template <class InputIterator>
vector <T, allocator <T> >::vector (InputIterator b, InputIterator e, const allocator_type&)
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
void vector <T, allocator <T> >::assign (InputIterator b, InputIterator e)
{
	destruct (this->data_.begin, this->data_.begin + this->data_.size);
	this->data_.size = 0;
	size_t count = distance (b, e);
	if (capacity () < count) {
		size_t space = allocated ();
		this->data_.begin = Nirvana::MemoryHelper::assign (this->data_.begin, space, 0, count * sizeof (T));
		allocated (space);
	}
	construct (p, p + count, b);
	this->data_.size = count;
}

}

#endif
