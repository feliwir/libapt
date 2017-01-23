cmake_minimum_required(VERSION 3.1)

project(libapt CXX)

set(LIBAPT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)

option(LIBAPT_BUILD_TEST "Build a simple apt player" ON)
if(LIBAPT_BUILD_TEST)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory(deps/glfw)
	set(LIBBIG_BUILD_TEST OFF CACHE BOOL "" FORCE)
	add_subdirectory(deps/libbig)
	add_subdirectory(test)
endif()