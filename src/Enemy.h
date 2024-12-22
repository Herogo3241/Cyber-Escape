#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "MyRay.h"
#include <vector>

class Enemy {
private:
    Vector2 pos;
    float angle;
    Vector2 movement;
    float movementSpeed;
    float swayRange;
    bool isMovingVertically;
    MyRay ray;
    const std::vector<std::vector<int>>* boardData;
    float detectionRadius;
    float trackingDuration;
    float maxTrackingTime;
    bool isTrackingPlayer;
    float fullRotationTimer;

public:
    Enemy(Vector2 pos, bool isMovingVertically, const std::vector<std::vector<int>>& boardData);
    bool update(float deltaTime, Vector2 playerPos);  // Removed Enemy:: qualification
    void draw();
    ~Enemy();
};

#endif