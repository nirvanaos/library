#include <stdlib.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/Module.h>

extern "C" int atexit (void (*function)(void))
{
  try {
    Nirvana::g_module->atexit (function);
  } catch (...) {
    return ENOMEM;
  }
  return 0;
}

extern "C" int at_quick_exit (void (*function)(void))
{
  return ENOSYS; // Currently not implemented
}

