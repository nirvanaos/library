#ifndef NIRVANA_MEMORYHELPER_H_
#define NIRVANA_MEMORYHELPER_H_

#include "Nirvana.h"
#include "Memory_c.h"

namespace Nirvana {

class MemoryHelper
{
public:
	MemoryHelper (Memory_ptr p) :
		mem_ (p)
	{}

	void* reserve (void* p, size_t data_size, size_t cur_capacity, size_t& new_capacity) const;
	void* assign (void* dst, size_t& capacity, const void* data_ptr, size_t data_size) const;

private:
	Memory_ptr mem_;
};

}

#endif
