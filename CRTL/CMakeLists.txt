add_library(crtl STATIC
	abort.cpp
	errno.cpp
	exit.cpp
	fnctl.cpp
	gmtime_r.c
	heap.cpp
	iswspace.cpp
	llabs.cpp
	locale.cpp
	math.cpp
	memcmp.cpp
	memcpy.cpp
	mkstemp.cpp
	raise.cpp
	rand.cpp
	sched.cpp
	stat.cpp
#	stdio.cpp
	stdlib.cpp
	strcat_s.cpp
	strcmp.cpp
	strcpy.cpp
#	strcpy_s.cpp
	strcspn.cpp
#	strlen.cpp
	strncmp.cpp
	strspn.cpp
	strtof.cpp
	strtol.cpp
	terminate.cpp
	time.cpp
	uncaught_exceptions.cpp
	unistd.cpp
)
target_link_libraries(crtl PUBLIC nirvana)
target_include_directories(crtl PUBLIC ../include/CRTL ../include/CRT)
