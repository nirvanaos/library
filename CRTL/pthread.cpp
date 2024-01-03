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
#include <pthread.h>
#include <CORBA/Server.h>
#include <Nirvana/Legacy/Legacy.h>
#include <Nirvana/Legacy/Runnable_s.h>

namespace Nirvana {
namespace Legacy {

class System;
extern const ImportInterfaceT <System> g_system;

}
}

class PThreadRunnable :
	public CORBA::servant_traits <Nirvana::Legacy::Runnable>::Servant <PThreadRunnable>,
	public CORBA::Internal::RefCountBase <PThreadRunnable>,
	public CORBA::Internal::LifeCycleRefCnt <PThreadRunnable>
{
public:
	PThreadRunnable (void* (*start_routine) (void*), void* arg) :
		start_routine_ (start_routine),
		arg_ (arg),
		ret_ (nullptr)
	{}

	void run ()
	{
		ret_ = (start_routine_) (arg_);
	}

private:
	void* (*start_routine_) (void*);
	void* arg_;
	void* ret_;
};

extern "C" int pthread_create (pthread_t * thread, const pthread_attr_t * attr,
	void* (*start_routine) (void*), void* arg)
{
	int ret = 0;
	try {
		*reinterpret_cast <Nirvana::Legacy::Thread::_ref_type*> (thread) =
			Nirvana::Legacy::g_system->create_thread (CORBA::make_reference <PThreadRunnable> (start_routine, arg)->_get_ptr ());
	} catch (const CORBA::NO_MEMORY&) {
		ret = ENOMEM;
	} catch (...) {
		ret = ENOTSUP;
	}
	return ret;
}

extern "C" int pthread_join (pthread_t thread, void** value_ptr)
{
	int ret = 0;
	try {
		Nirvana::Legacy::Thread::_ptr_type tp = Nirvana::Legacy::Thread::_check (reinterpret_cast <CORBA::Internal::Interface*> (thread));
		tp->join ();
		CORBA::Internal::interface_release (&tp);
	} catch (...) {
		ret = EINVAL;
	}
	return ret;
}

extern "C" int pthread_key_create (pthread_key_t* key, void (*destructor)(void*))
{
	int ret = ENOMEM;
	try {
		*key = Nirvana::g_system->TLS_alloc (destructor);
		ret = 0;
	} catch (const CORBA::IMP_LIMIT&) {
		ret = EAGAIN;
	} catch (...) {
	}
	return ret;
}

extern "C" void* pthread_getspecific (pthread_key_t key)
{
	return Nirvana::g_system->TLS_get (key);
}

extern "C" int pthread_setspecific (pthread_key_t key, const void* value)
{
	int ret = ENOMEM;
	try {
		Nirvana::g_system->TLS_set (key, const_cast <void*> (value));
	} catch (const CORBA::BAD_OPERATION&) {
		ret = ENOTSUP;
	} catch (...) {
	}
	return ret;
}
