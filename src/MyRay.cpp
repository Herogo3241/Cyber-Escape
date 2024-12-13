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

void MyRay::draw()
{
    float radAngle = this->angle * DEG2RAD;

    // Calculate the direction vector
    Vector2 direction = {cos(radAngle), sin(radAngle)};
    
    // Number of segments to divide the ray into
    int segments = 15;
    float segmentLength = this->length / segments;

    for (int i = 0; i < segments; i++)
    {
        // Calculate the start and end points of the segment
        Vector2 start = {
            this->pos.x + direction.x * segmentLength * i,
            this->pos.y + direction.y * segmentLength * i
        };
        Vector2 end = {
            this->pos.x + direction.x * segmentLength * (i + 1),
            this->pos.y + direction.y * segmentLength * (i + 1)
        };

  
        int alpha = 100 - (100 * i / segments); 
        Color segmentColor = {253, 249, 0, (unsigned char)alpha}; 

        DrawLine(start.x, start.y, end.x, end.y, segmentColor);
    }
}





// Destructor
MyRay::~MyRay()
{
}