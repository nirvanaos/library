add_executable(test-library
	TestLibrary.cpp
	TestString.cpp
	TestVector.cpp
)
target_link_libraries(test-library PRIVATE
	googletest-nirvana
	nirvana
	core-mock
)
