// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <SDL_mixer.h>

#include "AudioCache.hpp"
#include "Utilities.hpp"

namespace Handcrank
{

inline auto PlayMusic(Mix_Music *music) -> int
{
    if (music == nullptr)
    {
        return -1;
    }

    return Mix_PlayMusic(music, -1);
}

inline auto PlaySFX(Mix_Chunk *sfx) -> int
{
    if (sfx == nullptr)
    {
        return -1;
    }

    return Mix_PlayChannel(-1, sfx, 0);
}

inline auto LoadMusic(const char *path) -> Mix_Music *
{
    if (audioMusicCache.find(path) != audioMusicCache.end())
    {
        return audioMusicCache.find(path)->second;
    }

    if (!audioLoadedForFirstTime)
    {
        SetupAudio();

        audioLoadedForFirstTime = true;
    }

    auto *music = Mix_LoadMUS(path);

    if (music == nullptr)
    {
        return nullptr;
    }

    audioMusicCache.insert_or_assign(path, music);

    return music;
}

inline auto LoadMusic(const void *mem, const int size) -> Mix_Music *
{
    auto hash = MemHash(mem, size);

    if (audioMusicCache.find(hash) != audioMusicCache.end())
    {
        return audioMusicCache.find(hash)->second;
    }

    if (!audioLoadedForFirstTime)
    {
        SetupAudio();

        audioLoadedForFirstTime = true;
    }

    auto *rw = SDL_RWFromConstMem(mem, size);

    if (rw == nullptr)
    {
        return nullptr;
    }

    auto *music = Mix_LoadMUS_RW(rw, 1);

    if (music == nullptr)
    {
        return nullptr;
    }

    audioMusicCache.insert_or_assign(hash, music);

    return music;
}

inline auto LoadSFX(const char *path) -> Mix_Chunk *
{
    if (audioSFXCache.find(path) != audioSFXCache.end())
    {
        return audioSFXCache.find(path)->second;
    }

    auto *sfx = Mix_LoadWAV(path);

    if (sfx == nullptr)
    {
        return nullptr;
    }

    return sfx;
}

inline auto LoadSFX(const void *mem, const int size) -> Mix_Chunk *
{
    auto hash = MemHash(mem, size);

    if (audioSFXCache.find(hash) != audioSFXCache.end())
    {
        return audioSFXCache.find(hash)->second;
    }

    auto *rw = SDL_RWFromConstMem(mem, size);

    if (rw == nullptr)
    {
        return nullptr;
    }

    auto *sfx = Mix_LoadWAV_RW(rw, 1);

    if (sfx == nullptr)
    {
        return nullptr;
    }

    audioSFXCache.insert_or_assign(hash, sfx);

    return sfx;
}

} // namespace Handcrank
