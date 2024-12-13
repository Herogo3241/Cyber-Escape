#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "raylib.h"
class Board {
private:
    std::vector<std::vector<int>> boardData; 
    int width;        
    int height;
    int cols;
    int rows;  
    std::ifstream& file;    

public:
    Board(std::ifstream& file);  
    void draw(); 
    ~Board();                   
     

private:
    void loadBoard();
    
};