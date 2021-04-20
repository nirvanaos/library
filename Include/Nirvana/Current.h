// This file was generated from "Current.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_CURRENT_H_
#define IDL_CURRENT_H_

namespace Nirvana {

class Current;
typedef ::CORBA::Nirvana::TypeItf <Current>::C_ptr Current_ptr;
typedef ::CORBA::Nirvana::TypeItf <Current>::C_var Current_var;

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::Current> : TypeItf < ::Nirvana::Current>
{};

template <>
struct Definitions < ::Nirvana::Current>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Current, "IDL:Nirvana/Current:1.0")
Type <ULongLong>::ABI_ret (*_get_deadline) (Bridge < ::Nirvana::Current>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Current> :
	public T,
	public Definitions < ::Nirvana::Current>
{
public:
	Type <ULongLong>::Var deadline ();
};

template <class T>
Type <ULongLong>::Var Client <T, ::Nirvana::Current>::deadline ()
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	Type <ULongLong>::C_ret _ret = (_b._epv ().epv._get_deadline) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Current : public CORBA::Nirvana::ClientInterface <Current>
{
public:
};

}

#endif
