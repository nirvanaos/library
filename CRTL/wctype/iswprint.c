#include <wctype.h>
#include "categories.h"

int iswprint (wint_t c)
{
  enum category cat = category (c);
  return cat != -1
      && cat != CAT_Cc && cat != CAT_Cf
      && cat != CAT_Cs // Surrogate
      ;
}
