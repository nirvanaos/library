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
#include "../pch/pch.h"
#include <Nirvana/Nirvana.h>
#include <Nirvana/POSIX.h>
#include "Global.h"
#include "File.h"

namespace CRTL {

Global NIRVANA_SELECTANY global;

inline Global::Global ()
{
	cs_key_ = Nirvana::the_posix->CS_alloc (rtd_deleter);
}

inline Global::~Global ()
{
	Nirvana::the_posix->CS_free (cs_key_);
}

class Global::RuntimeData
{
public:
	RuntimeData () :
		std_streams_ { 0 }
	{}

	~RuntimeData ()
	{
		for (auto p : std_streams_) {
			delete p;
		}
	}

	FILE* get_std_stream (int fd)
	{
		if (fd < 3) {
			File*& p = std_streams_ [fd];
			if (!p)
				p = new File (fd, true);
			return p;
		}
		return nullptr;
	}

private:
	File* std_streams_ [3];
};

Global::RuntimeData& Global::runtime_data () const
{
	void* p = Nirvana::the_posix->CS_get (cs_key_);
	if (!p) {
		p = new RuntimeData;
		Nirvana::the_posix->CS_set (cs_key_, p);
	}
	return *reinterpret_cast <RuntimeData*> (p);
}

void Global::rtd_deleter (void* p) noexcept
{
	delete reinterpret_cast <RuntimeData*> (p);
}

inline FILE* Global::get_std_stream (int fd)
{
	return runtime_data ().get_std_stream (fd);
}

}

extern "C" FILE* __get_std_stream (int i)
{
	return CRTL::global.get_std_stream (i);
}
