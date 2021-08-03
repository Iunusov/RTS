#CMakeList.txt : CMake project for RTS, include source and define
#project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("RTS" CXX)

set (CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)

include_directories(inc) 
include_directories(kernel/inc)

aux_source_directory(kernel SUB_SOURCES)
aux_source_directory(src SUB_SOURCES)

add_executable(RTS "RTS.cpp" ${SUB_SOURCES})


if (MSVC)
target_compile_options(RTS PRIVATE /W4 /WX /Ot)

else()
target_compile_options(RTS PRIVATE -Wall -Wextra -pedantic -Werror -Ofast -fomit-frame-pointer)

endif()
