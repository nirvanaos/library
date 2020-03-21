// Core synchronization context.
#ifndef NIRVANA_CORE_SYNCHRONIZATIONCONTEXT_H_
#define NIRVANA_CORE_SYNCHRONIZATIONCONTEXT_H_

#include <Nirvana/Memory.h>
#include "Runnable.h"

namespace Nirvana {
namespace Core {

/// Core synchronization context.
class SynchronizationContext :
	public CoreInterface
{
public:
	/// Returns current synchronization context
	/// Returns smart pointer to ensure that context won't be destroyed
	/// until will be released.
	static Core_var <SynchronizationContext> current ();

	/// Enter to the synchronization domain.
	/// \param ret `true` on return to call source domain.
	///            If `true` then causes fatal on error.
	virtual void enter (bool ret) = 0;

	/// Call runnable in the new execution domain.
	virtual void async_call (Runnable* p, DeadlineTime dt) = 0;

	/// Returns `false` if there is no synchronization domain.
	virtual bool synchronized () = 0;

	/// Real core implementation will return pointer to the internal heap
	virtual Memory_ptr memory () = 0;

	/// Returns `true` if both contexts use common heap.
	/// For example, if it is the same context.
	virtual bool shared_memory (SynchronizationContext* other) = 0;
};

}
}

#endif
