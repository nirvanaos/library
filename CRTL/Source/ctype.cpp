/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#include <ctype.h>
#include "impl/locale.h"
#include <Nirvana/locale_defs.h>

extern "C" int isalnum (int c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

extern "C" int isalnum_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_ALNUM;
	else
		return isalnum (c);
}

extern "C" int isalpha (int c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

extern "C" int isalpha_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_ALPHA;
	else
		return isalpha (c);
}

extern "C" int isblank (int c)
{
	return c == ' ' || c == '\t';
}

extern "C" int isblank_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_BLANK;
	else
		return isblank (c);
}

extern "C" int iscntrl (int c)
{
	return (c <= 0x1F) || (c == 0x7F);
}

extern "C" int iscntrl_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_CNTRL;
	else
		return iscntrl (c);
}

extern "C" int isdigit (int c)
{
	return c >= '0' && c <= '9';
}

extern "C" int isdigit_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_DIGIT;
	else
		return isdigit (c);
}

extern "C" int isgraph (int c)
{
	return c >= 0x21 && c <= 0x7E;
}

extern "C" int isgraph_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_GRAPH;
	else
		return isgraph (c);
}

extern "C" int islower (int c)
{
	return c >= 'a' && c <= 'z';
}

extern "C" int islower_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_LOWER;
	else
		return islower (c);
}

extern "C" int isprint (int c)
{
	return c >= 0x20 && c <= 0x7E;
}

extern "C" int isprint_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_PRINT;
	else
		return isprint (c);
}

extern "C" int ispunct (int c)
{
	return c == '!' || c == '"' || c == '#' || c == '$' || c == '%' || c == '&'
	|| c == '\'' || c == '(' || c == ')' || c == '*' || c == '+' || c == ','
	|| c == '-' || c == '.' || c == '/'
	|| c == ':' || c == ';' || c == '<' || c == '=' || c == '>' || c == '?'
	|| c == '@'
	|| c == '[' || c == '\\' || c == ']' || c == '^' || c == '_' || c == '`'
	|| c == '{' || c == '|' || c == '}' || c == '~';
}

extern "C" int ispunct_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_PUNCT;
	else
		return ispunct (c);
}

extern "C" int isspace (int c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

extern "C" int isspace_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_SPACE;
	else
		return isspace (c);
}

extern "C" int isupper (int c)
{
	return c >= 'A' && c <= 'Z';
}

extern "C" int isupper_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_UPPER;
	else
		return isupper (c);
}

extern "C" int isxdigit (int c)
{
	return c >= '0' && c <= '9';
}

extern "C" int isxdigit_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->get_category (c) == Nirvana::CodePage::CC_XDIGIT;
	else
		return isxdigit (c);
}

extern "C" int tolower (int c)
{
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 'a';
	else
		return c;
}

extern "C" int tolower_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->to_lower (c);
	else
		return tolower (c);
}

extern "C" int toupper (int c)
{
	if(c >= 'a' && c <= 'z')
		return c - 'a' + 'A';
	else
		return c;
}

extern "C" int toupper_l (int c, locale_t l)
{
	auto cp = CRTL::get_cp (l);
	if (cp)
		return cp->to_upper (c);
	else
		return toupper (c);
}
