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
#include "../../pch/pch.h"
#include "../Include/Nirvana/Formatter.h"
#include "../Include/Nirvana/Debugger.h"

using namespace Nirvana;

extern "C" void Nirvana_debug (const char* msg, const char* file_name, int line_number, int warning)
{
	the_debugger->debug_event (
		warning ? Debugger::DebugEvent::DEBUG_WARNING : Debugger::DebugEvent::DEBUG_ASSERT,
		msg, file_name, line_number);
}

extern "C" void Nirvana_trace (int warning, const char* file_name, int line_number, const char* format, ...)
{
	std::string msg;
	va_list arglist;
	va_start (arglist, format);
	Formatter::append_format_v (msg, format, arglist);
	va_end (arglist);
	Nirvana::the_debugger->debug_event (
		warning ? Debugger::DebugEvent::DEBUG_WARNING : Debugger::DebugEvent::DEBUG_INFO,
		msg, file_name, line_number);
}
