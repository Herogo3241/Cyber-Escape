#include "Board.h"
#include <string>
#include <sstream>
#include <math.h>

Board::Board(std::ifstream& file) : file(file)
{
    this->width = GetScreenWidth();
    this->height = GetScreenHeight();

    loadBoard();

    // for(std::vector<int> row : boardData){
    //     for(int tile : row){
    //         std::cout << tile << "\t"; 
    //     }
    //     std::cout << std::endl;
    // }
    
}

void Board::loadBoard() {
    std::string line;
    int rowIndex = 0;

    // Read each line from the file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<int> rowData;

        // Split the line by commas and store each value in the rowData vector
        while (std::getline(ss, cell, ',')) {
            rowData.push_back(std::stoi(cell));  // Convert each string to an integer
        }

        // Once the row is populated, add it to the boardData
        boardData.push_back(rowData);
        rowIndex++;
    }

    // Get width and height from the board data
    rows = boardData.size();
    cols = boardData.empty() ? 0 : boardData[0].size();
}

void Board::draw() {
    const int tileSize = 64;
    const float time = GetTime(); 
    
    // Draw dark background grid
    for (int i = 0; i < boardData.size(); i++) {
        for (int j = 0; j < boardData[0].size(); j++) {
            // Draw thin grid lines
            DrawRectangleLines(j * tileSize, i * tileSize, tileSize, tileSize, 
                             Fade(DARKGRAY, 0.3f));
        }
    }

    // Draw main tiles with effects
    for (int i = 0; i < boardData.size(); i++) {
        for (int j = 0; j < boardData[0].size(); j++) {
            Color baseColor;
            Color glowColor;
            float alpha = 0.7f + sinf(time * 2 + i * j * 0.1f) * 0.3f; // Pulsing effect

            switch (boardData[i][j]) {
            case 0: // Path tiles
                baseColor = (Color){80, 90, 120, 255};    // Steel blue-gray
                glowColor = (Color){100, 150, 255, 100};  // Neon blue glow
                break;
            case 1: // Ground/Metal tiles
                baseColor = (Color){64, 64, 92, 255};     // Dark metallic
                glowColor = (Color){128, 128, 155, 100};  // Metal glow
                break;
            case 2: // Nature/Toxic tiles
                baseColor = (Color){0, 255, 128, 255};    // Toxic green
                glowColor = (Color){128, 255, 0, 100};    // Radioactive glow
                break;
            default:
                baseColor = (Color){20, 20, 35, 255};     // Dark void
                glowColor = (Color){40, 40, 70, 100};     // Void glow
                break;
            }

            // Draw base tile with pulsing alpha
            DrawRectangle(j * tileSize, i * tileSize, tileSize, tileSize, 
                         Fade(baseColor, alpha));

            // Draw glow effect
            float glowSize = 4.0f * (1.0f + sinf(time * 3 + i * j * 0.2f) * 0.5f);
            DrawRectangle(j * tileSize - glowSize, i * tileSize - glowSize,
                         tileSize + glowSize * 2, tileSize + glowSize * 2,
                         Fade(glowColor, 0.3f));

            // Draw scanline effect
            float scanlinePos = fmodf(time * 100, GetScreenHeight());
            if (abs(i * tileSize - scanlinePos) < tileSize) {
                DrawRectangle(j * tileSize, i * tileSize, tileSize, 2,
                             Fade(WHITE, 0.1f));
            }
        }
    }

    // Draw hexagonal grid overlay
    for (int i = 0; i < boardData.size(); i++) {
        for (int j = 0; j < boardData[0].size(); j++) {
            Vector2 center = {j * tileSize + tileSize/2.0f, i * tileSize + tileSize/2.0f};
            float radius = tileSize * 0.45f;
            
            for (int k = 0; k < 6; k++) {
                float angle1 = k * PI / 3;
                float angle2 = (k + 1) * PI / 3;
                Vector2 start = {
                    center.x + cosf(angle1) * radius,
                    center.y + sinf(angle1) * radius
                };
                Vector2 end = {
                    center.x + cosf(angle2) * radius,
                    center.y + sinf(angle2) * radius
                };
                DrawLineEx(start, end, 1.0f, Fade(WHITE, 0.1f));
            }
        }
    }
}

Board::~Board(){}
