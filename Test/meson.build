lib_test = executable('LibraryTest',
	'TestLibrary.cpp',
	'TestString.cpp',
	'TestVector.cpp',
	include_directories: test_inc_dirs,
	link_with: [nirvana_host, googletest_host, mock],
	cpp_args: '-mlzcnt'
 )
 test ('LibraryTest', lib_test, protocol: 'gtest')