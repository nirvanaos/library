#ifndef NIRVANA_BINDER_H_
#define NIRVANA_BINDER_H_

#include "CORBA/CORBA.h"

namespace Nirvana {

class Binder;
typedef ::CORBA::Nirvana::I_ptr <Binder> Binder_ptr;
typedef ::CORBA::Nirvana::I_var <Binder> Binder_var;
typedef ::CORBA::Nirvana::I_out <Binder> Binder_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Binder, NIRVANA_REPOSITORY_ID ("Binder"))
	Interface* (*bind) (Bridge < ::Nirvana::Binder>*, Type <String>::ABI_in, Type <String>::ABI_in, EnvironmentBridge*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Binder> :
	public T
{
public:
	Interface_var bind (CORBA::String_in name, CORBA::String_in interface_id);

	template <class I>
	I_var <I> bind (CORBA::String_in name)
	{
		return static_cast <I*> (bind (name, I::interface_id_));
	}
};

template <class T>
Interface_var Client <T, ::Nirvana::Binder>::bind (CORBA::String_in name, CORBA::String_in interface_id)
{
	Environment _env;
	Bridge < ::Nirvana::Binder>& _b (T::_get_bridge (_env));
	TypeI <Interface>::C_ret _ret ((_b._epv ().epv.bind) (&_b, &name, &interface_id, &_env));
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Binder : public ::CORBA::Nirvana::ClientInterface <Binder>
{};

}

#endif
