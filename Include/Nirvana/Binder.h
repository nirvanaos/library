// This file was generated from "Binder.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_BINDER_H_
#define IDL_BINDER_H_

namespace Nirvana {

class Binder;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::Binder> : TypeItf < ::Nirvana::Binder>
{};

}
}

namespace Nirvana {

#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Nirvana::Type <Binder>::C_ptr Binder_ptr;
typedef ::CORBA::Nirvana::Type <Binder>::C_var Binder_var;
typedef Binder_var& Binder_out;
#endif

}

namespace CORBA {
namespace Nirvana {

template <>
struct Definitions < ::Nirvana::Binder>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Binder, "IDL:Nirvana/Binder:1.0")
Interface* (*bind) (Bridge < ::Nirvana::Binder>* _b, Type <String>::ABI_in, Type <String>::ABI_in, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Binder> :
	public T,
	public Definitions < ::Nirvana::Binder>
{
public:
	Type <Interface>::Var bind (Type <String>::C_in name, Type <String>::C_in interface_id);
	template <class I>
	typename Type <I>::Var bind (Type <String>::C_in name)
	{
		return bind (name, I::repository_id_).downcast <I> ();
	}
};

template <class T>
Type <Interface>::Var Client <T, ::Nirvana::Binder>::bind (Type <String>::C_in name, Type <String>::C_in interface_id)
{
	Environment _env;
	Bridge < ::Nirvana::Binder>& _b (T::_get_bridge (_env));
	Type <Interface>::C_ret _ret = (_b._epv ().epv.bind) (&_b, &name, &interface_id, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Binder : public ::CORBA::Nirvana::ClientInterface <Binder>
{
public:
};

}

#endif
