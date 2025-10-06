/*
* Nirvana runtime library.
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
#include <Nirvana/platform.h>
#include <algorithm>

namespace Nirvana {

struct PlatformInfo
{
	unsigned id;
	const char* name;
};

static PlatformInfo platforms [] = {
	{ PLATFORM_X86, "Intel 386" },
	{ PLATFORM_X64, "AMD64" },
	{ PLATFORM_ARM, "ARM Little-Endian" },
	{ PLATFORM_ARM64, "ARM64 Little-Endian"}
};

struct PlatformLess
{
	bool operator () (const PlatformInfo& l, unsigned r) const noexcept
	{
		return l.id < r;
	}

	bool operator () (unsigned l, const PlatformInfo& r) const noexcept
	{
		return l < r.id;
	}

	bool operator () (const PlatformInfo& l, const PlatformInfo& r) const noexcept
	{
		return l.id < r.id;
	}
};

const char* get_platform_name (unsigned id) noexcept
{
	const PlatformInfo* f = std::lower_bound (platforms, std::end (platforms), id, PlatformLess ());
	if (f != std::end (platforms))
		return f->name;
	else
		return nullptr;
}

}
