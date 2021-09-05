cmake_minimum_required(VERSION 3.8)

project("RTS" CXX)

# set standard
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# add global compile options
if (MSVC)
add_compile_options(/O2 /DNDEBUG)
else()
add_compile_options(-Ofast -fomit-frame-pointer -pipe)
endif()

# set pthreads
if (NOT MSVC)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

# SDL options
set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )
set( SUPPORT_PNG ON CACHE BOOL "" FORCE )
set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )



add_subdirectory(deps/SDL/SDL)
add_subdirectory(deps/SDL/SDL_Image)

# add executable
include_directories(inc)
include_directories(inc/game)
include_directories(inc/kernel)
include_directories(inc/impl/2D)
include_directories(inc/interfaces)
aux_source_directory(src SUB_SOURCES)
aux_source_directory(src/game SUB_SOURCES)
aux_source_directory(src/kernel SUB_SOURCES)
aux_source_directory(src/impl/2D SUB_SOURCES)
add_executable(${PROJECT_NAME} ${SUB_SOURCES})

# link and private compile options
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}  PRIVATE SDL2-static SDL2_image Threads::Threads)
if (MSVC)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /MD)
else()
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -w -Wfatal-errors )
endif()

# VC Release folder hack
if (MSVC)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()
