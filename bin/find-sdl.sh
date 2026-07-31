#!/bin/bash

[[ ! -d "${SDL_PATH}" && -d "/opt/homebrew/Cellar/sdl3" ]] &&
    export SDL_PATH=$(find /opt/homebrew/Cellar/sdl3 -name "3.*" -type d | head -n 1)
[[ ! -d "${SDL_PATH}" && -d "/tmp/.sdl/" ]] &&
    export SDL_PATH=$(find /tmp/.sdl/ -name "SDL-*" -type d | head -n 1)

[ ! -d "${SDL_PATH}" ] && echo "SDL not found!" && exit 1

export SDL_INCLUDE_PATH="${SDL_PATH}/include"

echo "SDL found at ${SDL_PATH}"

[[ ! -d "${SDL_IMAGE_PATH}" && -d "/opt/homebrew/Cellar/sdl3_image" ]] &&
    export SDL_IMAGE_PATH=$(find /opt/homebrew/Cellar/sdl3_image -name "3.*" -type d | head -n 1)
[[ ! -d "${SDL_IMAGE_PATH}" && -d "/tmp/.sdl/" ]] &&
    export SDL_IMAGE_PATH=$(find /tmp/.sdl/ -name "SDL_image-*" -type d | head -n 1)

[ ! -d "${SDL_IMAGE_PATH}" ] && echo "SDL_image not found!" && exit 1

export SDL_IMAGE_INCLUDE_PATH="${SDL_IMAGE_PATH}/include"

echo "SDL_image found at ${SDL_IMAGE_PATH}"

[[ ! -d "${SDL_TTF_PATH}" && -d "/opt/homebrew/Cellar/sdl3_ttf" ]] &&
    export SDL_TTF_PATH=$(find /opt/homebrew/Cellar/sdl3_ttf -name "3.*" -type d | head -n 1)
[[ ! -d "${SDL_TTF_PATH}" && -d "/tmp/.sdl/" ]] &&
    export SDL_TTF_PATH=$(find /tmp/.sdl/ -name "SDL_ttf-*" -type d | head -n 1)

[ ! -d "${SDL_TTF_PATH}" ] && echo "SDL_ttf not found!" && exit 1

export SDL_TTF_INCLUDE_PATH="${SDL_TTF_PATH}/include"

echo "SDL_ttf found at ${SDL_TTF_PATH}"

[[ ! -d "${SDL_MIXER_PATH}" && -d "/opt/homebrew/Cellar/sdl3_mixer" ]] &&
    export SDL_MIXER_PATH=$(find /opt/homebrew/Cellar/sdl3_mixer -name "3.*" -type d | head -n 1)
[[ ! -d "${SDL_MIXER_PATH}" && -d "/tmp/.sdl/" ]] &&
    export SDL_MIXER_PATH=$(find /tmp/.sdl/ -name "SDL_mixer-*" -type d | head -n 1)

[ ! -d "${SDL_MIXER_PATH}" ] && echo "SDL_mixer not found!" && exit 1

export SDL_MIXER_INCLUDE_PATH="${SDL_MIXER_PATH}/include"

echo "SDL_mixer found at ${SDL_MIXER_PATH}"

export LD_LIBRARY_PATH="${SDL_PATH}/lib:${SDL_IMAGE_PATH}/lib:${SDL_TTF_PATH}/lib:${SDL_MIXER_PATH}/lib:${LD_LIBRARY_PATH}"
