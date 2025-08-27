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
#include <type_traits>

namespace CRTL {

class Compare
{
public:
	template <typename C>
	static int compare (const C* lp, const C* rp, size_t maxlen, bool zero_term) noexcept;

private:
	static int stop (int l, int r, int zeroterm)
	{
		return (l ^ r) | (zeroterm & (is_null (l) | is_null (r)));
	}

};

template <typename C>
int Compare::compare (const C* const lp, const C* const rp, size_t maxlen, bool zero_term) noexcept
{
  using UC = std::make_unsigned <C>::type;
  const UC* lcp = (const UC*)lp;
  const UC* rcp = (const UC*)rp;
	const UC* end = get_end (lcp, maxlen);

	unsigned ztc = zero_term ? ~0 : 0;

	if (sizeof (UWord) > sizeof (C) && ((uintptr_t)lp % sizeof (UWord) == (uintptr_t)rp % sizeof (UWord))) {
		const UWord* lwp = (const UWord*)Nirvana::round_up (lcp, sizeof (UWord));
		const UWord* lwp_end = (const UWord*)Nirvana::round_down (end, sizeof (UWord));
		if (lwp < lwp_end) {
			while (lcp < (const UC*)lwp) {
				unsigned l = static_cast <unsigned> (*(lcp++)), r = static_cast <unsigned> (*(rcp++));
				if (stop (l, r, ztc))
					return l - r;
			}

			UWord ztw = zero_term ? ~(UWord)0 : 0;
			const UWord* rwp = (const UWord*)rcp;
			do {
				UWord l = *lwp, r = *rwp;
				if ((l ^ r) | (ztw & (detect_null <sizeof (C)> (l) | detect_null <sizeof (C)> (r))))
					break;
				++rwp;
				++lwp;
			} while (lwp != lwp_end);
			lcp = (const UC*)lwp;
			rcp = (const UC*)rwp;
		}
	}

	while (lcp < end) {
		unsigned l = static_cast <unsigned> (*(lcp++)), r = static_cast <unsigned> (*(rcp++));
		if (stop (l, r, ztc))
			return l - r;
	}

	return 0;
}

}

#endif

