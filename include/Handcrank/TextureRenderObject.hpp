// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Handcrank.hpp"
#include "TextureCache.hpp"

namespace Handcrank
{

class TextureRenderObject : public RenderObject
{
  protected:
    SDL_Texture *texture;

  public:
    explicit TextureRenderObject() = default;
    explicit TextureRenderObject(const float x, const float y)
        : RenderObject(x, y) {};
    explicit TextureRenderObject(const float x, const float y, const float w,
                                 const float h)
        : RenderObject(x, y, w, h) {};

    ~TextureRenderObject() override = default;

    /**
     * Set texture from an existing texture reference.
     *
     * @param texture A texture.
     */
    void SetTexture(SDL_Texture *texture)
    {
        this->texture = texture;

        UpdateRectSizeFromTexture();
    }

    /**
     * Load texture from a path.
     *
     * @param renderer A structure representing rendering state.
     * @param path File path to texture file.
     *
     * @deprecated DEVELOPMENT USE ONLY! Use LoadTexture to load textures in a
     * release build.
     */
    void LoadTexture(SDL_Renderer *renderer, const char *path)
    {
        texture = LoadCachedTexture(renderer, path);

        UpdateRectSizeFromTexture();
    }

    /**
     * Load texture from a read-only buffer.
     *
     * @param renderer A structure representing rendering state.
     * @param mem A pointer to a read-only buffer.
     * @param size The buffer size, in bytes.
     */
    void LoadTexture(SDL_Renderer *renderer, const void *mem, const int size)
    {
        texture = LoadCachedTexture(renderer, mem, size);

        UpdateRectSizeFromTexture();
    }

    /**
     * Load SVG texture from a string.
     *
     * @param renderer A structure representing rendering state.
     * @param content Full SVG string including <svg></svg> tags.
     */
    void LoadSVGString(SDL_Renderer *renderer, const std::string &content)
    {
        texture = LoadCachedTexture(renderer, content.c_str(), content.size());

        UpdateRectSizeFromTexture();
    }

    void UpdateRectSizeFromTexture()
    {
        if (texture == nullptr)
        {
            return;
        }

        int textureWidth;
        int textureHeight;

        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth,
                         &textureHeight);

        rect.w = textureWidth;
        rect.h = textureHeight;
    }
};

} // namespace Handcrank
