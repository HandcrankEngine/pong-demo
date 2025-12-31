#include "HandcrankEngine/HandcrankEngine.hpp"
#include "HandcrankEngine/RectRenderObject.hpp"
#include "HandcrankEngine/TextRenderObject.hpp"

#include "../fonts/JustMyType/JustMyType.h"

using namespace HandcrankEngine;

auto game = std::make_unique<Game>();

const auto size = 35;

class Scoreboard : public RenderObject
{
  private:
    const int fontSize = 150;

    std::shared_ptr<TextRenderObject> leftScoreText;
    std::shared_ptr<TextRenderObject> rightScoreText;

    int leftScore = 0;
    int rightScore = 0;

  public:
    void Start() override
    {
        leftScoreText = std::make_shared<TextRenderObject>();
        rightScoreText = std::make_shared<TextRenderObject>();

        leftScoreText->LoadFontRW(fonts_JustMyType_JustMyType_ttf,
                                  fonts_JustMyType_JustMyType_ttf_len,
                                  fontSize);
        leftScoreText->SetColor({MAX_R, MAX_G, MAX_B, MAX_ALPHA});
        leftScoreText->SetRect(
            {((float)game->GetWidth() / 2) - ((float)fontSize * 1.5F),
             (float)fontSize});
        leftScoreText->SetText(std::to_string(leftScore));

        AddChildObject(leftScoreText);

        rightScoreText->LoadFontRW(fonts_JustMyType_JustMyType_ttf,
                                   fonts_JustMyType_JustMyType_ttf_len,
                                   fontSize);
        rightScoreText->SetColor({MAX_R, MAX_G, MAX_B, MAX_ALPHA});
        rightScoreText->SetRect(
            {((float)game->GetWidth() / 2) + (float)fontSize, (float)fontSize});
        rightScoreText->SetText(std::to_string(rightScore));

        AddChildObject(rightScoreText);
    }

    void IncrementLeftScore()
    {
        leftScore += 1;

        if (leftScoreText != nullptr)
        {
            leftScoreText->SetText(std::to_string(leftScore));
        }
    }

    void IncrementRightScore()
    {
        rightScore += 1;

        if (rightScoreText != nullptr)
        {
            rightScoreText->SetText(std::to_string(rightScore));
        }
    }
};

class Ball : public RectRenderObject
{
  private:
    const int startingMovementSpeed = 400;
    const int movementSpeedStep = 25;

    const int minX = 0;
    const int minY = 0;

    int maxX = 0;
    int maxY = 0;

    int xDirection = 1;
    int yDirection = -1;

    int movementSpeed = startingMovementSpeed;

    double previousDeltaTime;

  public:
    using RectRenderObject::RectRenderObject;

    void Start() override
    {
        SetFillColor(MAX_R, MAX_G, MAX_B, MAX_ALPHA);

        SetRect(0, 0, size, size);

        const auto transformedRect = GetTransformedRect();

        maxX = game->GetWidth() - transformedRect.w;
        maxY = game->GetHeight() - transformedRect.h;

        EnableCollider();

        Reset();
    }

    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        auto x = rect.x + (movementSpeed * xDirection * deltaTime);
        auto y = rect.y + (movementSpeed * yDirection * deltaTime);

        if (x > maxX || x < minX)
        {
            xDirection = -xDirection;
        }

        if (y > maxY || y < minY)
        {
            yDirection = -yDirection;
        }

        x = std::clamp<float>(x, minX, maxX);
        y = std::clamp<float>(y, minY, maxY);

        SetPosition(x, y);

        previousDeltaTime = deltaTime;
    }

    auto CalculateNextPosition() -> SDL_FRect
    {
        SDL_FRect tempRect = {rect.x, rect.y, rect.w, rect.h};

        tempRect.x += movementSpeed * xDirection * previousDeltaTime;
        tempRect.y += movementSpeed * yDirection * previousDeltaTime;

        tempRect.x = std::clamp<float>(tempRect.x, minX, maxX);
        tempRect.y = std::clamp<float>(tempRect.y, minY, maxY);

        return tempRect;
    }

    void ChangeDirection()
    {
        xDirection = -xDirection;

        movementSpeed += movementSpeedStep;
    }

    void Reset()
    {
        xDirection = -xDirection;

        SetPosition(maxX / 2, maxY / 2);

        movementSpeed = startingMovementSpeed;
    }
};

class BorderCollider : public RectRenderObject
{
  public:
    using RectRenderObject::RectRenderObject;

    std::string tag;

    void Start() override
    {
        SetFillColor(0, MAX_G, 0, 0);

        EnableCollider();
    }

