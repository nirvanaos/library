/**
~~~{.idl}
module Nirvana {

pseudo interface Current {
	readonly attribute DeadlineTime deadline;
};

}
~~~
*/
#ifndef NIRVANA_CURRENT_H_
#define NIRVANA_CURRENT_H_

namespace Nirvana {

class Current;

typedef ::CORBA::Nirvana::I_ptr <Current> Current_ptr;
typedef ::CORBA::Nirvana::I_var <Current> Current_var;
typedef ::CORBA::Nirvana::I_out <Current> Current_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Current, NIRVANA_REPOSITORY_ID ("Current"))
::Nirvana::DeadlineTime (*_get_deadline) (Bridge <::Nirvana::Current>*, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Current> :
	public T
{
public:
	::Nirvana::DeadlineTime deadline ();
};

template <class T>
::Nirvana::DeadlineTime Client <T, ::Nirvana::Current>::deadline ()
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	::Nirvana::DeadlineTime _ret = (_b._epv ().epv._get_deadline) (&_b, &_env);
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
