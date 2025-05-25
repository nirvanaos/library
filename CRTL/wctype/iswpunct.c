#include <wctype.h>
#include "categories.h"

int iswpunct (wint_t c)
{
  enum category cat = category (c);
  return cat == CAT_Pc || cat == CAT_Pd || cat == CAT_Pe || cat == CAT_Pf || cat == CAT_Pi || cat == CAT_Po || cat == CAT_Ps
      || cat == CAT_Sm // Math Symbols
      // the following are included for backwards consistency:
      || cat == CAT_Sc // Currency Symbols
      || cat == CAT_Sk // Modifier_Symbol
      || cat == CAT_So // Other_Symbol
      || cat == CAT_No // Other_Number
      ;
}
