find_package (mockhost CONFIG REQUIRED)

add_library (mockimport STATIC
	Debugger.cpp
	imports.cpp
	Memory.cpp
	Module.cpp
)

target_link_libraries (mockimport PRIVATE mockhost nirvana)
