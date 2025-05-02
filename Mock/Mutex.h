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

#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
//#define WINAPI_PARTITION_DESKTOP 0
//#define WINAPI_PARTITION_APP 0
//#define WINAPI_PARTITION_SYSTEM 0
//#define WINAPI_PARTITION_GAMES 0

#include <Windows.h>

#else

#include <pthread.h>

#endif

namespace Nirvana {
namespace Test {

class Mutex
{
public:
	Mutex ();
	void lock ();
	void unlock ();
private:
};

class LockGuard 
{
public:
	LockGuard (Mutex&);
	~LockGuard ();
};

}
}	

#endif
