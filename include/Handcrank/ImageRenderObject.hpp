// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Handcrank.hpp"
#include "TextureRenderObject.hpp"

namespace Handcrank
{

class ImageRenderObject : public TextureRenderObject
{
  protected:
    SDL_Rect srcRect = SDL_Rect();

    bool srcRectSet = false;

    SDL_FPoint centerPoint = SDL_FPoint();

    SDL_Color tintColor = SDL_Color{MAX_R, MAX_G, MAX_B, MAX_ALPHA};

    int alpha = MAX_ALPHA;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

  public:
    explicit ImageRenderObject() = default;
    explicit ImageRenderObject(const float x, const float y)
        : TextureRenderObject(x, y) {};
    explicit ImageRenderObject(const float x, const float y, const float w,
                               const float h)
        : TextureRenderObject(x, y, w, h) {};

    ~ImageRenderObject() override = default;

    void SetSrcRect(const SDL_Rect &srcRect)
    {
        this->srcRect.x = srcRect.x;
        this->srcRect.y = srcRect.y;
        this->srcRect.w = srcRect.w;
        this->srcRect.h = srcRect.h;

        srcRectSet = true;
    }

    void SetSrcRect(const int x, const int y, const int w, const int h)
    {
        this->srcRect.x = x;
        this->srcRect.y = y;
        this->srcRect.w = w;
        this->srcRect.h = h;

        srcRectSet = true;
    }

    void SetFlip(const SDL_RendererFlip flip) { this->flip = flip; }

    void SetTintColor(const SDL_Color &tintColor)
    {
        this->tintColor.r = tintColor.r;
        this->tintColor.g = tintColor.g;
        this->tintColor.b = tintColor.b;
    }

    void SetTintColor(const Uint8 r, const Uint8 g, const Uint8 b)
    {
        this->tintColor.r = r;
        this->tintColor.g = g;
        this->tintColor.b = b;
    }

    [[nodiscard]] auto GetTintColor() const -> const SDL_Color &
    {
        return tintColor;
    }

    void SetAlpha(const int alpha) { this->alpha = alpha; }

    [[nodiscard]] auto GetAlpha() const -> int { return alpha; }

    /**
     * Render image to the scene.
     *
     * @param renderer A structure representing rendering state.
     */
    void Render(SDL_Renderer *renderer) override
    {
        if (!CanRender() || texture == nullptr)
        {
            return;
        }

        auto transformedRect = GetTransformedRect();

        SDL_SetTextureColorMod(texture, tintColor.r, tintColor.g, tintColor.b);

        SDL_SetTextureAlphaMod(texture, alpha);

        SDL_RenderCopyExF(renderer, texture, srcRectSet ? &srcRect : nullptr,
                          &transformedRect, 0, &centerPoint, flip);

        RenderObject::Render(renderer);
    }
};

} // namespace Handcrank
