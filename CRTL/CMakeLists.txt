add_library (crtl STATIC)
add_subdirectory (Source)

#target_compile_options (crtl PRIVATE -v)

target_precompile_headers (crtl PRIVATE 
	$<$<COMPILE_LANGUAGE:CXX>:
    <Nirvana/Nirvana.h$<ANGLE-R>
    <Nirvana/POSIX.h$<ANGLE-R>>
)

target_link_libraries (crtl PUBLIC nirvana)

set (crtl_obj_src
	${CMAKE_CURRENT_SOURCE_DIR}/Source/impl/crtbegin.c
	${CMAKE_CURRENT_SOURCE_DIR}/Source/impl/crtend.c
	${CMAKE_CURRENT_SOURCE_DIR}/Source/impl/crt2.c
)

add_library (crtl_obj OBJECT ${crtl_obj_src})
target_compile_options (crtl_obj PRIVATE -v)

foreach (file_src ${crtl_obj_src})
	cmake_path (RELATIVE_PATH file_src OUTPUT_VARIABLE path)
	cmake_path (GET file_src STEM name)
	set (file_obj ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/crtl_obj.dir/${CMAKE_CFG_INTDIR}/${path}${CMAKE_C_OUTPUT_EXTENSION})
	set (file_o ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${name}.o)
	add_custom_command (
		OUTPUT ${file_o}
		DEPENDS crtl_obj
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file_obj} ${file_o}
	)
	list (APPEND crtl_o ${file_o})
endforeach ()

add_custom_target (crtl_start DEPENDS ${crtl_o})
add_dependencies (crtl crtl_start)
