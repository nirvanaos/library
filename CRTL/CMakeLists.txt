add_library (crtl STATIC)
add_subdirectory (Source)

target_precompile_headers (crtl PRIVATE 
	$<$<COMPILE_LANGUAGE:CXX>:
    <Nirvana/Nirvana.h$<ANGLE-R>
    <Nirvana/POSIX.h$<ANGLE-R>>
)

target_link_libraries (crtl PUBLIC nirvana)

add_subdirectory (obj)

get_target_property (obj_sources crtl_obj SOURCES)

foreach (obj_src ${obj_sources})
	cmake_path (GET obj_src FILENAME filename)
	cmake_path (GET obj_src STEM name)
	set (file_obj ${CMAKE_CURRENT_BINARY_DIR}/obj/CMakeFiles/crtl_obj.dir/${CMAKE_CFG_INTDIR}/${filename}${CMAKE_C_OUTPUT_EXTENSION})
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
