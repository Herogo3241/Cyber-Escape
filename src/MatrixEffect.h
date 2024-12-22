#ifndef MATRIX_EFFECT_H
#define MATRIX_EFFECT_H


#include "raylib.h"
#include <string>
#include <vector>

class MatrixEffect
{
private:
    Vector2 pos;
    float maxHeight;
    float maxWidth;
    Font font;
    std::string matrixSymbols;
    std::vector<std::string> matrixColumn;
    float updateInterval;
    float elapsedTime;
    int maxColumns;
    std::vector<std::vector<std::string>> matrixColumns;

public:
    MatrixEffect(Vector2 pos, float maxHeight,float maxWidth, float updateInterval);
    void update();
    void draw();
    void clear();
    ~MatrixEffect();
};


#endif