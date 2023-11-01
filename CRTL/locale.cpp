#include <limits.h>
#include <locale.h>

static char    lconv_static_decimal []{ "." };
static char    lconv_static_null []{ "" };
static wchar_t lconv_static_W_decimal []{ L"." };
static wchar_t lconv_static_W_null []{ L"" };

// TODO: Temporary stub, implement.

/* lconv settings for "C" locale */
static struct lconv lconv_c
{
  lconv_static_decimal,   // decimal_point
    lconv_static_null,      // thousands_sep
    lconv_static_null,      // grouping
    lconv_static_null,      // int_curr_symbol
    lconv_static_null,      // currency_symbol
    lconv_static_null,      // mon_decimal_point
    lconv_static_null,      // mon_thousands_sep
    lconv_static_null,      // mon_grouping
    lconv_static_null,      // positive_sign
    lconv_static_null,      // negative_sign
    CHAR_MAX,                      // int_frac_digits
    CHAR_MAX,                      // frac_digits
    CHAR_MAX,                      // p_cs_precedes
    CHAR_MAX,                      // p_sep_by_space
    CHAR_MAX,                      // n_cs_precedes
    CHAR_MAX,                      // n_sep_by_space
    CHAR_MAX,                      // p_sign_posn
    CHAR_MAX,                      // n_sign_posn
    lconv_static_W_decimal, // _W_decimal_point
    lconv_static_W_null,    // _W_thousands_sep
    lconv_static_W_null,    // _W_int_curr_symbol
    lconv_static_W_null,    // _W_currency_symbol
    lconv_static_W_null,    // _W_mon_decimal_point
    lconv_static_W_null,    // _W_mon_thousands_sep
    lconv_static_W_null,    // _W_positive_sign
    lconv_static_W_null,    // _W_negative_sign
};

extern "C" lconv * __cdecl localeconv ()
{
  return &lconv_c;
}
