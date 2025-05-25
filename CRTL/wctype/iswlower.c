#include <wctype.h>
#include "categories.h"

int iswlower (wint_t c)
{
  enum category cat = category (c);
  return cat == CAT_Ll || (cat == CAT_LC && towlower (c) == c);
}
