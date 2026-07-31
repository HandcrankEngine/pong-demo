#!/bin/bash

(

    mkdir -p /tmp/dist/linux

    g++ -std=c++17 -o /tmp/dist/linux/pong-demo src/*.cpp -Ifonts -Iimages -Iinclude -Isrc \
        /usr/local/lib/libSDL3_ttf.a \
        /usr/local/lib/libSDL3_mixer.a \
        /usr/local/lib/libSDL3_image.a \
        /usr/local/lib/libSDL3.a \
        $(pkg-config --libs --static sdl3 sdl3-image sdl3-ttf sdl3-mixer)

)
