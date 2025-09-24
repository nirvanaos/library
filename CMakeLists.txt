add_subdirectory (Include/Nirvana)

if (NIRVANA_BUILD)

add_subdirectory (Source)
add_subdirectory (CoreImport)
add_subdirectory (CRTL)

if (BUILD_TESTING)
	add_subdirectory (Mock)
	add_subdirectory (TestC)
	add_subdirectory (TestCXX)
	add_subdirectory (Test)
endif ()

endif ()
