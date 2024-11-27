set(idl_files
	BindError.idl
	Debugger.idl
	DecCalc.idl
	Domains.idl
	File.idl
	JSON.idl
	Main.idl
	Memory.idl
	Module.idl
	ModuleInit.idl
	NDBC_ex.idl
	NDBC.idl
	nls.idl
	Packages.idl
	POSIX.idl
	Security.idl
	Shell.idl
	System.idl
)

foreach(f IN LISTS idl_files)
	list(APPEND idl_idl ${CMAKE_CURRENT_SOURCE_DIR}/${f})
	cmake_path(REMOVE_EXTENSION f OUTPUT_VARIABLE f_no_ext)
	list(APPEND idl_h ${CMAKE_CURRENT_SOURCE_DIR}/${f_no_ext}.h)
	list(APPEND idl_h ${CMAKE_CURRENT_SOURCE_DIR}/${f_no_ext}_s.h)
	cmake_path(GET f STEM name)
	list(APPEND idl_cpp ${CMAKE_CURRENT_BINARY_DIR}/${name}.cpp)
endforeach()

add_custom_command(
	OUTPUT ${idl_h} ${idl_cpp}
	COMMAND ${NIDL2CPP} ARGS ${IDL_OPTIONS} ${idl_idl} -no_proxy -out_cpp ${CMAKE_CURRENT_BINARY_DIR}
	DEPENDS ${idl_idl}
	COMMENT "Compile IDL..."
	VERBATIM
)

add_custom_target(library_generated DEPENDS ${idl_cpp})
add_dependencies(nirvana library_generated)
