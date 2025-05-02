/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
// Don't use _FENV_H_ include guard because it is used inside openlibm_fenv.h
#ifndef _CRTL_FENV_H_
#define _CRTL_FENV_H_
#pragma once

#if defined (_MSC_VER) && !defined (__clang__)

#include <corecrt.h>

#pragma push_macro ("_ACRTIMP")

#undef _ACRTIMP
#define _ACRTIMP __declspec(dllimport)

#include <float.h>

#pragma pop_macro ("_ACRTIMP")

_CRT_BEGIN_C_HEADER

#define FE_TONEAREST  _RC_NEAR
#define FE_UPWARD     _RC_UP
#define FE_DOWNWARD   _RC_DOWN
#define FE_TOWARDZERO _RC_CHOP

#define FE_ROUND_MASK _MCW_RC

_ACRTIMP int __cdecl fegetround (void);
_ACRTIMP int __cdecl fesetround (_In_ int _Round);

typedef unsigned long fexcept_t;

typedef struct fenv_t
{
  unsigned long _Fe_ctl, _Fe_stat;
} fenv_t;

#define FE_INEXACT   _SW_INEXACT       // _EM_INEXACT     0x00000001 inexact (precision)
#define FE_UNDERFLOW _SW_UNDERFLOW     // _EM_UNDERFLOW   0x00000002 underflow
#define FE_OVERFLOW  _SW_OVERFLOW      // _EM_OVERFLOW    0x00000004 overflow
#define FE_DIVBYZERO _SW_ZERODIVIDE    // _EM_ZERODIVIDE  0x00000008 zero divide
#define FE_INVALID   _SW_INVALID       // _EM_INVALID     0x00000010 invalid

#define FE_ALL_EXCEPT (FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

_ACRTIMP int __cdecl fegetenv (_Out_ fenv_t* _Env);
_ACRTIMP int __cdecl fesetenv (_In_ fenv_t const* _Env);
_ACRTIMP int __cdecl feclearexcept (_In_ int _Flags);
_ACRTIMP _Success_ (return == 0) int __cdecl feholdexcept (_Out_ fenv_t * _Env);
_ACRTIMP int __cdecl fetestexcept (_In_ int _Flags);
_ACRTIMP int __cdecl fegetexceptflag (_Out_ fexcept_t * _Except, _In_ int _TestFlags);
_ACRTIMP int __cdecl fesetexceptflag (_In_ fexcept_t const* _Except, _In_ int _SetFlags);

__inline int __CRTDECL feraiseexcept (_In_ int _Except)
{
  static struct
  {
    int    _Except_Val;
    double _Num;
    double _Denom;
  } const _Table [] =
  {  // Raise exception by evaluating num / denom:
      {FE_INVALID,   0.0,    0.0    },
      {FE_DIVBYZERO, 1.0,    0.0    },
      {FE_OVERFLOW,  1e+300, 1e-300 },
      {FE_UNDERFLOW, 1e-300, 1e+300 },
      {FE_INEXACT,   2.0,    3.0    }
  };

  double _Ans = 0.0;
  (void)_Ans; // Suppress set-but-not-used warnings. _Ans is not "used" in the traditional static-analysis sense, but it is needed to trigger a floating point exception below.
  size_t _Index;

  if ((_Except &= FE_ALL_EXCEPT) == 0)
  {
    return 0;
  }

  // Raise the exceptions not masked:
  for (_Index = 0; _Index < sizeof (_Table) / sizeof (_Table [0]); ++_Index)
  {
    if ((_Except & _Table [_Index]._Except_Val) != 0)
    {
      _Ans = _Table [_Index]._Num / _Table [_Index]._Denom;

      // x87 exceptions are raised immediately before execution of the
      // next floating point instruction.  If we're using /arch:IA32,
      // force the exception to be raised immediately:
#if defined _M_IX86 && _M_IX86_FP == 0 && !defined _M_HYBRID_X86_ARM64
      __asm fwait;
#endif
    }
  }

  return 0;
}

_CRT_END_C_HEADER

#else

#include <openlibm_fenv.h>

#endif

#endif
