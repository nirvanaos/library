#ifndef NIRVANA_SYNCHRONIZED_H_
#define NIRVANA_SYNCHRONIZED_H_

#include "SynchronizationContext.h"

namespace Nirvana {

class Synchronized
{
public:
	Synchronized (SynchronizationContext_ptr sync) :
		domain_ (sync)
	{
		domain_->enter (context_);
	}

	~Synchronized ()
	{
		domain_->leave (context_);
	}

private:
	SynchronizationContext_ptr domain_; // Don't duplicate interface for performance
	ContextFrame context_;
};

}

#endif
