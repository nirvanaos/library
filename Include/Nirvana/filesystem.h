/// \file File system utilities.
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
#ifndef NIRVANA_FILESYSTEM_H_
#define NIRVANA_FILESYSTEM_H_
#pragma once

#include <CORBA/CORBA.h>
#include <CORBA/CosNaming.h>

namespace CosNaming {

/// Check NameComponent structures for equality.
inline bool operator == (const NameComponent& l, const NameComponent& r) noexcept
{
	return l.id () == r.id () && l.kind () == r.kind ();
}

/// Check NameComponent structures for inequality.
inline bool operator != (const NameComponent& l, const NameComponent& r) noexcept
{
	return l.id () != r.id () || l.kind () != r.kind ();
}

/// Compare NameComponent structures lexicographically.
/// Uses conversion to wide strings for the correct collation.
bool operator < (const NameComponent& l, const NameComponent& r);

}

namespace Nirvana {

/// Checks whether the name is absolute or relative. 
inline bool is_absolute (const CosNaming::Name& name) noexcept
{
	return !name.empty () && name.front ().id ().empty () && name.front ().kind ().empty ();
}

}

#endif
