/// \file
/// \brief Supported platforms
/*
* Nirvana runtime library.
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
#ifndef NIRVANA_PLATFORM_H_
#define NIRVANA_PLATFORM_H_
#pragma once

#include "NirvanaBase.h"
#include <stdint.h>
#include <stddef.h>
#include <float.h>

#define NIRVANA_X86() (defined (_M_IX86) || defined (__i386__))
#define NIRVANA_X64() (defined (_M_X64) || defined (__x86_64__))
#define NIRVANA_ARM() (defined (_M_ARM) || defined (__arm__))
#define NIRVANA_ARM64() (defined (_M_ARM64) || defined (__aarch64__))

#define NIRVANA_PLATFORM(x) (0 != NIRVANA_##x())

namespace Nirvana {

/// Intel 386.
const uint16_t PLATFORM_X86 = 0x014c;

/// AMD64
const uint16_t PLATFORM_X64 = 0x8664;

/// ARM Little-Endian
const uint16_t PLATFORM_ARM = 0x01c0;

/// ARM64 Little-Endian
const uint16_t PLATFORM_ARM64 = 0xAA64;

const uint16_t PLATFORM =

#if NIRVANA_PLATFORM (X86)
PLATFORM_X86;
#elif NIRVANA_PLATFORM (X64)
PLATFORM_X64;
#elif NIRVANA_PLATFORM (ARM)
PLATFORM_ARM;
#elif NIRVANA_PLATFORM (ARM64)
PLATFORM_ARM64;
#else
#error Unsupported platform
#endif

const char* get_platform_name (unsigned id) noexcept;

/// Is CPU requires strictly alignment
const bool STRICT_ALIGN = (PLATFORM == PLATFORM_ARM || PLATFORM == PLATFORM_ARM64);

/// \brief The machine word
/// TODO: For some 32-bit platform we can use 64-bit word
typedef size_t UWord;
typedef ptrdiff_t Word;

/// \brief Maximal supported floating point type
#if (LDBL_MANT_DIG != DBL_MANT_DIG)
typedef long double FloatMax;
#elif (DBL_MANT_DIG != FLT_MANT_DIG)
typedef double FloatMax;
#else
typedef float FloatMax;
#endif

// Minimal page size for platform
// Zero if no virtual memory.
const size_t PAGE_SIZE_MIN = 4096;

}

#endif
