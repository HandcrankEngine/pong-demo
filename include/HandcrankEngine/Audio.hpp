// Handcrank Engine - https://handcrankengine.com/
//
// ░█░█░█▀█░█▀█░█▀▄░█▀▀░█▀▄░█▀█░█▀█░█░█░░░█▀▀░█▀█░█▀▀░▀█▀░█▀█░█▀▀
// ░█▀█░█▀█░█░█░█░█░█░░░█▀▄░█▀█░█░█░█▀▄░░░█▀▀░█░█░█░█░░█░░█░█░█▀▀
// ░▀░▀░▀░▀░▀░▀░▀▀░░▀▀▀░▀░▀░▀░▀░▀░▀░▀░▀░░░▀▀▀░▀░▀░▀▀▀░▀▀▀░▀░▀░▀▀▀
//
// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <SDL3_mixer/SDL_mixer.h>

#include "AudioCache.hpp"

namespace HandcrankEngine
{

namespace
{

inline MIX_Track *musicTrack;

inline MIX_Track *sfxTrack;

} // namespace

inline auto PlayMusic(MIX_Audio *audio) -> bool
{
    if (audio == nullptr)
    {
        return false;
    }

    if (musicTrack == nullptr)
    {
        musicTrack = MIX_CreateTrack(mixer);
    }

    MIX_SetTrackAudio(musicTrack, audio);

    return MIX_PlayTrack(musicTrack, 0);
}

inline auto PlaySFX(MIX_Audio *audio) -> bool
{
    if (audio == nullptr)
    {
        return false;
    }

    if (sfxTrack == nullptr)
    {
        sfxTrack = MIX_CreateTrack(mixer);
    }

    MIX_SetTrackAudio(sfxTrack, audio);

    return MIX_PlayTrack(sfxTrack, 0);
}

inline void StopAllMusic() { MIX_StopTrack(musicTrack, 0); }

inline void StopAllSFX() { MIX_StopTrack(sfxTrack, 0); }

} // namespace HandcrankEngine
