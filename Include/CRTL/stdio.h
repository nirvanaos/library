#ifndef _STDIO_H_
#define _STDIO_H_

#include "sys/types.h"
#include <stdarg.h>

#ifndef _FILE_DEFINED
typedef struct _FILE
{
	int dummy;
} FILE;
#define _FILE_DEFINED
#endif

typedef uint64_t fpos_t;

#define BUFSIZ 512
#define EOF (-1)

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#define FILENAME_MAX 260
#define FOPEN_MAX 32767
#define TMP_MAX 32767

#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

inline FILE* __fd2file (int fd) noexcept
{
	if (fd > 0)
		return (FILE*)(uintptr_t)(fd + 1);
	else
		return nullptr;
}

inline int __file2fd (FILE* f) noexcept
{
	if (f)
		return (int)((uintptr_t)f - 1);
	else
		return -1;
}

#define stdin (__fd2file (0))
#define stdout (__fd2file (1))
#define stderr (__fd2file (2))

#ifdef __cplusplus
extern "C" {
#define restrict
#define rsize_t size_t
#endif

int asprintf (char** restrict, const char* restrict, ...);
int remove (const char*);
int rename (const char*, const char*);
FILE* tmpfile (void);
char* tmpnam (char* s);
int fclose (FILE*);
int fflush (FILE*);
FILE* fopen (const char* restrict, const char* restrict);
FILE* freopen (const char* restrict, const char* restrict, FILE* restrict);
void setbuf (FILE* restrict, char* restrict);
int setvbuf (FILE* restrict, char* restrict, int, size_t);
int fprintf (FILE* restrict, const char* restrict, ...);
int fscanf (FILE* restrict, const char* restrict, ...);
int printf (const char* restrict, ...);
int scanf (const char* restrict, ...);
int snprintf (char* restrict, size_t, const char* restrict, ...);
int sprintf (char* restrict, const char* restrict, ...);
int sprintf_s (char* restrict, rsize_t, const char* restrict, ...);
int sscanf (const char* restrict, const char* restrict, ...);
int vasprintf (char** restrict, const char* restrict, va_list);
int vfprintf (FILE* restrict, const char* restrict, va_list);
int vfscanf (FILE* restrict, const char* restrict, va_list);
int vprintf (const char* restrict, va_list);
int vscanf (const char* restrict, va_list);
int vsnprintf (char* restrict, size_t, const char* restrict, va_list);
int vsprintf (char* restrict, const char* restrict, va_list);
int vsscanf (const char* restrict, const char* restrict, va_list);
int fgetc (FILE*);
char* fgets (char* restrict, int, FILE* restrict);
int fputc (int, FILE*);
int fputs (const char* restrict, FILE* restrict);
int getc (FILE*);
int getchar (void);
char* gets (char*);  // removed in C++14
int putc (int, FILE*);
int putchar (int);
int puts (const char*);
int ungetc (int, FILE*);
size_t fread (void* restrict, size_t, size_t, FILE* restrict);
size_t fwrite (const void* restrict, size_t, size_t, FILE* restrict);
int fgetpos (FILE* restrict, fpos_t* restrict);
int fseek (FILE*, long, int);
int fsetpos (FILE*, const fpos_t*);
long ftell (FILE*);
void rewind (FILE*);
void clearerr (FILE*);
int feof (FILE*);
int ferror (FILE*);
void perror (const char*);

#ifdef _MSC_BUILD
#define _scprintf(format, ...) sprintf_s (nullptr, 0, format, __VA_ARGS__)
#endif

#ifdef __cplusplus
#undef restrict
#undef rsize_t
}
#endif

#endif
