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
#include <Nirvana/Nirvana.h>
#include <Nirvana/Formatter.h>
#include <assert.h>

using namespace std;

namespace Nirvana {

void assertion_failed (const char* msg, const char* file_name, int line_number)
{
	string s;
	if (file_name) {
		s = file_name;
		s += '(';
		append_format (s, "%i", line_number);
		s += "): ";
	}
	s += "Assertion failed: ";
	s += msg;
	s += '\n';
	g_system->debug_event (System::DebugEvent::DEBUG_ERROR, s);
}

}
