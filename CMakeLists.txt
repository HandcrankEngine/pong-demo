cmake_minimum_required(VERSION 3.12)

project(demo-template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")

set(SDL_LIBRARY_DIR "/opt/homebrew/Cellar/sdl2/2.32.2/lib")
set(SDL_INCLUDE_DIR "/opt/homebrew/Cellar/sdl2/2.32.2/include/SDL2")

set(SDL_IMAGE_LIBRARY_DIR "/opt/homebrew/Cellar/sdl2_image/2.8.8/lib")
set(SDL_IMAGE_INCLUDE_DIR "/opt/homebrew/Cellar/sdl2_image/2.8.8/include/SDL2")

set(SDL_TTF_LIBRARY_DIR "/opt/homebrew/Cellar/sdl2_ttf/2.24.0/lib")
set(SDL_TTF_INCLUDE_DIR "/opt/homebrew/Cellar/sdl2_ttf/2.24.0/include/SDL2")

find_library(SDL2_LIBRARY NAMES SDL2 HINTS ${SDL_LIBRARY_DIR})
find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image HINTS ${SDL_IMAGE_LIBRARY_DIR})
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf HINTS ${SDL_TTF_LIBRARY_DIR})

include_directories(
    ${SDL_INCLUDE_DIR}
    ${SDL_IMAGE_INCLUDE_DIR}
    ${SDL_TTF_INCLUDE_DIR}
)

include_directories("fonts")
include_directories("images")
include_directories("include")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE INCLUDES "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
