// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Utilities.hpp"

namespace Handcrank
{

namespace
{
std::unordered_map<std::string, SDL_Texture *> textureCache =
    std::unordered_map<std::string, SDL_Texture *>();
}

inline auto ClearTextureCache() -> void
{
    for (const auto &texture : textureCache)
    {
        if (texture.second != nullptr)
        {
            SDL_DestroyTexture(texture.second);
        }
    }

    textureCache.clear();
}

/**
 * Load texture from a path.
 *
 * @param renderer A structure representing rendering state.
 * @param path File path to texture file.
 */

inline auto LoadCachedTexture(SDL_Renderer *renderer, const char *path)
    -> SDL_Texture *
{
    if (textureCache.find(path) != textureCache.end())
    {
        return textureCache.find(path)->second;
    }

    auto *surface = IMG_Load(path);

    if (surface == nullptr)
    {
        return nullptr;
    }

    auto *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (texture == nullptr)
    {
        return nullptr;
    }

    textureCache.insert_or_assign(path, texture);

    return texture;
}

/**
 * Load texture from a read-only buffer.
 *
 * @param renderer A structure representing rendering state.
 * @param mem A pointer to a read-only buffer.
 * @param size The buffer size, in bytes.
 */
inline auto LoadCachedTexture(SDL_Renderer *renderer, const void *mem,
                              const int size) -> SDL_Texture *
{
    auto hash = MemHash(mem, size);

    if (textureCache.find(hash) != textureCache.end())
    {
        return textureCache.find(hash)->second;
    }

    auto *rw = SDL_RWFromConstMem(mem, size);

    auto *surface =
        IMG_isSVG(rw) == SDL_TRUE ? IMG_LoadSVG_RW(rw) : IMG_Load_RW(rw, 1);

    if (surface == nullptr)
    {
        return nullptr;
    }

    auto *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (texture == nullptr)
    {
        return nullptr;
    }

    textureCache.insert_or_assign(hash, texture);

    return texture;
}

} // namespace Handcrank
