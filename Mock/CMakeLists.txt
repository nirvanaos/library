add_library(coremock SHARED
	constants.cpp
	HostAPI.cpp
	Mutex.cpp
)

target_include_directories(coremock PRIVATE
	${CMAKE_SOURCE_DIR}/nirvana/library/Include/Nirvana # Add this for constants.cpp
	${CMAKE_SOURCE_DIR}/sdk/include/clang
	$ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}um
	$ENV{WindowsSdkDir}Include/$ENV{WindowsSDKVersion}shared
	$ENV{VCToolsInstallDir}include
)

target_link_libraries(coremock PRIVATE mincore
$<IF:$<CONFIG:Debug>,ucrtd.lib,ucrt.lib>
$<IF:$<CONFIG:Debug>,msvcprtd.lib,msvcprt.lib>
)

target_compile_options(coremock PRIVATE "--target=${NIRVANA_TARGET_ARCH}-pc-win32")
target_compile_definitions(coremock PRIVATE COREMOCK_EXPORTS)

add_library(mockimport STATIC
	Debugger.cpp
	Memory.cpp
	Module.cpp
	POSIX.cpp
)

target_link_libraries(mockimport PRIVATE coremock nirvana)
