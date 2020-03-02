/**
~~~{.idl}
module Nirvana {

pseudo interface Current {
	readonly attribute SyncDomainTraits sync_domain_traits;
	readonly attribute DeadlineTime deadline;
	attribute DeadlineTime next_async_deadline;
};

}
~~~
*/
#ifndef NIRVANA_CURRENT_H_
#define NIRVANA_CURRENT_H_

#include "SyncDomainTraits.h"

namespace Nirvana {

class Current;

typedef ::CORBA::Nirvana::I_ptr <Current> Current_ptr;
typedef ::CORBA::Nirvana::I_var <Current> Current_var;
typedef ::CORBA::Nirvana::I_out <Current> Current_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Current, NIRVANA_REPOSITORY_ID (Current))
Interface* (*sync_domain_traits) (Bridge <::Nirvana::Current>*, EnvironmentBridge*);
::Nirvana::DeadlineTime (*_get_deadline) (Bridge <::Nirvana::Current>*, EnvironmentBridge*);
::Nirvana::DeadlineTime (*_get_next_async_deadline) (Bridge <::Nirvana::Current>*, EnvironmentBridge*);
void (*_set_next_async_deadline) (Bridge <::Nirvana::Current>*, ::Nirvana::DeadlineTime, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Current> :
	public T
{
public:
	::Nirvana::SyncDomainTraits_var sync_domain_traits ();
	::Nirvana::DeadlineTime deadline ();
	::Nirvana::DeadlineTime next_async_deadline ();
	void next_async_deadline (::Nirvana::DeadlineTime dl);
};

template <class T>
::Nirvana::SyncDomainTraits_var Client <T, ::Nirvana::Current>::sync_domain_traits ()
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	I_ret < ::Nirvana::SyncDomainTraits> _ret = (_b._epv ().epv.sync_domain_traits) (&_b, &_env);
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
::Nirvana::DeadlineTime Client <T, ::Nirvana::Current>::next_async_deadline ()
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	::Nirvana::DeadlineTime _ret = (_b._epv ().epv._get_next_async_deadline) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::Current>::next_async_deadline (::Nirvana::DeadlineTime dl)
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	(_b._epv ().epv._set_next_async_deadline) (&_b, dl, &_env);
	_env.check ();
}

}
}

namespace Nirvana {

class Current : public ::CORBA::Nirvana::ClientInterface <Current>
{};

}

#endif
