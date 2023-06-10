/// \file
/// \brief Implementation of the debug C heap functions.
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
#ifndef NIRVANA_C_HEAP_DBG_H_
#define NIRVANA_C_HEAP_DBG_H_
#pragma once

#include "c_heap.h"

namespace Nirvana {

struct NoMansLand
{
	static const size_t SIGNATURE = (size_t)0xFDFDFDFDFDFDFDFDU;

	NoMansLand () noexcept :
	signature (SIGNATURE)
	{}

	bool check () const noexcept
	{
		return SIGNATURE == signature;
	}

	size_t signature;
};

class HeapBlockHdrDbg :
	public HeapBlockHdr
{
public:
	HeapBlockHdrDbg (size_t cb, int block_type, const char* file_name, int line_number) noexcept;

	static const size_t TRAILER_SIZE = sizeof (NoMansLand);

	static HeapBlockHdrDbg* hdr_from_ptr (void* p) noexcept
	{
		return (HeapBlockHdrDbg*)p - 1;
	}

	void resize (size_t new_size, int block_type, const char* file_name, int line_number) noexcept;

	void check (int block_type) const noexcept;

	void check (int block_type, const char*, int) const noexcept
	{
		check (block_type);
	}

private:
	int block_type_;
	int line_number_;
	const char* file_name_;
	NoMansLand no_mans_land_;
};

}

#endif
