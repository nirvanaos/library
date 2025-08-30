add_library(coreimport STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
  POSIX.cpp
)
target_link_libraries(coreimport PUBLIC nirvana)
