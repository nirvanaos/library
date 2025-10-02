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
#ifndef NIRVANA_MOCK_HOSTALLOCATOR_H_
#define NIRVANA_MOCK_HOSTALLOCATOR_H_
#pragma once

#include <memory>
#include <mockhost/HostAPI.h>

namespace Nirvana {
namespace Mock {

template <typename T>
class HostAllocator : public std::allocator <T>
{
public:
	static void deallocate (T* p, size_t cnt)
	{
		host_release (p);
	}

	static T* allocate (size_t cnt)
	{
		return (T*)host_allocate (cnt * sizeof (T), alignof (T));
	}

	template <class U>
	operator const HostAllocator <U>& () const noexcept
	{
		return *reinterpret_cast <const HostAllocator <U>*> (this);
	}

	template <class U> struct rebind
	{
		typedef HostAllocator <U> other;
	};

};

}
}

#endif
