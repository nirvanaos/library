/*
* Nirvana mock library.
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

#include <Nirvana/crt_startup.h>
#include <Nirvana/Windows/CmdLineParser.h>

extern int main (int argc, char* argv [], char** envp);

extern "C" unsigned long mainCRTStartup (void)
{
	if (!Nirvana::crt_init ())
		return -1;

	Nirvana::Windows::CmdLineParser <> cmd_line;
	int ret = main (cmd_line.argc (), cmd_line.argv (), cmd_line.envp ());

	Nirvana::crt_term ();
  return 0;
}
