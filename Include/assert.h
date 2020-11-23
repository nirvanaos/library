#ifndef ASSERT_H_
#define ASSERT_H_

#undef verify
#undef assert

#ifdef NDEBUG

#define assert(exp) ((void)0)
#define verify(exp) (exp)

#else

#define assert(exp) (void)((!!(exp)) ||  (__debugbreak (), 1))
#define verify(exp) assert(exp)

#endif

#endif
