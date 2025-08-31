target_sources (crtl PRIVATE
	initterm.cpp
	main.cpp
	tlsdtor.cpp
	tlsdyn.cpp
	tlssup.cpp
)

target_link_directories (crtl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/${NIRVANA_TARGET_PLATFORM}")
target_link_libraries (crtl PUBLIC kernel32.lib shell32.lib)
