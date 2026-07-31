#!/bin/bash

(

    mkdir -p /tmp/dist/web

    emcc -std=c++17 -Os -o /tmp/dist/web/index.html src/*.cpp -Ifonts -Iimages -Iinclude -Isrc \
        -I"${DEPS_PREFIX}"/include -L"${DEPS_PREFIX}"/lib \
        -lSDL3 -lSDL3_image -lSDL3_ttf -lSDL3_mixer \
        -s USE_FREETYPE=1 -s USE_HARFBUZZ=1 \
        -s MAX_WEBGL_VERSION=2 \
        --shell-file templates/web-minimal.html

    gzip -k /tmp/dist/web/index.wasm

)
