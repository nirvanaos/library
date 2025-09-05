#include <wctype.h>
#include "local.h"

int iswctype (wint_t c, wctype_t desc)
{
  switch (desc)
    {
    case WC_ALNUM:
      return iswalnum (c);
    case WC_ALPHA:
      return iswalpha (c);
    case WC_BLANK:
      return iswblank (c);
    case WC_CNTRL:
      return iswcntrl (c);
    case WC_DIGIT:
      return iswdigit (c);
    case WC_GRAPH:
      return iswgraph (c);
    case WC_LOWER:
      return iswlower (c);
    case WC_PRINT:
      return iswprint (c);
    case WC_PUNCT:
      return iswpunct (c);
    case WC_SPACE:
      return iswspace (c);
    case WC_UPPER:
      return iswupper (c);
    case WC_XDIGIT:
      return iswxdigit (c);
    default:
      return 0; /* eliminate warning */
    }

  /* otherwise unknown */
  return 0;
}
