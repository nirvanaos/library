#ifndef NIRVANA_ATOMICCOUNTER_H_
#define NIRVANA_ATOMICCOUNTER_H_

#include <atomic>

namespace Nirvana {

class AtomicCounter
{
public:
	AtomicCounter (uint32_t init) :
		cnt_ (init)
	{}

	operator uint32_t () const
	{
		return cnt_.load ();
	}

	uint32_t increment ()
	{
		return ++cnt_;
	}

	uint32_t decrement ()
	{
		return --cnt_;
	}

private:
	std::atomic_uint32_t cnt_;
};

}

#endif
