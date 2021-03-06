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
#include <IDL/System_s.h>
#include <Nirvana/ImportInterface.h>
#include <Nirvana/Legacy.h>
#include <CORBA/LifeCycleRefCnt.h>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <iostream>
#include "debug-trap/debug-trap.h"
#include <signal.h>

using namespace std;
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

	void _add_ref () NIRVANA_NOEXCEPT
	{
		++ref_cnt_;
	}

	void _remove_ref () NIRVANA_NOEXCEPT
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
		~RuntimeData ()
		{
			assert (proxy_map_.empty ());
		}

		::Nirvana::RuntimeProxy::_ptr_type proxy_get (const void* obj)
		{
			lock_guard <mutex> lock (mutex_);
			pair <ProxyMap::iterator, bool> ins = proxy_map_.insert (ProxyMap::value_type (obj, nullptr));
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
			lock_guard <mutex> lock (mutex_);
			ProxyMap::iterator f = proxy_map_.find (obj);
			if (f != proxy_map_.end ()) {
				f->second->remove ();
				f->second->_remove_ref ();
				proxy_map_.erase (f);
			}
		}

	private:
		typedef unordered_map <const void*, RuntimeProxy*> ProxyMap;
		ProxyMap proxy_map_;
		mutex mutex_;
	};

	static RuntimeData& runtime_data ()
	{
		static RuntimeData singleton;
		return singleton;
	}

public:
	static Object::_ref_type bind (const string&)
	{
		throw_NO_IMPLEMENT ();
	}

	static Interface::_ref_type bind_interface (const string&, const string&)
	{
		throw_NO_IMPLEMENT ();
	}

	static ::Nirvana::RuntimeProxy::_ref_type runtime_proxy_get (const void* obj)
	{
		return runtime_data ().proxy_get (obj);
	}

	static void runtime_proxy_remove (const void* obj)
	{
		runtime_data ().proxy_remove (obj);
	}

	static uint16_t epoch ()
	{
		return 1970;
	}

	static Duration system_clock ()
	{
		using r = ratio_multiply <chrono::system_clock::duration::period, ratio <1000000000, 1> >;
		Duration dur = chrono::system_clock::now ().time_since_epoch ().count ();
		return dur * r::num / r::den;
	}

	static Duration steady_clock ()
	{
		using r = ratio_multiply <chrono::steady_clock::duration::period, ratio <1000000000, 1> >;
		Duration dur = chrono::steady_clock::now ().time_since_epoch ().count ();
		return dur * r::num / r::den;
	}

	static Duration system_to_steady (uint16_t _epoch, Duration system)
	{
		assert (_epoch == epoch ());
		return system - (system_clock () - steady_clock ());
	}

	static Duration steady_to_system (Duration steady)
	{
		return steady + (system_clock () - steady_clock ());
	}

	static Duration deadline ()
	{
		return INFINITE_DEADLINE;
	}

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

	static void srand (uint32_t seed)
	{
		::srand (seed);
	}

	static short rand ()
	{
		return (short)::rand ();
	}

	static uint32_t hardware_concurrency ()
	{
		return std::thread::hardware_concurrency ();
	}

	static bool is_legacy_mode ()
	{
		return true;
	}

	static void debug_event (DebugEvent type, const std::string& message)
	{
		cerr << message;
		if (DebugEvent::DEBUG_ERROR == type) {
			psnip_trap ();
		}
	}

	static bool yield ()
	{
		return false;
	}

	static uint16_t TLS_alloc ()
	{
		throw_NO_IMPLEMENT ();
	}

	static void TLS_free (uint16_t idx)
	{
		throw_NO_IMPLEMENT ();
	}

	static void TLS_set (uint16_t idx, void* ptr, Deleter del)
	{
		throw_NO_IMPLEMENT ();
	}

	static void* TLS_get (uint16_t idx)
	{
		throw_NO_IMPLEMENT ();
	}

};

}

extern const ImportInterfaceT <System> g_system = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (System, Test::System) };

}
