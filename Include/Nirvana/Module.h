/**
~~~{.idl}
module Nirvana {

/// Module interface
pseudo interface Module
{
  /// Returns module base address.
  readonly attribute ConstPointer base_address;
};

}
~~~
*/
#ifndef NIRVANA_MODULE_H_
#define NIRVANA_MODULE_H_

#include <CORBA/Client.h>

namespace Nirvana {

class Module;
typedef ::CORBA::Nirvana::I_ptr <Module> Module_ptr;
typedef ::CORBA::Nirvana::I_var <Module> Module_var;
typedef ::CORBA::Nirvana::I_out <Module> Module_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Module, NIRVANA_REPOSITORY_ID ("Module"))
::Nirvana::ConstPointer (*get_base_address) (Bridge <::Nirvana::Module>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Module> :
	public T
{
public:
	::Nirvana::ConstPointer base_address ();
};

template <class T>
::Nirvana::ConstPointer Client <T, ::Nirvana::Module>::base_address ()
{
	Environment _env;
	Bridge < ::Nirvana::Module>& _b (T::_get_bridge (_env));
	::Nirvana::ConstPointer _ret = (_b._epv ().epv.get_base_address) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Module : public ::CORBA::Nirvana::ClientInterface <Module>
{};

}

#endif
