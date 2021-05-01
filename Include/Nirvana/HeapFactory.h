// This file was generated from "HeapFactory.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_HEAPFACTORY_H_
#define IDL_HEAPFACTORY_H_

namespace Nirvana {

class Memory;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::Memory> : TypeItf < ::Nirvana::Memory>
{};

}
}

namespace Nirvana {

class HeapFactory;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::HeapFactory> : TypeItf < ::Nirvana::HeapFactory>
{};

template <>
struct Definitions < ::Nirvana::HeapFactory>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::HeapFactory, "IDL:Nirvana/HeapFactory:1.0")
Interface* (*create) (Bridge < ::Nirvana::HeapFactory>* _b, Interface* _env);
Interface* (*create_with_granularity) (Bridge < ::Nirvana::HeapFactory>* _b, Type <ULong>::ABI_in, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::HeapFactory> :
	public T,
	public Definitions < ::Nirvana::HeapFactory>
{
public:
	Type < ::Nirvana::Memory>::Var create ();
	Type < ::Nirvana::Memory>::Var create_with_granularity (Type <ULong>::C_in granularity);
};

template <class T>
Type < ::Nirvana::Memory>::Var Client <T, ::Nirvana::HeapFactory>::create ()
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::Memory>::C_ret _ret = (_b._epv ().epv.create) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
Type < ::Nirvana::Memory>::Var Client <T, ::Nirvana::HeapFactory>::create_with_granularity (Type <ULong>::C_in granularity)
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::Memory>::C_ret _ret = (_b._epv ().epv.create_with_granularity) (&_b, &granularity, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class HeapFactory : public ::CORBA::Nirvana::ClientInterface <HeapFactory>
{
public:
};

}

#endif
