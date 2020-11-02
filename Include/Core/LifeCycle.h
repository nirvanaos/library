#ifndef NIRVANA_CORE_LIFECYCLE_H_
#define NIRVANA_CORE_LIFECYCLE_H_

#include <CORBA/LifeCycleRefCnt.h>
#include "AtomicCounter.h"

namespace Nirvana {
namespace Core {

/// Life cycle implementation for a core object with external interfaces.
template <class S>
class LifeCycle :
	public CORBA::Nirvana::LifeCycleRefCnt <S>
{
public:
	void* operator new (size_t size)
	{
		assert (sizeof (S) == size);
		return g_memory->allocate (0, size, 0);
	}

	void operator delete (void* p)
	{
		return g_memory->release (p, sizeof (S));
	}

	void _add_ref ()
	{
		ref_cnt_.increment ();
	}

	void _remove_ref ()
	{
		if (!ref_cnt_.decrement ())
			delete& static_cast <S&> (*this);
	}

private:
	RefCounter ref_cnt_;
};

}
}

#endif
