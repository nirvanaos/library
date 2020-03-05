/**
~~~{.idl}
module Nirvana {

pseudo interface Current {
	readonly attribute SynchronizationContext synchronization_context;
	readonly attribute DeadlineTime deadline;
	DeadlineTime set_next_async_deadline (DeadlineTime dt);
};

}
~~~
*/
#ifndef NIRVANA_CURRENT_H_
#define NIRVANA_CURRENT_H_

#include "SynchronizationContext.h"

namespace Nirvana {

class Current;

typedef ::CORBA::Nirvana::I_ptr <Current> Current_ptr;
typedef ::CORBA::Nirvana::I_var <Current> Current_var;
typedef ::CORBA::Nirvana::I_out <Current> Current_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Current, NIRVANA_REPOSITORY_ID ("Current"))
Interface* (*synchronization_context) (Bridge <::Nirvana::Current>*, EnvironmentBridge*);
::Nirvana::DeadlineTime (*_get_deadline) (Bridge <::Nirvana::Current>*, EnvironmentBridge*);
::Nirvana::DeadlineTime (*set_next_async_deadline) (Bridge <::Nirvana::Current>*, ::Nirvana::DeadlineTime, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Current> :
	public T
{
public:
	::Nirvana::SynchronizationContext_ptr synchronization_context ();
	::Nirvana::DeadlineTime deadline ();
	::Nirvana::DeadlineTime set_next_async_deadline (::Nirvana::DeadlineTime dl);
};

template <class T>
::Nirvana::SynchronizationContext_ptr Client <T, ::Nirvana::Current>::synchronization_context ()
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	I_VT_ret < ::Nirvana::SynchronizationContext> _ret = (_b._epv ().epv.synchronization_context) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::DeadlineTime Client <T, ::Nirvana::Current>::deadline ()
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	::Nirvana::DeadlineTime _ret = (_b._epv ().epv._get_deadline) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
::Nirvana::DeadlineTime Client <T, ::Nirvana::Current>::set_next_async_deadline (::Nirvana::DeadlineTime dl)
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	::Nirvana::DeadlineTime _ret = (_b._epv ().epv.set_next_async_deadline) (&_b, dl, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Current : public ::CORBA::Nirvana::ClientInterface <Current>
{};

}

#endif
