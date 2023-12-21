foreach platform, arch : host_platforms
	foreach config : configurations
		options = ['buildtype=' + config]
		cpp_args = arch ['cpp_args']

		test_name = 'LibraryTest-' + platform + '-' + config

		link_options = [
			'-Lnirvana/host/' + platform + '/' + config,
			'-Lgoogletest/host/' + platform + '/' + config,
			'-lnirvana', '-lgoogletest', '-lmock'
		] + arch ['link_args']

		lib_test = executable (test_name,
			'TestLibrary.cpp',
			'TestString.cpp',
			'TestVector.cpp',
			include_directories: test_inc_dirs,
			link_args: link_options,
			override_options: options,
			cpp_args: cpp_args
 		)

 		test (test_name, lib_test, protocol: 'gtest')

	endforeach
endforeach
