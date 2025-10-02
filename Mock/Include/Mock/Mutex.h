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
#ifndef NIRVANA_MOCK_MUTEX_H_
#define NIRVANA_MOCK_MUTEX_H_
#pragma once

#include <system_error>
#include <mockhost/HostAPI.h>

namespace Nirvana {
namespace Mock {

/// Drop-in replacement for std::mutex
class Mutex
{
public:
	Mutex () :
		impl_ (host_Mutex_create ())
	{
    if (!impl_)
			throw std::system_error (ENOMEM, std::system_category ());
}

	~Mutex () noexcept
	{
		host_Mutex_destroy (impl_);
	}

	void lock ()
	{
		int err = host_Mutex_lock (impl_);
		if (err)
				throw std::system_error (err, std::system_category ());
	}

	void unlock ()
	{
		int err = host_Mutex_unlock (impl_);
		if (err)
				throw std::system_error (err, std::system_category ());
	}

private:
	host_Mutex* impl_;
};

class LockGuard
{
public:
	LockGuard (Mutex& mutex) :
		mutex_ (mutex)
	{
		mutex.lock ();
	}

	~LockGuard ()
	{
		mutex_.unlock ();
	}

private:
	Mutex& mutex_;
};

}
}

#endif
