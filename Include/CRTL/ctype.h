#ifndef _CTYPE_H_
#define _CTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

int isalnum(int);
int isalpha(int);

inline int isascii (int c)
{
	return ((unsigned)c & ~0x7F) == 0;
}

int isblank(int);
int iscntrl(int);
int isdigit(int);
#define isdigit_l(c, l) isdigit (c)
int isgraph (int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace (int);
int isupper(int);
int isxdigit(int);
#define isxdigit_l(c, l) isxdigit (c)
int tolower(int);
int toupper(int);

#ifdef __cplusplus
}
#endif

#endif