/// System exception throw helpers.
/// TODO: Add more.
#ifndef NIRVANA_THROW_EXCEPTION_H_
#define NIRVANA_THROW_EXCEPTION_H_

#include "NirvanaBase.h"

namespace Nirvana {

NIRVANA_NORETURN void throw_INV_OBJREF ();
NIRVANA_NORETURN void throw_MARSHAL ();
NIRVANA_NORETURN void throw_BAD_PARAM ();

}

#endif