    void OnCollision(const std::shared_ptr<RenderObject> &other) override
    {
        if (auto ball = std::dynamic_pointer_cast<Ball>(other))
        {
            ball->Reset();

            auto scoreboard = game->GetChildByType<Scoreboard>(true);

            if (scoreboard != nullptr)
            {
                if (tag == "left")
                {
                    scoreboard->IncrementRightScore();
                }
                else if (tag == "right")
                {
                    scoreboard->IncrementLeftScore();
                }
            }
        }
    }
};

class Paddle : public RectRenderObject
{
  protected:
    int movementSpeed = 1000;

  public:
    using RectRenderObject::RectRenderObject;

    void Start() override
    {
        SetFillColor(MAX_R, MAX_G, MAX_B, MAX_ALPHA);

        SetRect(0, 0, size, 300);

        SetPosition(0, (game->GetHeight() / 2) - (rect.h / 2));

        EnableCollider();
    }

    void OnCollision(const std::shared_ptr<RenderObject> &other) override
    {
        if (auto ball = std::dynamic_pointer_cast<Ball>(other))
        {
            ball->ChangeDirection();
        }
    }
};

class LeftPaddle : public Paddle
{
  public:
    using Paddle::Paddle;

    void Start() override
    {
        Paddle::Start();

        SetPosition(size, rect.y);
    }

    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        auto y = rect.y;

        if (game->IsKeyDown(SDLK_w))
        {
            y -= movementSpeed * deltaTime;
        }
        else if (game->IsKeyDown(SDLK_s))
        {
            y += movementSpeed * deltaTime;
        }

        y = std::clamp<float>(y, 0, game->GetHeight() - rect.h);

        SetPosition(rect.x, y);
    }
};

class RightPaddle : public Paddle
{
  public:
    using Paddle::Paddle;

    void Start() override
    {
        Paddle::Start();

        SetPosition(game->GetWidth() - rect.w - size, rect.y);
    }

    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        auto y = rect.y;

        if (game->IsKeyDown(SDLK_UP))
        {
            y -= movementSpeed * deltaTime;
        }
        else if (game->IsKeyDown(SDLK_DOWN))
        {
            y += movementSpeed * deltaTime;
        }

        y = std::clamp<float>(y, 0, game->GetHeight() - rect.h);

        SetPosition(rect.x, y);
    }
};

class GameManager : public RenderObject
{
  private:
    std::shared_ptr<Ball> ball;

    std::shared_ptr<LeftPaddle> leftPaddle;
    std::shared_ptr<RightPaddle> rightPaddle;

    std::shared_ptr<BorderCollider> leftBorderCollider;
    std::shared_ptr<BorderCollider> rightBorderCollider;

    std::shared_ptr<Scoreboard> scoreboard;

  public:
    void Start() override
    {
        ball = std::make_shared<Ball>();

        AddChildObject(ball);

        leftPaddle = std::make_shared<LeftPaddle>();
        rightPaddle = std::make_shared<RightPaddle>();

        AddChildObject(leftPaddle);
        AddChildObject(rightPaddle);

        leftBorderCollider = std::make_shared<BorderCollider>();
        leftBorderCollider->tag = "left";

        rightBorderCollider = std::make_shared<BorderCollider>();
        rightBorderCollider->tag = "right";

        leftBorderCollider->SetRect(0, 0, 10, (float)game->GetHeight());
        rightBorderCollider->SetRect((float)game->GetWidth() - 10, 0, 10,
                                     (float)game->GetHeight());

        AddChildObject(leftBorderCollider);
        AddChildObject(rightBorderCollider);

        scoreboard = std::make_shared<Scoreboard>();

        AddChildObject(scoreboard);
    }

    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        if (game->IsKeyDown(SDLK_ESCAPE))
        {
            game->Quit();
        }
    }

    void Render(SDL_Renderer *renderer) override
    {
        RenderObject::Render(renderer);

        SDL_SetRenderDrawColor(renderer, MAX_R, MAX_G, MAX_B, MAX_ALPHA);

        const float width = 5;
        const float height = 50;

        for (float y = 0; y < 23; y += 1.5F)
        {
            SDL_FRect tempRect = {((float)game->GetWidth() / 2) - (width / 2),
                                  y * height, width, height};

            SDL_RenderFillRectF(renderer, &tempRect);
        }
    }
};

auto main(int argc, char *argv[]) -> int
{
    game->SetTitle("Pong Demo");

    game->AddChildObject(std::move(std::make_unique<GameManager>()));

    return game->Run();
}
