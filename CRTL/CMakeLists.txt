add_library (crtl STATIC)
add_subdirectory (Source)

target_precompile_headers (crtl PRIVATE 
	$<$<COMPILE_LANGUAGE:CXX>:
    <Nirvana/Nirvana.h$<ANGLE-R>
    <Nirvana/POSIX.h$<ANGLE-R>>
)

target_link_libraries (crtl PUBLIC nirvana)
