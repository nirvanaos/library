#include <stdint.h>

namespace CRTL {

template <typename C, typename U> inline
C* utoa (U value, C* str, unsigned base)
{
	/* Check base is supported. */
	if ((base < 2) || (base > 36)) {
		str [0] = '\0';
		return nullptr;
	}

	/* Convert to string. Digits are in reverse order.  */
	int i = 0;
	do {
		unsigned remainder = value % base;
		str [i++] = remainder < 10 ? '0' + remainder : 'a' + remainder - 10;
		value = value / base;
	} while (value != 0);
	str [i] = '\0';

	/* Reverse string. */
	int j;
	for (j = 0, i--; j < i; j++, i--) {
		C c = str [j];
		str [j] = str [i];
		str [i] = c;
	}

	return str;
}

template <typename C, typename I> inline
C* itoa (I value, C* str, unsigned base)
{
	/* Check base is supported. */
	if ((base < 2) || (base > 36)) {
		str [0] = '\0';
		return nullptr;
	}

	/* Negative numbers are only supported for decimal.
	 * Cast to unsigned to avoid overflow for maximum negative value.  */
	I uvalue;
	int i = 0;
	if ((base == 10) && (value < 0)) {
		str [i++] = '-';
		uvalue = -value;
	} else
		uvalue = value;

	utoa (uvalue, str + i, base);
	return str;
}

}

extern "C"
char* _itoa (int value, char* str, unsigned base)
{
	return CRTL::itoa (value, str, base);
}

extern "C"
wchar_t* _itow (int value, wchar_t* str, unsigned base)
{
	return CRTL::itoa (value, str, base);
}

extern "C"
char* _ultoa (unsigned long value, char* str, unsigned base)
{
	return CRTL::utoa (value, str, base);
}

extern "C"
wchar_t* _ultow (unsigned long value, wchar_t* str, unsigned base)
{
	return CRTL::utoa (value, str, base);
}

extern "C"
char* _i64toa (int64_t value, char* str, unsigned base)
{
	return CRTL::itoa (value, str, base);
}

extern "C"
wchar_t* _i64tow (int64_t value, wchar_t* str, unsigned base)
{
	return CRTL::itoa (value, str, base);
}

extern "C"
char* _ui64toa (uint64_t value, char* str, unsigned base)
{
	return CRTL::utoa (value, str, base);
}

extern "C"
wchar_t* _ui64tow (uint64_t value, wchar_t* str, unsigned base)
{
	return CRTL::utoa (value, str, base);
}
