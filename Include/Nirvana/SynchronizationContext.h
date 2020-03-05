/**
~~~{.idl}
module Nirvana {

pseudo interface SynchronizationContext {
	/// Copy inout data from the current domain memory to target domain memory
	Pointer copy_inout (ConstPointer src, UWord size);

	/// Move out (ret) data from target domain memory to current domain memory
	/// src memory will be released
	Pointer move_out (Pointer src, UWord size);

	/// Enter to the target synchronization domain
	void enter (out ContextFrame frame);

	/// Return to the current synchronization domain
	void leave (in ContextFrame frame);

	/// Call runnable in the new execution domain.
	/// Deadline by default is equal to the caller deadline.
	/// To change the deadline use `Current::set_next_async_deadline ()`.
	oneway void async_call (Runnable runnable);

	/// Returns `false` if there is no synchronization domain.
	readonly attribute boolean synchronized;
};

}
~~~
*/

#ifndef NIRVANA_SYNCHRONIZATIONCONTEXT_H_
#define NIRVANA_SYNCHRONIZATIONCONTEXT_H_

#include "Runnable.h"

namespace Nirvana {

struct ContextFrame
{
	void (*function) (void*);
	void* parameter;
};

class SynchronizationContext;

typedef ::CORBA::Nirvana::I_ptr <SynchronizationContext> SynchronizationContext_ptr;
typedef ::CORBA::Nirvana::I_var <SynchronizationContext> SynchronizationContext_var;
typedef ::CORBA::Nirvana::I_out <SynchronizationContext> SynchronizationContext_out;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type <::Nirvana::ContextFrame> :
	public TypeFixLen <::Nirvana::ContextFrame>
{};

BRIDGE_BEGIN (::Nirvana::SynchronizationContext, NIRVANA_REPOSITORY_ID ("SynchronizationContext"))
::Nirvana::Pointer (*copy_inout) (Bridge <::Nirvana::SynchronizationContext>*, ::Nirvana::ConstPointer, ::Nirvana::UWord, EnvironmentBridge*);
::Nirvana::Pointer (*move_out) (Bridge <::Nirvana::SynchronizationContext>*, ::Nirvana::Pointer, ::Nirvana::UWord, EnvironmentBridge*);
void (*enter) (Bridge <::Nirvana::SynchronizationContext>*, Type <::Nirvana::ContextFrame>::ABI_out, EnvironmentBridge*);
void (*leave) (Bridge <::Nirvana::SynchronizationContext>*, Type <::Nirvana::ContextFrame>::ABI_in, EnvironmentBridge*);
void (*async_call) (Bridge <::Nirvana::SynchronizationContext>*, Interface*, EnvironmentBridge*);
Type <Boolean>::ABI_ret (*_get_synchronized) (Bridge <::Nirvana::SynchronizationContext>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::SynchronizationContext> :
	public T
{
public:
	/// Copy inout data from the current domain memory to target domain memory
	::Nirvana::Pointer copy_inout (::Nirvana::ConstPointer src, ::Nirvana::UWord size);

	/// Move out (ret) data from target domain memory to current domain memory
	/// src memory will be released
	::Nirvana::Pointer move_out (::Nirvana::Pointer src, ::Nirvana::UWord size);

	/// Enter to the target synchronization domain
	void enter (Type <::Nirvana::ContextFrame>::C_out context);

	/// Return to the current synchronization domain
	void leave (Type <::Nirvana::ContextFrame>::C_in context);

	/// Call runnable in the new execution domain.
	/// Deadline by default is equal to the caller deadline.
	/// To change the deadline use Current::next_async_deadline.
	void async_call (TypeI <::Nirvana::Runnable>::C_in runnable);

	Boolean synchronized ();
};

template <class T>
::Nirvana::Pointer Client <T, ::Nirvana::SynchronizationContext>::copy_inout (::Nirvana::ConstPointer src, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.copy_inout) (&_b, src, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::Pointer Client <T, ::Nirvana::SynchronizationContext>::move_out (::Nirvana::Pointer src, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.move_out) (&_b, src, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::SynchronizationContext>::enter (Type <::Nirvana::ContextFrame>::C_out context)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.enter) (&_b, &context, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::SynchronizationContext>::leave (Type <::Nirvana::ContextFrame>::C_in context)
{
	Environment _env;
	Bridge < ::Nirvana::SynchronizationContext>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.leave) (&_b, &context, &_env);
	_env.check ();
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

}
}

namespace Nirvana {

class SynchronizationContext : public ::CORBA::Nirvana::ClientInterface <SynchronizationContext>
{};

}

#endif
