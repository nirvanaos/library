set (test_list
	tstring.c
	memcpy-1.c
	memmove1.c
	strcmp-1.c
  sprintf.c
  snprintf.c
  sscanf.c
)

foreach (file ${test_list})
  get_filename_component (test_name ${file} NAME_WE)
	add_executable (${test_name} ${file})
	target_link_libraries (${test_name} PRIVATE 
		crtl
		mockimport
    crtl-main
	)
	add_test (NAME ${test_name} COMMAND ${test_name})
endforeach ()


