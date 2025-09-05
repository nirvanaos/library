#include <wctype.h>
#include "local.h"

wint_t towupper (wint_t c)
{
  return towctrans (c, WCT_TOUPPER);
}
