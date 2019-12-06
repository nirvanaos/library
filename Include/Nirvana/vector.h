#ifndef NIRVANA_VECTOR_H_
#define NIRVANA_VECTOR_H_

#include <CORBA/SequenceABI.h>
#include "StlUtils.h"
#include <type_traits>
#include <vector>

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
		release_memory ();
	}

	// Constructors
	vector () NIRVANA_NOEXCEPT
	{
		this->reset ();
	}

	explicit vector (const allocator_type&) NIRVANA_NOEXCEPT :
		vector ()
	{}

	explicit vector (size_type count)
	{
		if (count) {
			if (count > max_size ())
				xlength_error ();
			this->data_.allocated = 0;
			pointer p = this->data_.ptr = (pointer)Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
			if (is_nothrow_default_constructible <value_type> ())
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
			pointer p = (pointer)Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
			if (is_nothrow_copy_constructible <value_type> ())
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

	vector (vector&& src) NIRVANA_NOEXCEPT
	{
		this->data_ = src.data_;
		src.reset ();
	}

	template <class InputIterator>
	vector (InputIterator b, InputIterator e);

	template <class InputIterator>
	vector (InputIterator b, InputIterator e, const allocator_type&) :
		vector (b, e)
	{}

#if __cplusplus >= 201103L
	vector (initializer_list <value_type> ilist) :
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
	void assign (initializer_list <value_type> ilist)
	{
		assign (ilist.begin (), ilist.end ());
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
		pointer ppos = get_ptr (pos);
		if (insert_internal (ppos, 1))
			*ppos = val;
		else {
			new (ppos) value_type (val);
			++(this->data_.size);
		}
		return iterator (ppos, *this);
	}

	iterator insert (const_iterator pos, value_type&& val)
	{
		pointer ppos = get_ptr (pos);
		if (insert_internal (ppos, 1))
			*ppos = std::move (val);
		else {
			new (ppos) value_type (std::move (val));
			++(this->data_.size);
		}
		return iterator (ppos, *this);
	}

	iterator insert (const_iterator pos, size_type count, const value_type& val);

	template <class InputIterator>
	iterator insert (const_iterator pos, InputIterator b, InputIterator e);

#if __cplusplus >= 201103L
	template <class ... Args>
	iterator emplace (const_iterator pos, Args&&... args)
	{
		pointer ppos = get_ptr (pos);
		if (insert_internal (ppos, 1))
			*ppos = value_type (std::forward (args));
		else {
			new (ppos) value_type (std::forward (args));
			++(this->data_.size);
		}
		return iterator (ppos, *this);
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

	void push_back (const value_type& v)
	{
		pointer end = data () + size ();
		insert_internal (end, 1);
		new (end) value_type (v);
		++(this->data_.size);
	}

	void push_back (value_type&& v)
	{
		pointer end = data () + size ();
		insert_internal (end, 1);
		new (end) value_type (std::move (v));
		++(this->data_.size);
	}

#if __cplusplus >= 201103L
	template <class ... Args>
	void emplace_back (Args&&... args)
	{
		pointer end = data () + size ();
		insert_internal (end, 1);
		new (end) value_type (std::forward(args)...);
		++(this->data_.size);
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

	void swap (vector& rhs)
	{
		typename ABI::Data tmp = this->data_;
		this->data_ = rhs.data_;
		rhs.data_ = tmp;
	}

	NIRVANA_NODISCARD allocator_type get_allocator () const
	{
		return allocator_type ();
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
		if (is_nothrow_default_constructible <value_type> ()) {
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
		if (is_nothrow_copy_constructible <value_type> ()) {
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
		if (is_nothrow_copy_constructible <value_type> ()) {
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
		if (is_nothrow_move_constructible <value_type> ()) {
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

	bool insert_internal (pointer& pos, size_type count);
};

template <class T>
template <class InputIterator>
vector <T, allocator <T> >::vector (InputIterator b, InputIterator e)
{
	size_t count = distance (b, e);
	if (count) {
		this->data_.allocated = 0;
		pointer p = (pointer)Nirvana::MemoryHelper ().assign (nullptr, this->data_.allocated, 0, count * sizeof (value_type));
		this->data_.ptr = p;
		if (is_nothrow_copy_constructible <value_type> ())
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
	pointer p = this->data_.ptr;
	destruct (p, p + this->data_.size);
	this->data_.size = 0;
	size_type count = distance (b, e);
	if (capacity () < count)
		this->data_.ptr = p = Nirvana::MemoryHelper::assign (p, this->data_.allocated, 0, count * sizeof (T));
	construct (p, p + count, b);
	this->data_.size = count;
}

template <class T>
typename vector <T, allocator <T> >::iterator
vector <T, allocator <T> >::insert (const_iterator pos, size_type count, const value_type& val)
{
	pointer ppos = get_ptr (pos);
	if (count) {
		if (insert_internal (ppos, count)) {
			std::fill_n (ppos, count, val);
		} else {
			construct (ppos, ppos + count, val);
			this->data_.size += count;
		}
	}
	return iterator (ppos, *this);
}

template <class T>
template <class InputIterator>
typename vector <T, allocator <T> >::iterator
vector <T, allocator <T> >::insert (const_iterator pos, InputIterator b, InputIterator e)
{
	pointer ppos = get_ptr (pos);
	size_type count = distance (b, e);
	if (count) {
		if (insert_internal (ppos, count))
			std::copy (b, e, ppos);
		else {
			construct (ppos, ppos + count, b);
			this->data_.size += count;
		}
	}
	return iterator (ppos, *this);
}

template <class T>
void vector <T, allocator <T> >::copy_constructor (const vector& src)
{
	this->reset ();
	if (is_trivially_copyable <value_type> () || is_nothrow_copy_constructible <value_type> ())
		copy_to_empty (src);
	else {
		try {
			copy_to_empty (src);
		} catch (...) {
			Nirvana::default_heap ()->release (this->data_.ptr, this->data_.allocated);
			throw;
		}
	}
}

template <class T>
void vector <T, allocator <T> >::copy_to_empty (const vector& src)
{
	size_type count = src.data_.size;
	if (count) {
		if (is_trivially_copyable <value_type> ())
			this->data_.ptr = (pointer)Nirvana::MemoryHelper ().assign (this->data_.ptr, this->data_.allocated, 0, count * sizeof (value_type), src.data_.ptr);
		else {
			pointer p = this->data_.ptr;
			if (count > capacity ())
				this->data_.ptr = p = (pointer)Nirvana::MemoryHelper ().assign (p, this->data_.allocated, 0, count * sizeof (value_type));
			construct (p, p + count, src.data_.ptr);
		}
		this->data_.size = count;
	}
}

template <class T>
void vector <T, allocator <T> >::erase_internal (pointer pb, pointer pe)
{
	pointer p = this->data_.ptr;
	if (is_trivially_copyable <value_type> ())
		Nirvana::MemoryHelper ().erase (p, size () * sizeof (value_type), (pb - p) * sizeof (value_type), (pe - pb) * sizeof (value_type));
	else {
		pointer end = p + size ();
		if (pe < end) {
			if (is_move_assignable <value_type> ())
				std::move (pe, end, pb);
			else
				std::copy (pe, end, pb);
			pb = end - (pe - pb);
		}
		destruct (pb, end);
		Nirvana::default_heap ()->decommit (pb, (end - pb) * sizeof (value_type));
	}
}

template <class T>
void vector <T, allocator <T> >::reserve (size_type count)
{
	if (count > capacity ()) {
		size_t new_space = count * sizeof (value_type);
		size_type size = this->data_.size;
		if (is_trivially_copyable <value_type> () || !size)
			this->data_.ptr = (pointer)Nirvana::MemoryHelper ().reserve (this->data_.ptr, this->data_.allocated, size * sizeof (value_type), new_space);
		else {
			size_t space = this->data_.allocated;
			size_t add = new_space - space;
			try {
				if (!Nirvana::default_heap ()->allocate ((uint8_t*)(this->data_.ptr) + space, add, Nirvana::Memory::EXACTLY)) {
					pointer new_ptr = (pointer)Nirvana::default_heap ()->allocate (nullptr, new_space, Nirvana::Memory::RESERVED);
					size_t au = Nirvana::default_heap ()->query (new_ptr, Nirvana::Memory::ALLOCATION_UNIT);
					new_space = Nirvana::round_up (new_space, au);
					pointer old_ptr = this->data_.ptr;
					try {
						Nirvana::default_heap ()->commit (new_ptr, size * sizeof (value_type));
						construct_move (new_ptr, new_ptr + size, old_ptr);
					} catch (...) {
						Nirvana::default_heap ()->release (new_ptr, new_space);
						throw;
					}
					destruct (old_ptr, old_ptr + size);
					this->data_.ptr = new_ptr;
					Nirvana::default_heap ()->release (old_ptr, space);
				}
			} catch (const CORBA::NO_MEMORY&) {
				throw std::bad_alloc ();
			}
			this->data_.allocated = new_space;
		}
	}
}

template <class T>
bool vector <T, allocator <T> >::insert_internal (pointer& pos, size_type count)
{
	pointer ptr = this->data_.ptr;
	size_type size = this->data_.size;
	if (pos == (ptr + size)) {
		reserve (size + count);
		pos = this->data_.ptr + size;
		Nirvana::default_heap ()->commit (pos, count * sizeof (value_type));
		return false;	// Not initialized, size not changed
	} else if (pos < ptr)
		xout_of_range ();

	if (is_trivially_copyable <value_type> () || !size) {
		pointer new_ptr = this->data_.ptr = (pointer)Nirvana::MemoryHelper ().insert (ptr, this->data_.allocated,
			size * sizeof (value_type), (pos - ptr) * sizeof (value_type), count * sizeof (value_type), nullptr);
		pos = new_ptr + (pos - ptr);
	} else {
		size_type new_size = size + count;
		if (new_size > capacity ()) {
			size_t new_space = new_size * sizeof (value_type);
			size_t space = this->data_.allocated;
			size_t add = new_space - space;
			try {
				if (Nirvana::default_heap ()->allocate ((uint8_t*)ptr + space, add, Nirvana::Memory::EXACTLY))
					this->data_.allocated = new_space;
				else {
					pointer new_ptr = (pointer)Nirvana::default_heap ()->allocate (nullptr, new_space, 0);
					size_t au = Nirvana::default_heap ()->query (new_ptr, Nirvana::Memory::ALLOCATION_UNIT);
					new_space = Nirvana::round_up (new_space, au);
					try {
						pointer head_end = new_ptr + (ptr - pos);
						construct_move (new_ptr, head_end, ptr);
						pointer tail = head_end + count;
						construct (head_end, tail);
						construct_move (tail, new_ptr + new_size, pos);
					} catch (...) {
						Nirvana::default_heap ()->release (new_ptr, new_space);
						throw;
					}
					destruct (ptr, ptr + size);
					this->data_.ptr = new_ptr;
					this->data_.size = new_size;
					Nirvana::default_heap ()->release (ptr, space);
					this->data_.allocated = new_space;
					pos = new_ptr + (pos - ptr);
					return true;	// Initialized
				}
			} catch (const CORBA::NO_MEMORY&) {
				throw std::bad_alloc ();
			}
		}

		// In-place insert
		pointer end = ptr + size;
		Nirvana::default_heap ()->commit (end, count * sizeof (value_type));
		pointer new_end = end + count;
		size_type move_count = end - pos;
		if (move_count < count) {
			construct_move (new_end - move_count, new_end, end - move_count);
			construct (end, end + count - move_count);
		} else {
			construct_move (end, new_end, end - count);
			if (is_move_assignable <value_type> ())
				move_backward (pos, end - count, new_end);
			else
				copy_backward (pos, end - count, new_end);
		}
	}

	return true;	// Initialized
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

		reference& operator = (bool v)
		{
			ref_ = v;
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

	private:
		reference (BooleanType& ref) :
			ref_ (ref)
		{}

		BooleanType& ref_;
	};

	typedef bool const_reference;

	class const_iterator : public BaseVector::const_iterator
	{
	public:
		using value_type = bool;
		using reference = bool;

		const_iterator ()
		{}

		const_iterator (const BaseVector::const_iterator& base) :
			BaseVector::const_iterator (base)
		{}

		NIRVANA_NODISCARD reference operator * () const
		{
			return BaseVector::const_iterator::operator* () != 0;
		}
	};

	class iterator : public BaseVector::iterator
	{
	public:
		using value_type = bool;
		using reference = vector <bool, allocator <bool> >::reference;

		iterator ()
		{}

		iterator (const BaseVector::iterator& base) :
			BaseVector::iterator (base)
		{}

		NIRVANA_NODISCARD reference operator * () const
		{
			return BaseVector::iterator::operator* ();
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

	vector (size_type count, const value_type& v) :
		BaseVector (count, v)
	{}

	vector (size_type count, const value_type& v, const allocator_type&) :
		vector (count, v)
	{}

	vector (const vector& src) :
		BaseVector (src)
	{}

	vector (vector&& src) :
		BaseVector (std::move (src))
	{}

	template <class InputIterator>
	vector (InputIterator b, InputIterator e) :
		BaseVector (b, e)
	{}

	template <class InputIterator>
	vector (InputIterator b, InputIterator e, const allocator_type&) :
		vector (b, e)
	{}

#if __cplusplus >= 201103L
	vector (initializer_list <bool> ilist) :
		vector (ilist.begin (), ilist.end ())
	{}
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

	template <class InputIterator>
	iterator insert (const_iterator pos, InputIterator b, InputIterator e)
	{
		return BaseVector::insert (pos, b, e);
	}

#if __cplusplus >= 201103L
	template <class ... Args>
	iterator emplace (const_iterator pos, Args&&... args)
	{
		return BaseVector::insert (pos, std::forward (args));
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
		for (BaseVector::pointer p = this->data_.ptr, end = p + this->data_.size; p != end; ++p) {
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
};

}

#endif
