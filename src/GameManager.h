// GameState.h
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "raylib.h"
#include "levels/level1.h"
#include "levels/level2.h"
#include "levels/level3.h"
#include "levels/Level.cpp"


enum class GameState {
    START_SCREEN,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETE,
    GAME_COMPLETE
};

class GameManager {
private:
    GameState currentState;
    float WIDTH;
    float HEIGHT;
    std::vector<Level*> levels;
    int currentLevelIndex;

    bool isGameOverDelayStarted = false;
    
    // UI Elements
    Rectangle playButton;
    Rectangle quitButton;     // New quit button
    Rectangle retryButton;
    Rectangle nextLevelButton;
    Rectangle returnToMenuButton;
    Rectangle returnToMenuButton2;
    
    void drawStartScreen();
    void drawGameOverScreen();
    void drawLevelCompleteScreen();

    void drawGameCompleteScreen();

    // Futuristic UI effects
    float glowTimer;
    float textGlowIntensity;
    const Color CYBER_BLUE = {0, 255, 255, 255};
    const Color CYBER_RED = {255, 30, 60, 255};



public:
    GameManager(float width, float height);
    void resetCurrentLevel();
    void update(float deltaTime);
    void draw();
    void setState(GameState newState);
    GameState getState() const { return currentState; }
    ~GameManager();
};
 #endif