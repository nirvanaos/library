add_library(crtl STATIC
	abort.cpp
	errno.cpp
	exit.cpp
	fnctl.cpp
	heap.cpp
	locale.cpp
	memcpy.cpp
	mkstemp.cpp
	raise.cpp
	rand.cpp
	stat.cpp
#	stdio.cpp
	terminate.cpp
	time.cpp
	unistd.cpp
)
target_link_libraries(crtl PUBLIC nirvana)
target_include_directories(crtl PUBLIC ../include/CRTL ../include/CRT)
