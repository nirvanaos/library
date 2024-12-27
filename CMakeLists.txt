add_subdirectory(Include/Nirvana)
add_subdirectory(Source)
add_subdirectory(CoreImport)
add_subdirectory(CRTL)

if(BUILD_TESTING)
	add_subdirectory(Mock)
	add_subdirectory(Test)
endif()

