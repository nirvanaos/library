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

#include "lpch.h"
#include <Nirvana/throw_exception.h>
#include <corecrt_startup.h>
#include <vcstartup_internal.h>
#include <assert.h>

// TODO: These functions are temporary solution for initialization the MS CRTL.
// They must be reworked for Nirvana CRTL.

namespace Nirvana {

void crt_init ()
{
	// The /GS security cookie must be initialized before any exception handling
	// targeting the current image is registered.  No function using exception
	// handling can be called in the current image until after this call:
	__security_init_cookie ();

	if (!__scrt_initialize_crt (__scrt_module_type::dll))
		throw_UNKNOWN ();

	if (!__scrt_dllmain_before_initialize_c ())
		throw_UNKNOWN ();

//	__scrt_initialize_type_info ();

	__scrt_initialize_default_local_stdio_options ();

	// Initialize C
	if (_initterm_e (__xi_a, __xi_z) != 0)
		throw_UNKNOWN ();

	if (!__scrt_dllmain_after_initialize_c ())
		throw_UNKNOWN ();

	// Initialize C++
	_initterm (__xc_a, __xc_z);

	// If this module has any dynamically initialized __declspec(thread)
	// variables, then we invoke their initialization for the primary thread
	// used to start the process:
	/*
	_tls_callback_type const* const tls_init_callback = __scrt_get_dyn_tls_init_callback ();
	if (*tls_init_callback != nullptr && __scrt_is_nonwritable_in_current_image (tls_init_callback)) {
		(*tls_init_callback)(nullptr, DLL_THREAD_ATTACH, nullptr);
	}
	*/
	assert (!*__scrt_get_dyn_tls_init_callback ());

	// If this module has any thread-local destructors, register the
	// callback function with the Unified CRT to run on exit.
	/*
	_tls_callback_type const* const tls_dtor_callback = __scrt_get_dyn_tls_dtor_callback ();
	if (*tls_dtor_callback != nullptr && __scrt_is_nonwritable_in_current_image (tls_dtor_callback)) {
		_register_thread_local_exe_atexit_callback (*tls_dtor_callback);
	}
	*/
	assert (!*__scrt_get_dyn_tls_dtor_callback ());
}

void crt_term ()
{
	__scrt_dllmain_uninitialize_c ();

	//__scrt_uninitialize_type_info ();

	__scrt_uninitialize_crt (false, false);
}

}
