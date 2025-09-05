/*
* Nirvana C runtime library.
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
#include <pthread.h>
#include <CORBA/CORBA.h>
#include <Nirvana/Module.h>
#include <Nirvana/POSIX.h>

extern "C" void* pthread_getspecific (pthread_key_t key)
{
	try {
		return Nirvana::the_module->CS_get (key);
	} catch (...) {}
	return nullptr;
}

extern "C" int pthread_key_create (pthread_key_t* key, void (*deleter)(void*))
{
	int ret = EINVAL;
	try {
		*key = Nirvana::the_module->CS_alloc (deleter);
		ret = 0;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			ret = e;
	} catch (...) {
	}
	return ret;
}

extern "C" int pthread_key_delete (pthread_key_t key)
{
	int ret = EINVAL;
	try {
		Nirvana::the_module->CS_free (key);
		ret = 0;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			ret = e;
	} catch (...) {
	}
	return ret;
}

extern "C" int pthread_setspecific (pthread_key_t key, const void* val)
{
	int ret = EINVAL;
	try {
		Nirvana::the_module->CS_set (key, const_cast <void*> (val));
		ret = 0;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			ret = e;
	} catch (...) {
	}
	return ret;
}

extern "C" int pthread_once (pthread_once_t *once_control, void (*init_routine)(void))
{
	Nirvana::the_posix->once (*once_control, init_routine);
	return 0;
}