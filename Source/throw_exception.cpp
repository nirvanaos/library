#include <Nirvana/throw_exception.h>
#include <CORBA/exceptions.h>

#define DEF_EXCEPTION(e) NIRVANA_NORETURN void throw_##e (void){throw CORBA::e ();}

namespace Nirvana {

DEF_EXCEPTION (INV_OBJREF)
DEF_EXCEPTION (MARSHAL)
DEF_EXCEPTION (BAD_PARAM)

}
