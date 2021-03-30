/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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

pseudo interface Current {
	readonly attribute DeadlineTime deadline;
};

}
~~~
*/
#ifndef NIRVANA_CURRENT_H_
#define NIRVANA_CURRENT_H_

namespace Nirvana {

class Current;

typedef ::CORBA::Nirvana::I_ptr <Current> Current_ptr;
typedef ::CORBA::Nirvana::I_var <Current> Current_var;
typedef ::CORBA::Nirvana::I_out <Current> Current_out;

}

namespace CORBA {
namespace Nirvana {

BRIDGE_BEGIN (::Nirvana::Current, NIRVANA_REPOSITORY_ID ("Current"))
::Nirvana::DeadlineTime (*_get_deadline) (Bridge <::Nirvana::Current>*, Interface*);
BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Current> :
	public T
{
public:
	::Nirvana::DeadlineTime deadline ();
};

template <class T>
::Nirvana::DeadlineTime Client <T, ::Nirvana::Current>::deadline ()
{
	Environment _env;
	Bridge < ::Nirvana::Current>& _b (T::_get_bridge (_env));
	::Nirvana::DeadlineTime _ret = (_b._epv ().epv._get_deadline) (&_b, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {

class Current : public ::CORBA::Nirvana::ClientInterface <Current>
{};

}

#endif
