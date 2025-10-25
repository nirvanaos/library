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
#ifndef NIRVANA_SEMVER_H_
#define NIRVANA_SEMVER_H_
#pragma once

#include <CORBA/CORBA.h>

namespace Nirvana {

// Semantic-versioned name
class SemVer
{
public:
	SemVer ();
	SemVer (IDL::String&& name, int64_t version, IDL::String&& prerelease) noexcept;

	bool parse (const IDL::String& full_name);

	const IDL::String& name () const noexcept
	{
		return name_;
	}

	int64_t version () const noexcept
	{
		return version_;
	}

	const IDL::String& prerelease () const noexcept
	{
		return prerelease_;
	}

	IDL::String to_string () const;

private:
	int64_t version_;
	IDL::String name_;
	IDL::String prerelease_;
};

}

#endif
