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

template <>
class Bridge < ::Nirvana::Binder> :
	public BridgeMarshal < ::Nirvana::Binder>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			Bridge <Interface>* (*bind) (Bridge < ::Nirvana::Binder>*, const StringABI <char>*, const StringABI <char>*, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_[];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal < ::Nirvana::Binder> (epv.interface)
	{}
};

template <class T>
class Client <T, ::Nirvana::Binder> :
	public T
{
public:
	Bridge <Interface>* bind (CORBA::String_in name, CORBA::String_in interface_id);

	template <class I>
	I_var <I> bind (CORBA::String_in name)
	{
		return static_cast <I*> (bind (name, I::interface_id_));
	}
};

template <class T>
Bridge <Interface>* Client <T, ::Nirvana::Binder>::bind (CORBA::String_in name, CORBA::String_in interface_id)
{
	Environment _env;
	Bridge < ::Nirvana::Binder>& _b (T::_get_bridge (_env));
	Bridge <Interface>* _ret = (_b._epv ().epv.bind) (&_b, &name, &interface_id, &_env);
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
