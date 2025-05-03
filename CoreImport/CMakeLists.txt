add_library(coreimport STATIC
	atexit.cpp
	Debugger.cpp
	Memory.cpp
	Module.cpp
)
target_link_libraries(coreimport PUBLIC nirvana)
