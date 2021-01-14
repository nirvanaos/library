#include <Nirvana/real_copy.h>
#include <Nirvana/NirvanaBase.h>

namespace Nirvana {

template <>
uint8_t* real_copy (const uint8_t* begin, const uint8_t* end, uint8_t* dst)
{
	const uint8_t* aligned_begin = round_up (begin, sizeof (UWord));
	const uint8_t* aligned_end = round_down (end, sizeof (UWord));

	while (begin != aligned_begin)
		*(dst++) = *(begin++);

	if (aligned_begin < aligned_end) {

		while (begin != aligned_begin)
			*(dst++) = *(begin++);

		do {
			*(UWord*)dst = *(UWord*)begin;
			dst += sizeof (UWord);
			begin += sizeof (UWord);
		} while (begin != aligned_end);

	}

	while (begin != end)
		*(dst++) = *(begin++);

	return dst;
}

template <>
void real_move (const uint8_t* begin, const uint8_t* end, uint8_t* dst)
{
	const uint8_t* aligned_begin = round_up (begin, sizeof (UWord));
	const uint8_t* aligned_end = round_down (end, sizeof (UWord));

	if (dst <= begin || dst >= end) {

		if (aligned_begin < aligned_end) {

			while (begin != aligned_begin)
				*(dst++) = *(begin++);

			do {
				*(UWord*)dst = *(UWord*)begin;
				dst += sizeof (UWord);
				begin += sizeof (UWord);
			} while (begin != aligned_end);

		}

		while (begin != end)
			*(dst++) = *(begin++);

	} else {

		if (aligned_begin < aligned_end) {

			while (end != aligned_end)
				*(--dst) = *(--end);

			do {
				dst -= sizeof (UWord);
				end -= sizeof (UWord);
				*(UWord*)dst = *(UWord*)end;
			} while (end != aligned_begin);

		}

		while (end != begin)
			*(--dst) = *(--end);

	}
}

}
