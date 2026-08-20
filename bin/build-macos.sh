#!/bin/bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

(

    mkdir -p build/

    cd "${SCRIPT_DIR}" || exit

    . find-sdl.sh

    cd ..

    MACOSX_DEPLOYMENT_TARGET="11"

    # CFBundleExecutable
    MACOSX_BUNDLE_EXECUTABLE_NAME="pong-demo"
    # CFBundleGetInfoString
    MACOSX_BUNDLE_INFO_STRING=""
    # CFBundleIconFile
    MACOSX_BUNDLE_ICON_FILE="icon.icns"
    # CFBundleIdentifier
    MACOSX_BUNDLE_GUI_IDENTIFIER="com.handcrankengine.pong-demo"
    # CFBundleLongVersionString
    MACOSX_BUNDLE_LONG_VERSION_STRING="1.0"
    # CFBundleName
    MACOSX_BUNDLE_BUNDLE_NAME="pong-demo"
    # CFBundleShortVersionString
    MACOSX_BUNDLE_SHORT_VERSION_STRING="1.0"
    # CFBundleVersion
    MACOSX_BUNDLE_BUNDLE_VERSION="1.0"
    # NSHumanReadableCopyright
    MACOSX_BUNDLE_COPYRIGHT="Copyright (c) Scott Doxey. All Rights Reserved."

    CONTENTS="build/${MACOSX_BUNDLE_EXECUTABLE_NAME}.app/Contents"
    MACOS="${CONTENTS}/MacOS"
    FRAMEWORKS="${CONTENTS}/Frameworks"
    RESOURCES="${CONTENTS}/Resources"
    EXECUTABLE="${MACOS}/${MACOSX_BUNDLE_EXECUTABLE_NAME}"

    g++ -std=c++17 -mmacosx-version-min=${MACOSX_DEPLOYMENT_TARGET} \
        -o "build/${MACOSX_BUNDLE_EXECUTABLE_NAME}" src/*.cpp -Ifonts -Iimages -Iinclude -Isrc \
        -I"${SDL_INCLUDE_PATH}" -L"${SDL_PATH}/lib" \
        -I"${SDL_IMAGE_INCLUDE_PATH}" -L"${SDL_IMAGE_PATH}/lib" \
        -I"${SDL_TTF_INCLUDE_PATH}" -L"${SDL_TTF_PATH}/lib" \
        -I"${SDL_MIXER_INCLUDE_PATH}" -L"${SDL_MIXER_PATH}/lib" \
        -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer \
        -rpath @loader_path/../Frameworks

    mkdir -p "${MACOS}"
    mkdir -p "${FRAMEWORKS}"
    mkdir -p "${RESOURCES}"

    chmod -R 755 "${CONTENTS}"

    cp "build/${MACOSX_BUNDLE_EXECUTABLE_NAME}" "${MACOS}/"

    cp "resources/${MACOSX_BUNDLE_ICON_FILE}" "${RESOURCES}"

    cp "${SDL_PATH}/lib/libSDL3.dylib" "${FRAMEWORKS}"
    cp "${SDL_IMAGE_PATH}/lib/libSDL3_image.dylib" "${FRAMEWORKS}"
    cp "${SDL_TTF_PATH}/lib/libSDL3_ttf.dylib" "${FRAMEWORKS}"
    cp "${SDL_MIXER_PATH}/lib/libSDL3_mixer.dylib" "${FRAMEWORKS}"

    find_dependencies() {
        local LIBRARY="${1}"
        otool -L "${LIBRARY}" | grep -E '\.dylib' | awk '{print $1}' | while read DEPENDENCY; do
            if [[ -f "${DEPENDENCY}" && "${DEPENDENCY}" != *"/usr/lib/"* && "${DEPENDENCY}" != *"/System/"* && ! -f "${FRAMEWORKS}/$(basename "${DEPENDENCY}")" ]]; then
                cp "${DEPENDENCY}" "${FRAMEWORKS}/"
                find_dependencies "${DEPENDENCY}"
            fi
        done
    }

    find_dependencies "${SDL_PATH}/lib/libSDL3.dylib"
    find_dependencies "${SDL_IMAGE_PATH}/lib/libSDL3_image.dylib"
    find_dependencies "${SDL_TTF_PATH}/lib/libSDL3_ttf.dylib"
    find_dependencies "${SDL_MIXER_PATH}/lib/libSDL3_mixer.dylib"

    install_name_tool -change /opt/homebrew/opt/sdl3/lib/libSDL3.dylib @executable_path/../Frameworks/libSDL3.dylib "${EXECUTABLE}"
    install_name_tool -change /opt/homebrew/opt/sdl3_image/lib/libSDL3_image.dylib @executable_path/../Frameworks/libSDL3_image.dylib "${EXECUTABLE}"
    install_name_tool -change /opt/homebrew/opt/sdl3_ttf/lib/libSDL3_ttf.dylib @executable_path/../Frameworks/libSDL3_ttf.dylib "${EXECUTABLE}"
    install_name_tool -change /opt/homebrew/opt/sdl3_mixer/lib/libSDL3_mixer.dylib @executable_path/../Frameworks/libSDL3_mixer.dylib "${EXECUTABLE}"

    install_name_tool -change @rpath/libSDL3.dylib @executable_path/../Frameworks/libSDL3.dylib "${FRAMEWORKS}/libSDL3_image.dylib"
    install_name_tool -change @rpath/libSDL3.dylib @executable_path/../Frameworks/libSDL3.dylib "${FRAMEWORKS}/libSDL3_ttf.dylib"
    install_name_tool -change @rpath/libSDL3.dylib @executable_path/../Frameworks/libSDL3.dylib "${FRAMEWORKS}/libSDL3_mixer.dylib"

    cat >"${CONTENTS}/Info.plist" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>English</string>
    <key>CFBundleExecutable</key>
    <string>${MACOSX_BUNDLE_EXECUTABLE_NAME}</string>
    <key>CFBundleGetInfoString</key>
    <string>${MACOSX_BUNDLE_INFO_STRING}</string>
    <key>CFBundleIconFile</key>
    <string>${MACOSX_BUNDLE_ICON_FILE}</string>
    <key>CFBundleIdentifier</key>
    <string>${MACOSX_BUNDLE_GUI_IDENTIFIER}</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleLongVersionString</key>
    <string>${MACOSX_BUNDLE_LONG_VERSION_STRING}</string>
    <key>CFBundleName</key>
    <string>${MACOSX_BUNDLE_BUNDLE_NAME}</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>${MACOSX_BUNDLE_SHORT_VERSION_STRING}</string>
    <key>CFBundleSignature</key>
    <string>????</string>
    <key>CFBundleVersion</key>
    <string>${MACOSX_BUNDLE_BUNDLE_VERSION}</string>
    <key>NSHumanReadableCopyright</key>
    <string>${MACOSX_BUNDLE_COPYRIGHT}</string>
    <key>NSHighResolutionCapable</key>
    <true/>
    <key>LSMinimumSystemVersion</key>
    <string>${MACOSX_DEPLOYMENT_TARGET}</string>
</dict>
</plist>
EOF

    rm -rf build/dist

    mkdir -p build/dist

    cp -r "build/${MACOSX_BUNDLE_EXECUTABLE_NAME}.app" "build/dist/"

    ln -s /Applications "build/dist/Applications"

    hdiutil create -volname "${MACOSX_BUNDLE_EXECUTABLE_NAME}" -srcfolder "build/dist" -ov -format UDZO "build/${MACOSX_BUNDLE_EXECUTABLE_NAME}.dmg"

)
