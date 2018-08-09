#ifndef NIRVANA_ATOMICCOUNTER_H_
#define NIRVANA_ATOMICCOUNTER_H_

#include <Nirvana.h>
#include <stdint.h>

#define USE_INTRINSIC_ATOMIC

#if (defined USE_INTRINSIC_ATOMIC && defined _MSC_BUILD)
#include <intrin.h>
#else
#include <atomic>
#endif

namespace Nirvana {

class AtomicCounter
{
public:
	AtomicCounter (uint32_t init) :
		cnt_ (init)
	{}

	operator uint32_t () const
	{
		return cnt_;
	}

	uint32_t increment ()
	{
#if (defined USE_INTRINSIC_ATOMIC && defined _MSC_BUILD)
		return _InterlockedIncrement ((volatile long*)&cnt_);
#else
		return ++cnt_;
#endif
	}

	uint32_t decrement ()
	{
#if (defined USE_INTRINSIC_ATOMIC && defined _MSC_BUILD)
		return _InterlockedDecrement ((volatile long*)&cnt_);
#else
		return --cnt_;
#endif
	}

private:
#if (defined USE_INTRINSIC_ATOMIC && defined _MSC_BUILD)
	volatile uint32_t cnt_;
#else
	std::atomic_uint32_t cnt_;
#endif
};

/// Reference counter always initialized to 1 and can't be incremented from zero.
class RefCounter : public AtomicCounter
{
public:
	RefCounter () :
		AtomicCounter (1)
	{}

	uint32_t increment ()
	{
		uint32_t ret = AtomicCounter::increment ();
		assert (ret > 1);
		return ret;
	}
};

}

#undef USE_INTRINSIC_ATOMIC

#endif
