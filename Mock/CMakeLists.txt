add_library(core-mock STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
)
target_link_libraries(core-mock PUBLIC nirvana)
