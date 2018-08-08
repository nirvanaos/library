#ifndef NIRVANA_INTERLOCKEDCOUNTER_H_
#define NIRVANA_INTERLOCKEDCOUNTER_H_

#include <atomic>

namespace Nirvana {

class InterlockedCounter
{
public:
	InterlockedCounter (uint32_t init) :
		cnt_ (init)
	{}

	operator uint32_t () const
	{
		return cnt_;
	}

	void increment ()
	{
		++cnt_;
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
