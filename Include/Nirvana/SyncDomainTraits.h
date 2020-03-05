/**
~~~{.idl}
module Nirvana {

pseudo interface SyncDomainTraits {
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

template <>
struct Type <::Nirvana::ContextFrame> :
	public TypeFixLen <::Nirvana::ContextFrame>
{};

BRIDGE_BEGIN (::Nirvana::SyncDomainTraits, NIRVANA_REPOSITORY_ID ("SyncDomainTraits"))
::Nirvana::Pointer (*copy_inout) (Bridge <::Nirvana::SyncDomainTraits>*, ::Nirvana::ConstPointer, ::Nirvana::UWord, EnvironmentBridge*);
::Nirvana::Pointer (*move_out) (Bridge <::Nirvana::SyncDomainTraits>*, ::Nirvana::Pointer, ::Nirvana::UWord, EnvironmentBridge*);
void (*enter) (Bridge <::Nirvana::SyncDomainTraits>*, Type <::Nirvana::ContextFrame>::ABI_out, EnvironmentBridge*);
void (*leave) (Bridge <::Nirvana::SyncDomainTraits>*, Type <::Nirvana::ContextFrame>::ABI_in, EnvironmentBridge*);
void (*async_call) (Bridge <::Nirvana::SyncDomainTraits>*, Interface*, EnvironmentBridge*);
Type <Boolean>::ABI_ret (*_get_synchronized) (Bridge <::Nirvana::SyncDomainTraits>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::SyncDomainTraits> :
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
::Nirvana::Pointer Client <T, ::Nirvana::SyncDomainTraits>::copy_inout (::Nirvana::ConstPointer src, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::SyncDomainTraits>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.copy_inout) (&_b, src, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::Pointer Client <T, ::Nirvana::SyncDomainTraits>::move_out (::Nirvana::Pointer src, ::Nirvana::UWord size)
{
	Environment _env;
	Bridge < ::Nirvana::SyncDomainTraits>& _b (T::_get_bridge (_env));
	::Nirvana::Pointer _ret = (_b._epv ().epv.move_out) (&_b, src, size, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::SyncDomainTraits>::enter (Type <::Nirvana::ContextFrame>::C_out context)
{
	Environment _env;
	Bridge < ::Nirvana::SyncDomainTraits>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.enter) (&_b, &context, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::SyncDomainTraits>::leave (Type <::Nirvana::ContextFrame>::C_in context)
{
	Environment _env;
	Bridge < ::Nirvana::SyncDomainTraits>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.leave) (&_b, &context, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::SyncDomainTraits>::async_call (TypeI <::Nirvana::Runnable>::C_in runnable)
{
	Environment _env;
	Bridge < ::Nirvana::SyncDomainTraits>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.async_call) (&_b, &runnable, &_env);
	_env.check ();
}

template <class T>
Boolean Client <T, ::Nirvana::SyncDomainTraits>::synchronized ()
{
	Environment _env;
	Bridge < ::Nirvana::SyncDomainTraits>& _b (T::_get_bridge (_env));
	Type <Boolean>::C_ret _ret = (_b._epv ().epv._get_synchronized) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class SyncDomainTraits : public ::CORBA::Nirvana::ClientInterface <SyncDomainTraits>
{};

}

#endif
