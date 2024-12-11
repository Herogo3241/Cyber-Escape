#include "Enemy.h"

Enemy::Enemy(Vector2 pos, bool isMovingVertically)
    : ray(pos, 0.f), isMovingVertically(isMovingVertically) {
    this->pos = pos;
    this->angle = 0.f;

    // Initialize movement based on the specified direction
    if (this->isMovingVertically) {
        this->movement = {0, 1}; // Initially moves down
    } else {
        this->movement = {1, 0}; // Initially moves right
    }
}

void Enemy::update(float deltaTime) {
    // Define smoothing parameters for rotation
    float rotationSmoothing = 5.0f; // Higher values make rotation slower

    // Check for boundary conditions and reverse direction
    if (this->isMovingVertically) {
        if (this->pos.y < 50 || this->pos.y > GetScreenHeight() - 50) {
            this->movement.y *= -1; // Reverse vertical direction
        }
    } else {
        if (this->pos.x < 50 || this->pos.x > GetScreenWidth() - 50) {
            this->movement.x *= -1; // Reverse horizontal direction
        }
    }

    // Calculate target angle based on movement direction
    float targetAngle;
    if (this->isMovingVertically) {
        targetAngle = this->movement.y > 0 ? 90.f : -90.f; // Looking left-right
    } else {
        targetAngle = this->movement.x > 0 ? 0.f : 180.f; // Looking up-down
    }

    // Smoothly interpolate current angle toward the target angle
    this->angle += (targetAngle - this->angle) * deltaTime * rotationSmoothing;

    // Update position based on movement vector
    this->pos.x += this->movement.x * this->movementSpeed * deltaTime;
    this->pos.y += this->movement.y * this->movementSpeed * deltaTime;

    // Add swaying effect to the rays
    float swayRange = 75.0f; // Max sway angle in degrees
    float swaySpeed = 2.0f;  // Speed of sway (adjust as needed)
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
