// Mock implementation of the RuntimeSupport interface.
#include <Nirvana/RuntimeSupport_s.h>
#include <map>

/* NOTE: We can't use std::unordered_map here because it depends on std::vector and this causes 
cyclic dependency in the debug version. For mock implementation we use simple std::map.
In the core implementation should be used more efficient implementation like folly/F14.
*/

using namespace std;
using namespace CORBA;
using namespace CORBA::Nirvana;

namespace Nirvana {
namespace Test {

class MockRuntimeProxy :
	public ImplementationPseudo <MockRuntimeProxy, RuntimeProxy>,
	public LifeCycleRefCnt <MockRuntimeProxy>
{
public:
	MockRuntimeProxy (const void* obj) :
		object_ (obj),
		ref_cnt_ (1)
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

class MockRuntimeSupport :
	public ::CORBA::Nirvana::ServantStatic <MockRuntimeSupport, RuntimeSupport>
{
	typedef map <const void*, MockRuntimeProxy*> ProxyMap;

	struct RuntimeData
	{
		~RuntimeData ()
		{
			assert (proxy_map.empty ());
		}

		ProxyMap proxy_map;
	};

	static RuntimeData& data ()
	{
		static RuntimeData singleton;
		return singleton;
	}

public:
	static RuntimeProxy_ptr runtime_proxy_get (const void* obj)
	{
		ProxyMap& proxy_map = data ().proxy_map;
		pair <ProxyMap::iterator, bool> ins = proxy_map.emplace (obj, nullptr);
		if (ins.second) {
			try {
				ins.first->second = new MockRuntimeProxy (obj);
			} catch (...) {
				proxy_map.erase (ins.first);
				throw;
			}
		}
		return RuntimeProxy::_duplicate (ins.first->second->_get_ptr ());
	}

	static void runtime_proxy_remove (const void* obj)
	{
		ProxyMap& proxy_map = data ().proxy_map;
		ProxyMap::iterator f = proxy_map.find (obj);
		if (f != proxy_map.end ()) {
			f->second->remove ();
			f->second->_remove_ref ();
			proxy_map.erase (f);
		}
	}
};

}

::CORBA::Nirvana::Bridge <RuntimeSupport>* const g_runtime_support = STATIC_BRIDGE (Test::MockRuntimeSupport, RuntimeSupport);

}
