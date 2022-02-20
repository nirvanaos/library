/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
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
#ifndef SIGINFO_H_
#define SIGINFO_H_
#pragma once

/// Data passed with signal notification.
union sigval
{
	int   sival_int; ///< Integer value
	void* sival_ptr; ///< Pointer value
};

/// siginfo_t
typedef struct
{
	/// The system-generated signal number.
	int          si_signo;

	/// If non-zero, it contains an error number associated with this signal, as defined in <errno.h>.
	int          si_errno;
	
	/// Contains a code identifying the cause of the signal.
	int          si_code;

	/// Signal value
	union sigval si_value;
}
siginfo_t;

/// `si_code' values for SIGILL signal.
enum
{
	ILL_ILLOPC = 1,                /* Illegal opcode.  */
# define ILL_ILLOPC        ILL_ILLOPC
	ILL_ILLOPN,                        /* Illegal operand.  */
# define ILL_ILLOPN        ILL_ILLOPN
	ILL_ILLADR,                        /* Illegal addressing mode.  */
# define ILL_ILLADR        ILL_ILLADR
	ILL_ILLTRP,                        /* Illegal trap. */
# define ILL_ILLTRP        ILL_ILLTRP
	ILL_PRVOPC,                        /* Privileged opcode.  */
# define ILL_PRVOPC        ILL_PRVOPC
	ILL_PRVREG,                        /* Privileged register.  */
# define ILL_PRVREG        ILL_PRVREG
	ILL_COPROC,                        /* Coprocessor error.  */
# define ILL_COPROC        ILL_COPROC
	ILL_BADSTK                        /* Internal stack error.  */
# define ILL_BADSTK        ILL_BADSTK
};

/// `si_code' values for SIGFPE signal.
enum
{
	FPE_INTDIV = 1,                /* Integer divide by zero.  */
#define FPE_INTDIV        FPE_INTDIV
	FPE_INTOVF,                        /* Integer overflow.  */
#define FPE_INTOVF        FPE_INTOVF
	FPE_FLTDIV,                        /* Floating point divide by zero.  */
#define FPE_FLTDIV        FPE_FLTDIV
	FPE_FLTOVF,                        /* Floating point overflow.  */
#define FPE_FLTOVF        FPE_FLTOVF
	FPE_FLTUND,                        /* Floating point underflow.  */
#define FPE_FLTUND        FPE_FLTUND
	FPE_FLTRES,                        /* Floating point inexact result.  */
#define FPE_FLTRES        FPE_FLTRES
	FPE_FLTINV,                        /* Floating point invalid operation.  */
#define FPE_FLTINV        FPE_FLTINV
	FPE_FLTSUB                        /* Subscript out of range.  */
#define FPE_FLTSUB        FPE_FLTSUB
};

/// `si_code' values for SIGSEGV signal.
enum
{
	SEGV_MAPERR = 1,                /* Address not mapped to object.  */
# define SEGV_MAPERR        SEGV_MAPERR
	SEGV_ACCERR                        /* Invalid permissions for mapped object.  */
# define SEGV_ACCERR        SEGV_ACCERR
};

#endif
