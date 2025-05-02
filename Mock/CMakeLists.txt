add_library(coremock SHARED
	constants.cpp
	HostAPI.cpp
	Mutex.cpp
)

target_include_directories(coremock PRIVATE ${CMAKE_SOURCE_DIR}/nirvana/library/Include)
target_link_libraries(coremock PRIVATE "mincore")

add_library(mockimport STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
	POSIX.cpp
)

target_link_libraries(mockimport PRIVATE coremock nirvana)
