#include "raylib.h"
#include "raymath.h"

class MyRay
{
private:
    /* data */
    Vector2 pos;
    float angle;
    float length = 100.f;
public:
    MyRay(Vector2 pos, float angle);
    void setPos(Vector2 pos);
    void setAngle(float angle);
    void setLength(float length);
    void draw();
    ~MyRay();
};




