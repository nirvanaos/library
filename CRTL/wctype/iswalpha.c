#include <wctype.h>
#include "categories.h"

int iswalpha (wint_t c)
{
  enum category cat = category (c);
  return cat == CAT_LC || cat == CAT_Lu || cat == CAT_Ll || cat == CAT_Lt
      || cat == CAT_Lm || cat == CAT_Lo
      || cat == CAT_Nl // Letter_Number
      ;
}
