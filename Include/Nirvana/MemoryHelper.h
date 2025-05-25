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
#ifndef NIRVANA_MEMORYHELPER_H_
#define NIRVANA_MEMORYHELPER_H_
#pragma once

#include "Memory_forward.h"
#include <CORBA/exceptions.h>
#include <algorithm>

#ifndef NIRVANA_NO_BAD_ALLOC
#define NIRVANA_BAD_ALLOC_TRY try {
#define NIRVANA_BAD_ALLOC_CATCH } catch (const CORBA::NO_MEMORY&) { throw std::bad_alloc (); }
#else
#define NIRVANA_BAD_ALLOC_TRY
#define NIRVANA_BAD_ALLOC_CATCH
#endif

namespace Nirvana {

/// \brief Helper class for memory management in STL collections.
class MemoryHelper
{
	// There is no platform with less page size.
	static const size_t MIN_COMMIT_PAGE = 4096;

public:
	/// \brief Allocate memory block.
	///
	/// \param [in,out] size  Size of the memory block.
	/// \param          flags Memory allocation flags.
	/// \returns              Pointer to the allocated memory block.
	/// \throw std::bad_alloc
	static void* allocate (size_t& size, uint_fast16_t flags)
	{
		if (is_constant_evaluated ())
			return operator new (size);
		else {
			NIRVANA_BAD_ALLOC_TRY
				return memory ()->allocate (nullptr, size, flags);
			NIRVANA_BAD_ALLOC_CATCH
		}
	}

	/// \brief Release memory block.
	///
	/// \param p    Memory block address.
	/// \param size Memory block size.
	static void release (void* p, size_t size)
	{
		if (is_constant_evaluated ())
			consteval_release (p, size);
		else
			memory ()->release (p, size);
	}

	/// \brief Commit memory block.
	///
	/// \param p    Memory block address.
	/// \param size Memory block size.
	/// \throw std::bad_alloc
	static void commit (void* p, size_t size)
	{
		if (!is_constant_evaluated ()) {
			NIRVANA_BAD_ALLOC_TRY
				commit_internal (p, size);
			NIRVANA_BAD_ALLOC_CATCH
		}
	}

	/// \brief Decommit memory block.
	///
	/// \param p    Memory block address.
	/// \param size Memory block size.
	static void decommit (void* p, size_t size)
	{
		if (!is_constant_evaluated ())
			decommit_internal (p, size);
	}

	/// \brief Reserves the memory block at least capacity size.
	///
	/// \param [in]     p         Current data block.
	/// \param [in,out] allocated The allocated size. 0 if data block is not allocated from heap.
	/// \param          data_size Size of the data at p.
	/// \param          capacity  The required capacity.
	///
	/// \return Pointer to the memory block.
	/// \throw std::bad_alloc
	static void* reserve (void* p, size_t& allocated, size_t data_size, size_t capacity)
	{
		if (allocated < capacity) {
			if (is_constant_evaluated ()) {
				void* pnew = operator new (capacity);
				std::copy ((const uint8_t*)p, (const uint8_t*)p + data_size, (uint8_t*)pnew);
				if (allocated)
					consteval_release (p, allocated);
				allocated = capacity;
				p = pnew;
			} else {
				NIRVANA_BAD_ALLOC_TRY
					p = reserve_internal (p, allocated, data_size, capacity);
				NIRVANA_BAD_ALLOC_CATCH
			}
		}
		return p;
	}

	/// \brief Shrink memory block to fit data.
	///
	/// \param [in]     p         Data block allocated from the heap.
	/// \param [in,out] allocated The allocated size.
	/// \param          data_size Size of the data.
	static void shrink_to_fit (void* p, size_t& allocated, size_t data_size)
	{
		if (!is_constant_evaluated ())
			shrink_to_fit_internal (p, allocated, data_size);
	}

	/// \brief Assigns data to the memory block.
	///
	/// \param [in,out] p         If non-null, a current block of data.
	/// \param [in,out] allocated The allocated size of the data block.
	/// \param          old_size  Current size of the data.
	/// \param          new_size  Source data size.
	/// \param          src_ptr   (Optional) Pointer to data to assign.
	///
	/// \return Pointer to the memory block.
	/// \throw std::bad_alloc
	static void* assign (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr = nullptr)
	{
		if (is_constant_evaluated ()) {
			if (allocated < new_size) {
				void* pnew = operator new (new_size);
				if (allocated)
					consteval_release (p, allocated);
				allocated = new_size;
				p = pnew;
			}
			if (src_ptr)
				std::copy ((const uint8_t*)src_ptr, (const uint8_t*)src_ptr + new_size, (uint8_t*)p);
		} else {
			NIRVANA_BAD_ALLOC_TRY
				p = assign_internal (p, allocated, old_size, new_size, src_ptr);
			NIRVANA_BAD_ALLOC_CATCH
		}
		return p;
	}

