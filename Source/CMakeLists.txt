target_sources(nirvana PRIVATE
	BindErrorUtl.cpp
	bitutils.cpp
	ByteIn.cpp
	ByteOut.cpp
	c_heap_dbg.cpp
	Converter.cpp
	crt_startup.cpp
	debug.cpp
	Decimal.cpp
	DirectoryIterator.cpp
	filesystem.cpp
	Fixed_io.cpp
	Fixed.cpp
	FloatToBCD.cpp
	FloatToPacked.cpp
	Formatter.cpp
	MemoryHelper.cpp
	ModuleMetadata.cpp
	MurmurHash2.c
#	new_delete.cpp Must be fixed
	nirvana_module.cpp
	nirvana_process.cpp
	nirvana_singleton.cpp
	Parser.cpp
	Polynomial.cpp
	printf.cpp
	real_copy.cpp
	rescale.cpp
	scanf.cpp
	stl_utils.cpp
	throw_exception.cpp
	UTF8.cpp
	WideIn.cpp
	WideInEx.cpp
	WideOut.cpp
)