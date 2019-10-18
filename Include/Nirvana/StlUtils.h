#ifndef NIRVANA_ORB_STLUTILS_H_
#define NIRVANA_ORB_STLUTILS_H_

#include "MemoryHelper.h"

namespace std {
template <typename C> class allocator;
template <typename A> struct allocator_traits;
struct random_access_iterator_tag;
template <class I> class reverse_iterator;
#if __cplusplus >= 201103L
template <class _Elem> class initializer_list;
#endif
}

namespace Nirvana {

class StdExceptions
{
public:
	NIRVANA_NORETURN static void xout_of_range (const char* msg);
	NIRVANA_NORETURN static void xlength_error (const char* msg);
};

class StdContainer;

class ContainerProxy
{
public:
	void on_container_destroy ()
	{
		assert (container_);
		container_ = nullptr;
	}

	void release ()
	{
		assert (ref_count_);
		if (!--ref_count_) {
			assert (!container_);
			delete this;
		}
	}

	ContainerProxy& duplicate ()
	{
		++ref_count_;
		return *this;
	}

	ContainerProxy (const StdContainer& container) :
		container_ (&container),
		ref_count_ (1)
	{}

	const StdContainer* container () const
	{
		return container_;
	}

private:
	const StdContainer* container_;
	unsigned ref_count_;
};

template <class Cont> class StdConstIterator;

class StdContainer :
	public StdExceptions
{
protected:
	StdContainer () :
		proxy_ (*new ContainerProxy (*this))
	{}

	~StdContainer ()
	{
		proxy_.on_container_destroy ();
		proxy_.release ();
	}

private:
	friend class ContainerProxyPtrBase;

	ContainerProxy& get_proxy () const
	{
		return proxy_.duplicate ();
	}

private:
	ContainerProxy& proxy_;
};

class ContainerProxyPtrBase
{
public:
	~ContainerProxyPtrBase ()
	{
		if (proxy_)
			proxy_->release ();
	}

	ContainerProxyPtrBase () :
		proxy_ (nullptr)
	{}

	ContainerProxyPtrBase (const StdContainer& container) :
		proxy_ (&container.get_proxy ())
	{}

	ContainerProxyPtrBase (const ContainerProxyPtrBase& src)
	{
		assign (src);
	}

	ContainerProxyPtrBase& operator = (const ContainerProxyPtrBase& src)
	{
		if (proxy_)
			proxy_->release ();
		assign (src);
		return *this;
	}

	const StdContainer* container () const
	{
		if (proxy_)
			return proxy_->container ();
		else
			return nullptr;
	}

private:
	void assign (const ContainerProxyPtrBase& src)
	{
		if (src.proxy_)
			proxy_ = &src.proxy_->duplicate ();
		else
			proxy_ = nullptr;
	}

private:
	ContainerProxy* proxy_;
};

template <class Cont>
class ContainerProxyPtr : public ContainerProxyPtrBase
{
public:
	ContainerProxyPtr ()
	{}

	ContainerProxyPtr (const Cont& container) :
		ContainerProxyPtrBase (container)
	{}

	const Cont* container () const
	{
		return static_cast <const Cont*> (ContainerProxyPtrBase::container ());
	}
};

template <class Cont>
class StdConstIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename Cont::value_type;
	using difference_type = ptrdiff_t;
	using pointer = const typename Cont::value_type*;
	using reference = const typename Cont::value_type&;

	StdConstIterator () :
		ptr_ (nullptr)
	{}

	StdConstIterator (pointer p, const Cont& c) :
#ifdef _DEBUG
		proxy_ (c),
#endif
		ptr_ (p)
	{}

	NIRVANA_NODISCARD reference operator * () const
	{
		_check_deref ();
		return *ptr_;
	}

	NIRVANA_NODISCARD pointer operator -> () const
	{
		_check_deref ();
		return ptr_;
	}

	StdConstIterator <Cont>& operator ++ ()
	{
		_check_offset (1);
		++ptr_;
		return (*this);
	}

	StdConstIterator <Cont> operator ++ (int)
	{
		_check_offset (1);
		StdConstIterator <Cont> tmp = *this;
		++ptr_;
		return tmp;
	}

	StdConstIterator <Cont>& operator -- ()
	{
		_check_offset (-1);
		--ptr_;
		return (*this);
	}

	StdConstIterator <Cont> operator -- (int)
	{
		_check_offset (-1);
		StdConstIterator <Cont> tmp = *this;
		--ptr_;
		return tmp;
	}

