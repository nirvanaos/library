target_include_directories(crtl PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include/um
	${CMAKE_CURRENT_SOURCE_DIR}/include/shared
	${CMAKE_CURRENT_SOURCE_DIR}/include/msvc
)

target_sources(crtl PRIVATE
	initterm.cpp
	mainCRTStartup.cpp
	tlsdtor.cpp
	tlsdyn.cpp
	tlssup.cpp
)