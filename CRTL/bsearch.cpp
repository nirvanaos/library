/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
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
*
* Copyright (C) 2015-2025 mlibc Contributors
*/

#include <stdlib.h>
#include <assert.h>

void *bsearch (const void *key, const void *base, size_t count, size_t size,
		int (*compare)(const void *, const void *)) {
	// Invariant: Element is in the interval [i, j).
	size_t i = 0;
	size_t j = count;

	while (i < j) {
		size_t k = (j - i) / 2;
		auto element = reinterpret_cast <const char*> (base) + (i + k) * size;
		auto res = compare (key, element);
		if (res < 0) {
			j = i + k;
		} else if (res > 0) {
			i = i + k + 1;
		} else {
			return const_cast <char*> (element);
		}
	}
	assert (i == j);

	return nullptr;
}

