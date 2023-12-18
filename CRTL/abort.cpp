/*
* Nirvana C runtime library.
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
#include "pch/pch.h"
#include <stdlib.h>
#include <signal.h>

using namespace CORBA::Internal;
using namespace Nirvana;

extern "C"
NIRVANA_NORETURN
void abort (void)
{
	Bridge <System>* br = static_cast <Bridge <System>*> (&static_cast <I_ptr <System> > (g_system));
	br->_epv ().epv.raise (br, SIGABRT, nullptr);
	NIRVANA_UNREACHABLE_CODE ();
}

