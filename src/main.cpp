
#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    const int screenWidth = 1000;
    const int screenHeight = 900;
    const float paddleWidth = 125.0f;
    const float paddleHeight = 25.0f;
    const float ballRadius = 10.0f;
    const float maxTurbo = 100.0f;
    const int winningScore = 10;

    // Paddle positions
    float rec_oneX = 375.0f; // Player 1: bottom paddle
    float rec_twoX = 375.0f; // Player 2: top paddle

    // Movement speeds (pixels per second)
    const float speedrec = 430.0f;
    const float turbo = 700.0f;

    // Turbo meters
    float turbo_meter_one = maxTurbo;
    float turbo_meter_two = maxTurbo;
    const float turboDrainRate = 35.0f;
    const float turboRegenRate = 20.0f;

    // Ball position and velocity
    float ballX = 500.0f;
    float ballY = 450.0f;
    const float speedball = 400.0f;
    float ballspeedX = 0.0f;
    float ballspeedY = speedball;

    // Scores and game state
    int pointsplayerone = 0;
    int pointsplayertwo = 0;
    bool gameStarted = false;
    bool gameOver = false;
    int winner = 0; // 1 = Player 1, 2 = Player 2

    InitWindow(screenWidth, screenHeight, "Ping Pong");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // Start the game from the title screen.
        if (!gameStarted && IsKeyPressed(KEY_ENTER))
        {
            gameStarted = true;
        }

        // Restart after someone wins.
        if (gameOver && IsKeyPressed(KEY_ENTER))
        {
            pointsplayerone = 0;
            pointsplayertwo = 0;

            rec_oneX = (screenWidth - paddleWidth) / 2.0f;
            rec_twoX = (screenWidth - paddleWidth) / 2.0f;

            turbo_meter_one = maxTurbo;
            turbo_meter_two = maxTurbo;

            ballX = screenWidth / 2.0f;
            ballY = screenHeight / 2.0f;
            ballspeedX = 0.0f;
            ballspeedY = (rand() % 2 == 0) ? speedball : -speedball;

            winner = 0;
            gameOver = false;
            gameStarted = true;
        }

        // Update gameplay only while the game is active.
        if (gameStarted && !gameOver)
        {
            // PLAYER 1: A/D to move, W + A/D for turbo.
            bool playerOneTurboUsed = false;

            if (IsKeyDown(KEY_W) && turbo_meter_one > 0.0f && IsKeyDown(KEY_A))
            {
                rec_oneX -= turbo * dt;
                playerOneTurboUsed = true;
            }
            else if (IsKeyDown(KEY_W) && turbo_meter_one > 0.0f && IsKeyDown(KEY_D))
            {
                rec_oneX += turbo * dt;
                playerOneTurboUsed = true;
            }
            else if (IsKeyDown(KEY_A))
            {
                rec_oneX -= speedrec * dt;
            }
            else if (IsKeyDown(KEY_D))
            {
                rec_oneX += speedrec * dt;
            }

            if (playerOneTurboUsed)
                turbo_meter_one -= turboDrainRate * dt;
            else
                turbo_meter_one += turboRegenRate * dt;

            if (turbo_meter_one < 0.0f) turbo_meter_one = 0.0f;
            if (turbo_meter_one > maxTurbo) turbo_meter_one = maxTurbo;

            // PLAYER 2: arrow keys to move, Up + Left/Right for turbo.
            bool playerTwoTurboUsed = false;

            if (IsKeyDown(KEY_UP) && turbo_meter_two > 0.0f && IsKeyDown(KEY_LEFT))
            {
                rec_twoX -= turbo * dt;
                playerTwoTurboUsed = true;
            }
            else if (IsKeyDown(KEY_UP) && turbo_meter_two > 0.0f && IsKeyDown(KEY_RIGHT))
            {
                rec_twoX += turbo * dt;
                playerTwoTurboUsed = true;
            }
            else if (IsKeyDown(KEY_LEFT))
            {
                rec_twoX -= speedrec * dt;
            }
            else if (IsKeyDown(KEY_RIGHT))
            {
                rec_twoX += speedrec * dt;
            }

            if (playerTwoTurboUsed)
                turbo_meter_two -= turboDrainRate * dt;
            else
                turbo_meter_two += turboRegenRate * dt;

            if (turbo_meter_two < 0.0f) turbo_meter_two = 0.0f;
            if (turbo_meter_two > maxTurbo) turbo_meter_two = maxTurbo;

            // Keep paddles inside the window.
            rec_oneX = std::clamp(rec_oneX, 0.0f, screenWidth - paddleWidth);
            rec_twoX = std::clamp(rec_twoX, 0.0f, screenWidth - paddleWidth);

            Rectangle Playerone = {rec_oneX, 775.0f, paddleWidth, paddleHeight};
            Rectangle Playertwo = {rec_twoX, 125.0f, paddleWidth, paddleHeight};

            // Move the ball.
            ballX += ballspeedX * dt;
            ballY += ballspeedY * dt;

            // Bottom paddle collision.
            if (CheckCollisionCircleRec({ballX, ballY}, ballRadius, Playerone) &&
                ballspeedY > 0.0f)
            {
                float hitPosition = (ballX - (rec_oneX + paddleWidth / 2.0f)) /
                                    (paddleWidth / 2.0f);
                hitPosition = std::clamp(hitPosition, -1.0f, 1.0f);

                float angle = hitPosition * 60.0f * PI / 180.0f;
                ballspeedX = speedball * sinf(angle);
                ballspeedY = -speedball * cosf(angle);
                ballY = 775.0f - ballRadius;
            }

            // Top paddle collision.
            if (CheckCollisionCircleRec({ballX, ballY}, ballRadius, Playertwo) &&
                ballspeedY < 0.0f)
            {
                float hitPosition = (ballX - (rec_twoX + paddleWidth / 2.0f)) /
                                    (paddleWidth / 2.0f);
                hitPosition = std::clamp(hitPosition, -1.0f, 1.0f);

                float angle = hitPosition * 60.0f * PI / 180.0f;
                ballspeedX = speedball * sinf(angle);
                ballspeedY = speedball * cosf(angle);
                ballY = 125.0f + paddleHeight + ballRadius;
            }

            // Left and right wall collisions.
            if (ballX <= ballRadius)
            {
                ballX = ballRadius;
                ballspeedX = std::abs(ballspeedX);
            }
            else if (ballX >= screenWidth - ballRadius)
            {
                ballX = screenWidth - ballRadius;
                ballspeedX = -std::abs(ballspeedX);
            }

            // Scoring: if the ball passes the top, Player 1 scores;
            // if it passes the bottom, Player 2 scores.
            if (ballY < 65.0f)
            {
                pointsplayerone++;
            }
            else if (ballY > 835.0f)
            {
                pointsplayertwo++;
            }
            else
            {
                // No point was scored this frame.
            }

            // If the ball passed either end, reset it for the next serve.
            if (ballY < 65.0f || ballY > 835.0f)
            {
                ballX = screenWidth / 2.0f;
                ballY = screenHeight / 2.0f;
                ballspeedX = 0.0f;
                int direction = (rand() % 2 == 0) ? 1 : -1;
                ballspeedY = direction * speedball;

                // Check for a winner after awarding the point.
                if (pointsplayerone >= winningScore)
                {
                    winner = 1;
                    gameOver = true;
                }
                else if (pointsplayertwo >= winningScore)
                {
                    winner = 2;
                    gameOver = true;
                }
            }
        }

        // DRAW EVERYTHING. Every BeginDrawing() must have one EndDrawing().
        BeginDrawing();
        ClearBackground(BLACK);

        if (!gameStarted)
        {
            DrawText("PING PONG", 350, 350, 60, YELLOW);
            DrawText("PRESS ENTER TO START", 315, 440, 30, WHITE);
        }
        else if (gameOver)
        {
            if (winner == 1)
                DrawText("PLAYER ONE WINS!", 315, 390, 35, PINK);
            else if (winner == 2)
                DrawText("PLAYER TWO WINS!", 315, 390, 35, PINK);

            DrawText("PRESS ENTER TO PLAY AGAIN", 290, 450, 28, WHITE);
        }
        else
        {
            // Top HUD: Player 2.
            DrawText("PLAYER 2", 50, 15, 24, MAGENTA);
            DrawText(TextFormat("SCORE: %i", pointsplayertwo),
                     50, 42, 22, WHITE);
            DrawText("TURBO", 750, 20, 20, WHITE);
            DrawRectangle(750, 48, 200, 14, DARKGRAY);
            DrawRectangle(750, 48,
                          static_cast<int>(2.0f * turbo_meter_two),
                          14, GREEN);

            // Bottom HUD: Player 1.
            DrawText("PLAYER 1", 50, 845, 24, PINK);
            DrawText(TextFormat("SCORE: %i", pointsplayerone),
                     50, 872, 22, WHITE);
            DrawText("TURBO", 750, 850, 20, WHITE);
            DrawRectangle(750, 878, 200, 14, DARKGRAY);
            DrawRectangle(750, 878,
                          static_cast<int>(2.0f * turbo_meter_one),
                          14, GREEN);

            // Playable area boundaries.
            DrawLine(0, 75, screenWidth, 75, DARKGRAY);
            DrawLine(0, 825, screenWidth, 825, DARKGRAY);

            // Paddles and ball.
            DrawRectangle(static_cast<int>(rec_oneX), 775, 125, 25, PINK);
            DrawRectangle(static_cast<int>(rec_twoX), 125, 125, 25, MAGENTA);
            DrawCircle(static_cast<int>(ballX), static_cast<int>(ballY),
                       static_cast<int>(ballRadius), YELLOW);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
