/*
* Nirvana IDL support library.
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
*/
#include <Nirvana/Fixed.h>
#include <sstream>
#include <istream>
#include <ostream>

using namespace std;

namespace Nirvana {

Fixed::Fixed (double val)
{
	stringstream ss;
	ss << val;
	g_dec_calc->from_string (val_, ss.str ().c_str ());
}

Fixed::Fixed (long double val)
{
	ostringstream ss;
	ss << val;
	g_dec_calc->from_string (val_, ss.str ().c_str ());
}

Fixed::operator long double () const
{
	string s = g_dec_calc->to_string (val_);
	istringstream ss (s);
	long double val;
	ss >> val;
	return val;
}

istream& operator >> (istream& is, Fixed& val)
{
	istream::sentry sentry (is);
	if (sentry) {
		istream::int_type c = is.peek ();
		if (!(('0' <= c && c <= '9') || '.' == c || '-' == c || '+' == c))
			is.setstate (istream::failbit);
		else {
			char buf [65];
			char* pbuf = buf;
			bool decpt = false;
			unsigned digits = 0;
			for (;;) {
				c = is.get ();
				if ('.' == c)
					decpt = true;
				else if ('0' <= c && c <= '9')
					++digits;
				*(pbuf++) = c;
				if (istream::traits_type::not_eof (c = is.peek ())) {
					if ('.' == c) {
						if (decpt)
							break;
					} else if (!('0' <= c && c <= '9'))
						break;
					else if (digits >= 62) {
						is.setstate (istream::failbit);
						break;
					}
				} else
					break;
			}
			if (!is.fail ()) {
				*pbuf = '\0';
				g_dec_calc->from_string (val.val_, buf);
				return is;
			}
		}
	}
	g_dec_calc->from_long (val.val_, 0);
	return is;
}

ostream& operator << (ostream& os, const Fixed& val)
{
	return os << val.to_string ();
}

}
