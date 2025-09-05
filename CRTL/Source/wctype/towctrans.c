#include <wctype.h>
#include <stdint.h>
#include <errno.h>
#include "local.h"

/*
   struct caseconv_entry describes the case conversion behaviour
   of a range of Unicode characters.
   It was designed to be compact for a minimal table size.
   The range is first...first + diff.
   Conversion behaviour for a character c in the respective range:
     mode == TOLO	towlower (c) = c + delta
     mode == TOUP	towupper (c) = c + delta
     mode == TOBOTH	(titling case characters)
			towlower (c) = c + 1
			towupper (c) = c - 1
     mode == TO1	capital/small letters are alternating
	delta == EVENCAP	even codes are capital
	delta == ODDCAP		odd codes are capital
			(this correlates with an even/odd first range value
			as of Unicode 10.0 but we do not rely on this)
   As of Unicode 10.0, the following field lengths are sufficient
	first: 17 bits
	diff: 8 bits
	delta: 17 bits
	mode: 2 bits
   The reserve of 4 bits (to limit the struct to 6 bytes)
   is currently added to the 'first' field;
   should a future Unicode version make it necessary to expand the others,
   the 'first' field could be reduced as needed, or larger ranges could
   be split up (reduce limit max=255 e.g. to max=127 or max=63 in 
   script mkcaseconv, check increasing table size).
 */
enum {TO1, TOLO, TOUP, TOBOTH};
enum {EVENCAP, ODDCAP};

static struct caseconv_entry {
  uint_least32_t first: 21;
  uint_least32_t diff: 8;
  uint_least32_t mode: 2;
  int_least32_t delta: 17;
}
caseconv_table [] = {
#include "caseconv.t"
};

#define first(ce)	ce.first
#define last(ce)	(ce.first + ce.diff)

/* auxiliary function for binary search in interval properties table */
static const struct caseconv_entry *
bisearch (wint_t ucs, const struct caseconv_entry *table, int max)
{
  int min = 0;
  int mid;

  if (ucs < first(table[0]) || ucs > last(table[max]))
    return 0;
  while (max >= min)
    {
      mid = (min + max) / 2;
      if (ucs > last(table[mid]))
	min = mid + 1;
      else if (ucs < first(table[mid]))
	max = mid - 1;
      else
	return &table[mid];
    }
  return 0;
}

static wint_t toulower (wint_t c)
{
  const struct caseconv_entry * cce =
    bisearch(c, caseconv_table,
             sizeof(caseconv_table) / sizeof(*caseconv_table) - 1);

  if (cce)
    switch (cce->mode)
      {
      case TOLO:
	return c + cce->delta;
      case TOBOTH:
	return c + 1;
      case TO1:
	switch (cce->delta)
	  {
	  case EVENCAP:
	    if (!(c & 1))
	      return c + 1;
	    break;
	  case ODDCAP:
	    if (c & 1)
	      return c + 1;
	    break;
	  default:
	    break;
	  }
	default:
	  break;
      }

  return c;
}

static wint_t touupper (wint_t c)
{
  const struct caseconv_entry * cce =
    bisearch(c, caseconv_table,
             sizeof(caseconv_table) / sizeof(*caseconv_table) - 1);

  if (cce)
    switch (cce->mode)
      {
      case TOUP:
	return c + cce->delta;
      case TOBOTH:
	return c - 1;
      case TO1:
	switch (cce->delta)
	  {
	  case EVENCAP:
	    if (c & 1)
	      return c - 1;
	    break;
	  case ODDCAP:
	    if (!(c & 1))
	      return c - 1;
	    break;
	  default:
	    break;
	  }
      default:
	break;
      }

  return c;
}

wint_t towctrans (wint_t c, wctrans_t w)
{
  if (w == WCT_TOLOWER)
    c = toulower (c);
  else if (w == WCT_TOUPPER)
    c = touupper (c);
  else
    errno = EINVAL;
  return c;
}
