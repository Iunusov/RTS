cmake_minimum_required(VERSION 3.8)

project("RTS" CXX)

set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
add_compile_options(/O2 /DNDEBUG)
else()
add_compile_options(-O3 -fomit-frame-pointer -DNDEBUG -fno-exceptions -pipe)
endif()


add_subdirectory(deps/SDL)

include_directories(inc)
aux_source_directory(src SUB_SOURCES)

add_executable(${PROJECT_NAME} ${SUB_SOURCES})

target_link_libraries(${PROJECT_NAME}  PRIVATE SDL_Image SDL2-static)

if (MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if (MSVC)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()
