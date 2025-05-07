#define SDL_MAIN_HANDLED

#include "Handcrank/Handcrank.hpp"
#include "Handcrank/RectRenderObject.hpp"
#include "Handcrank/TextRenderObject.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#include "../fonts/JustMyType/JustMyType.h"

using namespace Handcrank;

auto game = std::make_unique<Game>();

const auto size = 35;

class BorderCollider : public RectRenderObject
{
  public:
    void Start() override { SetFillColor(0, MAX_G, 0, 0); }
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
    void Start() override
    {
        SetFillColor(MAX_R, MAX_G, MAX_B, MAX_ALPHA);

        SetRect(0, 0, size, size);

        const auto transformedRect = GetTransformedRect();

        maxX = game->GetWidth() - transformedRect.w;
        maxY = game->GetHeight() - transformedRect.h;

        Reset();
    }

    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        rect->x += movementSpeed * xDirection * deltaTime;
        rect->y += movementSpeed * yDirection * deltaTime;

        if (rect->x > maxX || rect->x < minX)
        {
            xDirection = -xDirection;
        }

        if (rect->y > maxY || rect->y < minY)
        {
            yDirection = -yDirection;
        }

        rect->x = std::clamp<float>(rect->x, minX, maxX);
        rect->y = std::clamp<float>(rect->y, minY, maxY);

        previousDeltaTime = deltaTime;
    }

    auto CalculateNextPosition() -> SDL_FRect
    {
        SDL_FRect tempRect = {rect->x, rect->y, rect->w, rect->h};

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

class Paddle : public RectRenderObject
{
  protected:
    int movementSpeed = 1000;

  public:
    void Start() override
    {
        SetFillColor(MAX_R, MAX_G, MAX_B, MAX_ALPHA);

        SetRect(0, 0, size, 300);

        SetPosition(0, (game->GetHeight() / 2) - (rect->h / 2));
    }
};

class LeftPaddle : public Paddle
{
  public:
    void Start() override
    {
        Paddle::Start();

        SetPosition(size, rect->y);
    }

    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        if (game->keyState[SDLK_w])
        {
            rect->y -= movementSpeed * deltaTime;
        }
        else if (game->keyState[SDLK_s])
        {
            rect->y += movementSpeed * deltaTime;
        }

        rect->y = std::clamp<float>(rect->y, 0, game->GetHeight() - rect->h);
    }
};

class RightPaddle : public Paddle
{
  public:
    void Start() override
    {
        Paddle::Start();

        SetPosition(game->GetWidth() - rect->w - size, rect->y);
    }

    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        if (game->keyState[SDLK_UP])
        {
            rect->y -= movementSpeed * deltaTime;
        }
        else if (game->keyState[SDLK_DOWN])
        {
            rect->y += movementSpeed * deltaTime;
        }

        rect->y = std::clamp<float>(rect->y, 0, game->GetHeight() - rect->h);
    }
};

class ScoreBoard : public RenderObject
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

class GameManager : public RenderObject
{
  private:
    std::shared_ptr<Ball> ball;

    std::shared_ptr<LeftPaddle> leftPaddle;
    std::shared_ptr<RightPaddle> rightPaddle;

    std::shared_ptr<BorderCollider> leftBorderCollider;
    std::shared_ptr<BorderCollider> rightBorderCollider;

    std::shared_ptr<ScoreBoard> scoreBoard;

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
        rightBorderCollider = std::make_shared<BorderCollider>();

        leftBorderCollider->SetRect(0, 0, 10, (float)game->GetHeight());
        rightBorderCollider->SetRect((float)game->GetWidth() - 10, 0, 10,
                                     (float)game->GetHeight());

        AddChildObject(leftBorderCollider);
        AddChildObject(rightBorderCollider);

        scoreBoard = std::make_shared<ScoreBoard>();

        AddChildObject(scoreBoard);
    }
    void Update(double deltaTime) override
    {
        if (!game->HasFocus())
        {
            return;
        }

        if (game->keyState[SDLK_ESCAPE])
        {
            game->Quit();
        }

        auto ballCalculatedNextRect = ball->CalculateNextPosition();

        if (SDL_HasIntersectionF(leftPaddle->GetRect().get(),
                                 &ballCalculatedNextRect) == SDL_TRUE ||
            SDL_HasIntersectionF(rightPaddle->GetRect().get(),
                                 &ballCalculatedNextRect) == SDL_TRUE)
        {
            ball->ChangeDirection();
        }

        if (SDL_HasIntersectionF(leftBorderCollider->GetRect().get(),
                                 &ballCalculatedNextRect) == SDL_TRUE)
        {
            scoreBoard->IncrementRightScore();

            ball->Reset();
        }
        else if (SDL_HasIntersectionF(rightBorderCollider->GetRect().get(),
                                      &ballCalculatedNextRect) == SDL_TRUE)
        {
            scoreBoard->IncrementLeftScore();

            ball->Reset();
        }
    }

    void Render(const std::shared_ptr<SDL_Renderer> &renderer) override
    {
        RenderObject::Render(renderer);

        SDL_SetRenderDrawColor(renderer.get(), MAX_R, MAX_G, MAX_B, MAX_ALPHA);

        const float width = 5;
        const float height = 50;

        for (float y = 0; y < 23; y += 1.5F)
        {
            SDL_FRect tempRect = {((float)game->GetWidth() / 2) - (width / 2),
                                  y * height, width, height};

            SDL_RenderFillRectF(renderer.get(), &tempRect);
        }
    }
};

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd)
#else
auto main() -> int
#endif
{
    game->SetTitle("Pong Demo");

    game->AddChildObject(std::move(std::make_unique<GameManager>()));

    return game->Run();
}
