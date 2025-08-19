// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "FontCache.hpp"
#include "Handcrank.hpp"

namespace Handcrank
{

class TextRenderObject : public RenderObject
{
  protected:
    TTF_Font *font;

    SDL_Color color{MAX_R, MAX_G, MAX_B, MAX_ALPHA};

    const char *text;

    SDL_Surface *textSurface = nullptr;

    SDL_Texture *textTexture = nullptr;

  public:
    explicit TextRenderObject() = default;

    explicit TextRenderObject(const float x, const float y)
        : RenderObject(x, y) {};
    explicit TextRenderObject(const float x, const float y, const float w,
                              const float h)
        : RenderObject(x, y, w, h) {};

    ~TextRenderObject() override
    {
        if (textTexture != nullptr)
        {
            SDL_DestroyTexture(textTexture);
        }

        if (textSurface != nullptr)
        {
            SDL_FreeSurface(textSurface);
        }
    };

    /**
     * Set text font.
     *
     * @param font Font value to set.
     */
    void SetFont(TTF_Font *font) { this->font = font; }

    /**
     * Load font from a path.
     *
     * @param path File path to font file.
     * @param ptSize The size of the font.
     *
     * @deprecated DEVELOPMENT USE ONLY! Use SDL_LoadFont to load
     * fonts in a release build.
     */
    void LoadFont(const char *path, const int ptSize = 24)
    {
        font = LoadCachedFont(path, ptSize);
    }

    /**
     * Load font from a read-only buffer.
     *
     * @param mem A pointer to a read-only buffer.
     * @param size The buffer size, in bytes.
     * @param ptSize The size of the font.
     */
    void LoadFontRW(const void *mem, const int size, const int ptSize = 24)
    {
        font = LoadCachedFont(mem, size, ptSize);
    }

    /**
     * Set text color.
     *
     * @param color Color value to set.
     */
    void SetColor(const SDL_Color color) { this->color = color; }

    /**
     * Set text content.
     *
     * @param text Text value to set.
     */
    void SetText(const char *text)
    {
        if (font == nullptr)
        {
            throw std::runtime_error("ERROR! Missing font reference.");

            return;
        }

        this->text = text;

        if (textTexture != nullptr)
        {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }

        if (textSurface != nullptr)
        {
            SDL_FreeSurface(textSurface);
            textSurface = nullptr;
        }

        textSurface = TTF_RenderText_Blended(font, this->text, color);

        if (textSurface == nullptr)
        {
            throw std::runtime_error("ERROR! Failed to generate text surface.");

            return;
        }

        rect.w = textSurface->w;
        rect.h = textSurface->h;

        textTexture = nullptr;
    }

    /**
     * Set text content.
     *
     * @param text Text value to set.
     */
    void SetText(const std::string &text) { SetText(text.c_str()); }

    /**
     * Set text content.
     *
     * @param text Text value to set.
     */
    void SetWrappedText(const char *text)
    {
        if (font == nullptr)
        {
            throw std::runtime_error("ERROR! Missing font reference.");

            return;
        }

        this->text = text;

        if (textTexture != nullptr)
        {
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        }

        if (textSurface != nullptr)
        {
            SDL_FreeSurface(textSurface);
            textSurface = nullptr;
        }

        textSurface =
            TTF_RenderText_Blended_Wrapped(font, this->text, color, rect.w);

        if (textSurface == nullptr)
        {
            throw std::runtime_error("ERROR! Failed to generate text surface.");

            return;
        }

        rect.w = textSurface->w;
        rect.h = textSurface->h;

        textTexture = nullptr;
    }

    /**
     * Set text content.
     *
     * @param text Text value to set.
     */
    void SetWrappedText(const std::string &text) { SetText(text.c_str()); }

    auto GetText() -> std::string { return text; }

    /**
     * Render text to the scene.
     *
     * @param renderer A structure representing rendering state.
     */
    void Render(SDL_Renderer *renderer) override
    {
        if (!CanRender())
        {
            return;
        }

        if (textTexture == nullptr && textSurface != nullptr)
        {
            textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        }

        auto transformedRect = GetTransformedRect();

        SDL_RenderCopyF(renderer, textTexture, nullptr, &transformedRect);

        RenderObject::Render(renderer);
    }
};

} // namespace Handcrank
