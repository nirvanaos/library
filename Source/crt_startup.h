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
#ifndef NIRVANA_CRT_STARTUP_H_
#define NIRVANA_CRT_STARTUP_H_

#if _WIN32

// Use MS UCRT

#include <CORBA/CORBA.h>
#include <Nirvana/Module.h>

typedef void* HINSTANCE;

#define DLL_PROCESS_ATTACH   1
#define DLL_THREAD_ATTACH    2
#define DLL_THREAD_DETACH    3
#define DLL_PROCESS_DETACH   0

extern "C" int __stdcall _DllMainCRTStartup (HINSTANCE inst, unsigned long reason, void* reserved);

namespace Nirvana {

inline bool crt_init () noexcept
{
	return _DllMainCRTStartup ((HINSTANCE)the_module->base_address (), DLL_PROCESS_ATTACH, 0);
}

inline void crt_term () noexcept
{
	_DllMainCRTStartup (nullptr, DLL_PROCESS_DETACH, 0);
}

}

#else

#include <Nirvana/CRTL/initterm.h>

// Use Nirvana CRTL

namespace Nirvana {

bool crt_init () noexcept
{
	return CRTL::initialize ();
}

void crt_term () noexcept
{
	return CRTL::terminate ();
}

}

#endif
#endif
