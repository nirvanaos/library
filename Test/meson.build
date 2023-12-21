foreach platform, arch : host_platforms
	foreach config : configurations
		options = ['buildtype=' + config]

		key = platform + '-' + config
		test_name = 'LibraryTest-' + key

		link_options = [
			'-L' + meson.project_build_root () + '/nirvana/host/' + platform + '/' + config,
			'-L' + meson.project_build_root () + '/googletest/host/' + platform + '/' + config,
			'-lnirvana', '-lgoogletest', '-lmock'
		]

		#libraries = [nirvana_lib_host [key], googletest_lib_host [key], mock_lib_host [key]]

		lib_test = executable (test_name,
			'TestLibrary.cpp',
			'TestString.cpp',
			'TestVector.cpp',
			include_directories: test_inc_dirs,
			link_args: link_options,
			override_options: options,
			cpp_args: '-mlzcnt'
 		)

 		test (test_name, lib_test, protocol: 'gtest')

	endforeach
endforeach
