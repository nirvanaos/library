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
	MemoryHelper (Memory_ptr p) :
		mem_ (p)
	{}

	void* reserve (void* p, size_t data_size, size_t cur_capacity, size_t& new_capacity) const;

	void* reserve (size_t& capacity) const
	{
		capacity = 0;
		return reserve (nullptr, 0, 0, capacity);
	}

	void* assign (void* p, size_t& capacity, size_t size, const void* src_ptr, size_t src_size) const;
	void shrink_to_fit (void* p, size_t& capacity, size_t size);
	void* commit (void* p, size_t& capacity, size_t old_size, size_t new_size) const;
	void erase (void* p, size_t size, size_t off, size_t count) const;
	void* insert (void* p, size_t& capacity, size_t size, size_t where, const void* src_ptr, size_t src_size) const;

private:
	Memory_ptr mem_;
};

}

#endif
