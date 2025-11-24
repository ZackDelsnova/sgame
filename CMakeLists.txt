# CMakeList.txt : CMake project for sgame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)
project(sgame VERSION 0.1.0 LANGUAGES C CXX)

# cpp version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# raylib paths
set(RAYLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/raylib/include)
set(RAYLIB_LIB_DIR ${CMAKE_SOURCE_DIR}/external/raylib/lib)

# include dir
include_directories(
	${RAYLIB_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR}/include)

# lib dir
link_directories(${RAYLIB_LIB_DIR})

# src file
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(sgame WIN32 ${SRC_FILES})

# raylib and windows libs
target_link_libraries(sgame raylib
	winmm
	gdi32
	shell32
)

# warnings
if (MSVC)
	target_compile_options(sgame PRIVATE /W4 /WX /permissive-)
else()
	target_compile_options(sgame PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# cpy resource folder
if (EXISTS "${CMAKE_SOURCE_DIR}/resources")
	add_custom_target(copy_resources ALL 
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:sgame>/resources
	)
	add_dependencies(sgame copy_resources)
else()
	message(WARNING "Resources folder not found, skipping copy step.")
endif()
