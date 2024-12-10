#include "src/raylib.h"
#include "src/raymath.h"
#include <math.h>
#include "src/Enemy.h"

#define HEIGHT 600
#define WIDTH 800


int main() {

    InitWindow(WIDTH, HEIGHT, "Secret Santa Jam");


    
    Enemy enemy = Enemy({WIDTH / 2, HEIGHT / 2});
    
    

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