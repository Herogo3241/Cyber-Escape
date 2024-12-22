#include "Enemy.h"
#include <iostream> // debugging

Enemy::Enemy(Vector2 pos, bool isMovingVertically, const std::vector<std::vector<int>>& boardData)
    : ray(pos, 0.f), isMovingVertically(isMovingVertically) {
    this->pos = pos;
    this->angle = 0.f;
    this->movementSpeed = (float)(GetRandomValue(50, 100));
    this->swayRange = (float)(GetRandomValue(30, 75));
    this->boardData = &boardData;
    this->detectionRadius = 125.0f;  // Range to detect player
    this->trackingDuration = 0.0f;   // Time spent tracking player
    this->maxTrackingTime = 3.0f;    // Maximum time to track player before resuming patrol
    this->isTrackingPlayer = false;
    this->fullRotationTimer = 0.0f;

    if (this->isMovingVertically) {
        this->movement = {0, 1}; // Initially moves down
    } else {
        this->movement = {1, 0}; // Initially moves right
    }
}

bool Enemy::update(float deltaTime, Vector2 playerPos) {
    float rotationSmoothing = 5.0f;

    // Check board boundaries (assuming boardData contains the grid)
    int gridX = static_cast<int>(pos.x / 64); // Assuming 32 is tile size
    int gridY = static_cast<int>(pos.y / 64);
    
    // Ensure we're not at a -1 tile
    if (boardData && gridX >= 0 && gridY >= 0 && 
        gridX < boardData->at(0).size() && gridY < boardData->size()) {
        if ((*boardData)[gridY][gridX] == -1) {
            // Move away from -1 tile
            pos.x -= movement.x * movementSpeed * deltaTime * 2;
            pos.y -= movement.y * movementSpeed * deltaTime * 2;
            // Reverse direction
            movement.x *= -1;
            movement.y *= -1;
            return false;
        }
    }

    // Check if player is within detection radius
    float distToPlayer = Vector2Distance(pos, playerPos);
    if (distToPlayer < detectionRadius) {
        if (!isTrackingPlayer) {
            isTrackingPlayer = true;
            trackingDuration = 0.0f;
            fullRotationTimer = 0.0f;
        }
    }

    if (isTrackingPlayer) {
        trackingDuration += deltaTime;
        fullRotationTimer += deltaTime;
        
        // Do a full 360-degree rotation first
        if (fullRotationTimer < 1.0f) {  // 1 second rotation
            angle += 360.0f * deltaTime;
        } else {
            // After rotation, face the player
            float angleToPlayer = atan2f(playerPos.y - pos.y, playerPos.x - pos.x) * RAD2DEG;
            angle += (angleToPlayer - angle) * deltaTime * rotationSmoothing;
        }

        // Stay in position while tracking
        movement = {0, 0};

        // Reset to normal patrolling after max tracking time
        if (trackingDuration > maxTrackingTime) {
            isTrackingPlayer = false;
            if (this->isMovingVertically) {
                this->movement = {0, 1};
            } else {
                this->movement = {1, 0};
            }
        }


    } else {
        // Normal patrol behavior
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
            targetAngle = this->movement.y > 0 ? 90.f : -90.f;
        } else {
            targetAngle = this->movement.x > 0 ? 0.f : 180.f;
        }

        this->angle += (targetAngle - this->angle) * deltaTime * rotationSmoothing;
    }

    // Update position
    this->pos.x += this->movement.x * this->movementSpeed * deltaTime;
    this->pos.y += this->movement.y * this->movementSpeed * deltaTime;

    // Ray casting with appropriate sway based on state
    float swayRange = this->swayRange;
    float swaySpeed = isTrackingPlayer ? 6.f : 3.f;  // Faster sway when tracking
    float swayOffset;
    
    if (isTrackingPlayer && fullRotationTimer < 1.0f) {
        // Full 360-degree ray casting during initial detection
        for (float i = 0; i < 360; i += 0.5f) {
            this->ray.setPos(this->pos);
            this->ray.setAngle(i);
            this->ray.setLength(200.0f);
            if (boardData) {
                this->ray.checkCollision(*boardData);
            }
            this->ray.draw();
        }
        return true;
    } else {
        // Normal ray casting with sway
        swayOffset = swayRange * sin(GetTime() * swaySpeed);
        for (float i = this->angle - 30 + swayOffset; i < this->angle + 30 + swayOffset; i += 0.5f) {
            this->ray.setPos(this->pos);
            this->ray.setAngle(i);
            this->ray.setLength(200.0f);
            if (boardData) {
                this->ray.checkCollision(*boardData);
            }
            this->ray.draw();
        }
    }


    return false;
}

