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

#include "export.h"

namespace Nirvana {
namespace Test {

class NIRVANA_MOCK_EXP Mutex
{
public:
	Mutex ();
	~Mutex ();
	void lock ();
	void unlock ();

private:
	class Implementation;

	Implementation* impl_;
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
