#ifndef _ERRNO_H_
#define _ERRNO_H_

typedef int errno_t;

#ifdef __cplusplus
extern "C" {
#endif

int* _errno (void);
#define errno (*_errno())

#ifdef __cplusplus
}
#endif

#endif