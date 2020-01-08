//! \file MemoryHelper.h.
//!
//! \brief Declares the memory helper class

#ifndef NIRVANA_MEMORYHELPER_H_
#define NIRVANA_MEMORYHELPER_H_

#include "Memory_c.h"

namespace Nirvana {

//! \class MemoryHelper
//!
//! \brief Helper class for dynamic collections memory management.

class MemoryHelper
{
public:
	MemoryHelper (Memory_ptr p = default_heap ()) :
		mem_ (p)
	{}

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

	void* reserve (void* p, size_t& allocated, size_t data_size, size_t capacity) const;

	//! \fn void MemoryHelper::shrink_to_fit (void* p, size_t& allocated, size_t data_size);
	//!
	//! \brief Shrink memory block to fit data.
	//!
	//! \param [in]     p         Data block allocated from the heap.
	//! \param [in,out] allocated The allocated size.
	//! \param          data_size Size of the data.

	void shrink_to_fit (void* p, size_t& allocated, size_t data_size);

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

	void* assign (void* p, size_t& allocated, size_t old_size, size_t new_size, const void* src_ptr = nullptr) const;

	//! \fn void MemoryHelper::erase (void* p, size_t data_size, size_t offset, size_t count) const;
	//!
	//! \brief Erases the data from memory block
	//!
	//! \param [in,out] p         If non-null, a memory block to process.
	//! \param          data_size Size of the data.
	//! \param          offset    The offset where to erased data starts.
	//! \param          count     Number of bytes to erase.

	void erase (void* p, size_t data_size, size_t offset, size_t count) const;

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

	void* insert (void* p, size_t& allocated, size_t data_size, size_t offset, size_t count, const void* src_ptr = nullptr) const
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

	void* replace (void* p, size_t& allocated, size_t data_size, size_t offset, size_t old_size, size_t new_size, const void* src_ptr = nullptr) const;

	bool expand (void* cur_end, size_t append, unsigned flags) const NIRVANA_NOEXCEPT;

private:
	Memory_ptr mem_;
};

}

#endif
