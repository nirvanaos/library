#include <wctype.h>
#include "categories.h"

int iswgraph (wint_t c)
{
  enum category cat = category (c);
  return cat != -1
      && cat != CAT_Cc && cat != CAT_Cf
      && cat != CAT_Cs // Surrogate
      && cat != CAT_Zs
      && cat != CAT_Zl && cat != CAT_Zp // Line/Paragraph Separator
      ;
}
