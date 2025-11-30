set (test_list
	string1.cpp
	string2.cpp
  vector.cpp
)

foreach (file ${test_list})
  get_filename_component (test_name ${file} NAME_WE)
	add_executable (${test_name} ${file})
	target_link_libraries (${test_name} PRIVATE 
		crtl
		MockImport
    crtl-main
	)
	add_test (NAME ${test_name} COMMAND ${test_name})
endforeach ()

add_executable (static static1.cpp static2.cpp)

target_link_libraries (static PRIVATE 
	crtl
	MockImport
	crtl-main
)
add_test (NAME static COMMAND static)

