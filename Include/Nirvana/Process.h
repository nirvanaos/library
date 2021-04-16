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
module Legacy {

/// Process interface.
/// Nirvana legacy executable module must export exactly one instance of
/// this interface with instance name "main".
pseudo interface Process
{
	Word main (Size argc, char** argv);
};

}
}
~~~
*/
#ifndef NIRVANA_LEGACY_PROCESS_H_
#define NIRVANA_LEGACY_PROCESS_H_

namespace Nirvana {
namespace Legacy {

class Process;
typedef ::CORBA::Nirvana::I_ptr <Process> Process_ptr;
typedef ::CORBA::Nirvana::I_var <Process> Process_var;
typedef ::CORBA::Nirvana::I_out <Process> Process_out;

}
}

namespace CORBA {
namespace Nirvana {

NIRVANA_BRIDGE_BEGIN (::Nirvana::Legacy::Process, NIRVANA_REPOSITORY_ID ("Legacy/Process"))
::Nirvana::Word (*main) (Bridge <::Nirvana::Legacy::Process>*, ::Nirvana::Size argc, Char** argv, Interface*);
NIRVANA_BRIDGE_END ()

template <class T>
class Client <T, ::Nirvana::Legacy::Process> :
	public T
{
public:
	::Nirvana::Word main (::Nirvana::Size argc, Char** argv);
};

template <class T>
::Nirvana::Word Client <T, ::Nirvana::Legacy::Process>::main (::Nirvana::Size argc, Char** argv)
{
	Environment _env;
	Bridge < ::Nirvana::Legacy::Process>& _b (T::_get_bridge (_env));
	Long _ret = (_b._epv ().epv.main) (&_b, argc, argv, &_env);
	_env.check ();
	return _ret;
}

}
}

namespace Nirvana {
namespace Legacy {

class Runnable : public ::CORBA::Nirvana::ClientInterface <Runnable>
{};

}
}


#endif
