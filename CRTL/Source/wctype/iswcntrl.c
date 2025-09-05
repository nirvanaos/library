#include <wctype.h>
#include "categories.h"

int iswcntrl (wint_t c)
{
  enum category cat = category (c);
  return cat == CAT_Cc
      || cat == CAT_Zl || cat == CAT_Zp // Line/Paragraph Separator
      ;
}
