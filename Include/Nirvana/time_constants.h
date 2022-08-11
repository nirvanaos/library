/// \file
/*
* Nirvana runtime library.
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
#ifndef NIRVANA_TIME_CONSTANTS_H_
#define NIRVANA_TIME_CONSTANTS_H_
#pragma once

#include <stdint.h>

namespace TimeBase {

/** TimeT represents a single time value, which is 64 bits in size, and holds the number of
100 nanoseconds that have passed since the base time. For absolute time the base is 15
October 1582 00:00 of the Gregorian Calendar. All absolute time shall be computed
using dates from the Gregorian Calendar. */
typedef uint64_t TimeT;

///@{
/// Time constants for convenience.

const TimeT MICROSECOND = 10;
const TimeT MILLISECOND = MICROSECOND * 1000;
const TimeT SECOND = MILLISECOND * 1000;
const TimeT MINUTE = SECOND * 60;
const TimeT HOUR = MINUTE * 60;
const TimeT DAY = HOUR * 24;

///@}

}

#endif
