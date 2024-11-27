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

idl_target(library_generated IDL_FILES ${idl_files})

add_dependencies(nirvana library_generated)
