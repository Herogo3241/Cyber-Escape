#include "src/raylib.h"
#include "src/raymath.h"
#include <math.h>
#include "src/Enemy.h"
#include "src/Player.h"
#include "src/Monitor.h"
#include <vector>
#include <iostream>
#include "src/Board.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else // PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

#define MAX_POSTPRO_SHADERS 12

int main()
{

    // Get the current monitor
    int monitor = GetCurrentMonitor();
    float HEIGHT = GetMonitorHeight(monitor);
    float WIDTH = GetMonitorWidth(monitor);

    // float HEIGHT = 600.f;
    // float WIDTH = 800.f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

    // Create the window
    InitWindow(WIDTH, HEIGHT, "Secret Santa Jam");

    SetWindowState(FLAG_FULLSCREEN_MODE);
    std::ifstream file("./assets/levels/level1.csv"); 
    if (!file)
    {
        std::cerr << "Failed to open the file!" << std::endl;
        return 1;
    }
    Board board(file);

    WIDTH = GetScreenWidth();
    HEIGHT = GetScreenHeight();

    std::cout << WIDTH << " " << HEIGHT;

    SetTargetFPS(60);

    Player player({WIDTH / 2, HEIGHT / 2});
    Monitor level1mon("$$ Level 1 $$", {WIDTH / 2, HEIGHT / 2});

    // Enemies creation
    int count = 6;
    std::vector<Enemy> enemies;
    for (int i = 0; i < count; i++)
    {
        enemies.emplace_back(Vector2{(float)GetRandomValue(50, WIDTH - 50),
                                     (float)GetRandomValue(50, HEIGHT - 50)},
                             GetRandomValue(0, 1), board.boardData);
    }

    bool isFullscreen = true;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        board.draw();
        player.update(GetFrameTime());
        for (auto &enemy : enemies)
        {
            enemy.update(GetFrameTime());
        }

        if (IsKeyPressed(KEY_E))
            level1mon.toggleOpen();
        if (IsKeyPressed(KEY_ESCAPE))
            level1mon.toggleClose();

        level1mon.update(GetFrameTime());
        
        
        
        for (auto &enemy : enemies)
        {
            enemy.draw();
        }
        player.draw();
        level1mon.draw();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}