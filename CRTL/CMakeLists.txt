add_library(crtl STATIC
	abort.cpp
	ctype.cpp
	errno.cpp
	exit.cpp
	fcntl.cpp
	gmtime_r.c
	heap.cpp
	iswspace.cpp
	llabs.cpp
	locale.cpp
	math.cpp
	memcmp.cpp
	memcpy.cpp
	mkstemp.cpp
	printf.cpp
	pthread.cpp
	raise.cpp
	rand.cpp
	scanf.cpp
	sched.cpp
	stat.cpp
	stdio.cpp
#	stdlib.cpp
	strcat_s.cpp
	strcmp.cpp
	strcpy.cpp
	strcpy_s.cpp
	strcspn.cpp
	stream.cpp
	strlen.cpp
	strncmp.cpp
	strspn.cpp
	strtof.cpp
	strtol.cpp
	terminate.cpp
	time.cpp
#	uncaught_exceptions.cpp
	unistd.cpp
	impl/file.cpp
	impl/fdio.cpp
	impl/Global.cpp
)

target_link_libraries(crtl PUBLIC nirvana)
