/*
* Nirvana mock library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#include "Memory.h"
#include "Module.h"
#include "Debugger.h"
#include "POSIX.h"
#include <Nirvana/CRTL/initterm.h>

namespace Nirvana {

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT NIRVANA_CONSTINIT ImportInterfaceT <Memory> the_memory = { OLF_IMPORT_INTERFACE,
	nullptr, nullptr, Mock::Memory::_bridge () };


NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT NIRVANA_CONSTINIT ImportInterfaceT <Module> the_module = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, Mock::Module::_bridge () };


NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT NIRVANA_CONSTINIT ImportInterfaceT <Debugger> the_debugger = { OLF_IMPORT_INTERFACE,
	nullptr, nullptr, Mock::Debugger::_bridge () };


NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT NIRVANA_CONSTINIT ImportInterfaceT <POSIX> the_posix = { OLF_IMPORT_INTERFACE,
	nullptr, nullptr, Mock::POSIX::_bridge () };

namespace Mock {

void initialize ()
{
	Memory::initialize ();
	Debugger::initialize ();
	Module::initialize ();
}

void terminate ()
{
#ifndef _WIN32
	Module::terminate ();
	Debugger::terminate ();
	Memory::terminate ();
#endif
}

}

}

CRTL_CUSTOM_INITIALIZER (Nirvana::Mock::initialize)
CRTL_CUSTOM_TERMINATOR (Nirvana::Mock::terminate)
