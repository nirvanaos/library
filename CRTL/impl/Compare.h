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
#ifndef CRTL_IMPL_COMPARE_H_
#define CRTL_IMPL_COMPARE_H_
#pragma once

#include "strutl.h"

namespace CRTL {

class Compare
{
public:
	template <typename C>
	static int compare (const C* lp, const C* rp, size_t maxlen, bool zero_term) noexcept;

private:
	static int stop (int l, int r, int not_zeroterm)
	{
		int not_equal = l ^ r;
		int lzt = ~(not_zeroterm | l);
		int rzt = ~(not_zeroterm | r);
		// return not_equal || lzt || rzt;
		return not_equal | lzt | rzt;
	}

};

template <typename C>
int Compare::compare (const C* lp, const C* rp, size_t maxlen, bool zero_term) noexcept
{
	const C* end = (const C*)std::numeric_limits <const C*>::max ();
	if ((size_t)(end - lp) > maxlen)
		end = lp + maxlen;

	int ztc = zero_term ? ~0 : 0;

	if (sizeof (UWord) > sizeof (C) && ((uintptr_t)lp % sizeof (UWord) == (uintptr_t)rp % sizeof (UWord))) {
		const UWord* lwp = (const UWord*)Nirvana::round_up (lp, sizeof (UWord));
		const UWord* lwp_end = (const UWord*)Nirvana::round_down (end, sizeof (UWord));
		if (lwp < lwp_end) {
			while (lp < (const C*)lwp) {
				int l = *(lp++), r = *(rp++);
				if (stop (l, r, ztc))
					return l - r;
			}

			UWord ztw = zero_term ? ~0 : 0;
			const UWord* rwp = (const UWord*)rp;
			for (;;) {
				UWord l = *lwp, r = *rwp;
				if ((l ^ r) | (ztw & (detect_null <sizeof (C)> (l) | detect_null <sizeof (C)> (r))))
					break;
			}
			lp = (const C*)lwp;
			rp = (const C*)rwp;
		}
	}

	while (lp < end) {
		int l = *(lp++), r = *(rp++);
		if (stop (l, r, ztc))
			return l - r;
	}

	return 0;
}

}

#endif

