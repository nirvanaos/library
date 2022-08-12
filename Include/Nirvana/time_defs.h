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
#ifndef NIRVANA_TIME_DEFS_H_
#define NIRVANA_TIME_DEFS_H_
#pragma once

#include <stdint.h>

namespace Nirvana {

typedef uint64_t SteadyTime;
typedef SteadyTime DeadlineTime;

const DeadlineTime INFINITE_DEADLINE = UINT64_MAX;

}

namespace TimeBase {

// See TimeBase.idl for details.
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
