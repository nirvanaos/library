// This file was generated from "Module.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_MODULE_H_
#define IDL_MODULE_H_

namespace Nirvana {

class Module;
typedef ::CORBA::Nirvana::TypeItf <Module>::C_ptr Module_ptr;
typedef ::CORBA::Nirvana::TypeItf <Module>::C_var Module_var;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::Module> : TypeItf < ::Nirvana::Module>
{};

template <>
struct Definitions < ::Nirvana::Module>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Module, "IDL:Nirvana/Module:1.0")
Type < ::Nirvana::ConstPointer>::ABI_ret (*_get_base_address) (Bridge < ::Nirvana::Module>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Module> :
	public T,
	public Definitions < ::Nirvana::Module>
{
public:
	Type < ::Nirvana::ConstPointer>::Var base_address ();
};

template <class T>
Type < ::Nirvana::ConstPointer>::Var Client <T, ::Nirvana::Module>::base_address ()
{
	Environment _env;
	Bridge < ::Nirvana::Module>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::ConstPointer>::C_ret _ret = (_b._epv ().epv._get_base_address) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Module : public CORBA::Nirvana::ClientInterface <Module>
{
public:
};

}

#endif
