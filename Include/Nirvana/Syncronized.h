#ifndef NIRVANA_SYNCHRONIZED_H_
#define NIRVANA_SYNCHRONIZED_H_

#include "SyncDomainTraits.h"

namespace Nirvana {

class Synchronized
{
public:
	Synchronized (SyncDomainTraits_ptr sync_domain) :
		domain_ (sync_domain)
	{
		domain_->enter (context_);
	}

	~Synchronized ()
	{
		domain_->leave (context_);
	}

private:
	SyncDomainTraits_ptr domain_; // Don't duplicate interface for performance
	ContextFrame context_;
};

}

#endif
