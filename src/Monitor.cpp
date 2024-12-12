#include "Monitor.h"
#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>

Monitor::Monitor(std::string Title, Vector2 pos)
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

    screenTime += deltaTime;
    scanLineOffset += deltaTime * 12.5f;

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
    if (IsKeyPressed(KEY_BACKSPACE) && !currentInput.empty())
    {
        currentInput.pop_back();
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
    if (!isOpen){
        return;
    }
        

    

    // Draw the monitor background with scan lines
    Rectangle monitor = {pos.x - 480, pos.y - 360, 960.0f, 720.0f};
    DrawRectangleRec(monitor, {192,184,155, 255});
    DrawRectangleRec(this->rec, {0, 0, 0, 255});

                              // You can load a custom font if needed
    Vector2 textPosition = {(float) GetScreenWidth() / 2 - 20, monitor.y + 10}; // Position for the brand name

    // Draw the brand name "IVM" on the monitor
    DrawTextEx(IBMfont, "IVM", textPosition, 40, 2, {0, 0, 0, 255});

    
    // Draw scan lines
    for (float y = this->rec.y; y < this->rec.y + this->rec.height; y += 2.0f)
    {
        DrawLine(
            this->rec.x, y,
            this->rec.x + this->rec.width, y,
            {0, 20, 0, 50});
    }

    // Draw monitor border
    DrawRectangleLinesEx(this->rec, 3, {49,53,63,255});

    // Draw the title text
    float titleY = this->rec.y + 20;
    DrawTextEx(font, this->Title.c_str(), {this->rec.x + 20, titleY},
               20, 0, GetScanLineColor(titleY));

    // Draw command history
    float currentY = this->rec.y + 50;
    const float lineHeight = 20.0f;
    const float maxY = this->rec.y + this->rec.height - 40;

    for (const auto &command : commandHistory)
    {
        if (currentY >= maxY)
        {
            commandHistory.erase(commandHistory.begin());
            continue;
        }
        float leftSpace = 10.f;
        if (command[0] != '$')
            leftSpace = 10.f;
        DrawTextEx(font, command.c_str(),
                   {this->rec.x + leftSpace, currentY},
                   16, 0, GetScanLineColor(currentY));
        currentY += lineHeight + 5;
    }

    // Draw the current input at the bottom
    float inputY = this->rec.y + this->rec.height - 30;
    std::string displayInput = "$ " + currentInput + (cursorVisible ? "|" : " ");
    DrawTextEx(font, displayInput.c_str(),
               {this->rec.x + 20, inputY},
               16, 0, GetScanLineColor(inputY));
}
Monitor::~Monitor()
{
    UnloadFont(font);
    UnloadFont(IBMfont);
}
