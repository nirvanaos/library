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
#include "Mutex.h"

#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

struct host_Mutex
{
	host_Mutex ()
	{
		InitializeCriticalSection (&cs_);
	}

	~host_Mutex ()
	{
		DeleteCriticalSection (&cs_);
	}

	void lock ()
	{
		EnterCriticalSection (&cs_);
	}

	void unlock ()
	{
		LeaveCriticalSection (&cs_);
	}

	CRITICAL_SECTION cs_;
};

#else

#include <pthread.h>

struct host_Mutex
{
	host_Mutex ()
	{
		pthread_mutex_init (&mutex_, nullptr);
	}

	~host_Mutex ()
	{
		pthread_mutex_destroy (&mutex_);
	}

	void lock ()
	{
		pthread_mutex_lock (&mutex_);
	}

	void unlock ()
	{
		pthread_mutex_unlock (&mutex_);
	}

	pthread_mutex_t mutex_;
};

#endif

NIRVANA_MOCK_EXPORT host_Mutex* host_Mutex_create ()
{
	return new host_Mutex;
}

NIRVANA_MOCK_EXPORT void host_Mutex_destroy (host_Mutex* p)
{
	delete p;
}

NIRVANA_MOCK_EXPORT void host_Mutex_lock (host_Mutex* p)
{
	p->lock ();
}

NIRVANA_MOCK_EXPORT void host_Mutex_unlock (host_Mutex* p)
{
	p->unlock ();
}
