add_library(coremock SHARED
	constants.cpp
	Debugger.cpp
	Memory.cpp
	Module.cpp
	POSIX.cpp
)

set (HOST_INCLUDES
	"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um"
	"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared"
)
		
target_link_libraries(coremock PRIVATE nirvana)
target_include_directories(coremock PRIVATE ${NIRVANA_INCLUDES} ${HOST_INCLUDES})
target_compile_options(coremock PRIVATE "-nostdinc")

if (WIN32)
target_include_directories(coremock PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/msvc)
endif ()

add_library(mockimport STATIC imports.cpp)
target_link_libraries(mockimport PRIVATE coremock)
