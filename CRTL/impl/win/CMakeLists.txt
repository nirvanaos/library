target_sources (crtl PRIVATE
	initterm.cpp
	tlsdtor.cpp
	tlsdyn.cpp
	tlssup.cpp
)

add_library (crtl-main STATIC main.cpp)

target_link_libraries (crtl-main PUBLIC nirvana crtl
  "${CMAKE_CURRENT_SOURCE_DIR}/${NIRVANA_TARGET_PLATFORM}/kernel32.Lib"
  "${CMAKE_CURRENT_SOURCE_DIR}/${NIRVANA_TARGET_PLATFORM}/shell32.lib"
)
