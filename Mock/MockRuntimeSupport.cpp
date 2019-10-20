#include <Nirvana/RuntimeSupport_s.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using namespace CORBA;
using namespace CORBA::Nirvana;

namespace Nirvana {
namespace Test {

class MockRuntimeSupport :
	public ::CORBA::Nirvana::ServantStatic <MockRuntimeSupport, RuntimeSupport>
{
	typedef unordered_set <const void*> ObjectSet;
	typedef unordered_map <const void*, Interface_var> SharedObjects;

	struct RuntimeData
	{
		ObjectSet object_set;
		SharedObjects shared_objects;
	};

	static RuntimeData& data ()
	{
		static RuntimeData singleton;
		return singleton;
	}

public:
	static Boolean object_set_add (const void* key)
	{
		return data ().object_set.insert (key).second;
	}

	static Boolean object_set_remove (const void* key)
	{
		return data ().object_set.erase (key) != 0;
	}

	static Boolean object_set_check (const void* key)
	{
		const ObjectSet& os = data ().object_set;
		return os.find (key) != os.end ();
	}

	static void shared_object_set (const void* key, Interface_ptr obj)
	{
		Interface_var v = Interface::_duplicate (obj);
		data ().shared_objects.emplace (key, v);
	}

	static Boolean shared_object_remove (const void* key)
	{
		return data ().shared_objects.erase (key) != 0;
	}

	Interface_ptr shared_object_get (const void* key)
	{
		const SharedObjects& so = data ().shared_objects;
		SharedObjects::const_iterator f = so.find (key);
		if (f != so.end ())
			return Interface::_duplicate (f->second);
		else
			return Interface::_nil ();
	}
};

}

::CORBA::Nirvana::Bridge <RuntimeSupport>* const g_runtime_support = STATIC_BRIDGE (Test::MockRuntimeSupport, RuntimeSupport);

}
