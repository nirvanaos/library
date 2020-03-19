#include <CORBA/tc_constants.h>
#include <Nirvana/OLF.h>

namespace CORBA {

#define IMPORT_TC(tc) extern const ::Nirvana::ImportInterfaceT <TypeCode> tc = { ::Nirvana::OLF_IMPORT_INTERFACE, "CORBA/" #tc, TypeCode::repository_id_ }

IMPORT_TC (_tc_void);
IMPORT_TC (_tc_short);
IMPORT_TC (_tc_long);
IMPORT_TC (_tc_longlong);
IMPORT_TC (_tc_ushort);
IMPORT_TC (_tc_ulong);
IMPORT_TC (_tc_ulonglong);
IMPORT_TC (_tc_float);
IMPORT_TC (_tc_double);
IMPORT_TC (_tc_longdouble);
IMPORT_TC (_tc_boolean);
IMPORT_TC (_tc_char);
IMPORT_TC (_tc_wchar);
IMPORT_TC (_tc_octet);
IMPORT_TC (_tc_any);
IMPORT_TC (_tc_TypeCode);
IMPORT_TC (_tc_string);
IMPORT_TC (_tc_wstring);

extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_Object = { ::Nirvana::OLF_IMPORT_INTERFACE, Object::repository_id_, TypeCode::repository_id_ };
extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_ValueBase = { ::Nirvana::OLF_IMPORT_INTERFACE, CORBA_REPOSITORY_ID ("ValueBase"), TypeCode::repository_id_ };;

}
