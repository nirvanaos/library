#ifndef NIRVANA_CORE_REAL_COPY_H_
#define NIRVANA_CORE_REAL_COPY_H_

namespace Nirvana {

template <class InIt, class OutIt>
inline OutIt real_copy (InIt begin, InIt end, OutIt dst)
{
	while (begin != end)
		*(dst++) = *(begin++);

	return dst;
}

// Partial specialization for performance

template <>
uint8_t* real_copy (const uint8_t* begin, const uint8_t* end, uint8_t* dst);

template <class BidIt1, class BidIt2>
inline void real_move (BidIt1 begin, BidIt1 end, BidIt2 dst)
{
	if (dst <= begin || dst >= end)
		while (begin != end)
			*(dst++) = *(begin++);
	else
		while (begin != end)
			*(--dst) = *(--end);
}

// Partial specialization for performance

template <>
void real_move (const uint8_t* begin, const uint8_t* end, uint8_t* dst);

}

#endif
