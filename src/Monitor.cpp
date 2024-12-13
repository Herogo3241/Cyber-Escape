#include "Monitor.h"
#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include <random>


Monitor::Monitor(std::string Title, Vector2 pos) : mxEffect({pos.x - 440, pos.y - 300}, 600, 900, 0.1f)
{
    this->Title = Title;
    this->pos = pos;
    this->rec = {pos.x - 400, pos.y - 300, 800.0f, 600.0f};
    this->isOpen = false;
    this->cursorVisible = true;
    this->timeSinceLastBlink = 0.0f;
    this->currentInput = "";
    this->textY = this->rec.y + 20;
    this->font = LoadFont("./assets/terminal.ttf");
    this->IBMfont = LoadFont("./assets/IBM-Bold.ttf");

    // Initialize screen effect
    this->scanLineOffset = 0.0f;
    this->screenTime = 0.0f;
    this->screenIntensity = 1.0f;
    commandHistory.push_back("Type \'help\' to show commands");
    this->closeTimer = 0.0f;

    
    
}

Color Monitor::GetScanLineColor(float y)
{
    // Create scan line effect
    float scanLineY = fmod(y + scanLineOffset, 4.0f);
    float scanIntensity = (scanLineY < 2.0f) ? 1.0f : 0.8f;

    // Add screen flicker
    float flickerIntensity = 0.95f + 0.05f * sin(screenTime * 30.0f);
    float finalIntensity = scanIntensity * flickerIntensity * screenIntensity;

    return Color{
        static_cast<unsigned char>(GREEN.r * finalIntensity),
        static_cast<unsigned char>(GREEN.g * finalIntensity),
        static_cast<unsigned char>(GREEN.b * finalIntensity),
        255};
}

void Monitor::toggleOpen()
{
    this->isOpen = true;
}

void Monitor::toggleClose()
{
    this->isOpen = false;
    currentInput.clear();
}



int Monitor::processCommand()
{
    commandHistory.push_back("$ " + currentInput);
    std::ostringstream output;
    int n = 1;

    if (currentInput == "ls")
    {
        output << "file1.txt\nfile2.txt\ndir1/";
        n = 3;
    }
    else if (currentInput == "help")
    {
        output << "ls - display all files/folders in directory\nopen - open door\nclear - clear terminal\nexit - exit terminal";
        n = 4;
    }
    else if (currentInput == "exit")
    {
        commandHistory.clear();
        isOpen = false;
        n = 0;
    }
    else if (currentInput == "open"){
        commandHistory.clear();
        toggleMatrixEffect = true;      
    }
    else if (currentInput.rfind("chmod", 0) == 0)
    {
        output << "chmod: Permission granted (simulated)";
    }
    else if (currentInput.rfind("./", 0) == 0)
    {
        output << "Running " << currentInput.substr(2);
    }
    else if (currentInput.rfind("clear", 0) == 0)
    {
        commandHistory.clear();
        textY = this->rec.y + 50; // Reset textY when clearing
        n = 0;
    }
    else
    {
        output << currentInput << ": command not found";
    }

    // Split the output string by newlines and add each line separately
    std::string outputStr = output.str();
    size_t pos = 0;
    std::string token;
    while ((pos = outputStr.find("\n")) != std::string::npos)
    {
        token = outputStr.substr(0, pos);
        commandHistory.push_back(token);
        outputStr.erase(0, pos + 1);
    }
    if (!outputStr.empty())
    {
        commandHistory.push_back(outputStr);
    }

    currentInput.clear();
    return n;
}

void Monitor::update(float deltaTime)
{
    if (!isOpen)
        return;


    if(toggleMatrixEffect) {
        
        closeTimer += deltaTime;
        mxEffect.update();
    }

    if(closeTimer > 10.0f){
        toggleMatrixEffect = false;
        toggleClose();
        closeTimer = 0.0f;
    }
        

    screenTime += deltaTime;
    scanLineOffset += deltaTime * 100.f;

    // Random screen distortion
    if (GetRandomValue(0, 100) < 2)
    {
        screenIntensity = 0.85f + (GetRandomValue(-5, 100) / 100.0f) * 0.15f;
    }
    else
    {
        screenIntensity = std::min(1.0f, screenIntensity + deltaTime);
    }

    // Handle special keys first
    if (IsKeyPressed(KEY_ENTER))
    {
        int n = processCommand();

        // Calculate if we need to scroll
        float totalHeight = (commandHistory.size() + 1) * 20; // +1 for input line
        float maxVisibleHeight = this->rec.height - 60;       // Leave space for title and input

        if (totalHeight > maxVisibleHeight)
        {
            // Scroll content up by removing oldest entries
            while (totalHeight > maxVisibleHeight)
            {
                commandHistory.erase(commandHistory.begin());
                totalHeight -= 20;
            }
        }

        // Reset textY to accommodate visible history
        textY = this->rec.y + 50; // Start below title
        return;
    }

    // Rest of the update logic remains the same
    if (IsKeyDown(KEY_BACKSPACE) && !currentInput.empty())
    {
        backspaceTimer += deltaTime;
        if (backspaceTimer >= BACKSPACE_DELAY)
        {
            currentInput.pop_back();
            backspaceTimer = 0.0f;
        }
    }
    else
    {
        backspaceTimer = BACKSPACE_DELAY; // Reset timer when key is released
    }

    int key = GetCharPressed();
    while (key > 0)
    {
        if (key >= 32 && key <= 126)
        {
            currentInput += static_cast<char>(key);
        }
        key = GetCharPressed();
    }

    timeSinceLastBlink += deltaTime;
    if (timeSinceLastBlink >= 0.5f)
    {
        cursorVisible = !cursorVisible;
        timeSinceLastBlink = 0.0f;
    }
}

