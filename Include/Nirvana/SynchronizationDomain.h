#ifndef NIRVANA_SYNCHRONIZATIONDOMAIN_H_
#define NIRVANA_SYNCHRONIZATIONDOMAIN_H_

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
  /// Copy inout data from the current domain memory to target domain memory
  Pointer copy_inout (ConstPointer src, UWord cb);

	/// Move out (ret) data from target domain memory to current domain memory
	/// src memory will be released
	Pointer move_out (Pointer src, UWord cb);

	/// Enter to the target synchronization domain
	enter (ContextFrame frame);

	/// Return to the current synchronization domain
	leave (ContextFrame frame);

	/// Call runnable in the new execution domain.
	/// Deadline by default is equal to the caller deadline.
	/// To change the deadline use Current::set_next_async_deadline().
	async_call (Runnable runnable);
};
*/

typedef ::CORBA::Nirvana::I_ptr <SynchronizationDomain> SynchronizationDomain_ptr;
typedef ::CORBA::Nirvana::I_var <SynchronizationDomain> SynchronizationDomain_var;
typedef ::CORBA::Nirvana::I_out <SynchronizationDomain> SynchronizationDomain_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::SynchronizationDomain)
::Nirvana::Pointer (*copy_inout) (Bridge <::Nirvana::SynchronizationDomain>*, ::Nirvana::ConstPointer src, ::Nirvana::UWord cb, EnvironmentBridge*);
::Nirvana::Pointer (*move_out) (Bridge <::Nirvana::SynchronizationDomain>*, ::Nirvana::Pointer src, ::Nirvana::UWord cb, EnvironmentBridge*);
void (*enter) (Bridge <::Nirvana::SynchronizationDomain>*, ::Nirvana::ContextFrame*, EnvironmentBridge*);
void (*leave) (Bridge <::Nirvana::SynchronizationDomain>*, ::Nirvana::ContextFrame*, EnvironmentBridge*);
void (*async_call) (Bridge <::Nirvana::SynchronizationDomain>*, Bridge <::Nirvana::Runnable>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::SynchronizationDomain> :
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

class SynchronizationDomain : public ::CORBA::Nirvana::ClientInterface <SynchronizationDomain>
{};

}

#endif
