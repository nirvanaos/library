#include <wctype.h>
#include "categories.h"

int iswupper (wint_t c)
{
  // The wide-character class "upper" contains at least those characters wc
  // which are equal to towupper(wc) and different from towlower(wc).
  enum category cat = category (c);
  return cat == CAT_Lu || (cat == CAT_LC && towupper (c) == c);
}
