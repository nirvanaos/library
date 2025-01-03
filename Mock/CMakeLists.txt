add_library(coremock SHARED
	constants.cpp
	Debugger.cpp
	Memory.cpp
	Module.cpp
	POSIX.cpp
)
target_link_libraries(coremock PUBLIC nirvana)

add_library(mockimport STATIC imports.cpp)
target_link_libraries(mockimport PUBLIC nirvana coremock)
