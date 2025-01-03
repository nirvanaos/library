add_library(coreimport STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
)
target_link_libraries(coreimport PUBLIC nirvana)
