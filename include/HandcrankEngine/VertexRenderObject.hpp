// Handcrank Engine - https://handcrankengine.com/
//
// ░█░█░█▀█░█▀█░█▀▄░█▀▀░█▀▄░█▀█░█▀█░█░█░░░█▀▀░█▀█░█▀▀░▀█▀░█▀█░█▀▀
// ░█▀█░█▀█░█░█░█░█░█░░░█▀▄░█▀█░█░█░█▀▄░░░█▀▀░█░█░█░█░░█░░█░█░█▀▀
// ░▀░▀░▀░▀░▀░▀░▀▀░░▀▀▀░▀░▀░▀░▀░▀░▀░▀░▀░░░▀▀▀░▀░▀░▀▀▀░▀▀▀░▀░▀░▀▀▀
//
// Copyright (c) Scott Doxey. All Rights Reserved. Licensed under the MIT
// License. See LICENSE in the project root for license information.

#pragma once

#include <SDL3/SDL.h>

#include "HandcrankEngine.hpp"
#include "TextureRenderObject.hpp"

namespace HandcrankEngine
{

class VertexRenderObject : public TextureRenderObject
{

  public:
    struct VertexRenderItem
    {
        SDL_FRect rect;
        SDL_FRect srcRect;
        SDL_FColor color;
    };

  protected:
    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    std::vector<VertexRenderItem> vertexRenderItems;

  public:
    using TextureRenderObject::TextureRenderObject;

    void Render(SDL_Renderer *renderer) override
    {
        SDL_RenderGeometry(game->GetRenderer(), texture, vertices.data(),
                           vertices.size(), indices.data(), indices.size());

        RenderObject::Render(renderer);
    }

    void AddVertexRenderItem(const VertexRenderItem &vertexRenderItem)
    {
        vertexRenderItems.emplace_back(vertexRenderItem);

        GenerateTextureQuad(vertices, indices, vertexRenderItem.rect,
                            vertexRenderItem.srcRect, vertexRenderItem.color,
                            textureWidth, textureHeight);
    }

    void UpdateVertexRenderItemPosition(int index, const SDL_FRect &position)
    {
        UpdateTextureQuad(vertices.data() + (index * 4), position);
    }
};

} // namespace HandcrankEngine
