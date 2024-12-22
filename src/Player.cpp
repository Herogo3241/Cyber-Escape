#include "Player.h"
#include "raylib.h"
#include "raymath.h"
#include <iostream>

Player::Player(Vector2 pos, const std::vector<std::vector<int>> &boardData)
{
    this->pos = pos;
    this->boardData = &boardData;
}

bool Player::isValidPosition(Vector2 newPos)
{
    int gridX = static_cast<int>(newPos.x / TILE_SIZE);
    int gridY = static_cast<int>(newPos.y / TILE_SIZE);

    if (gridX < 0 || gridY < 0 ||
        gridX >= boardData->at(0).size() ||
        gridY >= boardData->size())
    {
        return false;
    }

    int tileValue = (*boardData)[gridY][gridX];
    return tileValue != 1 && tileValue != -1;
}

void Player::keyMovements(float deltaTime)
{
    Vector2 newPos = pos;
    bool isSprinting = IsKeyDown(KEY_LEFT_SHIFT);
    float currentSpeed = speed * (isSprinting ? 1.6f : 1.0f);

    if (IsKeyDown(KEY_W))
        newPos.y -= currentSpeed * deltaTime;
    if (IsKeyDown(KEY_S))
        newPos.y += currentSpeed * deltaTime;
    if (IsKeyDown(KEY_A))
        newPos.x -= currentSpeed * deltaTime;
    if (IsKeyDown(KEY_D))
        newPos.x += currentSpeed * deltaTime;

    // Update glow intensity based on sprinting
    if (isSprinting)
    {
        glowIntensity = Lerp(glowIntensity, 1.5f, deltaTime * 2.0f);
    }
    else
    {
        glowIntensity = Lerp(glowIntensity, 1.0f, deltaTime * 2.0f);
    }

    // Validate position
    bool valid = true;
    valid &= isValidPosition(newPos);

    Vector2 checkPoints[] = {
        {newPos.x + radius, newPos.y},
        {newPos.x - radius, newPos.y},
        {newPos.x, newPos.y + radius},
        {newPos.x, newPos.y - radius}};

    for (const auto &point : checkPoints)
    {
        valid &= isValidPosition(point);
    }

    if (valid)
        pos = newPos;
}

bool Player::checkIsOver(Vector2 pos){
    int gridX = static_cast<int>(pos.x / TILE_SIZE);
    int gridY = static_cast<int>(pos.y / TILE_SIZE);

    
    int tileValue = (*boardData)[gridY][gridX];
    return tileValue == 2;
}

bool Player::update(float deltaTime)
{
    keyMovements(deltaTime);

    

    // Create periodic pulse effect
    float pulseRate = 1.0f; // Pulse every second
    float pulseTime = fmodf(GetTime(), pulseRate);
    if (pulseTime < 0.1f)
    {
        pulseSize = 1.0f;
        pulseAlpha = 0.5f;
    }

    // Update pulse effect
    pulseSize = Lerp(pulseSize, 0.0f, deltaTime * 5.0f);
    pulseAlpha = Lerp(pulseAlpha, 0.0f, deltaTime * 3.0f);

    // Keep player within screen bounds
    pos.x = Clamp(pos.x, radius, GetScreenWidth() - radius);
    pos.y = Clamp(pos.y, radius, GetScreenHeight() - radius);

    bool x = this->checkIsOver(this->pos); 
    return x;
}

void Player::draw()
{
    // Draw outer glow (creates a soft light effect)
    for (int i = 0; i < 3; i++)
    {
        float alpha = (1.0f - (i / 3.0f)) * 0.3f * glowIntensity;
        Color outerGlow = {playerColor.r, playerColor.g, playerColor.b,
                           static_cast<unsigned char>(255 * alpha)};
        DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y),
                   outerGlowRadius - (i * 20), outerGlow);
    }

    // Draw pulse effect
    Color pulseColor = {playerColor.r, playerColor.g, playerColor.b,
                        static_cast<unsigned char>(pulseAlpha * 255)};
    DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y),
               radius * (1.0f + pulseSize), pulseColor);

    // Draw inner glow
    float innerGlow = radius * (1.0f + glowIntensity * 0.3f);
    Color glowColor = {playerColor.r, playerColor.g, playerColor.b,
                       static_cast<unsigned char>(128 * glowIntensity)};
    DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y),
               innerGlow, glowColor);

    // Draw player core
    DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y),
               radius, playerColor);
}
Player::~Player()
{
    // Destructor implementation
}