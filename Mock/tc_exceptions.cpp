#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>

namespace CORBA {

#define IMPORT_TC(E) extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##E = { ::Nirvana::OLF_IMPORT_INTERFACE };

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

const ::Nirvana::ImportInterfaceT <TypeCode> CORBA::Nirvana::Definitions <TypeCode>::_tc_BadKind = { ::Nirvana::OLF_IMPORT_INTERFACE };

const ::Nirvana::ImportInterfaceT <TypeCode> CORBA::Nirvana::Definitions <TypeCode>::_tc_Bounds = { ::Nirvana::OLF_IMPORT_INTERFACE };

}
