#include <wctype.h>
#include "categories.h"

int iswblank (wint_t c)
{
  enum category cat = category (c);
  // exclude "<noBreak>"?
  return cat == CAT_Zs
      || c == '\t';
}
