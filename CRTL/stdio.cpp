#include <stdio.h>
#include <unistd.h>
#include <fnctl.h>
#include <errno.h>
#include <sys/stat.h>
#include "fd2file.h"

extern "C" int fclose (FILE * f)
{
	return close (file2fd (f));
}

extern "C" int fflush (FILE * f)
{
	return fsync (file2fd (f));
}

extern "C" int fileno (FILE * f)
{
	return file2fd (f);
}

extern "C" FILE * fopen (const char* file, const char* mode)
{
	unsigned flags;

	switch (mode [0]) {
	case 'r': // open for reading
		flags = O_RDONLY;
		break;

	case 'w':  // open for writing
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		break;

	case 'a':  // open for appending
		flags = O_WRONLY | O_CREAT | O_APPEND;
		break;

	default:  // illegal mode
		errno = EINVAL;
		return nullptr;
	}

	char c;
	while (c = *++mode) {
		switch (c) {
		case '+':
			flags = (flags & ~O_ACCMODE) | O_RDWR;
			break;

		case 'b':
			break;

		case 't':
			flags |= O_TEXT;
			break;

		case 'x':
			flags |= O_EXCL;
			break;

		default:
			break;
		}
	}

	return fd2file (open (file, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
}

extern "C" int fputc (int c, FILE * f)
{
	unsigned char u = (unsigned char)c;
	if (write (file2fd (f), &u, 1) == 1)
		return c;
	else
		return EOF;
}

extern "C" size_t fread (void* buffer, size_t size, size_t count, FILE * f)
{
	return read (file2fd (f), buffer, size * count) / size;
}

extern "C" int fseek (FILE * f, long offset, int origin)
{
	return lseek (file2fd (f), offset, origin) >= 0 ? 0 : -1;
}

extern "C" off_t ftello (FILE * f)
{
	return lseek (file2fd (f), 0, SEEK_CUR);
}

extern "C" long ftell (FILE * f)
{
	return (long)lseek (file2fd (f), 0, SEEK_CUR);
}

extern "C" int remove (const char* path)
{
	return unlink (path);
}
