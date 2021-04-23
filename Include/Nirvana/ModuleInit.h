// This file was generated from "ModuleInit.idl"
// Nirvana IDL compiler version 1.0
#ifndef IDL_MODULEINIT_H_
#define IDL_MODULEINIT_H_

namespace Nirvana {

class ModuleInit;
#ifdef LEGACY_CORBA_CPP
typedef ::CORBA::Nirvana::TypeItf <ModuleInit>::C_ptr ModuleInit_ptr;
typedef ::CORBA::Nirvana::TypeItf <ModuleInit>::C_var ModuleInit_var;
typedef ModuleInit_var& ModuleInit_out;
#endif

}

namespace CORBA {
namespace Nirvana {

template <>
struct Type < ::Nirvana::ModuleInit> : TypeItf < ::Nirvana::ModuleInit>
{};

template <>
struct Definitions < ::Nirvana::ModuleInit>
{
};

NIRVANA_BRIDGE_BEGIN (::Nirvana::ModuleInit, "IDL:Nirvana/ModuleInit:1.0")
void (*initialize) (Bridge < ::Nirvana::ModuleInit>* _b, Interface* _env);
void (*terminate) (Bridge < ::Nirvana::ModuleInit>* _b, Interface* _env);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::ModuleInit> :
	public T,
	public Definitions < ::Nirvana::ModuleInit>
{
public:
	void initialize ();
	void terminate ();
};

template <class T>
void Client <T, ::Nirvana::ModuleInit>::initialize ()
{
	Environment _env;
	Bridge < ::Nirvana::ModuleInit>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.initialize) (&_b, &_env);
	_env.check ();
}

template <class T>
void Client <T, ::Nirvana::ModuleInit>::terminate ()
{
	Environment _env;
	Bridge < ::Nirvana::ModuleInit>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.terminate) (&_b, &_env);
	_env.check ();
}

}
}

namespace Nirvana {

class ModuleInit : public ::CORBA::Nirvana::ClientInterface <ModuleInit>
{
public:
};

}

#endif
