cmake_minimum_required(VERSION 3.8)

project("RTS" CXX)

# set standard
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set pthreads
if (NOT MSVC)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

# add global compile options
if (MSVC)
add_compile_options(/O2 /DNDEBUG)
else()
add_compile_options(-O3 -fomit-frame-pointer -DNDEBUG -fno-exceptions -pipe)
endif()

# build SDL
add_subdirectory(deps/SDL)

# add executable
include_directories(inc)
aux_source_directory(src SUB_SOURCES)
add_executable(${PROJECT_NAME} ${SUB_SOURCES})

# link and private compile options
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}  PRIVATE SDL_Image SDL2-static Threads::Threads)
if (MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# VC Release folder hack
if (MSVC)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()
