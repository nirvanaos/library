#include <wctype.h>
#include "categories.h"

int iswspace (wint_t c)
{
  enum category cat = category (c);
  // exclude "<noBreak>"?
  return cat == CAT_Zs
      || cat == CAT_Zl || cat == CAT_Zp // Line/Paragraph Separator
      || (c >= 0x9 && c <= 0xD);
}
