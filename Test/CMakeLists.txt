add_executable (test-library
	TestLibrary.cpp
	TestString.cpp
	TestVector.cpp
)

target_link_libraries (test-library PRIVATE
	googletest-nirvana
	crtl
	mockimport
	crtl-main
)

include (GoogleTest)
gtest_discover_tests (test-library WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
