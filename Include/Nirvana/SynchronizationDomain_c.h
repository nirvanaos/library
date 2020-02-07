#ifndef NIRVANA_ORB_SYNCHRONIZATIONDOMAIN_C_H_
#define NIRVANA_ORB_SYNCHRONIZATIONDOMAIN_C_H_

#include "Runnable.h"

namespace Nirvana {

struct ContextFrame
{
	void (*function) (void*);
	void* parameter;
};

class SynchronizationDomain;

/*
pseudo interface SynchronizationDomain {
  enter (ContextFrame frame);
	leave (ContextFrame frame);

	// Call runnable in the new execution domain.
	// Deadline by default is equal to the caller deadline.
	// To change the deadline use Current::set_next_async_deadline().
	async_call (Runnable runnable);
};
*/

typedef ::CORBA::Nirvana::T_ptr <SynchronizationDomain> SynchronizationDomain_ptr;
typedef ::CORBA::Nirvana::T_var <SynchronizationDomain> SynchronizationDomain_var;
typedef ::CORBA::Nirvana::T_out <SynchronizationDomain> SynchronizationDomain_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge <::Nirvana::SynchronizationDomain> :
	public BridgeMarshal <::Nirvana::SynchronizationDomain>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			void (*enter) (Bridge <::Nirvana::SynchronizationDomain>*, ::Nirvana::ContextFrame*, EnvironmentBridge*);
			void (*leave) (Bridge <::Nirvana::SynchronizationDomain>*, ::Nirvana::ContextFrame*, EnvironmentBridge*);
			void (*async_call) (Bridge <::Nirvana::SynchronizationDomain>*, Bridge <::Nirvana::Runnable>*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal <::Nirvana::SynchronizationDomain> (epv.interface)
	{}
};

template <class T>
class Client <T, ::Nirvana::SynchronizationDomain> :
	public T
{
public:
	void enter (::Nirvana::ContextFrame& context);
	void leave (::Nirvana::ContextFrame& context);
	void async_call (::Nirvana::Runnable_ptr runnable);
};

}
}

namespace Nirvana {

class SynchronizationDomain : public ::CORBA::Nirvana::ClientInterface <SynchronizationDomain>
{};

}

#endif
