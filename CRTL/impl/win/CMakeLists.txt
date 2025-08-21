set(MS_LIB_DIRS $ENV{LIB})
target_link_directories(crtl PUBLIC ${MS_LIB_DIRS})
target_link_libraries(crtl PUBLIC Kernel32.lib Shell32.lib)

target_sources(crtl PRIVATE
	initterm.cpp
	mainCRTStartup.cpp
	tlsdtor.cpp
	tlsdyn.cpp
	tlssup.cpp
)