add_library (crtl STATIC)
add_subdirectory (Source)

target_precompile_headers (crtl PRIVATE 
	$<$<COMPILE_LANGUAGE:CXX>:
    <Nirvana/Nirvana.h$<ANGLE-R>
    <Nirvana/POSIX.h$<ANGLE-R>>
)

set (crtl_init_src
	${CMAKE_CURRENT_SOURCE_DIR}/Source/impl/crtbegin.c
	${CMAKE_CURRENT_SOURCE_DIR}/Source/impl/crtend.c
	${CMAKE_CURRENT_SOURCE_DIR}/Source/impl/crt2.c
)

foreach (file_src ${crtl_init_src})
	cmake_path (GET file_src STEM name)
	set (file_o "${CMAKE_CURRENT_BINARY_DIR}/${name}.o")
	add_custom_command (TARGET crtl POST_BUILD
		COMMAND ${CMAKE_C_COMPILER} -v -c $<TARGET_PROPERTY:crtl,COMPILE_OPTIONS_$<CONFIG>> -o $<TARGET_FILE_DIR:crtl>/${name}.o ${file_src}
		COMMAND_EXPAND_LISTS
	)
endforeach ()

target_link_libraries (crtl PUBLIC nirvana)
