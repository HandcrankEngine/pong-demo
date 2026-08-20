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

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Utilities.hpp"

namespace HandcrankEngine
{

namespace
{
inline std::unordered_map<std::size_t, std::shared_ptr<SDL_Texture>>
    textureCache =
        std::unordered_map<std::size_t, std::shared_ptr<SDL_Texture>>();
}

struct TextureDeleter
{
    void operator()(SDL_Texture *texture) const
    {
        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
        }
    }
};

inline auto ClearTextureCache() -> void { textureCache.clear(); }

/**
 * Load texture from a path.
 *
 * @param renderer A structure representing rendering state.
 * @param path File path to texture file.
 */

inline auto LoadCachedTexture(SDL_Renderer *renderer, const char *path)
    -> SDL_Texture *
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "[Handcrank Engine] Image file not found: " << path
                  << "\n";

        return nullptr;
    }

    auto cacheKey = std::hash<std::string_view>{}(std::string_view(path));

    auto match = textureCache.find(cacheKey);

    if (match != textureCache.end())
    {
        return match->second.get();
    }

    auto *surface = IMG_Load(path);

    if (surface == nullptr)
    {
        return nullptr;
    }

    auto texture = std::shared_ptr<SDL_Texture>(
        SDL_CreateTextureFromSurface(renderer, surface), TextureDeleter{});

    SDL_DestroySurface(surface);

    if (texture == nullptr)
    {
        return nullptr;
    }

    textureCache.insert_or_assign(cacheKey, texture);

    return texture.get();
}

/**
 * Load texture from a path.
 *
 * @param renderer A structure representing rendering state.
 * @param path File path to texture file.
 * @param color The color to use as the transparent color key.
 */

inline auto LoadCachedTransparentTexture(SDL_Renderer *renderer,
                                         const char *path,
                                         const SDL_Color colorKey)
    -> SDL_Texture *
{
    if (!std::filesystem::exists(path))
    {
        std::cerr << "[Handcrank Engine] Image file not found: " << path
                  << "\n";

        return nullptr;
    }

    auto cacheKey = std::hash<std::string_view>{}(std::string_view(path));

    auto match = textureCache.find(cacheKey);

    if (match != textureCache.end())
    {
        return match->second.get();
    }

    auto *surface = IMG_Load(path);

    if (surface == nullptr)
    {
        return nullptr;
    }

    SDL_SetSurfaceColorKey(
        surface, true,
        SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), nullptr,
                   colorKey.r, colorKey.g, colorKey.b));

    auto texture = std::shared_ptr<SDL_Texture>(
        SDL_CreateTextureFromSurface(renderer, surface), TextureDeleter{});

    SDL_DestroySurface(surface);

    if (texture == nullptr)
    {
        return nullptr;
    }

    textureCache.insert_or_assign(cacheKey, texture);

    return texture.get();
}

/**
 * Load texture from a read-only buffer.
 *
 * @param renderer A structure representing rendering state.
 * @param mem A pointer to a read-only buffer.
 * @param size The buffer size, in bytes.
 */
inline auto LoadCachedTexture(SDL_Renderer *renderer, const void *mem, int size)
    -> SDL_Texture *
{
    auto cacheKey = MemHash(mem, size);

    auto match = textureCache.find(cacheKey);

    if (match != textureCache.end())
    {
        return match->second.get();
    }

    auto *rw = SDL_IOFromConstMem(mem, size);

    auto *surface = IMG_isSVG(rw) ? IMG_LoadSVG_IO(rw) : IMG_Load_IO(rw, true);

    if (surface == nullptr)
    {
        return nullptr;
    }

    auto texture = std::shared_ptr<SDL_Texture>(
        SDL_CreateTextureFromSurface(renderer, surface), TextureDeleter{});

    SDL_DestroySurface(surface);

    if (texture == nullptr)
    {
        return nullptr;
    }

    textureCache.insert_or_assign(cacheKey, texture);

    return texture.get();
}

/**
 * Load texture from a read-only buffer.
 *
 * @param renderer A structure representing rendering state.
 * @param mem A pointer to a read-only buffer.
 * @param size The buffer size, in bytes.
 * @param color The color to use as the transparent color key.
 */
inline auto LoadCachedTransparentTexture(SDL_Renderer *renderer,
                                         const void *mem, int size,
                                         const SDL_Color colorKey)
    -> SDL_Texture *
{
    auto cacheKey = MemHash(mem, size);

    auto match = textureCache.find(cacheKey);

    if (match != textureCache.end())
    {
        return match->second.get();
    }

    auto *rw = SDL_IOFromConstMem(mem, size);

    auto *surface = IMG_isSVG(rw) ? IMG_LoadSVG_IO(rw) : IMG_Load_IO(rw, true);

    if (surface == nullptr)
    {
        return nullptr;
    }

    SDL_SetSurfaceColorKey(
        surface, true,
        SDL_MapSurfaceRGB(surface, colorKey.r, colorKey.g, colorKey.b));

    auto texture = std::shared_ptr<SDL_Texture>(
        SDL_CreateTextureFromSurface(renderer, surface), TextureDeleter{});

    SDL_DestroySurface(surface);

    if (texture == nullptr)
    {
        return nullptr;
    }

    textureCache.insert_or_assign(cacheKey, texture);

    return texture.get();
}

} // namespace HandcrankEngine
