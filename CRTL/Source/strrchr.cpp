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
#include <string.h>
#include <wchar.h>

extern "C" char* strrchr (const char *s, int c)
{
  const char *last = nullptr;
  if (c) {
    while ((s = strchr (s, c))) {
	  	last = s;
	  	s++;
		}
  } else
    last = strchr (s, c);

  return const_cast <char*> (last);
}

extern "C" wchar_t* wcsrchr (const wchar_t* s, wchar_t c)
{
  const wchar_t *last = nullptr;
  if (c) {
    while ((s = wcschr (s, c))) {
	  	last = s;
	  	s++;
		}
  } else
    last = wcschr (s, c);

  return const_cast <wchar_t*> (last);
}
