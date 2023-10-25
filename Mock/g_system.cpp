// Mock implementation of the RuntimeSupport interface.
// For mock implementation we use std::unordered_map.
// In the core implementation should be used more efficient implementation like folly / F14.
// std::unordered_map depends on std::vector. 
// std::vector can use debug iterators.
// This can cause cyclic dependency.
// So we disable debug iterators for this file.
#define NIRVANA_DEBUG_ITERATORS 0

#include <CORBA/Server.h>
#include <signal.h>
#include <Nirvana/System_s.h>
#include <Nirvana/ImportInterface.h>
#include <CORBA/LifeCycleRefCnt.h>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <iostream>
#include "debug-trap/debug-trap.h"
#include <signal.h>
#include <Nirvana/File.h>

using namespace CORBA;
using namespace CORBA::Internal;

namespace Nirvana {
namespace Test {

class RuntimeProxy :
	public servant_traits <Nirvana::RuntimeProxy>::Servant <RuntimeProxy>,
	public LifeCycleRefCnt <RuntimeProxy>
{
public:
	RuntimeProxy (const void* obj) :
		object_ (obj),
		ref_cnt_ (1)
	{}

	~RuntimeProxy ()
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
	unsigned ref_cnt_;
};

class System :
	public servant_traits <Nirvana::System>::ServantStatic <System>
{
	class RuntimeData
	{
	public:
		RuntimeData () :
			deadline_policy_oneway_ (DEADLINE_POLICY_INFINITE)
		{
			constructed_ = true;
		}

		~RuntimeData ()
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
					ins.first->second = new RuntimeProxy (obj);
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
				f->second->_remove_ref ();
				proxy_map_.erase (f);
			}
		}

		const DeadlinePolicy& deadline_policy_async () const
		{
			return deadline_policy_async_;
		}

		const DeadlinePolicy& deadline_policy_oneway () const
		{
			return deadline_policy_oneway_;
		}

	private:
		typedef std::unordered_map <const void*, RuntimeProxy*> ProxyMap;
		ProxyMap proxy_map_;
		std::mutex mutex_;
		DeadlinePolicy deadline_policy_async_;
		DeadlinePolicy deadline_policy_oneway_;
		static bool constructed_;
	};

	// CORBA Time Service duration type
	using DurationTS = std::chrono::duration <int64_t, std::ratio <10000000i64, 1i64> >;

	static TimeBase::TimeT from_time_point (std::chrono::system_clock::time_point t)
	{
		// Offset from 15 Oct 1582 to 1 Jan 1970
		const auto unix_offset = std::chrono::seconds { 12219292800I64 };

		// Offset for current system to Time Service time
		const auto offset = unix_offset - std::chrono::system_clock::from_time_t (0).time_since_epoch ();

		return std::chrono::duration_cast <DurationTS> (t.time_since_epoch () + offset).count ();
	}

public:
	static Object::_ref_type bind (const IDL::String&)
	{
		throw_NO_IMPLEMENT ();
	}

	static Interface::_ref_type bind_interface (const IDL::String&, const IDL::String&)
	{
		throw_NO_IMPLEMENT ();
	}

	static Nirvana::RuntimeProxy::_ref_type runtime_proxy_get (const void* obj)
	{
		if (RuntimeData::constructed ())
			return runtime_data_.proxy_get (obj);
		else
			return Nirvana::RuntimeProxy::_nil ();
	}

	static void runtime_proxy_remove (const void* obj)
	{
		if (RuntimeData::constructed ())
			runtime_data_.proxy_remove (obj);
	}

	static TimeBase::UtcT UTC ()
	{
		return TimeBase::UtcT (from_time_point (std::chrono::system_clock::now ()),
			10000000 / std::chrono::system_clock::period::den, 0, 0);
	}

	static TimeBase::UtcT system_clock ()
	{
		// Do not use time zone for C++ 11 compatibility.
		return UTC ();
	}

	static SteadyTime steady_clock ()
	{
		return std::chrono::steady_clock::now ().time_since_epoch ().count ();
	}

	static DeadlineTime deadline_clock ()
	{
		return std::chrono::steady_clock::now ().time_since_epoch ().count ();
	}

	static DeadlineTime deadline_clock_frequency ()
	{
		return std::chrono::steady_clock::period::den;
	}

