add_library(core-import STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
)
target_link_libraries(core-import PUBLIC nirvana)
