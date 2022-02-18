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

typedef int sig_atomic_t;

typedef void (*_crt_signal_t)(int);

#define SIGILL          4   // illegal instruction - invalid function image
#define SIGABRT         6   // abnormal termination triggered by abort call
#define SIGFPE          8   // floating point exception
#define SIGSEGV         11  // segment violation
#define SIGTERM         15  // Software termination signal from kill

#define NSIG            16  // maximum signal number + 1

// Signal action codes
#define SIG_DFL ((_crt_signal_t)0)     // default signal action
#define SIG_IGN ((_crt_signal_t)1)     // ignore signal

// Signal error value (returned by signal() call on error)
#define SIG_ERR ((_crt_signal_t)-1)    // signal error value

_crt_signal_t signal (int signal, _crt_signal_t func);

int raise (int signal);

#endif
