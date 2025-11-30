target_sources (crtl PRIVATE
  dyn_tls_init.c
	initterm.cpp
	tlsdtor.cpp
	tlsdyn.cpp
	tlssup.cpp
)

add_library (crtl-main STATIC main.cpp)
target_include_directories (crtl-main PRIVATE ${CMAKE_SOURCE_DIR}/nirvana/library/Port/Include)

# Get Windows SDK and Visual Studio environment
execute_process (COMMAND powershell.exe -File "${CMAKE_CURRENT_SOURCE_DIR}/vsdevenv.ps1" ${NIRVANA_TARGET_PLATFORM}
	ENCODING UTF-8
	OUTPUT_VARIABLE vs_environment
)

string (JSON WindowsSdkDir GET ${vs_environment} WindowsSdkDir)
string (JSON WindowsSDKVersion GET ${vs_environment} WindowsSDKVersion)
string (JSON VCToolsInstallDir GET ${vs_environment} VCToolsInstallDir)

set (WIN_LIB_DIR "${WindowsSdkDir}Lib/${WindowsSDKVersion}um/${NIRVANA_TARGET_PLATFORM}")

target_link_libraries (crtl-main PUBLIC Nirvana crtl
	${WIN_LIB_DIR}/kernel32.Lib
	${WIN_LIB_DIR}/shell32.lib
)
