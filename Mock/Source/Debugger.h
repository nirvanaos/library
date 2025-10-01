/// \file Mock implementation of the Nirvana::Debugger interface.
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
#ifndef NIRVANA_TEST_DEBUGGER_H_
#define NIRVANA_TEST_DEBUGGER_H_
#pragma once

#include <CORBA/Server.h>
#include <Nirvana/Debugger_s.h>
#include <unordered_map>
#include <atomic>
#include "HostAllocator.h"
#include "StaticallyAllocated.h"
#include <mockhost/mutex.h>

namespace Nirvana {
namespace Test {

using Nirvana::Mock::mutex;
using Nirvana::Mock::lock_guard;

class Debugger :
	public CORBA::servant_traits <Nirvana::Debugger>::ServantStatic <Debugger>
{
public:
	static void initialize ()
	{
		data_.construct ();
	}

	static void terminate ()
	{
		data_.destruct ();
	}

	// Debugger::

	static void debug_event (DebugEvent type, const IDL::String& message, const IDL::String& file_name, int32_t line_number)
	{
		if (!file_name.empty ()) {
			host_print_error (file_name.c_str ());
			host_print_error ("(");
			host_print_error (std::to_string (line_number).c_str ());
			host_print_error ("): ");
		}

		static const char* const ev_prefix [(size_t)DebugEvent::DEBUG_ERROR + 1] = {
			"INFO: ",
			"WARNING: ",
			"Assertion failed: ",
			"ERROR: "
		};
		host_print_error (ev_prefix [(unsigned)type]);
		host_print_error (message.c_str ());
		host_print_error ("\n");
		if (type >= DebugEvent::DEBUG_ASSERT)
			host_debug_break ();
	}

	static Nirvana::RuntimeProxy::_ref_type proxy_get (const void* obj)
	{
		return data_->proxy_get (obj);
	}

	static void proxy_reset (const void* obj)
	{
		data_->proxy_remove (obj);
	}

private:
	class Proxy :
		public CORBA::servant_traits <Nirvana::RuntimeProxy>::Servant <Proxy>,
		public CORBA::Internal::LifeCycleRefCnt <Proxy>
	{
	public:
		Proxy (const void* obj) :
			object_ (obj),
			ref_cnt_ (1)
		{}

		~Proxy ()
		{}

		void _add_ref () noexcept
		{
			++ref_cnt_;
		}

		void _remove_ref () noexcept
		{
			if (!--ref_cnt_)
				delete this;
		}

		const void* object () const
		{
			return object_;
		}

		void remove ()
		{
			assert (object_);
			object_ = nullptr;
		}

	private:
		const void* object_;
		std::atomic <unsigned> ref_cnt_;
	};

	class Data
	{
	public:
		Nirvana::RuntimeProxy::_ref_type proxy_get (const void* obj)
		{
			lock_guard lock (mutex_);
			std::pair <ProxyMap::iterator, bool> ins = proxy_map_.insert (ProxyMap::value_type (obj, nullptr));
			if (ins.second) {
				try {
					ins.first->second = CORBA::make_reference <Proxy> (obj);
				} catch (...) {
					proxy_map_.erase (ins.first);
					throw;
				}
			}
			return ins.first->second->_get_ptr ();
		}

		void proxy_remove (const void* obj)
		{
			lock_guard <mutex> lock (mutex_);
			ProxyMap::iterator f = proxy_map_.find (obj);
			if (f != proxy_map_.end ()) {
				f->second->remove ();
				proxy_map_.erase (f);
			}
		}

	private:
		typedef std::unordered_map <const void*, CORBA::servant_reference <Proxy>,
			std::hash <const void*>, std::equal_to <const void*>,
			HostAllocator <std::pair <const void* const, CORBA::servant_reference <Proxy> > > > ProxyMap;
		ProxyMap proxy_map_;
		mutex mutex_;
	};

	static StaticallyAllocated <Data> data_;
};

}
}

#endif
