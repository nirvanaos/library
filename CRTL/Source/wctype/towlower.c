#include <wctype.h>
#include "local.h"

wint_t towlower (wint_t c)
{
  return towctrans (c, WCT_TOLOWER);
}
