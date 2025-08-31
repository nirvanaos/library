target_sources (crtl PRIVATE
	initterm.cpp
	main.cpp
	tlsdtor.cpp
	tlsdyn.cpp
	tlssup.cpp
)

target_link_libraries (crtl PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/${NIRVANA_TARGET_PLATFORM}/kernel32.Lib"
  "${CMAKE_CURRENT_SOURCE_DIR}/${NIRVANA_TARGET_PLATFORM}/shell32.lib"
)
