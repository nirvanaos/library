#include <stdint.h>
#include <stdlib.h>

extern "C" void __cdecl _invalid_parameter (
  wchar_t const* const expression,
  wchar_t const* const function_name,
  wchar_t const* const file_name,
  unsigned int   const line_number,
  uintptr_t      const reserved
)
{
  abort ();
}

extern "C" void __cdecl _invalid_parameter_noinfo ()
{
  _invalid_parameter (nullptr, nullptr, nullptr, 0, 0);
}

// This is used by inline code in the C++ Standard Library and the SafeInt
// library.  Because it is __declspec(noreturn), the compiler can better
// optimize use of the invalid parameter handler for inline code.
extern "C" __declspec(noreturn) void __cdecl _invalid_parameter_noinfo_noreturn ()
{
  _invalid_parameter (nullptr, nullptr, nullptr, 0, 0);
}
