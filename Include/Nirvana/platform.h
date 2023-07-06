/// \file
/// \brief Supported platforms
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
#ifndef NIRVANA_PLATFORM_H_
#define NIRVANA_PLATFORM_H_
#pragma once

#include "NirvanaBase.h"

namespace Nirvana {

/// Intel 386.
const uint16_t PLATFORM_I386 = 0x014c;

/// AMD64
const uint16_t PLATFORM_X64 = 0x8664;

/// ARM Little-Endian
const uint16_t PLATFORM_ARM = 0x01c0;

/// ARM64 Little-Endian
const uint16_t PLATFORM_ARM64 = 0xAA64;

#if defined (_M_IX86)
const uint16_t PLATFORM = PLATFORM_I386;
#elif defined (_M_X64)
const uint16_t PLATFORM = PLATFORM_X64;
#elif defined (_M_ARM)
const uint16_t PLATFORM = PLATFORM_ARM;
#elif defined (_M_ARM64)
const uint16_t PLATFORM = PLATFORM_ARM64;
#else
#error Unsupported platform
#endif

}

#endif
