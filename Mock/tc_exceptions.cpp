#include <CORBA/CORBA.h>
#include <Nirvana/OLF.h>

namespace CORBA {

#define IMPORT_TC(E) extern const ::Nirvana::ImportInterfaceT <TypeCode> _tc_##E = { ::Nirvana::OLF_IMPORT_INTERFACE };

SYSTEM_EXCEPTIONS (IMPORT_TC)

IMPORT_TC (UnknownUserException)

}
