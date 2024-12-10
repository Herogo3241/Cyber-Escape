#include "MyRay.h"
#include "raylib.h"
class Enemy
{
private:
    MyRay ray;
    Vector2 pos;
    float angle;
public:
    Enemy(Vector2 pos);
    void update(float deltaTime);
    void draw();
    ~Enemy();
};

