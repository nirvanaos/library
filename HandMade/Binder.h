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
#ifndef NIRVANA_BINDER_H_
#define NIRVANA_BINDER_H_

#include "CORBA/CORBA.h"

namespace Nirvana {

class Binder;
typedef ::CORBA::Internal::I_ptr <Binder> Binder_ptr;
typedef ::CORBA::Internal::I_var <Binder> Binder_var;

}

namespace CORBA {
namespace Internal {

template <>
struct Type < ::Nirvana::Binder> : TypeItf < ::Nirvana::Binder>
{};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Binder, NIRVANA_REPOSITORY_ID ("Binder"))
	Interface* (*bind) (Bridge < ::Nirvana::Binder>*, Type <String>::ABI_in, Type <String>::ABI_in, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Binder> :
	public T
{
public:
	TypeItf <Interface>::Var bind (CORBA::String_in name, CORBA::String_in interface_id);

	template <class I>
	typename TypeItf <I>::Var bind (CORBA::String_in name)
	{
		return bind (name, I::repository_id_).downcast <I> ();
	}
};

template <class T>
Interface_var Client <T, ::Nirvana::Binder>::bind (CORBA::String_in name, CORBA::String_in interface_id)
{
	Environment _env;
	Bridge < ::Nirvana::Binder>& _b (T::_get_bridge (_env));
	Type <Interface>::C_ret _ret ((_b._epv ().epv.bind) (&_b, &name, &interface_id, &_env));
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Binder : public ::CORBA::Internal::ClientInterface <Binder>
{};

}

#endif
