#include "raylib.h"

struct Box
{
    int x, y;
    Color color;

    void draw()
    {
        DrawRectangle(x, y, 30, 30, color);
    }
};

struct Player
{
    int x, y;
    int speed;
    Color color;

    void move(Box boxes[], int numBoxes)
    {
        // Create a copy of the player's current position
        int newX = x;
        int newY = y;

        // Update position based on key press
        if (IsKeyDown(KEY_D))
        {
            newX += speed;
        }
        if (IsKeyDown(KEY_A))
        {
            newX -= speed;
        }
        if (IsKeyDown(KEY_W))
        {
            newY -= speed;
        }
        if (IsKeyDown(KEY_S))
        {
            newY += speed;
        }

        // Check collision with all boxes
        Rectangle newPlayerRect = {(float)newX, (float)newY, 30, 30};
        bool collision = false;

        for (int i = 0; i < numBoxes; i++)
        {
            Rectangle boxRect = {(float)boxes[i].x, (float)boxes[i].y, 30, 30};
            if (CheckCollisionRecs(newPlayerRect, boxRect))
            {
                collision = true;
                break;
            }
        }

        // If no collision, update the player's position
        if (!collision)
        {
            x = newX;
            y = newY;
        }

        // Ensure the player stays within screen boundaries
        if (x < 0 || x > GetScreenWidth() - 30 || y < 0 || y > GetScreenHeight() - 30)
        {
            x = x < 0 ? 0 : x > GetScreenWidth() - 30 ? GetScreenWidth() - 30
                                                      : x;
            y = y < 0 ? 0 : y > GetScreenHeight() - 30 ? GetScreenHeight() - 30
                                                       : y;
        }
    }

    void draw()
    {
        DrawRectangle(x, y, 30, 30, color);
    }
};

const char *text = "Press WASD to move the square";

int main()
{
    int display = GetCurrentMonitor();
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "First Game");

    SetTargetFPS(60);

    Player player;
    player.x = screenWidth / 2 - 25;
    player.y = screenHeight / 2 - 25;
    player.speed = 5;
    player.color = RED;

    const int numBoxes = 10;
    Box boxes[numBoxes];

    // Randomly place boxes
    for (int i = 0; i < numBoxes; i++)
    {
        boxes[i].x = GetRandomValue(0, screenWidth - 50);
        boxes[i].y = GetRandomValue(0, screenHeight - 50);
        boxes[i].color = (Color){(unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), 255};
    }

    while (!WindowShouldClose())
    {
        player.move(boxes, numBoxes);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(text, screenWidth / 2 - MeasureText(text, 20) / 2, 20, 20, BLACK);

        for (int i = 0; i < numBoxes; i++)
        {
            boxes[i].draw();
        }

        if (IsKeyDown(KEY_Q))
        {
            CloseWindow();
            return 0;
        }
        if (IsKeyDown(KEY_R))
        {
            for (int i = 0; i < numBoxes; i++)
            {
                boxes[i].x = GetRandomValue(0, screenWidth - 50);
                boxes[i].y = GetRandomValue(0, screenHeight - 50);
                boxes[i].color = (Color){(unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), (unsigned char)(GetRandomValue(20, 255)), 255};
            }
        }

        player.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
