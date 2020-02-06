#include <Nirvana/throw_exception.h>
#include <CORBA/exceptions.h>

#define DEFINE_FUNCTION(e) NIRVANA_NORETURN void throw_##e (void){throw CORBA::e ();}

namespace Nirvana {

SYSTEM_EXCEPTIONS (DEFINE_FUNCTION)

}