void Enemy::draw() {
    const float time = GetTime();
    const Color CYBER_RED = {255, 30, 60, 255};
    const Color DARK_RED = {120, 0, 20, 255};
    const float baseRadius = 20.0f;
    
    // Draw threat indicator ring
    float warningRadius = baseRadius * 1.4f + sinf(time * 5.0f) * 2.0f;
    DrawCircleLines(pos.x, pos.y, warningRadius, 
                   Fade(CYBER_RED, 0.3f + sinf(time * 3.0f) * 0.2f));
    
    // Draw main body
    DrawCircle(pos.x, pos.y, baseRadius, DARK_RED);
    
    // Draw robotic core with spinning effect
    const int NUM_SEGMENTS = 4;
    for(int i = 0; i < NUM_SEGMENTS; i++) {
        float angle = time * 2.0f + (i * (2 * PI / NUM_SEGMENTS));
        float nextAngle = time * 2.0f + ((i + 1) * (2 * PI / NUM_SEGMENTS));
        
        Vector2 start = {
            pos.x + cosf(angle) * (baseRadius * 0.7f),
            pos.y + sinf(angle) * (baseRadius * 0.7f)
        };
        Vector2 end = {
            pos.x + cosf(nextAngle) * (baseRadius * 0.7f),
            pos.y + sinf(nextAngle) * (baseRadius * 0.7f)
        };
        
        DrawLineEx(start, end, 2.0f, CYBER_RED);
    }
    
    // Draw targeting reticle
    for(int i = 0; i < 4; i++) {
        float angle = (i * PI / 2.0f) + time * 1.5f;
        float lineLength = baseRadius * 0.8f;
        
        Vector2 start = {
            pos.x + cosf(angle) * (baseRadius * 0.3f),
            pos.y + sinf(angle) * (baseRadius * 0.3f)
        };
        Vector2 end = {
            pos.x + cosf(angle) * lineLength,
            pos.y + sinf(angle) * lineLength
        };
        
        DrawLineEx(start, end, 1.0f, 
                  Fade(CYBER_RED, 0.7f + sinf(time * 4.0f + i) * 0.3f));
    }
    
    // Draw scanning sensor effect
    float scanAngle = time * 4.0f;
    DrawLineEx(
        (Vector2){pos.x, pos.y},
        (Vector2){
            pos.x + cosf(scanAngle) * baseRadius,
            pos.y + sinf(scanAngle) * baseRadius
        },
        1.0f,
        Fade(CYBER_RED, 0.8f)
    );
    
    // Draw warning triangles
    const int NUM_WARNINGS = 3;
    for(int i = 0; i < NUM_WARNINGS; i++) {
        float triAngle = time * -3.0f + (i * (2 * PI / NUM_WARNINGS));
        float triDist = baseRadius * 1.2f;
        float triSize = baseRadius * 0.3f;
        
        Vector2 triPos = {
            pos.x + cosf(triAngle) * triDist,
            pos.y + sinf(triAngle) * triDist
        };
        
        Vector2 point1 = {
            triPos.x,
            triPos.y - triSize
        };
        Vector2 point2 = {
            triPos.x - triSize,
            triPos.y + triSize
        };
        Vector2 point3 = {
            triPos.x + triSize,
            triPos.y + triSize
        };
        
        DrawTriangle(point1, point2, point3, 
                    Fade(CYBER_RED, 0.3f + sinf(time * 6.0f + i) * 0.2f));
    }
    
    // Draw center eye/sensor
    float innerPulse = 3.0f + sinf(time * 8.0f) * 1.0f;
    DrawCircle(pos.x, pos.y, innerPulse, CYBER_RED);
    
    // Draw glitch effect (occasional)
    if(fmodf(time, 2.0f) < 0.1f) {
        float glitchOffset = sinf(time * 30.0f) * 2.0f;
        DrawCircle(pos.x + glitchOffset, pos.y, baseRadius * 0.3f, 
                  Fade(CYBER_RED, 0.5f));
    }
}


Enemy::~Enemy() {
    // Clean up resources if necessary
}
