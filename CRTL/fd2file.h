#ifndef _FDTOFILE_H_
#define _FDTOFILE_H_
#pragma once

#include <stdio.h>
#include <limits>

inline FILE* fd2file (int fd) noexcept
{
	if (fd > 0)
		return (FILE*)(uintptr_t)(fd + 1);
	else
		return nullptr;
}

inline int file2fd (FILE* f) noexcept
{
	if (f) {
		uintptr_t fd = (uintptr_t)f - 1;
		if (fd <= (uintptr_t)std::numeric_limits <short>::max ())
			return (int)fd;
	}
	return -1;
}

#endif
