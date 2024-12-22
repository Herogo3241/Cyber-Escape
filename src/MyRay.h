#include "raylib.h"
#include "raymath.h"
#include <vector>

class MyRay
{
private:
    /* data */
    Vector2 pos;
    float angle;
    float length = 150.f;
    bool hasCollided;  
    Vector2 collisionPoint;
public:
    MyRay(Vector2 pos, float angle);
    void setPos(Vector2 pos);
    void setAngle(float angle);
    void setLength(float length);
    bool checkCollision(const std::vector<std::vector<int>>& boardData);
    Vector2 getCollisionPoint() const { return collisionPoint; }
    bool getHasCollided() const { return hasCollided; }
    void draw();
    ~MyRay();
};




