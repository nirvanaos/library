#include <stddef.h>

typedef void (*fp)(void);
extern fp __CTOR_LIST__[];
extern fp __CTOR_LIST_END__[];
extern fp __DTOR_LIST__[];
extern fp __DTOR_LIST_END__[];

void __do_init(void) {
  static _Bool __initialized;
  if (__builtin_expect(__initialized, 0))
    return;
  __initialized = 1;

  const size_t n = __CTOR_LIST_END__ - __CTOR_LIST__ - 1;
  for (size_t i = n; i >= 1; i--) __CTOR_LIST__[i]();
}

void __do_fini(void) {
  static _Bool __finalized;
  if (__builtin_expect(__finalized, 0))
    return;
  __finalized = 1;

  const size_t n = __DTOR_LIST_END__ - __DTOR_LIST__ - 1;
  for (size_t i = 1; i <= n; i++) __DTOR_LIST__[i]();
}
