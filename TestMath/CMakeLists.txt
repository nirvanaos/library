set (openlibm ${CMAKE_SOURCE_DIR}/openlibm)
set (test_list
	test-float.c
	test-double.c
	test-long-double.c
)

foreach (file ${test_list})
  get_filename_component (test_name ${file} NAME_WE)
	add_executable (${test_name} ${openlibm}/test/${file})
	target_link_libraries (${test_name} PRIVATE 
		crtl
		mockimport
    crtl-main
	)
	target_include_directories (${test_name} PRIVATE ${openlibm}/include)
	target_compile_definitions (${test_name} PRIVATE __XSI_VISIBLE)
	target_compile_definitions (${test_name} PRIVATE __BSD_VISIBLE)
	target_compile_options (${test_name} PRIVATE -Wno-unused-value -ffp-model=strict)
	add_test (NAME ${test_name} COMMAND ${test_name})
	add_custom_command(TARGET ${test_name} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${test_name}> $<TARGET_FILE_DIR:${test_name}>
		COMMAND_EXPAND_LISTS
	)
endforeach ()
