#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <vector>

class Player {
private:
    Vector2 pos;
    const float speed = 200.0f;
    const float radius = 15.0f;
    const std::vector<std::vector<int>>* boardData;
    const float TILE_SIZE = 64.0f;  // Assuming 32x32 tiles

    bool isValidPosition(Vector2 newPos);


    // Glow effect properties
    float glowIntensity = 0.0f;
    float innerGlowRadius = 20.0f;
    float outerGlowRadius = 30.0f;
    Color playerColor = {0, 255, 196, 255}; // Cyan-ish glow
    float pulseSize = 0.0f;
    float pulseAlpha = 0.0f;

public:
    Player(Vector2 pos, const std::vector<std::vector<int>>& boardData);
    void keyMovements(float deltaTime);
    bool checkIsOver(Vector2 pos);
    bool update(float deltaTime);
    void draw();
    Vector2 getPos() const { return pos; }
    ~Player();
};

#endif