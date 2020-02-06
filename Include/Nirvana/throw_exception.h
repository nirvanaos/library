/// System exception throw helpers.
/// TODO: Add more.
#ifndef NIRVANA_THROW_EXCEPTION_H_
#define NIRVANA_THROW_EXCEPTION_H_

#include "NirvanaBase.h"
#include <CORBA/system_exceptions.h>

#define DECLARE_FUNCTION(e) void throw_##e (void);

namespace Nirvana {

SYSTEM_EXCEPTIONS (DECLARE_FUNCTION)

}

#undef DECLARE_FUNCTION

#endif
