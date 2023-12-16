#include <limits.h>
#include <locale.h>

static char lconv_static_decimal []{ "." };
static char lconv_static_null []{ "" };
static char lconv_static_minus []{ "-" };

// TODO: Temporary stub, implement.

/* lconv settings for "C" locale */
static struct lconv lconv_c
{
  .decimal_point = lconv_static_decimal,
  .thousands_sep = lconv_static_null,
  .grouping = lconv_static_null,
  .mon_decimal_point = lconv_static_null,
  .mon_thousands_sep = lconv_static_null,
  .mon_grouping = lconv_static_null,
  .positive_sign = lconv_static_null,
  .negative_sign = lconv_static_minus,
  .int_curr_symbol = lconv_static_null,
  .currency_symbol = lconv_static_null,
  .int_frac_digits = CHAR_MAX,
  .frac_digits = CHAR_MAX,
  .p_cs_precedes = CHAR_MAX,
  .p_sep_by_space = CHAR_MAX,
  .n_cs_precedes = CHAR_MAX,
  .n_sep_by_space = CHAR_MAX,
  .p_sign_posn = CHAR_MAX,
  .n_sign_posn = CHAR_MAX,
  .int_n_cs_precedes = CHAR_MAX,
  .int_n_sign_posn = CHAR_MAX,
  .int_p_cs_precedes = CHAR_MAX,
  .int_p_sep_by_space = CHAR_MAX,
  .int_p_sign_posn = CHAR_MAX
};

extern "C" lconv * __cdecl localeconv ()
{
  return &lconv_c;
}
