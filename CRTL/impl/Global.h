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

#ifndef CRTL_IMPL_GLOBAL_H_
#define CRTL_IMPL_GLOBAL_H_
#pragma once

#include <Nirvana/Nirvana.h>
#include <Nirvana/Module.h>
#include "File.h"
#include "RandomGen.h"

namespace CRTL {

class Global
{
public:
	Global () :
		cs_key_ (Nirvana::the_module->CS_alloc (deleter))
	{}

	~Global ()
	{
		Nirvana::the_module->CS_free (cs_key_);
	}

	File* get_std_stream (int fd) const noexcept
	{
		try {
			return runtime_data ().get_std_stream (fd);
		} catch (...) {
			errno = ENOMEM;
			return nullptr;
		}
	}

	FILE* fopen (const char* file, const char* mode) const noexcept
	{
		try {
			return runtime_data ().fopen (file, mode);
		} catch (...) {
			errno = ENOMEM;
			return nullptr;
		}
	}

	int flush_all () const noexcept
	{
		try {
			return runtime_data ().flush_all ();
		} catch (...) {
			return 0;
		}
	}

	int rand () const noexcept
	{
		try {
			return runtime_data ().rand ();
		} catch (...) {
			return 0;
		}
	}

	void srand (unsigned seed) noexcept
	{
		try {
			return runtime_data ().srand (seed);
		} catch (...) {
		}
	}

private:
	class RuntimeData : public Nirvana::ObjectMemory,
		public RandomGen
	{
	public:
		RuntimeData () noexcept :
			std_streams_ { {0, true}, {1, true}, {2, true} }
		{}

		~RuntimeData ()
		{
			while (!streams_.empty ())
				delete &streams_.front ();
		}

		File* get_std_stream (int fd) noexcept
		{
			assert (1 <= fd && fd <= 3);
			return std_streams_ + (fd - 1);
		}

		FILE* fopen (const char* file, const char* mode)
		{
			int fd;
			int e = File::open (file, File::parse_modestring (mode), fd);
			if (e)
				errno = e;
			else {
				try {
					return File::cast (new FileDyn (fd, streams_));
				} catch (...) {
					CRTL::close (fd);
					throw;
				}
			}
			return nullptr;
		}

		int flush_all ()
		{
			int e = 0;
			for (auto& f : streams_) {
				int e1 = f.flush ();
				if (!e)
					e = e1;
			}
			for (int i = 1; i < 3; ++i) {
				int e1 = std_streams_ [i].flush ();
				if (!e)
					e = e1;
			}
			return e;
		}

	private:
		File std_streams_ [3];
		Nirvana::SimpleList <FileDyn> streams_;
	};

	RuntimeData& runtime_data () const;

	static void deleter (void* p) noexcept;

private:
	Nirvana::Module::CS_Key cs_key_;
};

extern Global global;

}

#endif
