cmake_minimum_required(VERSION 3.8)

project("RTS" CXX)

# set standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add global compile options
if(MSVC)
  add_compile_options(/MT)
  add_compile_options(/MP)
  add_compile_options(/nologo)

  #add_compile_options("$<$<CONFIG:Release>:/fp:strict>")
  add_compile_options("$<$<CONFIG:Release>:/O2>")
else()
  add_compile_options("$<$<CONFIG:Release>:-O2>")
  add_compile_options("$<$<CONFIG:Release>:-fomit-frame-pointer>")
  add_compile_options("$<$<CONFIG:Release>:-fno-exceptions>")
  add_compile_options("$<$<CONFIG:Release>:-pipe>")
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
set(SDLIMAGE_AVIF
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_XV
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_XPM
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_XCF
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_PNM
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_PCX
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_LBM
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_QOI
    OFF
    CACHE BOOL "" FORCE)
set(SDLIMAGE_TGA
    OFF
    CACHE BOOL "" FORCE)
	
	       


#FIXME:
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL/SDL_Image/include/SDL3_image)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/SDL/SDL/include/SDL3)

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
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static SDL3_image-static
                                              Threads::Threads)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
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
