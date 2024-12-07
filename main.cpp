#include "raylib.h"

struct Player
{
  int x, y;
  int speed;
  Color color;
  void move()
  {
    if (IsKeyDown(KEY_D))
    {
      x += speed;
    }
    if (IsKeyDown(KEY_A))
    {
      x -= speed;
    }
    if (IsKeyDown(KEY_W))
    {
      y -= speed;
    }
    if (IsKeyDown(KEY_S))
    {
      y += speed;
    }
  }
  void draw()
  {
    DrawRectangle(x, y, 50, 50, color);
  }

};

int main()
{
  const int screenWidth = 1080;
  const int screenHeight = 960;
  InitWindow(screenWidth, screenHeight, "First Game");
  SetTargetFPS(60);

  Player player;
  player.x = 400;
  player.y = 400;
  player.speed = 5;
  player.color = RED;

  while (!WindowShouldClose())
  {

    player.move();


    BeginDrawing();
    ClearBackground(RAYWHITE);
    // DrawText("It works!", 20, 20, 20, BLACK);
    player.draw();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}