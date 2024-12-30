/// \file Mock implementation of the POSIX interface.
/*
* Nirvana mock library.
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
#include <CORBA/Server.h>
#include <Nirvana/POSIX_s.h>
#include "export.h"
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

namespace Nirvana {
namespace Test {

class POSIX :
	public CORBA::servant_traits <Nirvana::POSIX>::ServantStatic <POSIX>
{
public:
	static int* error_number ()
	{
		return &errno;
	}

	static void raise (int signal)
	{
		::raise (signal);
	}

	static void sigaction (int signal, const struct sigaction* act, struct sigaction* oldact)
	{
		throw_NO_IMPLEMENT ();
	}

	static void srand (unsigned seed)
	{
		::srand (seed);
	}

	static int rand ()
	{
		return ::rand ();
	}

};

}
/*
NIRVANA_MOCK_EXPORT CORBA::Internal::Interface* mock_posix = NIRVANA_STATIC_BRIDGE (Nirvana::POSIX, POSIX);

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <POSIX> the_posix = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, NIRVANA_STATIC_BRIDGE (POSIX, Test::POSIX) };
*/
}
