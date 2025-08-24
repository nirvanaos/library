target_sources(crtl PRIVATE 
	ByteInFile.cpp
	ByteOutFile.cpp
	external_threading.cpp
	fdio.cpp
	File.cpp
	Global.cpp
	locale.cpp
	mbcs.cpp
	UTF8In.cpp
)

if(WIN32)
add_subdirectory (win)
endif()
