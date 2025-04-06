cmake_minimum_required(VERSION 3.14)

project(pong-demo LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/build")

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.32.4
)

FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.8
)

FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.24.0
)

set(SDL2_DISABLE_INSTALL ON)
set(SDL2IMAGE_INSTALL OFF)
set(SDL2TTF_INSTALL OFF)

FetchContent_MakeAvailable(SDL2 SDL2_image SDL2_ttf)

include_directories("fonts")
include_directories("include")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE INCLUDES "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)
