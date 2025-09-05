set (test_list
	tstring.c
	memcpy-1.c
	memmove1.c
	strcmp-1.c
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
	add_custom_command(TARGET ${test_name} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${test_name}> $<TARGET_FILE_DIR:${test_name}>
		COMMAND_EXPAND_LISTS
	)
endforeach ()


