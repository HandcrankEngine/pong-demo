// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <string>
#include <unordered_map>

#include <SDL_mixer.h>

namespace Handcrank
{

inline const int DEFAULT_AUDIO_FREQUENCY = 44100;
inline const int DEFAULT_AUDIO_CHUNK_SIZE = 1024;

namespace
{
bool audioLoadedForFirstTime = false;

std::unordered_map<std::string, Mix_Music *> audioMusicCache =
    std::unordered_map<std::string, Mix_Music *>();
std::unordered_map<std::string, Mix_Chunk *> audioSFXCache =
    std::unordered_map<std::string, Mix_Chunk *>();
} // namespace

inline auto ClearAudioCache() -> void
{
    for (const auto &audio : audioMusicCache)
    {
        if (audio.second != nullptr)
        {
            Mix_FreeMusic(audio.second);
        }
    }

    for (const auto &audio : audioSFXCache)
    {
        if (audio.second != nullptr)
        {
            Mix_FreeChunk(audio.second);
        }
    }

    audioMusicCache.clear();
    audioSFXCache.clear();
}

inline auto SetupAudio() -> int
{
    return Mix_OpenAudio(DEFAULT_AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, 2,
                         DEFAULT_AUDIO_CHUNK_SIZE);
}

} // namespace Handcrank
