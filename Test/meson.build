host_depends = declare_dependency (link_with: host_libs)

foreach platform, arch : host_platforms
	cpp_args = arch ['cpp_args']

	test_name = 'LibraryTest-' + platform

	link_options = [
		'-Lnirvana/host/' + platform,
		'-Lgoogletest/host/' + platform,
		'-lnirvana', '-lgoogletest', '-lmock'
	] + arch ['link_args']

	lib_test = executable (test_name,
		'TestLibrary.cpp',
		'TestString.cpp',
		'TestVector.cpp',
		include_directories: test_inc_dirs,
		link_args: link_options,
		dependencies: host_depends,
		cpp_args: cpp_args
	)

	test (test_name, lib_test, protocol: 'gtest')

endforeach
