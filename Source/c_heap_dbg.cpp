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
#include "lpch.h"
#include <Nirvana/c_heap_dbg.h>

namespace Nirvana {

HeapBlockHdrDbg::HeapBlockHdrDbg (size_t cb, int block_type, const char* file_name, int line_number) noexcept :
	HeapBlockHdr (cb),
	block_type_ (block_type),
	line_number_ (line_number),
	file_name_ (file_name)
{
	// Trailer
	new ((uint8_t*)(this + 1) + cb) NoMansLand ();
}

void HeapBlockHdrDbg::resize (size_t new_size, int block_type, const char* file_name, int line_number) noexcept
{
	size_ = new_size;
	block_type_ = block_type;
	file_name_ = file_name;
	line_number_ = line_number;
	// Reinitialize no mans land in case it was corrupted.
	new (&no_mans_land_) NoMansLand ();
	// Trailer
	new ((uint8_t*)(this + 1) + new_size) NoMansLand ();
}

void HeapBlockHdrDbg::check (int block_type) const noexcept
{
	// TODO: Improve diagnostics.
	assert (block_type_ == block_type);
	assert (no_mans_land_.check ());
	assert (reinterpret_cast <const NoMansLand*> ((uint8_t*)(this + 1) + size_)->check ());
}

}

