cmake_minimum_required(VERSION 3.15)

set(CMAKE_POLICY_VERSION_MINIMUM 3.15)

project(pong-demo VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

include(FetchContent)

function(INSTALL_SDL_LIBRARY NAME VERSION REPO_NAME REPO_URL)
    string(TOUPPER "${NAME}" NAME_LIBRARY)

    if(EXISTS "/opt/homebrew/Cellar/${NAME}/${VERSION}/")
        set(${NAME_LIBRARY}_LIBRARY_DIR "/opt/homebrew/Cellar/${NAME}/${VERSION}/lib")
        set(${NAME_LIBRARY}_INCLUDE_DIR "/opt/homebrew/Cellar/${NAME}/${VERSION}/include")

        find_library(${NAME_LIBRARY}_LIBRARY NAMES ${NAME_LIBRARY} HINTS ${${NAME_LIBRARY}_LIBRARY_DIR})

        include_directories(${${NAME_LIBRARY}_INCLUDE_DIR})

        message(STATUS "Found ${NAME} ${VERSION} installed via brew ${${NAME_LIBRARY}_LIBRARY_DIR}")
    else()
        message(STATUS "Installing ${NAME} ${VERSION} via FetchContent")

        FetchContent_Declare(
            ${REPO_NAME}
            GIT_REPOSITORY "${REPO_URL}"
            GIT_TAG release-${VERSION}
        )

        set(${NAME_LIBRARY}_LIBRARY ${REPO_NAME}::${REPO_NAME} PARENT_SCOPE)

        FetchContent_MakeAvailable(${NAME_LIBRARY})
    endif()
endfunction()

# SDL3 Flags
set(SDL3_DISABLE_INSTALL ON)

# SDL3_image Flags
set(SDL3IMAGE_AVIF OFF)
set(SDL3IMAGE_BMP OFF)
set(SDL3IMAGE_GIF OFF)
set(SDL3IMAGE_LBM OFF)
set(SDL3IMAGE_PCX OFF)
set(SDL3IMAGE_PNM OFF)
set(SDL3IMAGE_QOI OFF)
set(SDL3IMAGE_TGA OFF)
set(SDL3IMAGE_TIF OFF)
set(SDL3IMAGE_WEBP OFF)
set(SDL3IMAGE_XCF OFF)
set(SDL3IMAGE_XPM OFF)
set(SDL3IMAGE_INSTALL OFF)

# SDL3_ttf Flags
set(SDL3TTF_INSTALL OFF)

# SDL3_mixer Flags
set(SDL3MIXER_OPUS OFF)
set(SDL3MIXER_MIDI_FLUIDSYNTH OFF)
set(SDL3MIXER_WAVPACK OFF)
set(SDL3MIXER_XMP OFF)
set(SDL3MIXER_INSTALL OFF)

INSTALL_SDL_LIBRARY("sdl3" "3.4.14" "SDL3" "https://github.com/libsdl-org/SDL.git")
INSTALL_SDL_LIBRARY("sdl3_image" "3.4.4" "SDL3_image" "https://github.com/libsdl-org/SDL_image.git")
INSTALL_SDL_LIBRARY("sdl3_ttf" "3.2.2" "SDL3_ttf" "https://github.com/libsdl-org/SDL_ttf.git")
INSTALL_SDL_LIBRARY("sdl3_mixer" "3.2.4" "SDL3_mixer" "https://github.com/libsdl-org/SDL_mixer.git")

include_directories("fonts")
include_directories("images")
include_directories("include")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE INCLUDES "include/*.hpp")

if(WIN32 AND CMAKE_BUILD_TYPE MATCHES "[Rr]elease")
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${INCLUDES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES} ${INCLUDES})
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_LIBRARY} SDL3main)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_LIBRARY})
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_IMAGE_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_TTF_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL3_MIXER_LIBRARY})

if(APPLE AND CMAKE_BUILD_TYPE MATCHES "[Rr]elease")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.handcrankengine.pong-demo"
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/resources/Info.plist.in"
    )

    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    set(APP_ICON_MACOSX ${CMAKE_SOURCE_DIR}/resources/icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    target_sources(${PROJECT_NAME} PRIVATE ${APP_ICON_MACOSX})

    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        COMPONENT Runtime
    )

    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
    set(CPACK_DMG_FORMAT "UDZO")
    set(CPACK_SYSTEM_NAME "Darwin")
    set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}")

    set(CPACK_BINARY_STGZ OFF)
    set(CPACK_BINARY_TGZ OFF)
    set(CPACK_BINARY_TZ OFF)

    include(CPack)
endif()
