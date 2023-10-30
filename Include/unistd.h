#ifndef _UNISTD_H_
#define _UNISTD_H_
#pragma once

#include <stdint.h>

typedef uintptr_t ssize_t;

extern "C" {

ssize_t read (int fildes, void* buf, size_t count);
ssize_t write (int fildes, const void* buf, size_t count);

}

#endif
