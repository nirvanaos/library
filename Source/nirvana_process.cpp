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
#include "lpch.h"
#include <CORBA/Server.h>
#include <Nirvana/Legacy/Main_s.h>
#include "crt_startup.h"

extern int main (int argc, char* argv [], char* envp []);

namespace Nirvana {
namespace Legacy {

class ProcessMain :
	public CORBA::servant_traits <Main>::ServantStatic <ProcessMain>
{
public:
	static int32_t main (uint16_t argc, void* argv, void* envp)
	{
		crt_init ();
		return ::main (argc, (char**)argv, (char**)envp);
	}

	static void cleanup ()
	{
		crt_term ();
	}

	static void _s_raise_exception (CORBA::Internal::Bridge <Main>*,
		CORBA::Short code, CORBA::UShort minor, CORBA::Internal::Interface*)
	{
		CORBA::SystemException::_raise_by_code ((CORBA::Exception::Code)code, minor);
	}
};

}
}

extern "C" NIRVANA_OLF_SECTION_OPT constexpr Nirvana::ModuleStartup nirvana_process
{ Nirvana::OLF_MODULE_STARTUP, NIRVANA_STATIC_BRIDGE (Nirvana::Legacy::Main, Nirvana::Legacy::ProcessMain) };
