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

static int qsort_callback(const void *a, const void *b, void *arg) {
	auto compare = reinterpret_cast<int (*)(const void *, const void *)>(arg);

	return compare(a, b);
}

extern "C" void qsort(void *base, size_t count, size_t size,
		int (*compare)(const void *, const void *)) {
	return qsort_r(base, count, size, qsort_callback, (void *) compare);
}

extern "C" void qsort_r(void *base, size_t count, size_t size,
		int (*compare)(const void *, const void *, void *),
		void *arg) {
	// TODO: implement a faster sort
	for(size_t i = 0; i < count; i++) {
		void *u = (void *)((uintptr_t)base + i * size);
		for(size_t j = i + 1; j < count; j++) {
			void *v = (void *)((uintptr_t)base + j * size);
			if(compare(u, v, arg) <= 0)
				continue;

			// swap u and v
			char *u_bytes = (char *)u;
			char *v_bytes = (char *)v;
			for(size_t k = 0; k < size; k++) {
				char temp = u_bytes[k];
				u_bytes[k] = v_bytes[k];
				v_bytes[k] = temp;
			}
		}
	}
}
