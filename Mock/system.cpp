#include <CORBA/Server.h>
#include <signal.h>
#include <Nirvana/System_s.h>
#include <Nirvana/ImportInterface.h>
#include <CORBA/LifeCycleRefCnt.h>
#include <chrono>
#include <thread>
#include <signal.h>
#include <Nirvana/File.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

using namespace CORBA;
using namespace CORBA::Internal;

namespace Nirvana {
namespace Test {

class System :
	public servant_traits <Nirvana::System>::ServantStatic <System>
{
	// CORBA Time Service duration type
	using DurationTS = std::chrono::duration <int64_t, std::ratio <10000000LL, 1LL> >;

	static TimeBase::TimeT from_time_point (std::chrono::system_clock::time_point t)
	{
		// Offset from 15 Oct 1582 to 1 Jan 1970
		const auto unix_offset = std::chrono::seconds { 12219292800LL };

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
		return deadline_policy_async_;
	}

	static void deadline_policy_async (const DeadlinePolicy& dp)
	{
		deadline_policy_async_ = dp;
	}

	static const DeadlinePolicy& deadline_policy_oneway ()
	{
		return deadline_policy_oneway_;
	}

	static void deadline_policy_oneway (const DeadlinePolicy& dp)
	{
		deadline_policy_oneway_ = dp;
	}

	static void* error_number ()
	{
		return &errno;
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

	static ContextType context_type ()
	{
		return ContextType::PROCESS;
	}

	static bool yield ()
	{
		return false;
	}

	static void sleep (TimeBase::TimeT period100ns)
	{
#ifdef _WIN32
		Sleep ((DWORD)(period100ns / TimeBase::MILLISECOND));
#else
		struct timespec ts { period100ns / TimeBase::SECOND, period100ns % TimeBase::SECOND * 100 };
		nanosleep (&ts, nullptr);
#endif
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

	static IDL::String to_string (const CosNaming::Name& name)
	{
		throw_NO_IMPLEMENT ();
	}

	static CosNaming::Name to_name (const IDL::String& sn)
	{
		throw_NO_IMPLEMENT ();
	}

	static void append_path (CosNaming::Name& name, const IDL::String& path, bool absolute)
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

	static unsigned fd_add (Access::_ptr_type access)
	{
		throw_NO_IMPLEMENT ();
	}

	static void close (unsigned idx)
	{
		throw_NO_IMPLEMENT ();
	}

	static size_t read (unsigned fd, void* p, size_t size)
	{
		throw_NO_IMPLEMENT ();
	}

	static void write (unsigned fd, const void* p, size_t size)
	{
		throw_NO_IMPLEMENT ();
	}

	static FileSize seek (unsigned fd, const FileOff& offset, uint_fast16_t whence)
	{
		throw_NO_IMPLEMENT ();
	}

	static int_fast16_t fcntl (unsigned fd, int_fast16_t cmd, uint_fast16_t arg)
	{
		throw_NO_IMPLEMENT ();
	}

	static void flush (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static void dup2 (unsigned src, unsigned dst)
	{
		throw_NO_IMPLEMENT ();
	}

	static bool isatty (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static void ungetc (unsigned fd, int c)
	{
		throw_NO_IMPLEMENT ();
	}

	static bool ferror (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static bool feof (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static void clearerr (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static size_t exec_domain_id ()
	{
		return 1;
	}

	static DeadlinePolicy deadline_policy_async_;
	static DeadlinePolicy deadline_policy_oneway_;
};

System::DeadlinePolicy System::deadline_policy_async_ = DEADLINE_POLICY_INHERIT;
System::DeadlinePolicy System::deadline_policy_oneway_ = DEADLINE_POLICY_INFINITE;

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <System> the_system = { OLF_IMPORT_INTERFACE, nullptr, nullptr, NIRVANA_STATIC_BRIDGE (System, Test::System) };

}
