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
#include <fnctl.h>
#include <Nirvana/System.h>

using namespace Nirvana;
using namespace CORBA;

extern "C" int open (const char* path, uint_fast16_t oflag, uint_fast16_t mode)
{
	try {
		return g_system->fd_open (path, oflag, mode);
	} catch (const RuntimeError& ex) {
		*(int*)g_system->error_number () = ex.error_number ();
	} catch (const NO_MEMORY&) {
		*(int*)g_system->error_number () = ENOMEM;
	} catch (...) {
		*(int*)g_system->error_number () = EIO;
	}
	return -1;
}