	static DeadlineTime deadline_from_UTC (const TimeBase::UtcT& utc)
	{
		return (
			std::chrono::steady_clock::now ().time_since_epoch () +
			std::chrono::duration_cast <std::chrono::steady_clock::duration> (DurationTS (
			utc.time () - from_time_point (std::chrono::system_clock::now ())))
		).count ();
	}

	static TimeBase::UtcT deadline_to_UTC (const DeadlineTime& deadline)
	{
		TimeBase::UtcT utc = UTC ();
		utc.time (utc.time () + std::chrono::duration_cast <DurationTS> (
			std::chrono::steady_clock::duration (deadline) - std::chrono::steady_clock::now ().time_since_epoch ()
			).count ());

		return utc;
	}

	static DeadlineTime make_deadline (TimeBase::TimeT timeout)
	{
		return (
			std::chrono::steady_clock::now ().time_since_epoch ()
			+ std::chrono::duration_cast <std::chrono::steady_clock::duration> (DurationTS (timeout))
			).count ();
	}

	static const DeadlineTime& deadline ()
	{
		static const DeadlineTime dt = INFINITE_DEADLINE;
		return dt;
	}

	static const DeadlinePolicy& deadline_policy_async ()
	{
		return runtime_data_.deadline_policy_async ();
	}

	static void deadline_policy_async (const DeadlinePolicy&)
	{}

	static const DeadlinePolicy& deadline_policy_oneway ()
	{
		return runtime_data_.deadline_policy_oneway ();
	}

	static void deadline_policy_oneway (const DeadlinePolicy&)
	{}

	static void* error_number ()
	{
		return &errno;
	}

	static Memory::_ref_type create_heap (uint16_t granularity)
	{
		throw_NO_IMPLEMENT ();
	}

	static void raise (int signal)
	{
		if (SIGABRT == signal)
			::abort ();
	}

	static void sigaction (int signal, const struct sigaction* act, struct sigaction* oldact)
	{
		throw_NO_IMPLEMENT ();
	}

	static void srand (unsigned seed)
	{
		::srand (seed);
	}

	static int rand ()
	{
		return ::rand ();
	}

	static uint32_t hardware_concurrency ()
	{
		return std::thread::hardware_concurrency ();
	}

	static bool is_legacy_mode ()
	{
		return true;
	}

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

	static bool yield ()
	{
		return false;
	}

	static uint16_t TLS_alloc (Deleter deleter)
	{
		throw_NO_IMPLEMENT ();
	}

	static void TLS_free (uint16_t idx)
	{
		throw_NO_IMPLEMENT ();
	}

	static void TLS_set (uint16_t idx, void* ptr)
	{
		throw_NO_IMPLEMENT ();
	}

	static void* TLS_get (uint16_t idx)
	{
		throw_NO_IMPLEMENT ();
	}

	static void get_name_from_path (CosNaming::Name& name, const IDL::String& path)
	{
		throw_NO_IMPLEMENT ();
	}

	static CosNaming::Name get_current_dir_name ()
	{
		throw_NO_IMPLEMENT ();
	}

	static void chdir (const IDL::String& path)
	{
		throw_NO_IMPLEMENT ();
	}

	static unsigned fd_open (const IDL::String& path, uint_fast16_t flags, uint_fast16_t mode)
	{
		throw_NO_IMPLEMENT ();
	}

	static void fd_close (unsigned idx)
	{
		throw_NO_IMPLEMENT ();
	}

	static size_t fd_read (unsigned fd, void* p, size_t size)
	{
		throw_NO_IMPLEMENT ();
	}

	static void fd_write (unsigned fd, const void* p, size_t size)
	{
		throw_NO_IMPLEMENT ();
	}

	static FileSize fd_seek (unsigned fd, const FileOff& offset, uint_fast16_t whence)
	{
		throw_NO_IMPLEMENT ();
	}

	static int_fast16_t fcntl (unsigned fd, int_fast16_t cmd, uint_fast16_t arg)
	{
		throw_NO_IMPLEMENT ();
	}

	static RuntimeData runtime_data_;
};

System::RuntimeData System::runtime_data_;

bool System::RuntimeData::constructed_ = false;

}

NIRVANA_SELECTANY extern
const ImportInterfaceT <System> g_system = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (System, Test::System) };

}