	/// \brief Erases the data from memory block
	///
	/// \param [in,out] p         A memory block to process.
	/// \param          data_size Size of the data.
	/// \param          offset    The offset where to erased data starts.
	/// \param          count     Number of bytes to erase.
	static void erase (void* p, size_t data_size, size_t offset, size_t count)
	{
		if (is_constant_evaluated ()) {
			uint8_t* dst = (uint8_t*)p + offset;
			uint8_t* src = dst + count;
			uint8_t* end = (uint8_t*)p + data_size;
			std::copy (src, end, dst);
		} else
			erase_internal (p, data_size, offset, count);
	}

	/// \brief Inserts the data to the memory block.
	///
	/// \param [in,out] p         If non-null, a memory block to process.
	/// \param [in,out] allocated The memory block allocated size.
	/// \param          data_size Size of the data.
	/// \param          offset    The offset where to insert.
	/// \param          count     Number of bytes to insert.
	/// \param          src_ptr   (Optional) Pointer to data to insert.
	///
	/// \return Pointer to the memory block.
	/// \throw std::bad_alloc

	static void* insert (void* p, size_t& allocated, size_t data_size, size_t offset, size_t count, const void* src_ptr = nullptr)
	{
		return replace (p, allocated, data_size, offset, 0, count, src_ptr);
	}

	/// \brief Replaces the range of bytes.
	///
	/// \param [in,out] p         A memory block to process.
	/// \param [in,out] allocated The memory block allocated size.
	/// \param          data_size Size of the data.
	/// \param          offset    The offset to the byte range.
	/// \param          old_size  Old size of the byte range.
	/// \param          new_size  New size of the byte range.
	/// \param          src_ptr   (Optional) Source data pointer.
	///
	/// \return Pointer to the memory block.
	/// \throw std::bad_alloc

	static void* replace (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr = nullptr)
	{
		if (is_constant_evaluated ()) {
			size_t space_required = data_size + (new_size - old_size);
			void* pnew;
			if (allocated < space_required) {
				pnew = operator new (space_required);
				std::copy ((uint8_t*)p, (uint8_t*)p + offset, (uint8_t*)pnew);
				if (new_size == old_size)
					std::copy ((uint8_t*)p + offset + old_size, (uint8_t*)p + data_size, (uint8_t*)pnew + offset + old_size);
			} else
				pnew = p;
			if (src_ptr)
				std::copy ((const uint8_t*)src_ptr, (const uint8_t*)src_ptr + new_size, (uint8_t*)pnew + offset);
			if (new_size < old_size)
				std::copy ((uint8_t*)p + old_size, (uint8_t*)p + data_size, (uint8_t*)pnew + new_size);
			else if (new_size > old_size)
				std::copy_backward ((uint8_t*)p + old_size, (uint8_t*)p + data_size, (uint8_t*)pnew + data_size + (new_size - old_size));
			if (pnew != p) {
				if (allocated)
					consteval_release (p, allocated);
				allocated = space_required;
				p = pnew;
			}
		} else {
			NIRVANA_BAD_ALLOC_TRY
				p = replace_internal (p, allocated, data_size, offset, old_size, new_size, src_ptr);
			NIRVANA_BAD_ALLOC_CATCH
		}
		return p;
	}

	/// \brief Try to expand memory block.
	///
	/// \param p        Memory block.
	/// \param cur_size Current block size.
	/// \param new_size New block size.
	/// \param flags    Memory allocation flags.
	/// \returns        `true` if memory block was successfully expanded.
	static bool expand (void* p, size_t cur_size, size_t& new_size, unsigned flags) noexcept
	{
		if (is_constant_evaluated ())
			return false;
		else
			return expand_internal (p, cur_size, new_size, flags);
	}

private:
	static Memory::_ptr_type memory () noexcept
	{
		return Nirvana::the_memory;
	}

	static void* reserve_internal (void* p, size_t& allocated, size_t data_size, size_t capacity);
	static void* assign_internal (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr = nullptr);
	static void* replace_internal (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr = nullptr);
	static void shrink_to_fit_internal (void* p, size_t& allocated, size_t data_size);
	static void erase_internal (void* p, size_t data_size, size_t offset, size_t count);
	static bool expand_internal (void* p, size_t cur_size, size_t& new_size, unsigned flags) noexcept;

	static void commit_internal (void* p, size_t size)
	{
		if (!on_the_page (p, size))
			memory ()->commit (p, size);
	}

	static void decommit_internal (void* p, size_t size)
	{
		if (!on_the_page (p, size))
			memory ()->decommit (p, size);
	}

	static bool on_the_page (void* p, size_t size) noexcept;

	constexpr static bool is_constant_evaluated () noexcept
	{
#ifdef NIRVANA_C20
		return std::is_constant_evaluated ();
#else
		return false;
#endif
	}

	static void consteval_release (void* p, size_t size)
	{
#ifdef NIRVANA_C14
		operator delete (p, size);
#else
		operator delete (p);
#endif
	}
};

}

#endif
