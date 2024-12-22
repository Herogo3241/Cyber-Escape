#include "raylib.h"
#include "raymath.h"
#include "GameManager.h"




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

    // Create the window and sound device
    InitWindow(WIDTH, HEIGHT, "Secret Santa Jam");
    InitAudioDevice();

    SetWindowState(FLAG_FULLSCREEN_MODE);
    

    WIDTH = GetScreenWidth();
    HEIGHT = GetScreenHeight();

    std::cout << WIDTH << " " << HEIGHT;
    Sound sound = LoadSound("./assets/ambient.wav");
    GameManager gameManager(WIDTH, HEIGHT);

    SetTargetFPS(60);
    PlaySound(sound);

                          

    bool isFullscreen = true;

    while (!WindowShouldClose())
    {


        if(!IsSoundPlaying(sound)){
            PlaySound(sound);
        }
        BeginDrawing();
        ClearBackground(BLACK);
        

        
        gameManager.update(GetFrameTime());
        gameManager.draw();
        
        EndDrawing();
    }

    
    UnloadSound(sound);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}