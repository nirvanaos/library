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
#ifndef NIRVANA_LEGACY_RUNNABLE_H_
#define NIRVANA_LEGACY_RUNNABLE_H_

namespace Nirvana {
namespace Legacy {

class Runnable;
typedef ::CORBA::Internal::I_ptr <Runnable> Runnable_ptr;
typedef ::CORBA::Internal::I_var <Runnable> Runnable_var;

}
}

namespace CORBA {
namespace Internal {

template <>
struct Type < ::Nirvana::Legacy::Runnable> : TypeItf < ::Nirvana::Legacy::Runnable>
{};

NIRVANA_BRIDGE_BEGIN (::Nirvana::Legacy::Runnable, NIRVANA_REPOSITORY_ID ("Legacy/Runnable"))
	void (*run) (Bridge <::Nirvana::Legacy::Runnable>*, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Legacy::Runnable> :
	public T
{
public:
	void run ();
};

template <class T>
void Client <T, ::Nirvana::Legacy::Runnable>::run ()
{
	Environment _env;
	Bridge < ::Nirvana::Legacy::Runnable>& _b (T::_get_bridge (_env));
	(_b._epv ().epv.run) (&_b, &_env);
	_env.check ();
}

}
}

namespace Nirvana {
namespace Legacy {

class Runnable : public ::CORBA::Internal::ClientInterface <Runnable>
{};

}
}

#endif
