#include "raylib.h"
#include <string>
#include <vector>

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

public:
    Monitor(std::string Title, Vector2 pos);
    void draw();
    void update(float deltaTime);
    int processCommand();
    void toggleOpen();
    void toggleClose();
    ~Monitor();
};
