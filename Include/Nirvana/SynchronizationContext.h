/**
~~~{.idl}
module Nirvana {

pseudo interface SynchronizationContext {
  /// Set current heap to this context heap.
	void enter_memory ();

	/// Enter to the synchronization domain.
	/// \param ret `true` on return to call source domain.
	///            Causes fatal on error.
	void enter (boolean ret);

	/// Move output returned data from current context heap to this context heap.
	/// src memory will be released.
	Pointer adopt_output (Pointer src, UWord data_size, inout UWord allocated_size);

	/// Allocate memory in this context.
	Pointer allocate (inout UWord size);

	/// Call runnable in the new execution domain.
	/// Deadline by default is equal to the caller deadline.
	/// To change the deadline use `Current::set_next_async_deadline ()`.
	oneway void async_call (Runnable runnable);

	/// Returns `false` if there is no synchronization domain.
	readonly attribute boolean synchronized;

	/// Returns `true` if both contexts use common heap.
	/// For example, if it is the same context.
	boolean shared_memory (SynchronizationContext other);
};

}
~~~
*/

#ifndef NIRVANA_SYNCHRONIZATIONCONTEXT_H_
#define NIRVANA_SYNCHRONIZATIONCONTEXT_H_

#include "Runnable.h"

namespace Nirvana {

class SynchronizationContext;

typedef ::CORBA::Nirvana::I_ptr <SynchronizationContext> SynchronizationContext_ptr;
typedef ::CORBA::Nirvana::I_var <SynchronizationContext> SynchronizationContext_var;
typedef ::CORBA::Nirvana::I_out <SynchronizationContext> SynchronizationContext_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::SynchronizationContext, NIRVANA_REPOSITORY_ID ("SynchronizationContext"))
void (*enter_memory) (Bridge <::Nirvana::SynchronizationContext>*, EnvironmentBridge*);
void (*enter) (Bridge <::Nirvana::SynchronizationContext>*, Type <Boolean>::ABI_in, EnvironmentBridge*);
::Nirvana::Pointer (*adopt_output) (Bridge <::Nirvana::SynchronizationContext>*, ::Nirvana::Pointer, ::Nirvana::UWord, ::Nirvana::UWord*, EnvironmentBridge*);
::Nirvana::Pointer (*allocate) (Bridge <::Nirvana::SynchronizationContext>*, ::Nirvana::UWord*, EnvironmentBridge*);
void (*async_call) (Bridge <::Nirvana::SynchronizationContext>*, Interface*, EnvironmentBridge*);
Type <Boolean>::ABI_ret (*_get_synchronized) (Bridge <::Nirvana::SynchronizationContext>*, EnvironmentBridge*);
Type <Boolean>::ABI_ret (*shared_memory) (Bridge <::Nirvana::SynchronizationContext>*, Interface*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::SynchronizationContext> :
	public T
{
public:
	/// Set current heap to this context heap.
	void enter_memory ();

	/// Enter to the synchronization domain.
	void enter (Boolean ret);

	/// Move output returned data from current context heap to this context heap.
	/// src memory will be released.
	::Nirvana::Pointer adopt_output (::Nirvana::Pointer src, ::Nirvana::UWord data_size, ::Nirvana::UWord& allocated_size);

	/// Allocate memory in this context.
	::Nirvana::Pointer allocate (::Nirvana::UWord& size);

	/// Call runnable in the new execution domain.
	/// Deadline by default is equal to the caller deadline.
	/// To change the deadline use Current::next_async_deadline.
	void async_call (TypeI <::Nirvana::Runnable>::C_in runnable);

	/// Returns `false` if there is no synchronization domain.
	Boolean synchronized ();

	/// Returns `true` if both contexts use common heap.
	/// For example, if it is the same context.
	Boolean shared_memory (TypeI <::Nirvana::SynchronizationContext>::C_in other);
};

template <class T>
void Client <T, ::Nirvana::SynchronizationContext>::enter_memory ()
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.enter_memory) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::SynchronizationContext>::enter (Boolean ret)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.enter) (&_b, ret, &_env);
	_env.check ();
}

template <class T>
::Nirvana::Pointer Client <T, ::Nirvana::SynchronizationContext>::adopt_output (
	::Nirvana::Pointer src, ::Nirvana::UWord data_size, ::Nirvana::UWord& allocated_size)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.adopt_output) (&_b, src, data_size, &allocated_size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::Pointer Client <T, ::Nirvana::SynchronizationContext>::allocate (::Nirvana::UWord& size)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.allocate) (&_b, &size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::SynchronizationContext>::async_call (TypeI <::Nirvana::Runnable>::C_in runnable)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.async_call) (&_b, &runnable, &_env);
	_env.check ();
}

template <class T>
Boolean Client <T, ::Nirvana::SynchronizationContext>::synchronized ()
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv._get_synchronized) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Boolean Client <T, ::Nirvana::SynchronizationContext>::shared_memory (TypeI <::Nirvana::SynchronizationContext>::C_in other)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv.shared_memory) (&_b, &other, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class SynchronizationContext : public ::CORBA::Nirvana::ClientInterface <SynchronizationContext>
{};

}

#endif
