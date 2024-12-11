#include "raylib.h"


class Player
{
private:
    /* data */
    Vector2 pos;
    float speed = 200.f;
    float radius = 10.f;

public:
    Player(Vector2 pos);
    void keyMovements(float deltaTime);
    void update(float deltaTime);
    void draw();
    ~Player();
};

