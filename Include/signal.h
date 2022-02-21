/// \file
/// POSIX C signal API.
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
#ifndef SIGNAL_H_
#define SIGNAL_H_
#pragma once

#define SIGINT          2   ///< Interrupt
#define SIGILL          4   ///< Illegal instruction - invalid function image
#define SIGABRT         6   ///< Abnormal termination triggered by abort call
#define SIGFPE          8   ///< Floating point exception
#define SIGSEGV         11  ///< Access violation
#define SIGTERM         15  ///< Software termination signal from kill

#define NSIG            16  ///< Maximum signal number + 1

typedef int sig_atomic_t;
typedef unsigned int sigset_t;

typedef void (*SignalHandler)(int);

// Signal action codes
#define SIG_DFL ((SignalHandler)0)     ///< default signal action
#define SIG_IGN ((SignalHandler)1)     ///< ignore signal
#define SIG_GET ((SignalHandler)2)     ///< return current value

/// Signal error value (returned by signal() call on error)
#define SIG_ERR ((SignalHandler)-1)

/// Data passed with signal notification.
typedef union sigval
{
	int   sival_int; ///< Integer value
	void* sival_ptr; ///< Pointer value
} sigval_t;

/// siginfo_t
typedef struct siginfo
{
	/// The system-generated signal number.
	int si_signo;

	/// If non-zero, an errno value associated with
	/// this signal, as defined in <errno.h>.
	int si_errno;

	/// Contains a code identifying the cause of the signal.
	int si_code;

	/// Signal value
	sigval_t si_value;

	/// Exception code or CORBA::Exception::EC_NO_EXCEPTION
	int si_excode;

	/// Exit value or signal.
	int si_status;
} siginfo_t;

/// `si_code' values for SIGILL signal.
enum
{
	ILL_ILLOPC = 1,                /* Illegal opcode.  */
	ILL_ILLOPN,                        /* Illegal operand.  */
	ILL_ILLADR,                        /* Illegal addressing mode.  */
	ILL_ILLTRP,                        /* Illegal trap. */
	ILL_PRVOPC,                        /* Privileged opcode.  */
	ILL_PRVREG,                        /* Privileged register.  */
	ILL_COPROC,                        /* Coprocessor error.  */
	ILL_BADSTK                        /* Internal stack error.  */
};

/// `si_code' values for SIGFPE signal.
enum
{
	FPE_INTDIV = 1,                /* Integer divide by zero.  */
	FPE_INTOVF,                        /* Integer overflow.  */
	FPE_FLTDIV,                        /* Floating point divide by zero.  */
	FPE_FLTOVF,                        /* Floating point overflow.  */
	FPE_FLTUND,                        /* Floating point underflow.  */
	FPE_FLTRES,                        /* Floating point inexact result.  */
	FPE_FLTINV,                        /* Floating point invalid operation.  */
	FPE_FLTSUB                        /* Subscript out of range.  */
};

/// `si_code' values for SIGSEGV signal.
enum
{
	SEGV_MAPERR = 1, ///< Access to uncommitted memory range.
	SEGV_ACCERR      ///< Attempting to write into read-only memory.
};

typedef struct sigaction
{
	///  Pointer to a signal - catching function or one of the SIG_IGN or SIG_DFL.
	SignalHandler sa_handler;

	/// Set of signals to be blocked during execution of the signal handling function.
	sigset_t sa_mask;

	/// Special flags.
	int      sa_flags;

	/// Pointer to a signal - catching function.
	void (*sa_sigaction) (int, siginfo_t*, void*);
} sigaction_t;

#ifdef __cplusplus

namespace Nirvana {

// Native types for IDL
typedef sigaction_t* SigactionPtr;
typedef const sigaction_t* SigactionConstPtr;

}

extern "C" {
#endif

SignalHandler signal (int signum, SignalHandler func);

/// Raise signal.
/// 
/// \param signum Signal number.
/// \returns 0 if success.
int raise (int signum);

int sigaction (int signal, const struct sigaction* act, struct sigaction* oldact);

#ifdef __cplusplus
}
#endif

#endif
