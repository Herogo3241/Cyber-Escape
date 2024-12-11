#include "Enemy.h"

Enemy::Enemy(Vector2 pos, bool isMovingVertically)
    : ray(pos, 0.f), isMovingVertically(isMovingVertically) {
    this->pos = pos;
    this->angle = 0.f;
    this->movementSpeed = (float)(GetRandomValue(50, 100)); 
    this->swayRange = (float)(GetRandomValue(30, 75));


    if (this->isMovingVertically) {
        this->movement = {0, 1}; // Initially moves down
    } else {
        this->movement = {1, 0}; // Initially moves right
    }
}



void Enemy::update(float deltaTime) {
 
    float rotationSmoothing = 5.0f; 

    // Bounce off the screen edges
    if (this->isMovingVertically) {
        if (this->pos.y < 50 || this->pos.y > GetScreenHeight() - 50) {
            this->movement.y *= -1; 
        }
    } else {
        if (this->pos.x < 50 || this->pos.x > GetScreenWidth() - 50) {
            this->movement.x *= -1; 
        }
    }


    float targetAngle;
    if (this->isMovingVertically) {
        targetAngle = this->movement.y > 0 ? 90.f : -90.f; // Looking left-right
    } else {
        targetAngle = this->movement.x > 0 ? 0.f : 180.f; // Looking up-down
    }

  
    this->angle += (targetAngle - this->angle) * deltaTime * rotationSmoothing;

 
    this->pos.x += this->movement.x * this->movementSpeed * deltaTime;
    this->pos.y += this->movement.y * this->movementSpeed * deltaTime;

  
    float swayRange = this->swayRange; 
    float swaySpeed = 2.0f;  
    float swayOffset = swayRange * sin(GetTime() * swaySpeed);

    // Cast rays with swaying effect
    for (float i = this->angle - 30 + swayOffset; i < this->angle + 30 + swayOffset; i += 0.5f) {
        this->ray.setPos(this->pos);
        this->ray.setAngle(i);
        this->ray.draw();
    }
}



void Enemy::draw() {
    // Draw the enemy as a red circle
    DrawCircle(this->pos.x, this->pos.y, 10, RED);
}

Enemy::~Enemy() {
    // Clean up resources if necessary
}
