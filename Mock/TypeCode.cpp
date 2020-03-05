#include <CORBA/TypeCodeBasic.h>
#include <CORBA/TypeCodeString.h>
#include <CORBA/TypeCodeObject.h>
//#include <CORBA/Any.h>
#include <CORBA/MarshalTraits.h>
#include <Nirvana/ImportInterface.h>

namespace CORBA {
namespace Nirvana {
const ::Nirvana::ImportInterfaceT <LocalMarshal> g_local_marshal{ 0, nullptr, nullptr, nullptr };
}
}

#define TC_IMPL(tc) extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##tc { 0, nullptr, nullptr, STATIC_BRIDGE (Nirvana::TC_##tc, TypeCode)};

namespace CORBA {

TC_IMPL (null)
TC_IMPL (void)
TC_IMPL (short)
TC_IMPL (long)
TC_IMPL (longlong)
TC_IMPL (ushort)
TC_IMPL (ulong)
TC_IMPL (ulonglong)
TC_IMPL (float)
TC_IMPL (double)
TC_IMPL (longdouble)
TC_IMPL (boolean)
TC_IMPL (char)
TC_IMPL (wchar)
TC_IMPL (octet)
//TC_IMPL (any)
TC_IMPL (TypeCode)
TC_IMPL (Object)
TC_IMPL (string)
TC_IMPL (wstring)
//TC_IMPL (ValueBase)

}
