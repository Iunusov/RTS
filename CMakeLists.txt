cmake_minimum_required(VERSION 3.8)

project("RTS" CXX)

# set standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# add global compile options
if(MSVC)
  add_compile_options(/DNDEBUG)
else()
  add_compile_options(-O2 -fomit-frame-pointer -fno-exceptions -pipe)
endif()

# set pthreads
if(NOT MSVC)
  set(CMAKE_THREAD_LIBS_INIT "-lpthread")
  set(CMAKE_HAVE_THREADS_LIBRARY 1)
  set(CMAKE_USE_WIN32_THREADS_INIT 0)
  set(CMAKE_USE_PTHREADS_INIT 1)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
endif()

# SDL options
set(SDL_STATIC
    ON
    CACHE BOOL "" FORCE)
set(SDL_SHARED
    OFF
    CACHE BOOL "" FORCE)
set(SUPPORT_PNG
    ON
    CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS
    OFF
    CACHE BOOL "" FORCE)

add_subdirectory(deps/SDL/SDL)
add_subdirectory(deps/SDL/SDL_Image)

# add executable
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stuff)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/interfaces)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/custom)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gfx)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/platform)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/engine SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/custom SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/gfx SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/platform SUB_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ SUB_SOURCES)

add_executable(${PROJECT_NAME} ${SUB_SOURCES})

# link and private compile options
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2-static SDL2_image
                                              Threads::Threads)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /MD)
else()
  target_compile_options(
    ${PROJECT_NAME}
    PRIVATE -Wall
            -Wextra
            -Wpedantic
            -pedantic
            -Werror
            -Wfatal-errors
            -Wshadow
            -Wunused
            -Wdouble-promotion
            -Wundef
            -Wconversion
            -fstack-protector-all
            -fstack-protector-strong
            -Werror=format-security
            -D_FORTIFY_SOURCE=2
            -D_GLIBCXX_ASSERTIONS
            -fpie)
endif()

# VC Release folder hack
if(MSVC)
  file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
  file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
endif()
