// Mock implementation of the RuntimeSupport interface.
// For mock implementation we use std::unordered_map.
// In the core implementation should be used more efficient implementation like folly / F14.
// std::unordered_map depends on std::vector. 
// std::vector can use debug iterators.
// This can cause cyclic dependency.
// So we disable debug iterators for this file.
#define NIRVANA_DEBUG_ITERATORS 0

#include <CORBA/Server.h>
#include <RuntimeSupport_s.h>
#include <Nirvana/ImportInterface.h>
#include <CORBA/LifeCycleRefCnt.h>
#include <unordered_map>
#include <mutex>

using namespace std;
using namespace CORBA;
using namespace CORBA::Nirvana;

namespace Nirvana {
namespace Test {

class RuntimeProxy :
	public ImplementationPseudo <RuntimeProxy, ::Nirvana::RuntimeProxy>,
	public LifeCycleRefCnt <RuntimeProxy>
{
public:
	RuntimeProxy (const void* obj) :
		object_ (obj),
		ref_cnt_ (1)
	{}

	~RuntimeProxy ()
	{}

	void _add_ref ()
	{
		++ref_cnt_;
	}

	void _remove_ref ()
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

class RuntimeSupport :
	public ::CORBA::Nirvana::ServantStatic <RuntimeSupport, ::Nirvana::RuntimeSupport>
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

	static RuntimeData& data ()
	{
		static RuntimeData singleton;
		return singleton;
	}

public:
	static ::Nirvana::RuntimeProxy::_ref_type runtime_proxy_get (const void* obj)
	{
		return data ().proxy_get (obj);
	}

	static void runtime_proxy_remove (const void* obj)
	{
		data ().proxy_remove (obj);
	}
};

}

extern const ImportInterfaceT <RuntimeSupport> g_runtime_support = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (RuntimeSupport, Test::RuntimeSupport) };

}
