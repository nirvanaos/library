/// \file
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
#ifndef NIRVANA_DIRECTORYITERATOR_H_
#define NIRVANA_DIRECTORYITERATOR_H_
#pragma once

#include <CORBA/CORBA.h>
#include <Nirvana/File.h>

namespace Nirvana {

class DirectoryIterator
{
public:
	static const uint_fast16_t SKIP_PERMISSION_DENIED = Dir::SKIP_PERMISSION_DENIED;

	DirectoryIterator (Dir::_ptr_type dir, uint_fast16_t flags = 0,
		const std::string& regexp = std::string (),
		unsigned block_size = 16);

	const DirEntry* readdir ();

	bool end () const noexcept
	{
		return cur_pos_ >= block_.size () && !iterator_;
	}

	DirectoryIterator () = default;
	DirectoryIterator (const DirectoryIterator&) = delete;
	DirectoryIterator (DirectoryIterator&&) = default;

	DirectoryIterator& operator = (const DirectoryIterator&) = delete;
	DirectoryIterator& operator = (DirectoryIterator&&) = default;

private:
	DirEntryList block_;
	unsigned block_size_;
	unsigned cur_pos_;
	DirIterator::_ref_type iterator_;
};

}

#endif
