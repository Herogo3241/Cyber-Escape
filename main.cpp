#include "src/raylib.h"
#include "src/raymath.h"
#include <math.h>
#include "src/Enemy.h"


#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX_POSTPRO_SHADERS         12

#define HEIGHT 600
#define WIDTH 800


int main() {

    InitWindow(WIDTH, HEIGHT, "Secret Santa Jam");


    
    Enemy enemy = Enemy({WIDTH / 2, HEIGHT / 2}, false);
    
    

    while (!WindowShouldClose()) {
        enemy.update(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        enemy.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
    
}