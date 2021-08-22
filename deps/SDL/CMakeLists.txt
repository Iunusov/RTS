project("SDL2_image" C)

set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )

add_subdirectory(SDL2)
include_directories(SDL2/include)

include_directories(${PROJECT_NAME})

add_library(${PROJECT_NAME} STATIC ${PROJECT_NAME}/IMG.c ${PROJECT_NAME}/IMG_bmp.c )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_NAME})
