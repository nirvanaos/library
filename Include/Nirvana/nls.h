// This file was generated from "nls.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_NLS_H_
#define IDL_NLS_H_

namespace Nirvana {

class Locale;

}

namespace CORBA {
namespace Internal {

template <>
struct Type < ::Nirvana::Locale> : TypeItf < ::Nirvana::Locale>
{};

template <>
struct Definitions < ::Nirvana::Locale>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Locale, "IDL:Nirvana/Locale:1.0")
void (*to_wide) (Bridge < ::Nirvana::Locale>* _b, Type <String>::ABI_in, Type <WString>::ABI_out, Interface* _env);
void (*to_multibyte) (Bridge < ::Nirvana::Locale>* _b, Type <WString>::ABI_in, Type <String>::ABI_out, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Locale> :
	public T,
	public Definitions < ::Nirvana::Locale>
{
public:
	void to_wide (Type <String>::C_in multibute, Type <WString>::C_out wide);
	void to_multibyte (Type <WString>::C_in wide, Type <String>::C_out multibyte);
};

template <class T>
void Client <T, ::Nirvana::Locale>::to_wide (Type <String>::C_in multibute, Type <WString>::C_out wide)
{
	Environment _env;
	Bridge < ::Nirvana::Locale>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.to_wide) (&_b, &multibute, &wide, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::Locale>::to_multibyte (Type <WString>::C_in wide, Type <String>::C_out multibyte)
{
	Environment _env;
	Bridge < ::Nirvana::Locale>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.to_multibyte) (&_b, &wide, &multibyte, &_env);
	_env.check ();
}

}
}

namespace Nirvana {

class Locale : public ::CORBA::Internal::ClientInterface <Locale>
{
public:
};

}

#endif
