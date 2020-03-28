/// \file Array.h
#ifndef NIRVANA_CORE_ARRAY_H_
#define NIRVANA_CORE_ARRAY_H_

#include <Nirvana/Nirvana.h>

namespace Nirvana {
namespace Core {

/// \brief Fixed size array.
template <class T>
class Array
{
public:
	Array () :
		size_ (0)
	{}

	Array (Array&& src) :
		p_ (src.p_),
		size_ (src.size_)
	{
		src.size_ = 0;
	}

	/// \brief Allocate memory and initialize it with zeroes.
	void allocate (Size size)
	{
		p_ = (T*)g_memory->allocate (0, size * sizeof (T), Memory::ZERO_INIT);
	}

	/// \brief Allocate and call default constructors. Rarely used.
	void construct (Size size)
	{
		assert (!size_);
		if (size) {
			p_ = (T*)g_memory->allocate (0, size * sizeof (T), 0);
			T* p = p_, * end = p_ + size;
			do {
				new (p) T ();
			} while (end != ++p);
		}
	}

	~Array ()
	{
		if (size_) {
			T* p = p_, * end = p_ + size_;
			do {
				p->~T ();
			} while (end != ++p);
			g_memory->release (p_, size_ * sizeof (T));
		}
	}

	Size size () const
	{
		return size_;
	}

	const T* begin () const
	{
		return p_;
	}

	T* begin ()
	{
		return p_;
	}

	const T* end () const
	{
		return p_ + size_;
	}

	T* end ()
	{
		return p_ + size_;
	}

	const T* cbegin () const
	{
		return begin ();
	}

	const T* cend () const
	{
		return end ();
	}

	const T& operator [] (Size i) const
	{
		return p_ [i];
	}

	T& operator [] (Size i)
	{
		return p_ [i];
	}

private:
	T* p_;
	Size size_;
};

}
}

#endif
