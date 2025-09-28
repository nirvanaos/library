target_sources (crtl PRIVATE
	abort.cpp
	abs.cpp
	atexit.cpp
	bsearch.cpp
	ctype.cpp
	collate.cpp
	errno.cpp
	exit.cpp
	fcntl.cpp
	fflush.cpp
	fopen.cpp
  getenv.cpp
	gmtime_r.cpp
	heap.cpp
	locale.cpp
	math.cpp
	mbcs.cpp
	memchr.cpp
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
	strcat.cpp
	strchr.cpp
	strcmp.cpp
	strcpy.cpp
	strcspn.cpp
	strdup.cpp
	stream.cpp
	strerror.cpp
	strftime.cpp
	strlen.cpp
	strrchr.cpp
	strspn.cpp
	strstr.cpp
	strtof.cpp
	strtol.cpp
	time.cpp
	unistd.cpp
)

add_subdirectory (impl)
add_subdirectory (wctype)
add_subdirectory (machine)
