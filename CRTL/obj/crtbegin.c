//===-- crtbegin.c - Start of constructors and destructors ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <stddef.h>

__extension__ void *const __EH_FRAME_LIST__[]
    __attribute__((section(".eh_frame"), aligned(sizeof(void *)))) = {};

extern char __attribute__ ((alias("__EH_FRAME_LIST__"))) __eh_frame_start;

#ifndef CRT_HAS_INITFINI_ARRAY

typedef void (*fp)(void);
fp __CTOR_LIST__[]
    __attribute__((section(".ctors"), aligned(sizeof(fp)))) = {(fp)-1};
fp __DTOR_LIST__[]
    __attribute__((section(".dtors"), aligned(sizeof(fp)))) = {(fp)-1};

#endif
