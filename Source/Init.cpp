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
#include <Nirvana/Nirvana.h>
#include <CORBA/Server.h>
#include <Nirvana/ModuleInit_s.h>
#include "crt_startup.h"

namespace Nirvana {

class Init :
	public CORBA::servant_traits <ModuleInit>::ServantStatic <Init>
{
public:
	static void initialize ()
	{
		crt_init ();
	}

	static void terminate () NIRVANA_NOEXCEPT
	{
		crt_term ();
	}
};

}

extern "C" NIRVANA_OLF_SECTION const Nirvana::ModuleStartup nirvana_module
{ Nirvana::OLF_MODULE_STARTUP, NIRVANA_STATIC_BRIDGE (Nirvana::ModuleInit, Nirvana::Init), 0 };

extern "C" NIRVANA_OLF_SECTION const Nirvana::ModuleStartup nirvana_singleton
{ Nirvana::OLF_MODULE_STARTUP, NIRVANA_STATIC_BRIDGE (Nirvana::ModuleInit, Nirvana::Init), Nirvana::OLF_MODULE_SINGLETON };
