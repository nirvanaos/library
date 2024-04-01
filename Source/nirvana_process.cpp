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
#include "../../pch/pch.h"
#include <CORBA/Server.h>
#include <Nirvana/Main_s.h>
#include "crt_startup.h"
#include <memory>

extern "C" int main (int argc, char* argv []);
extern int nmain (Nirvana::Main::Strings& argv);

namespace Nirvana {

class ArgPtrs
{
public:
	ArgPtrs (Main::Strings& strings);

	~ArgPtrs ()
	{
		the_memory->release (pointers_, size_);
	}

	operator char** () const noexcept
	{
		return pointers_;
	}

private:
	char** pointers_;
	size_t size_;
};

ArgPtrs::ArgPtrs (Main::Strings& strings) :
	pointers_ (nullptr),
	size_ ((strings.size () + 1) * sizeof (char*))
{
	pointers_ = (char**)the_memory->allocate (nullptr, size_, 0);
	for (size_t i = 0; i < strings.size (); ++i) {
		pointers_ [i] = const_cast <char*> (strings [i].data ());
	}
	pointers_ [strings.size ()] = nullptr;
}

class ProcessMain :
	public CORBA::servant_traits <Main>::ServantStatic <ProcessMain>
{
public:
	static int32_t main (Strings& argv)
	{
		crt_init ();
		return nmain (argv);
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

int nmain (Nirvana::Main::Strings& argv)
{
	return main ((int)argv.size (), Nirvana::ArgPtrs (argv));
}

extern "C" NIRVANA_OLF_SECTION_OPT const Nirvana::ModuleStartup nirvana_process
{ Nirvana::OLF_MODULE_STARTUP, NIRVANA_STATIC_BRIDGE (Nirvana::Main, Nirvana::ProcessMain) };
