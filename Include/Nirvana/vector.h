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
	public CORBA::Nirvana::SequenceABI <T>,
	private Nirvana::StdVector
{
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

	// Constructors
	vector ()
	{
		this->begin_ = nullptr;
		this->size_ = 0;
		this->allocated_ = 0;
	}

	// Assignments
	
	vector& operator = (const vector& src);
	vector& operator = (const vector&& src);

	void assign (size_type count, const value_type& val);

	template <class InIt>
	void assign (InIt b, InIt e);

#if __cplusplus >= 201103L
	void assign (initializer_list <T> ilist);
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

	void clear ();

	static size_type max_size ()
	{
		return SIZE_MAX / sizeof (T);
	}

	void reserve (size_type count)
	{
		if (count > capacity ()) {
			size_t space = allocated ();
			if (is_trivially_copyable (T))
				this->ptr_ = (pointer)MemoryHelper ().reserve (this->ptr_, space, size () * sizeof (T), count * sizeof (T));
			else {

			}
			allocated (space);
		}
	}

	size_type size () const
	{
		return this->size_;
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

	void destroy ()
	{

	}
};

}

#include <vector>

#endif
