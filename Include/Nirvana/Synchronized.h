#ifndef NIRVANA_SYNCHRONIZED_H_
#define NIRVANA_SYNCHRONIZED_H_

#include "SynchronizationContext.h"

namespace Nirvana {

class Synchronized
{
public:
	Synchronized () :
		call_context_ (SynchronizationContext::_duplicate (g_current->synchronization_context ()))
	{}

	Synchronized (SynchronizationContext_ptr target) :
		call_context_ (SynchronizationContext::_duplicate (g_current->synchronization_context ()))
	{
		target->enter (false);
	}

	~Synchronized ()
	{
		call_context_->enter (true);
	}

	SynchronizationContext_ptr call_context () const
	{
		return call_context_;
	}

private:
	SynchronizationContext_var call_context_;
};

}

#endif