void Monitor::draw()
{
    if (!isOpen)
        return;

    const float time = GetTime();
    // Constants for monitor design
    const float BEZEL_THICKNESS = 50.0f;  // Increased thickness for chunky retro look
    const float SCREEN_INSET = 25.0f;
    const Color MAIN_COLOR = {40, 45, 55, 255};  // Dark metallic base
    const Color ACCENT_COLOR = {0, 255, 200, 255};  // Cyan accent
    const Color DARK_ACCENT = {0, 100, 80, 255};  // Darker cyan
    const Color SCREEN_COLOR = {10, 12, 15, 255};  // Darker screen
    
    // Monitor outer casing
    Rectangle outerCase = {
        pos.x - 480 - BEZEL_THICKNESS,
        pos.y - 360 - BEZEL_THICKNESS,
        960.0f + (BEZEL_THICKNESS * 2),
        720.0f + (BEZEL_THICKNESS * 2)
    };

    // Draw main monitor body with metallic effect
    DrawRectangleRec(outerCase, MAIN_COLOR);
    
    // Draw decorative angular cuts in corners
    const float CORNER_SIZE = 40.0f;
    const float ANGLE_SIZE = 20.0f;
    
    // Top left corner
    DrawTriangle(
        (Vector2){outerCase.x, outerCase.y},
        (Vector2){outerCase.x + CORNER_SIZE, outerCase.y},
        (Vector2){outerCase.x, outerCase.y + CORNER_SIZE},
        DARK_ACCENT
    );
    
    // Top right corner
    DrawTriangle(
        (Vector2){outerCase.x + outerCase.width, outerCase.y},
        (Vector2){outerCase.x + outerCase.width - CORNER_SIZE, outerCase.y},
        (Vector2){outerCase.x + outerCase.width, outerCase.y + CORNER_SIZE},
        DARK_ACCENT
    );

    // Draw glowing accent lines
    float glowIntensity = (sinf(time * 2.0f) + 1.0f) * 0.5f;
    Color glowColor = {
        ACCENT_COLOR.r,
        ACCENT_COLOR.g,
        ACCENT_COLOR.b,
        (unsigned char)(ACCENT_COLOR.a * glowIntensity)
    };

    // Horizontal accent lines
    for (int i = 0; i < 3; i++) {
        float lineY = outerCase.y + 10.0f + (i * 5.0f);
        DrawLineEx(
            (Vector2){outerCase.x + CORNER_SIZE, lineY},
            (Vector2){outerCase.x + outerCase.width - CORNER_SIZE, lineY},
            2,
            glowColor
        );
    }

    Rectangle screenArea = {
        pos.x - 480 + SCREEN_INSET,
        pos.y - 360 + SCREEN_INSET,
        960.0f - (SCREEN_INSET * 2),
        720.0f - (SCREEN_INSET * 2)
    };
    DrawRectangleRec(screenArea, SCREEN_COLOR);

    // Draw tech pattern border
    const int SEGMENTS = 20;
    for (int i = 0; i < SEGMENTS; i++) {
        float segmentLength = screenArea.width / SEGMENTS;
        float x = screenArea.x + (i * segmentLength);
        float alpha = (sinf(time * 3.0f + i * 0.5f) + 1.0f) * 0.5f;
        
        DrawRectangle(x, screenArea.y - 5, segmentLength * 0.8f, 3,
                     Fade(ACCENT_COLOR, alpha * 0.7f));
        DrawRectangle(x, screenArea.y + screenArea.height + 2, segmentLength * 0.8f, 3,
                     Fade(ACCENT_COLOR, alpha * 0.7f));
    }

    // Draw inset screen border with glowing effect
    DrawRectangleLinesEx(screenArea, 2, DARK_ACCENT);
    DrawRectangleLinesEx(
        (Rectangle){screenArea.x - 2, screenArea.y - 2,
                   screenArea.width + 4, screenArea.height + 4},
        1,
        Fade(ACCENT_COLOR, glowIntensity * 0.5f)
    );

    // Draw futuristic logo area
    Rectangle logoArea = {
        screenArea.x + (screenArea.width / 2) - 70,
        screenArea.y - 40,
        140,
        35
    };
    DrawRectangleRec(logoArea, MAIN_COLOR);
    
    // Animated chevrons around logo
    const float CHEVRON_SIZE = 10.0f;
    for (int i = 0; i < 3; i++) {
        float xOffset = sinf(time * 2.0f + i * 1.0f) * 5.0f;
        DrawTriangle(
            (Vector2){logoArea.x - 20.0f - (i * 15.0f) + xOffset, logoArea.y + logoArea.height/2},
            (Vector2){logoArea.x - 10.0f - (i * 15.0f) + xOffset, logoArea.y},
            (Vector2){logoArea.x - 10.0f - (i * 15.0f) + xOffset, logoArea.y + logoArea.height},
            Fade(ACCENT_COLOR, 0.5f - (i * 0.1f))
        );
    }

    // Draw IVM logo with tech effect
    Vector2 logoPos = {logoArea.x + 35, logoArea.y - 5};
    DrawTextEx(IBMfont, "IVM", logoPos, 40, 2, ACCENT_COLOR);
    DrawTextEx(IBMfont, "IVM", (Vector2){logoPos.x + 1, logoPos.y + 1}, 40, 2, 
               Fade(DARK_ACCENT, glowIntensity));

    // Draw power LED with pulse effect
    float ledSize = 5.0f + sinf(time * 4.0f) * 1.0f;
    DrawCircle(screenArea.x + screenArea.width - 30,
               screenArea.y - 20,
               ledSize,
               Fade(ACCENT_COLOR, 0.7f));
    DrawCircle(screenArea.x + screenArea.width - 30,
               screenArea.y - 20,
               ledSize * 0.6f,
               ACCENT_COLOR);

    // Enhanced scan lines with CRT effect
    for (float y = screenArea.y; y < screenArea.y + screenArea.height; y += 2.0f) {
        float scanIntensity = (sinf(y * 0.1f + screenTime * 2.0f) + 1.0f) * 0.5f;
        float distortionOffset = sinf(y * 0.02f + time) * 2.0f;
        DrawLine(
            screenArea.x + distortionOffset,
            y,
            screenArea.x + screenArea.width + distortionOffset,
            y,
            (Color){ACCENT_COLOR.r, ACCENT_COLOR.g, ACCENT_COLOR.b, 
                    (unsigned char)(scanIntensity * 40.0f)}
        );
    }

    // Draw terminal content
    float currentY = screenArea.y + 40;
    const float lineHeight = 20.0f;

    // Draw title with glitch effect
    std::string titleText = Title;
    if (fmodf(time * 10.0f, 1.0f) < 0.05f) {
        // Occasional glitch effect
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, titleText.length() - 1);
        titleText[dis(gen)] = char(33 + dis(gen) % 94);
    }
    DrawTextEx(font, titleText.c_str(),
               (Vector2){screenArea.x + 20, screenArea.y + 20},
               20, 0, GetScanLineColor(currentY));

    // Draw command history
    currentY += 30;
    for (const auto &command : commandHistory) {
        if (currentY >= screenArea.y + screenArea.height - 40)
            break;

        float leftSpace = command[0] == '$' ? 10.0f : 10.0f;
        DrawTextEx(font, command.c_str(),
                   (Vector2){screenArea.x + leftSpace, currentY},
                   16, 0, GetScanLineColor(currentY));
        currentY += lineHeight;
    }

    if(toggleMatrixEffect) {
        mxEffect.draw();
    }

    // Draw current input line with pulsing cursor
    float inputY = screenArea.y + screenArea.height - 30;
    std::string displayInput = "$ " + currentInput + 
        (cursorVisible ? "|" : " ");
    DrawTextEx(font, displayInput.c_str(),
               (Vector2){screenArea.x + 20, inputY},
               16, 0, GetScanLineColor(inputY));

    // Enhanced vignette effect
    float vignetteSize = 250.0f;
    for (int i = 0; i < vignetteSize; i++) {
        float alpha = (i / vignetteSize) * 150;
        float distortion = sinf(time + i * 0.1f) * 2.0f;
        DrawRectangleLinesEx(
            (Rectangle){
                screenArea.x - i/2 + distortion,
                screenArea.y - i/2,
                screenArea.width + i,
                screenArea.height + i
            },
            1,
            (Color){0, 0, 0, (unsigned char)(150 - alpha)}
        );
    }
}

Monitor::~Monitor()
{
    UnloadFont(font);
    UnloadFont(IBMfont);
    
}
