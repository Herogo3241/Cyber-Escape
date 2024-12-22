#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "raylib.h"
class Board {
private:
    int width;        
    int height;
    int cols;
    int rows;  
    std::ifstream& file;    

public:
    std::vector<std::vector<int>> boardData; 
    Board(std::ifstream& file);  
    const std::vector<std::vector<int>>& getBoardData() const { return boardData; }
    void draw(); 
    ~Board();                   
     

private:
    void loadBoard();
    
};

#endif