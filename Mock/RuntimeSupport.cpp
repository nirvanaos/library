// Mock implementation of the RuntimeSupport interface.
// For mock implementation we use std::unordered_map.
// In the core implementation should be used more efficient implementation like folly / F14.
// std::unordered_map depends on std::vector.
// std::vector can use debug iterators.
// This can cause cyclic dependency.
// So we disable debug iterators for this file.
#define NIRVANA_DEBUG_ITERATORS 0

#include <CORBA/Server.h>
#include <Nirvana/RuntimeSupport_s.h>
#include <unordered_map>
#include <mutex>

namespace Nirvana {
namespace Test {

class RuntimeSupport :
	public CORBA::servant_traits <Nirvana::RuntimeSupport>::ServantStatic <RuntimeSupport>
{
public:
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
		unsigned ref_cnt_;
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
					ins.first->second = new Proxy (obj);
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

	private:
		typedef std::unordered_map <const void*, Proxy*> ProxyMap;
		ProxyMap proxy_map_;
		std::mutex mutex_;
		static bool constructed_;
	};

	static Data data_;
};

RuntimeSupport::Data RuntimeSupport::data_;

bool RuntimeSupport::Data::constructed_ = false;

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <RuntimeSupport> runtime_support = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, NIRVANA_STATIC_BRIDGE (RuntimeSupport, Test::RuntimeSupport) };

}
