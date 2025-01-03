set(idl_files
	BindError.idl
	Debugger.idl
	DecCalc.idl
	Domains.idl
	File.idl
	FileStat.idl
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

idl_generate(OUT_VAR src IDL_FILES ${idl_files})
target_sources(nirvana PRIVATE ${src})
add_custom_target(lib_gen DEPENDS ${src})
add_dependencies(nirvana lib_gen)
