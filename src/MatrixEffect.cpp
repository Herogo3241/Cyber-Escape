#include "MatrixEffect.h"
#include <iostream>

// Constructor
MatrixEffect::MatrixEffect(Vector2 pos, float maxHeight, float maxWidth, float updateInterval) {
    this->pos = pos;
    this->maxHeight = maxHeight;
    this->maxWidth = maxWidth;
    this->updateInterval = updateInterval; // Interval for updating symbols
    this->elapsedTime = 0.0f; // Initialize elapsed time to 0

    this->font = LoadFont("./assets/terminal.ttf"); 
    if (font.baseSize == 0) {
        std::cerr << "Failed to load font from ./assets/terminal.ttf" << std::endl;
    }

    this->matrixSymbols = u8"                                                                @#$%^&*1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzｱｲｳｴｵｶｷｸｹｺ";

 
    this->maxColumns = static_cast<int>(maxWidth / 25); 

    // Initialize matrixColumns with empty columns
    matrixColumns.resize(maxColumns); // Resize to fit the number of columns based on maxWidth
}

// Update method
void MatrixEffect::update() {
    // Increment elapsed time
    elapsedTime += GetFrameTime(); // Time since the last frame

    // Only add a symbol if elapsedTime exceeds the updateInterval
    if (elapsedTime >= updateInterval) {
        elapsedTime = 0.0f; // Reset elapsed time

        // For each column, add a random symbol
        for (int i = 0; i < maxColumns; ++i) {
            int randomSymbol = GetRandomValue(0, matrixSymbols.length() - 1);
            char symbol = matrixSymbols[randomSymbol];

            // Add new symbol to the current column
            matrixColumns[i].insert(matrixColumns[i].begin(), std::string(1, symbol));

            // Limit column height to maxHeight
            if (matrixColumns[i].size() * 14 > maxHeight) { // Assuming 14 is the font size
                matrixColumns[i].erase(matrixColumns[i].end()); // Remove the top symbol
            }
        }
    }
}

// Draw method
void MatrixEffect::draw() {
    float currentX = pos.x; // Start drawing from the initial x-position
    float currentY = pos.y; // Start drawing from the initial y-position

    // Ensure matrixColumns has valid data to draw
    if (matrixColumns.empty()) {
        std::cout << "[DEBUG] matrixColumns is empty!" << std::endl;
    }

    for (size_t columnIndex = 0; columnIndex < matrixColumns.size(); ++columnIndex) {
        currentY = pos.y; // Reset Y position for each column
        for (std::string& symbol : matrixColumns[columnIndex]) {
            DrawTextEx(font, symbol.c_str(), {currentX, currentY}, 14, 1, GREEN);
            currentY += 14; // Move to the next line
        }
        currentX += 25; // Move to the next column (Assuming font width is 14)
    }
}

void MatrixEffect::clear() {
    matrixColumns.clear();
}


MatrixEffect::~MatrixEffect() {
    UnloadFont(font);
    clear();
}
