/// \file
/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/

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

namespace Nirvana {

class Module;
typedef ::CORBA::Internal::I_ptr <Module> Module_ptr;
typedef ::CORBA::Internal::I_var <Module> Module_var;

}

namespace CORBA {
namespace Internal {

NIRVANA_BRIDGE_BEGIN (::Nirvana::Module, NIRVANA_REPOSITORY_ID ("Module"))
::Nirvana::ConstPointer (*get_base_address) (Bridge <::Nirvana::Module>*, Interface*);
NIRVANA_BRIDGE_END ()

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

class Module : public ::CORBA::Internal::ClientInterface <Module>
{};

}

#endif
