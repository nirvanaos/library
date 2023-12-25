foreach platform, arch : host_platforms [target_machine.system ()]
	test_name = 'LibraryTest-' + platform

	lib_test = executable (test_name,
		'TestLibrary.cpp',
		'TestString.cpp',
		'TestVector.cpp',
		include_directories: test_inc_dirs,
		link_args: arch ['link_args'],
		link_with: host_libs [platform],
		cpp_args: arch ['cpp_args']
	)

	test (test_name, lib_test, protocol: 'gtest')

endforeach
