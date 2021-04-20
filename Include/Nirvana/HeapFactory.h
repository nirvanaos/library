// This file was generated from "HeapFactory.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_HEAPFACTORY_H_
#define IDL_HEAPFACTORY_H_

namespace Nirvana {

class Memory;
typedef ::CORBA::Nirvana::TypeItf <Memory>::C_ptr Memory_ptr;
typedef ::CORBA::Nirvana::TypeItf <Memory>::C_var Memory_var;

class HeapFactory;
typedef ::CORBA::Nirvana::TypeItf <HeapFactory>::C_ptr HeapFactory_ptr;
typedef ::CORBA::Nirvana::TypeItf <HeapFactory>::C_var HeapFactory_var;

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
	TypeItf < ::Nirvana::Memory>::Var create ();
	TypeItf < ::Nirvana::Memory>::Var create_with_granularity (Type <ULong>::C_in granularity);
};

template <class T>
TypeItf < ::Nirvana::Memory>::Var Client <T, ::Nirvana::HeapFactory>::create ()
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	TypeItf < ::Nirvana::Memory>::C_ret _ret = (_b._epv ().epv.create) (&_b, &_env);
	_env.check ();
	return _ret;
}

template <class T>
TypeItf < ::Nirvana::Memory>::Var Client <T, ::Nirvana::HeapFactory>::create_with_granularity (Type <ULong>::C_in granularity)
{
	Environment _env;
	Bridge < ::Nirvana::HeapFactory>& _b (T::_get_bridge (_env));
	TypeItf < ::Nirvana::Memory>::C_ret _ret = (_b._epv ().epv.create_with_granularity) (&_b, &granularity, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class HeapFactory : public CORBA::Nirvana::ClientInterface <HeapFactory>
{
public:
};

}

#endif
