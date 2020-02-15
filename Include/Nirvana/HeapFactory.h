#ifndef NIRVANA_HEAPFACTORY_H_
#define NIRVANA_HEAPFACTORY_H_

#include "Memory.h"

namespace Nirvana {

class HeapFactory;
typedef ::CORBA::Nirvana::I_ptr <HeapFactory> HeapFactory_ptr;
typedef ::CORBA::Nirvana::I_var <HeapFactory> HeapFactory_var;
typedef ::CORBA::Nirvana::I_out <HeapFactory> HeapFactory_out;

}

namespace CORBA {
namespace Nirvana {

template <>
class Bridge < ::Nirvana::HeapFactory> :
	public BridgeMarshal < ::Nirvana::HeapFactory>
{
public:
	struct EPV
	{
		Bridge <Interface>::EPV interface;

		struct
		{
			BridgeMarshal < ::Nirvana::Memory>* (*create) (Bridge <::Nirvana::HeapFactory>*, EnvironmentBridge*);
			BridgeMarshal < ::Nirvana::Memory>* (*create_with_granularity) (Bridge <::Nirvana::HeapFactory>*, ULong granularity, EnvironmentBridge*);
		}
		epv;
	};

	const EPV& _epv () const
	{
		return (EPV&)Bridge <Interface>::_epv ();
	}

	static const Char interface_id_ [];

protected:
	Bridge (const EPV& epv) :
		BridgeMarshal < ::Nirvana::HeapFactory> (epv.interface)
	{}
};

template <class T>
class Client <T, ::Nirvana::HeapFactory> :
	public T
{
public:
	I_var < ::Nirvana::Memory> create ();
	I_var < ::Nirvana::Memory> create_with_granularity (ULong granularity);
};

template <class T>
I_var < ::Nirvana::Memory> Client <T, ::Nirvana::HeapFactory>::create ()
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	I_ret < ::Nirvana::Memory> _ret = (_b._epv ().epv.create) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
I_var < ::Nirvana::Memory> Client <T, ::Nirvana::HeapFactory>::create_with_granularity (ULong granularity)
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	I_ret < ::Nirvana::Memory> _ret = (_b._epv ().epv.create_with_granularity) (&_b, granularity, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class HeapFactory : public ::CORBA::Nirvana::ClientInterface <HeapFactory>
{};

}

#endif
