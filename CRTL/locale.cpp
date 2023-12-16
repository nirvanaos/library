#include <limits.h>
#include <locale.h>

static char lconv_static_decimal []{ "." };
static char lconv_static_null []{ "" };
static char lconv_static_minus []{ "-" };

// TODO: Temporary stub, implement.

/* lconv settings for "C" locale */
static struct lconv lconv_c
{
  lconv_static_decimal,
  lconv_static_null,
  lconv_static_null,
  lconv_static_null,
  lconv_static_null,
  lconv_static_null,
  lconv_static_null,
  lconv_static_minus,
  lconv_static_null,
  lconv_static_null,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX,
  CHAR_MAX
};

extern "C" lconv * __cdecl localeconv ()
{
  return &lconv_c;
}
