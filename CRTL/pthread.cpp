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

extern const ImportInterfaceT <System> g_system;

}
}

class PThread :
	public CORBA::servant_traits <Nirvana::Legacy::Runnable>::Servant <PThread>
{
public:
	PThread (void* (*start_routine) (void*), void* arg) :
		start_routine_ (start_routine),
		arg_ (arg),
		ret_ (nullptr)
	{
		thread_ = Nirvana::Legacy::g_system->create_thread (this);
	}

	void run ()
	{
		ret_ = (start_routine_) (arg_);
	}

	bool join (void** value_ptr)
	{
		Nirvana::Legacy::Thread::_ref_type t (std::move (thread_));
		if (t) {
			t->join ();
			if (value_ptr)
				*value_ptr = ret_;
			return true;
		}
		return false;
	}

	void detach () noexcept
	{
		thread_ = nullptr;
	}

private:
	void* (*start_routine_) (void*);
	void* arg_;
	void* ret_;
	Nirvana::Legacy::Thread::_ref_type thread_;
};

extern "C" int pthread_create (pthread_t* thread, const pthread_attr_t * attr,
	void* (*start_routine) (void*), void* arg)
{
	int ret = 0;
	try {
		new (thread) CORBA::servant_reference <PThread> (CORBA::make_reference <PThread> (start_routine, arg));
	} catch (const CORBA::NO_MEMORY&) {
		ret = ENOMEM;
	} catch (...) {
		ret = ENOTSUP;
	}
	return ret;
}

extern "C" int pthread_detach (pthread_t thread)
{
	CORBA::servant_reference <PThread>& t = reinterpret_cast <CORBA::servant_reference <PThread>&> (thread);
	t->detach ();
	t = nullptr;
	return 0;
}

extern "C" int pthread_join (pthread_t thread, void** value_ptr)
{
	CORBA::servant_reference <PThread>& t = reinterpret_cast <CORBA::servant_reference <PThread>&> (thread);
	int ret = EINVAL;
	try {
		if (!t->join (value_ptr))
			ret = EINVAL;
	} catch (const CORBA::SystemException& ex) {
		int err = Nirvana::get_minor_errno (ex.minor ());
		if (err)
			ret = err;
	}
	return ret;
}

extern "C" void* pthread_getspecific (pthread_key_t key)
{
	return Nirvana::g_system->TLS_get (key);
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

extern "C" int pthread_key_delete (pthread_key_t key)
{
	int ret = 0;
	try {
		Nirvana::g_system->TLS_free (key);
	} catch (...) {
		ret = EINVAL;
	}
	return ret;
}

extern "C" int pthread_mutex_destroy (pthread_mutex_t* mutex)
{
	if (!mutex || !*mutex)
		return EINVAL;
	*reinterpret_cast <Nirvana::Legacy::Mutex::_ref_type*> (mutex) = nullptr;
	return 0;
}

extern "C" int pthread_mutex_init (pthread_mutex_t* mutex, const pthread_mutexattr_t * att)
{
	int ret = 0;
	if (!mutex)
		ret = EINVAL;
	else {
		try {
			new (mutex) Nirvana::Legacy::Mutex::_ref_type (Nirvana::Legacy::g_system->create_mutex ());
		} catch (const CORBA::NO_MEMORY&) {
			ret = ENOMEM;
		} catch (...) {
			ret = ENOTSUP;
		}
	}
	return ret;
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
