/**
~~~{.idl}
module Nirvana {

/// ModuleInit interface
/// Module exports exactly one instance of this interface with instance name "main".
/// If the module is a "singleton", the interface id must be `NIRVANA_REPOSITORY_ID ("ModuleSingleton")`.
pseudo interface ModuleInit
{
  /// Constructs all module static objects.
  void initialize ();

  /// Destructs all module static objects.
  void terminate ();
};

}
~~~
*/
#ifndef NIRVANA_MODULEINIT_H_
#define NIRVANA_MODULEINIT_H_

#include <CORBA/Client.h>

namespace Nirvana {

class ModuleInit;
typedef ::CORBA::Nirvana::I_ptr <ModuleInit> ModuleInit_ptr;
typedef ::CORBA::Nirvana::I_var <ModuleInit> ModuleInit_var;
typedef ::CORBA::Nirvana::I_out <ModuleInit> ModuleInit_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::ModuleInit, NIRVANA_REPOSITORY_ID ("ModuleInit"))
void (*initialize) (Bridge <::Nirvana::ModuleInit>*, Interface*);
void (*terminate) (Bridge <::Nirvana::ModuleInit>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::ModuleInit> :
	public T
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

class Module : public ::CORBA::Nirvana::ClientInterface <ModuleInit>
{};

}

#endif
