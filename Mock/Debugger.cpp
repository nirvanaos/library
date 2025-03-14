/// \file Mock implementation of the RuntimeSupport interface.
/*
* Nirvana mock library.
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

// For mock implementation we use std::unordered_map.
// In the core implementation should be used more efficient implementation like folly / F14.
// std::unordered_map depends on std::vector.
// std::vector can use debug iterators.
// This can cause cyclic dependency.
// So we disable debug iterators for this file.
#define NIRVANA_DEBUG_ITERATORS 0

#include <CORBA/Server.h>
#include <Nirvana/Debugger_s.h>
#include <unordered_map>
#include <mutex>
#include <iostream>
#include <atomic>
#include "debug-trap/debug-trap.h"
#include "export.h"

namespace Nirvana {
namespace Test {

class Debugger :
	public CORBA::servant_traits <Nirvana::Debugger>::ServantStatic <Debugger>
{
public:
	static void debug_event (DebugEvent type, const IDL::String& message, const IDL::String& file_name, int32_t line_number)
	{
		if (!file_name.empty ())
			std::cerr << file_name << '(' << line_number << "): ";

		static const char* const ev_prefix [(size_t)DebugEvent::DEBUG_ERROR + 1] = {
			"INFO: ",
			"WARNING: ",
			"Assertion failed: ",
			"ERROR: "
		};
		std::cerr << ev_prefix [(unsigned)type] << message << std::endl;
		if (type >= DebugEvent::DEBUG_ASSERT) {
			psnip_trap ();
		}
	}

	static Nirvana::RuntimeProxy::_ref_type proxy_get (const void* obj)
	{
		if (Data::constructed ())
			return data_.proxy_get (obj);
		else
			return Nirvana::RuntimeProxy::_nil ();
	}

	static void proxy_reset (const void* obj)
	{
		if (Data::constructed ())
			data_.proxy_remove (obj);
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
		Data ()
		{
			constructed_ = true;
		}

		~Data ()
		{
			constructed_ = false;
		}

		static bool constructed ()
		{
			return constructed_;
		}

		Nirvana::RuntimeProxy::_ref_type proxy_get (const void* obj)
		{
			std::lock_guard <std::mutex> lock (mutex_);
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
			std::lock_guard <std::mutex> lock (mutex_);
			ProxyMap::iterator f = proxy_map_.find (obj);
			if (f != proxy_map_.end ()) {
				f->second->remove ();
				proxy_map_.erase (f);
			}
		}

	private:
		typedef std::unordered_map <const void*, CORBA::servant_reference <Proxy> > ProxyMap;
		ProxyMap proxy_map_;
		std::mutex mutex_;
		static bool constructed_;
	};

	static Data data_;
};

Debugger::Data Debugger::data_;

bool Debugger::Data::constructed_ = false;

NIRVANA_MOCK_EXPORT CORBA::Internal::Interface* mock_debugger = NIRVANA_STATIC_BRIDGE (Nirvana::Debugger, Debugger);

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <Debugger> the_debugger = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, NIRVANA_STATIC_BRIDGE (Debugger, Test::Debugger) };

}
