add_library(crtl STATIC
	abort.cpp
	ctype.cpp
	collate.cpp
	errno.cpp
	exit.cpp
	fcntl.cpp
	fflush.cpp
	fopen.cpp
	gmtime_r.cpp
	heap.cpp
#	iswspace.cpp
	llabs.cpp
	locale.cpp
	math.cpp
	mbcs.cpp
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
	strcasecmp.cpp
	strcat_s.cpp
	strcmp.cpp
	strcpy.cpp
	strcpy_s.cpp
	strcspn.cpp
	strdup.cpp
	stream.cpp
	strftime.cpp
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
	impl/RandomGen.cpp
	impl/UTF8In.cpp
)

add_subdirectory (wctype)
add_subdirectory (machine)

target_link_libraries(crtl PUBLIC nirvana)
