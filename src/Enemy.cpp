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
    float swaySpeed = 1.5f;  
    float swayOffset = swayRange * sin(GetTime() * swaySpeed);

    // Cast rays with swaying effect
    for (float i = this->angle - 30 + swayOffset; i < this->angle + 30 + swayOffset; i += 0.5f) {
        this->ray.setPos(this->pos);
        this->ray.setAngle(i);
        this->ray.draw();
    }
}


void Enemy::draw() {
    const float time = GetTime();
    const Color CYBER_RED = {255, 30, 60, 255};
    const Color DARK_RED = {120, 0, 20, 255};
    const float baseRadius = 10.0f;
    
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
