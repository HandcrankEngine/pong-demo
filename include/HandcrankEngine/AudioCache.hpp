// Handcrank Engine - https://handcrankengine.com/
//
// ░█░█░█▀█░█▀█░█▀▄░█▀▀░█▀▄░█▀█░█▀█░█░█░░░█▀▀░█▀█░█▀▀░▀█▀░█▀█░█▀▀
// ░█▀█░█▀█░█░█░█░█░█░░░█▀▄░█▀█░█░█░█▀▄░░░█▀▀░█░█░█░█░░█░░█░█░█▀▀
// ░▀░▀░▀░▀░▀░▀░▀▀░░▀▀▀░▀░▀░▀░▀░▀░▀░▀░▀░░░▀▀▀░▀░▀░▀▀▀░▀▀▀░▀░▀░▀▀▀
//
// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <filesystem>
#include <iostream>
#include <unordered_map>

#include <SDL3_mixer/SDL_mixer.h>

#include "Utilities.hpp"

namespace HandcrankEngine
{

namespace
{

inline MIX_Mixer *mixer;

inline std::unordered_map<std::size_t, std::shared_ptr<MIX_Audio>> audioCache =
    std::unordered_map<std::size_t, std::shared_ptr<MIX_Audio>>();

} // namespace

inline auto ClearAudioCache() -> void { audioCache.clear(); }

struct MixAudioDeleter
{
    void operator()(MIX_Audio *audio) const
    {
        if (audio != nullptr)
        {
            MIX_DestroyAudio(audio);
        }
    }
};

inline auto SetupAudio() -> bool
{
    if (mixer != nullptr)
    {
        return true;
    }

    if (!MIX_Init())
    {
        SDL_Log("MIX_Init %s", SDL_GetError());

        return false;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

    if (mixer == nullptr)
    {
        SDL_Log("MIX_CreateMixerDevice %s", SDL_GetError());

        return false;
    }

    return true;
}

inline auto LoadCachedMusic(const char *path) -> std::shared_ptr<MIX_Audio>
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "[Handcrank Engine] Audio file not found: " << path
                  << "\n";

        return nullptr;
    }

    auto cacheKey = std::hash<std::string_view>{}(std::string_view(path));

    auto match = audioCache.find(cacheKey);

    if (match != audioCache.end())
    {
        return match->second;
    }

    if (!SetupAudio())
    {
        return nullptr;
    }

    auto music = std::shared_ptr<MIX_Audio>(MIX_LoadAudio(mixer, path, true),
                                            MixAudioDeleter{});

    if (music == nullptr)
    {
        return nullptr;
    }

    audioCache.insert_or_assign(cacheKey, music);

    return music;
}

inline auto LoadCachedMusic(const void *mem, int size)
    -> std::shared_ptr<MIX_Audio>
{
    auto cacheKey = MemHash(mem, size);

    auto match = audioCache.find(cacheKey);

    if (match != audioCache.end())
    {
        return match->second;
    }

    if (!SetupAudio())
    {
        return nullptr;
    }

    auto *rw = SDL_IOFromConstMem(mem, size);

    if (rw == nullptr)
    {
        return nullptr;
    }

    auto music = std::shared_ptr<MIX_Audio>(
        MIX_LoadAudio_IO(mixer, rw, true, true), MixAudioDeleter{});

    if (music == nullptr)
    {
        return nullptr;
    }

    audioCache.insert_or_assign(cacheKey, music);

    return music;
}

inline auto LoadCachedSFX(const char *path) -> std::shared_ptr<MIX_Audio>
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "[Handcrank Engine] Audio file not found: " << path
                  << "\n";

        return nullptr;
    }

    auto cacheKey = std::hash<std::string_view>{}(std::string_view(path));

    auto match = audioCache.find(cacheKey);

    if (match != audioCache.end())
    {
        return match->second;
    }

    if (!SetupAudio())
    {
        return nullptr;
    }

    auto sfx = std::shared_ptr<MIX_Audio>(MIX_LoadAudio(mixer, path, true),
                                          MixAudioDeleter{});

    if (sfx == nullptr)
    {
        return nullptr;
    }

    audioCache.insert_or_assign(cacheKey, sfx);

    return sfx;
}

inline auto LoadCachedSFX(const void *mem, int size)
    -> std::shared_ptr<MIX_Audio>
{
    auto cacheKey = MemHash(mem, size);

    auto match = audioCache.find(cacheKey);

    if (match != audioCache.end())
    {
        return match->second;
    }

    if (!SetupAudio())
    {
        return nullptr;
    }

    auto *rw = SDL_IOFromConstMem(mem, size);

    if (rw == nullptr)
    {
        return nullptr;
    }

    auto sfx = std::shared_ptr<MIX_Audio>(
        MIX_LoadAudio_IO(mixer, rw, true, true), MixAudioDeleter{});

    if (sfx == nullptr)
    {
        return nullptr;
    }

    audioCache.insert_or_assign(cacheKey, sfx);

    return sfx;
}

inline auto TeardownAudio() -> void
{
    if (mixer == nullptr)
    {
        return;
    }

    MIX_DestroyMixer(mixer);
}

} // namespace HandcrankEngine
