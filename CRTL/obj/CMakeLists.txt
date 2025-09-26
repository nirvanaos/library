add_library (crtl_obj OBJECT
	crt2.c
	crtbegin.c
	crtend.c
)

target_compile_definitions (crtl_obj PRIVATE CRT_HAS_INITFINI_ARRAY)
