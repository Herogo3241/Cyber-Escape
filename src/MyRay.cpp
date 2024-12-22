#include "MyRay.h"


MyRay::MyRay(Vector2 pos, float angle)
{
    this->pos = pos;
    this->angle = angle;
}

void MyRay::setPos(Vector2 pos)
{
    this->pos = pos;
}

void MyRay::setAngle(float angle)
{
    this->angle = angle;
}

void MyRay::setLength(float length)
{
    this->length = length;
}   

bool MyRay::checkCollision(const std::vector<std::vector<int>>& boardData) {
    float radAngle = this->angle * DEG2RAD;
    Vector2 direction = {cos(radAngle), sin(radAngle)};
    
    const int tileSize = 64;  // Match the tile size from Board.cpp
    float stepSize = 1.0f;    // Small steps for precise collision detection
    
    // Reset collision status
    hasCollided = false;
    
    // Ray marching
    for (float dist = 0; dist < length; dist += stepSize) {
        // Calculate current position along the ray
        Vector2 currentPos = {
            pos.x + direction.x * dist,
            pos.y + direction.y * dist
        };
        
        // Convert position to board grid coordinates
        int gridX = static_cast<int>(currentPos.x / tileSize);
        int gridY = static_cast<int>(currentPos.y / tileSize);
        
        // Check if we're within board bounds
        if (gridX >= 0 && gridX < boardData[0].size() && 
            gridY >= 0 && gridY < boardData.size()) {
            
            // Check if we hit a wall (0 or -1)
            if (boardData[gridY][gridX] == -1 || boardData[gridY][gridX] == 1) {
                hasCollided = true;
                collisionPoint = currentPos;
                length = dist;  // Set ray length to collision point
                return true;
            }
        }
    }
    
    return false;
}

void MyRay::draw() {
    float radAngle = this->angle * DEG2RAD;
    Vector2 direction = {cos(radAngle), sin(radAngle)};
    
    int segments = 20;
    float segmentLength = this->length / segments;
    
    for (int i = 0; i < segments; i++) {
        Vector2 start = {
            this->pos.x + direction.x * segmentLength * i,
            this->pos.y + direction.y * segmentLength * i
        };
        Vector2 end = {
            this->pos.x + direction.x * segmentLength * (i + 1),
            this->pos.y + direction.y * segmentLength * (i + 1)
        };
        
        int alpha = 255 - (255 * i / segments);
        Color segmentColor = {255, 0, 0, (unsigned char)alpha};
        
        // Only draw up to collision point if collision occurred
        if (hasCollided && i * segmentLength >= length) {
            break;
        }
        
        DrawLine(start.x, start.y, end.x, end.y, segmentColor);
    }
    
    // Optionally, draw collision point
    if (hasCollided) {
        DrawCircle(collisionPoint.x, collisionPoint.y, 2,  {255, 0, 0, 255 - 255 * 18/20});
    }
}








// Destructor
MyRay::~MyRay()
{
}