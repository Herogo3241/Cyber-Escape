#include "MyRay.h"
#include "raylib.h"
class Enemy
{
private:
    MyRay ray;
    Vector2 pos;
    float angle;
    Vector2 movement;
    float movementSpeed;
    float swayRange;
    bool isMovingVertically;
    const std::vector<std::vector<int>>* boardData;
public:
    Enemy(Vector2 pos, bool isMovingVertically, const std::vector<std::vector<int>>& boardData);
    void update(float deltaTime);
    void draw();
    ~Enemy();
};

