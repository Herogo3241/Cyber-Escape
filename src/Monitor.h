#ifndef MONITOR_H
#define MONITOR_H

#include "raylib.h"
#include <string>
#include <vector>
#include "MatrixEffect.h"

class Monitor
{
private:
    /* data */
    std::string Title;
    Vector2 pos;
    Rectangle rec;
    bool isOpen;
    bool cursorVisible;
    float timeSinceLastBlink;
    std::string currentInput;
    std::vector<std::string> commandHistory;
    Font font;
    Font IBMfont;
    float textY = this->rec.y + 50;
    float scanLineOffset;
    float screenTime;
    float screenIntensity;
    Color GetScanLineColor(float y);
    float backspaceTimer = 0.0f;
    const float BACKSPACE_DELAY = 0.1f;
    MatrixEffect mxEffect;
    bool toggleMatrixEffect = false;
    float closeTimer;


public:
    bool isCompleted = false;
    Monitor(std::string Title, Vector2 pos);
    void draw();
    bool update(float deltaTime);
    int processCommand();
    
    void toggleOpen();
    void toggleClose();
    ~Monitor();
};


#endif