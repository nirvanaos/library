#ifndef NIRVANA_CORE_COREINTERFACE_H_
#define NIRVANA_CORE_COREINTERFACE_H_

#include <Nirvana/Nirvana.h>
#include "AtomicCounter.h"

namespace Nirvana {
namespace Core {

/// Core interface.
class CoreInterface
{
public:
	virtual void core_add_ref () = 0;
	virtual void core_remove_ref () = 0;
};

/// Core interface smart pointer.
template <class I>
class Core_var
{
public:
	Core_var () :
		p_ (nullptr)
	{}

	Core_var (I* p) :
		p_ (p)
	{}

	Core_var (const Core_var& src) :
		Core_var (src.p_)
	{
		if (p_)
			p_->core_add_ref ();
	}

	Core_var (Core_var&& src) :
		p_ (src.p_)
	{
		src.p_ = nullptr;
	}

	~Core_var ()
	{
		if (p_)
			p_->core_remove_ref ();
	}

	/// Note that unlike I_var, Core_var increments reference counter.
	Core_var& operator = (I* src)
	{
		reset (src);
		return *this;
	}

	Core_var& operator = (const Core_var& src)
	{
		reset (src.p_);
		return *this;
	}

	Core_var& operator = (Core_var&& src)
	{
		if (this != &src) {
			reset (nullptr);
			p_ = src.p_;
			src.p_ = nullptr;
		}
		return *this;
	}

	I* operator -> () const
	{
		return p_;
	}

	operator I* () const
	{
		return p_;
	}

private:
	void reset (I* p)
	{
		if (p != p_) {
			if (p)
				p->core_add_ref ();
			I* tmp = p_;
			p_ = p;
			if (tmp)
				tmp->core_remove_ref ();
		}
	}

private:
	I* p_;
};

/// Dynamic implementation of a core interface.
/// \tparam I Core interface.
template <class I>
class CoreDynamic : public I
{
public:
	virtual ~CoreDynamic () {}

	virtual void core_add_ref ()
	{
		ref_cnt_.increment ();
	}

	virtual void core_remove_ref ()
	{
		if (!ref_cnt_.decrement ())
			delete this;
	}

#ifdef NIRVANA_C14

	void* operator new (size_t size)
	{
		return g_memory->allocate (0, size, 0);
	}

	void operator delete (void* p, size_t size)
	{
		g_memory->release (p, size);
	}

#else

	void* operator new (size_t size)
	{
		size += sizeof (size_t);
		size_t* hdr = (size_t*)g_memory->allocate (size);
		*hdr = size;
		return hdr + 1;
	}

	void operator delete (void* p)
	{
		size_t* hdr = (size_t*)p - 1;
		g_memory->release (hdr, *hdr);
	}

#endif

private:
	RefCounter ref_cnt_;
};

}
}

#endif
