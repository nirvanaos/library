/// \file Mock implementation of the Module interface.
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
#include <Nirvana/Module_s.h>
#include "export.h"

namespace Nirvana {
namespace Test {

class Module :
	public CORBA::servant_traits <Nirvana::Module>::ServantStatic <Module>
{
public:
	static const void* base_address ()
	{
		return nullptr;
	}

	template <class Itf>
	static CORBA::Internal::Interface* __duplicate (CORBA::Internal::Interface* itf, CORBA::Internal::Interface* env) noexcept
	{
		return itf;
	}

	template <class Itf>
	static void __release (CORBA::Internal::Interface*) noexcept
	{
	}

	static void atexit (AtExitFunc f)
	{
		::atexit (f);
	}

	static int32_t id () noexcept
	{
		return 100;
	}
};

NIRVANA_MOCK_EXPORT CORBA::Internal::Interface* mock_module = NIRVANA_STATIC_BRIDGE (Nirvana::Module, Module);

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <Module> the_module = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Module, Test::Module) };

}