	StdConstIterator <Cont>& operator += (difference_type off)
	{
		_check_offset (off);
		ptr_ += off;
		return (*this);
	}

	NIRVANA_NODISCARD StdConstIterator <Cont> operator + (difference_type off) const
	{
		StdConstIterator <Cont> tmp = *this;
		return tmp += off;
	}

	StdConstIterator <Cont>& operator -= (difference_type off)
	{
		_check_offset (-off);
		ptr_ -= off;
		return (*this);
	}

	NIRVANA_NODISCARD StdConstIterator <Cont> operator - (difference_type off) const
	{
		StdConstIterator <Cont> tmp = *this;
		return tmp -= off;
	}

	NIRVANA_NODISCARD difference_type operator - (const StdConstIterator <Cont>& rhs) const
	{	// return difference of iterators
		_check_compat (rhs);
		return ptr_ - rhs.ptr_;
	}

	NIRVANA_NODISCARD reference operator [] (difference_type off) const
	{	// subscript
		return *(*this + off);
	}

	NIRVANA_NODISCARD bool operator == (const StdConstIterator <Cont>& rhs) const
	{	// test for iterator equality
		return ptr_ == rhs.ptr_;
	}

	NIRVANA_NODISCARD bool operator != (const StdConstIterator <Cont>& rhs) const
	{	// test for iterator inequality
		return !(*this == rhs);
	}

	NIRVANA_NODISCARD bool operator < (const StdConstIterator <Cont>& rhs) const
	{	// test if this < rhs
		_check_compat (rhs);
		return (ptr_ < rhs.ptr_);
	}

	NIRVANA_NODISCARD bool operator > (const StdConstIterator <Cont>& rhs) const
	{	// test if this > rhs
		return (rhs < *this);
	}

	NIRVANA_NODISCARD bool operator <= (const StdConstIterator <Cont>& rhs) const
	{	// test if this <= rhs
		return (!(rhs < *this));
	}

	NIRVANA_NODISCARD bool operator >= (const StdConstIterator <Cont>& rhs) const
	{	// test if this >= rhs
		return (!(*this < rhs));
	}

private:
	// Iterator debugging

#ifdef _DEBUG
	struct Margins
	{
		Margins (const Cont* cont)
		{
			assert (cont);
			begin = cont->data ();
			end = begin + cont->size ();
		}

		Margins (const StdConstIterator& it) :
			Margins (it.proxy_.container ())
		{}

		pointer begin, end;
	};
#endif

	void _check_deref () const
	{
#ifdef _DEBUG
		Margins m (*this);
		assert (m.begin <= ptr_ && ptr_ < m.end);
#endif
	}

	void _check_offset (difference_type off) const
	{
#ifdef _DEBUG
		Margins m (*this);
		pointer p = ptr_;
		assert (m.begin <= p && p <= m.end);
		p += off;
		assert (m.begin <= p && p <= m.end);
#endif
	}

	void _check_compat (const StdConstIterator <Cont>& rhs) const
	{
#ifdef _DEBUG
		const Cont* cont = proxy_.container ();
		assert (cont == rhs.proxy_.container ());
		Margins m (cont);
		assert (m.begin <= ptr_ && ptr_ <= m.end);
		assert (m.begin <= rhs.ptr_ && ptr_ <= rhs.m.end);
#endif
	}

private:
	pointer ptr_;

	friend Cont;

#ifdef _DEBUG
	ContainerProxyPtr <Cont> proxy_;
#endif
};

template <class Cont>
class StdIterator : public StdConstIterator <Cont>
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = typename Cont::value_type;
	using difference_type = ptrdiff_t;
	using pointer = typename Cont::value_type*;
	using reference = typename Cont::value_type&;

	StdIterator ()
	{}

	StdIterator (pointer p, const Cont& c) :
		StdConstIterator <Cont> (p, c)
	{}

	NIRVANA_NODISCARD reference operator * () const
	{
		return const_cast <reference> (StdConstIterator <Cont>::operator * ());
	}

	NIRVANA_NODISCARD pointer operator -> () const
	{
		return const_cast <pointer> (StdConstIterator <Cont>::operator -> ());
	}

	NIRVANA_NODISCARD reference operator [] (difference_type off) const
	{	// subscript
		return const_cast <reference> (StdConstIterator <Cont>::operator [] (off));
	}
};

}

#endif
