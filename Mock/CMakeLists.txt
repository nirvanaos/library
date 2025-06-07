add_library(coremock SHARED
	constants.cpp
	HostAPI.cpp
	Mutex.cpp
)

# Add this for constants.cpp
target_include_directories(coremock PRIVATE ${CMAKE_SOURCE_DIR}/nirvana/library/Include/Nirvana)

target_link_libraries(coremock PRIVATE mincore
	$<IF:$<CONFIG:Debug>,ucrtd.lib,ucrt.lib>
)

target_compile_options(coremock PRIVATE "--target=${NIRVANA_TARGET_TRIPLE}")

add_library(mockimport STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
	POSIX.cpp
)

target_link_libraries(mockimport PRIVATE coremock nirvana)
