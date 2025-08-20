find_package (mockhost CONFIG REQUIRED)

add_library (mockimport STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
	POSIX.cpp
)

target_link_libraries (mockimport PRIVATE mockhost nirvana)
