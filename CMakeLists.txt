#CMakeList.txt : CMake project for RTS, include source and define
#project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("RTS" CXX)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release)

include_directories(inc) 
include_directories(kernel/inc)

aux_source_directory(kernel SUB_SOURCES)
aux_source_directory(src SUB_SOURCES)

add_executable(RTS ${SUB_SOURCES})

if (MSVC)
target_compile_options(RTS PRIVATE /MT /W4 /WX /O2 /Ot /Oy /fp:fast /GF /Gy /DNDEBUG)

else()
target_compile_options(RTS PRIVATE -Wall -Wextra -pedantic -Werror -Ofast -fomit-frame-pointer -fno-exceptions -DNDEBUG)

endif()
