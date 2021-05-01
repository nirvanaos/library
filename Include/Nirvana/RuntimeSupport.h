// This file was generated from "RuntimeSupport.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_RUNTIMESUPPORT_H_
#define IDL_RUNTIMESUPPORT_H_

namespace Nirvana {

class RuntimeProxy;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::RuntimeProxy> : TypeItf < ::Nirvana::RuntimeProxy>
{};

template <>
struct Definitions < ::Nirvana::RuntimeProxy>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::RuntimeProxy, "IDL:Nirvana/RuntimeProxy:1.0")
Type < ::Nirvana::ConstPointer>::ABI_ret (*object) (Bridge < ::Nirvana::RuntimeProxy>* _b, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::RuntimeProxy> :
	public T,
	public Definitions < ::Nirvana::RuntimeProxy>
{
public:
	Type < ::Nirvana::ConstPointer>::Var object ();
};

template <class T>
Type < ::Nirvana::ConstPointer>::Var Client <T, ::Nirvana::RuntimeProxy>::object ()
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeProxy>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::ConstPointer>::C_ret _ret = (_b._epv ().epv.object) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class RuntimeProxy : public ::CORBA::Nirvana::ClientInterface <RuntimeProxy>
{
public:
};

class RuntimeSupport;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::RuntimeSupport> : TypeItf < ::Nirvana::RuntimeSupport>
{};

template <>
struct Definitions < ::Nirvana::RuntimeSupport>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::RuntimeSupport, "IDL:Nirvana/RuntimeSupport:1.0")
Interface* (*runtime_proxy_get) (Bridge < ::Nirvana::RuntimeSupport>* _b, Type < ::Nirvana::ConstPointer>::ABI_in, Interface* _env);
void (*runtime_proxy_remove) (Bridge < ::Nirvana::RuntimeSupport>* _b, Type < ::Nirvana::ConstPointer>::ABI_in, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::RuntimeSupport> :
	public T,
	public Definitions < ::Nirvana::RuntimeSupport>
{
public:
	Type < ::Nirvana::RuntimeProxy>::Var runtime_proxy_get (Type < ::Nirvana::ConstPointer>::C_in obj);
	void runtime_proxy_remove (Type < ::Nirvana::ConstPointer>::C_in obj);
};

template <class T>
Type < ::Nirvana::RuntimeProxy>::Var Client <T, ::Nirvana::RuntimeSupport>::runtime_proxy_get (Type < ::Nirvana::ConstPointer>::C_in obj)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	Type < ::Nirvana::RuntimeProxy>::C_ret _ret = (_b._epv ().epv.runtime_proxy_get) (&_b, &obj, &_env);
	_env.check ();
	return _ret;
}

template <class T>
void Client <T, ::Nirvana::RuntimeSupport>::runtime_proxy_remove (Type < ::Nirvana::ConstPointer>::C_in obj)
{
	Environment _env;
	Bridge < ::Nirvana::RuntimeSupport>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.runtime_proxy_remove) (&_b, &obj, &_env);
	_env.check ();
}

}
}

namespace Nirvana {

class RuntimeSupport : public ::CORBA::Nirvana::ClientInterface <RuntimeSupport>
{
public:
};

}

#endif
