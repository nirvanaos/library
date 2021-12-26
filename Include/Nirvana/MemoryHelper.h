/// \file MemoryHelper.h.
///
/// \brief Declares the memory helper class

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
#ifndef NIRVANA_MEMORYHELPER_H_
#define NIRVANA_MEMORYHELPER_H_

#include "core_objects.h"
#include "native.h"
#include <CORBA/Client.h>
#include <CORBA/basic_types.h>
#include <CORBA/Type_interface.h>
#include <Nirvana/Memory.h>

namespace Nirvana {

//! \class MemoryHelper
//!
//! \brief Helper class for dynamic collections memory management.

class MemoryHelper
{
public:
	static Memory::_ptr_type memory ()
	{
		return g_memory;
	}

	//! \fn void* MemoryHelper::reserve (void* p, size_t& allocated, size_t data_size, size_t capacity) const;
	//!
	//! \brief Reserves the memory block at least capacity size.
	//!
	//! \param [in]     p         Current data block.
	//! \param [in,out] allocated The allocated size. 0 if data block is not allocated from heap.
	//! \param          data_size Size of the data at p.
	//! \param          capacity  The required capacity.
	//!
	//! \return Pointer to the memory block.
	//! \throw std::bad_alloc

	static void* reserve (void* p, size_t& allocated, size_t data_size, size_t capacity);

	//! \fn void MemoryHelper::shrink_to_fit (void* p, size_t& allocated, size_t data_size);
	//!
	//! \brief Shrink memory block to fit data.
	//!
	//! \param [in]     p         Data block allocated from the heap.
	//! \param [in,out] allocated The allocated size.
	//! \param          data_size Size of the data.

	static void shrink_to_fit (void* p, size_t& allocated, size_t data_size);

	//! \fn void* MemoryHelper::assign (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr = nullptr) const;
	//!
	//! \brief Assigns data to the memory block.
	//!
	//! \param [in,out] p         If non-null, a current block of data.
	//! \param [in,out] allocated The allocated size of the data block.
	//! \param          old_size  Current size of the data.
	//! \param          new_size  Source data size.
	//! \param          src_ptr   (Optional) Pointer to data to assign.
	//!
	//! \return Pointer to the memory block.
	//! \throw std::bad_alloc

	static void* assign (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr = nullptr);

	//! \fn void MemoryHelper::erase (void* p, size_t data_size, size_t offset, size_t count) const;
	//!
	//! \brief Erases the data from memory block
	//!
	//! \param [in,out] p         If non-null, a memory block to process.
	//! \param          data_size Size of the data.
	//! \param          offset    The offset where to erased data starts.
	//! \param          count     Number of bytes to erase.

	static void erase (void* p, size_t data_size, size_t offset, size_t count);

	//! \fn void* MemoryHelper::insert (void* p, size_t& allocated, size_t data_size, size_t offset, size_t count, const void* src_ptr = nullptr) const;
	//!
	//! \brief Inserts the data to the memory block.
	//!
	//! \param [in,out] p         If non-null, a memory block to process.
	//! \param [in,out] allocated The memory block allocated size.
	//! \param          data_size Size of the data.
	//! \param          offset    The offset where to insert.
	//! \param          count     Number of bytes to insert.
	//! \param          src_ptr   (Optional) Pointer to data to insert.
	//!
	//! \return Pointer to the memory block.
	//! \throw std::bad_alloc

	static void* insert (void* p, size_t& allocated, size_t data_size, size_t offset, size_t count, const void* src_ptr = nullptr)
	{
		return replace (p, allocated, data_size, offset, 0, count, src_ptr);
	}

	//! \fn void* MemoryHelper::replace (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr = nullptr) const;
	//!
	//! \brief Replaces the range of bytes.
	//!
	//! \param [in,out] p         If non-null, a memory block to process.
	//! \param [in,out] allocated The memory block allocated size.
	//! \param          data_size Size of the data.
	//! \param          offset    The offset to.
	//! \param          old_size  Old size of the byte range.
	//! \param          new_size  New size of the byte range.
	//! \param          src_ptr   (Optional) Source data pointer.
	//!
	//! \return Pointer to the memory block.
	//! \throw std::bad_alloc

	static void* replace (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr = nullptr);

	static bool expand (void* cur_end, size_t append, unsigned flags) NIRVANA_NOEXCEPT;
};

}

#endif
