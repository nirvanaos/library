target_sources (crtl PRIVATE
  dyn_tls_init.c
	initterm.cpp
	tlsdtor.cpp
	tlsdyn.cpp
	tlssup.cpp
)

add_library (crtl-main STATIC main.cpp)
target_include_directories (crtl-main PRIVATE ${CMAKE_SOURCE_DIR}/nirvana/library/Port/Windows/Include)

target_link_libraries (crtl-main PUBLIC nirvana crtl
  "${CMAKE_CURRENT_SOURCE_DIR}/${NIRVANA_TARGET_PLATFORM}/kernel32.Lib"
  "${CMAKE_CURRENT_SOURCE_DIR}/${NIRVANA_TARGET_PLATFORM}/shell32.lib"
)
