cmake_minimum_required (VERSION 3.14)
project (
  "IStream-Extra"
  VERSION 0.5.0
  DESCRIPTION "Overrides of operator>> for common structured types."
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library( ${PROJECT_NAME} INTERFACE )

file(GLOB_RECURSE PUB_FILES LIST_DIRECTORIES false CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/public/*)
target_sources(${PROJECT_NAME}
  INTERFACE  ${PUB_FILES}
  )

foreach(pub_file ${PUB_FILES})
  message(STATUS "${PROJECT_NAME} pub file: ${pub_file}")
endforeach(pub_file)

#
# The public include directory for this library's clients
#
target_include_directories(${PROJECT_NAME}
  INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/public)

#
# Unit tests
#
include(CTest)
enable_testing()
add_subdirectory ("Unit-Tests")
