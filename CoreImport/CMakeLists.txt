add_library(CoreImport STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
  POSIX.cpp
)
target_link_libraries(CoreImport PUBLIC Nirvana)
