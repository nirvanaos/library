/**
~~~{.idl}
module Nirvana {

pseudo interface SyncDomainTraits {
	/// Copy inout data from the current domain memory to target domain memory
	Pointer copy_inout (ConstPointer src, UWord cb);

	/// Move out (ret) data from target domain memory to current domain memory
	/// src memory will be released
	Pointer move_out (Pointer src, UWord cb);

	/// Enter to the target synchronization domain
	void enter (ContextFrame frame);

	/// Return to the current synchronization domain
	void leave (ContextFrame frame);

	/// Call runnable in the new execution domain.
	/// Deadline by default is equal to the caller deadline.
	/// To change the deadline use Current::next_async_deadline.
	oneway void async_call (Runnable runnable);

	/// Returns `false` if there is no synchronization domain.
	readonly attribute boolean synchronized;
};

}
~~~
*/

#ifndef NIRVANA_SYNCDOMAINTRAITS_H_
#define NIRVANA_SYNCDOMAINTRAITS_H_

#include "Runnable.h"

namespace Nirvana {

struct ContextFrame
{
	void (*function) (void*);
	void* parameter;
};

class SyncDomainTraits;

typedef ::CORBA::Nirvana::I_ptr <SyncDomainTraits> SyncDomainTraits_ptr;
typedef ::CORBA::Nirvana::I_var <SyncDomainTraits> SyncDomainTraits_var;
typedef ::CORBA::Nirvana::I_out <SyncDomainTraits> SyncDomainTraits_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::SyncDomainTraits, NIRVANA_REPOSITORY_ID (SyncDomainTraits))
::Nirvana::Pointer (*copy_inout) (Bridge <::Nirvana::SyncDomainTraits>*, ::Nirvana::ConstPointer src, ::Nirvana::UWord cb, EnvironmentBridge*);
::Nirvana::Pointer (*move_out) (Bridge <::Nirvana::SyncDomainTraits>*, ::Nirvana::Pointer src, ::Nirvana::UWord cb, EnvironmentBridge*);
void (*enter) (Bridge <::Nirvana::SyncDomainTraits>*, ::Nirvana::ContextFrame*, EnvironmentBridge*);
void (*leave) (Bridge <::Nirvana::SyncDomainTraits>*, ::Nirvana::ContextFrame*, EnvironmentBridge*);
void (*async_call) (Bridge <::Nirvana::SyncDomainTraits>*, Bridge <::Nirvana::Runnable>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::SyncDomainTraits> :
	public T
{
public:
	/// Copy inout data from the current domain memory to target domain memory
	::Nirvana::Pointer copy_inout (::Nirvana::ConstPointer src, ::Nirvana::UWord cb);

	/// Move out (ret) data from target domain memory to current domain memory
	/// src memory will be released
	::Nirvana::Pointer move_out (::Nirvana::Pointer src, ::Nirvana::UWord cb);

	/// Enter to the target synchronization domain
	void enter (::Nirvana::ContextFrame& context);

	/// Return to the current synchronization domain
	void leave (::Nirvana::ContextFrame& context);

	/// Call runnable in the new execution domain.
	/// Deadline by default is equal to the caller deadline.
	/// To change the deadline use Current::set_next_async_deadline().
	void async_call (::Nirvana::Runnable_ptr runnable);
};

}
}

namespace Nirvana {

class SyncDomainTraits : public ::CORBA::Nirvana::ClientInterface <SyncDomainTraits>
{};

}

#endif
