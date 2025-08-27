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
#ifndef NIRVANA_CRTL_WINDOWS_CRTDEFS_H_
#define NIRVANA_CRTL_WINDOWS_CRTDEFS_H_
#pragma once

#pragma section(".CRT$XIA", long, read) // First C Initializer
#pragma section(".CRT$XIZ", long, read) // Last C Initializer
#pragma section(".CRT$XCA", long, read) // First C++ Initializer
#pragma section(".CRT$XCZ", long, read) // Last C++ Initializer
#pragma section(".CRT$XPA", long, read) // First Pre-Terminator
#pragma section(".CRT$XPZ", long, read) // Last Pre-Terminator
#pragma section(".CRT$XTA", long, read) // First Terminator
#pragma section(".CRT$XTZ", long, read) // Last Terminator
#pragma section(".CRT$XDA", long, read) // First Dynamic TLS Initializer
#pragma section(".CRT$XDZ", long, read) // Last Dynamic TLS Initializer

#pragma section(".CRT$XLC", long, read) // CRT TLS Constructor
#pragma section(".CRT$XLD", long, read) // CRT TLS Terminator

#pragma section(".CRT$XLA", long, read) // First Loader TLS Callback
#pragma section(".CRT$XLZ", long, read) // Last Loader TLS Callback

#pragma section(".rdata$T", long, read)

#if defined _MSC_VER && !defined (__clang__)

#define _CRTALLOC(x) __declspec (allocate(x))

#define _THREAD_LOCAL __declspec(thread)

#else

#define _CRTALLOC(x) __attribute__ ((section (x)))

#define _THREAD_LOCAL _Thread_local

#endif

typedef void (__cdecl* _PVFV) (void);
typedef int (__cdecl* _PIFV) (void);

typedef void (__stdcall *PIMAGE_TLS_CALLBACK) (
	void* DllHandle,
	unsigned long Reason,
	void* Reserved
);

#define DLL_PROCESS_ATTACH   1    
#define DLL_THREAD_ATTACH    2    
#define DLL_THREAD_DETACH    3    
#define DLL_PROCESS_DETACH   0    

#endif
