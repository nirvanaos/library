add_library(crtl STATIC
	abort.cpp
	atexit.cpp
	bsearch.cpp
	ctype.cpp
	collate.cpp
	errno.cpp
	exit.cpp
	fcntl.cpp
	fflush.cpp
	fopen.cpp
	gmtime_r.cpp
	heap.cpp
	llabs.cpp
	locale.cpp
	math.cpp
	mbcs.cpp
	memcmp.cpp
	memcpy.cpp
	memset.cpp
	mkstemp.cpp
	printf.cpp
	pthread.cpp
	qsort.cpp
	raise.cpp
	rand.cpp
	scanf.cpp
	sched.cpp
	stat.cpp
	stdio.cpp
	stdlib.cpp
	strcasecmp.cpp
	strcat_s.cpp
	strchr.cpp
	strcmp.cpp
	strcpy.cpp
	strcpy_s.cpp
	strcspn.cpp
	strdup.cpp
	stream.cpp
	strerror.cpp
	strftime.cpp
	strlen.cpp
	strncmp.cpp
	strrchr.cpp
	strspn.cpp
	strstr.cpp
	strtof.cpp
	strtol.cpp
	terminate.cpp
	time.cpp
	unistd.cpp
)

add_subdirectory (impl)
add_subdirectory (wctype)
add_subdirectory (machine)

target_link_libraries(crtl PUBLIC nirvana)
