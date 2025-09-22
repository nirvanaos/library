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

add_custom_command (TARGET test-library POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:test-library> $<TARGET_FILE_DIR:test-library>
  COMMAND_EXPAND_LISTS
)

#include (GoogleTest)
#gtest_discover_tests (test-library WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
