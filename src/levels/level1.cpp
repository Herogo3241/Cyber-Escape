#include "level1.h"


Level1::Level1(int width, int height) : 
    WIDTH(width),
    HEIGHT(height),
    isOver(false),
    board([&]{
        std::ifstream file("./assets/levels/level1.csv");
        if (!file) {
            std::cerr << "Failed to open level1.csv!" << std::endl;
            throw std::runtime_error("Failed to load level file");
        }
        Board b(file);
        file.close();
        return b;
    }()),
    player({static_cast<float>(width - 96), 
            static_cast<float>(height - 96)}, 
           board.boardData),
    monitor("$$ Level 1 $$", 
            {static_cast<float>(width) / 2, 
             static_cast<float>(height) / 2})
{

    // Initialize enemies
    enemies.emplace_back(Vector2{96, 160}, 0, board.boardData);
    enemies.emplace_back(Vector2{288, 64}, 1, board.boardData);
}

void Level1::run() {
    if (isOver) return;  // Early return if level is over

    // Draw background first
    board.draw();

    // Update and draw player
    isGoal = player.update(GetFrameTime());
    player.draw();  // Make sure to draw the player

    // Update and draw enemies
    if(!isGoal){
        for (auto& enemy : enemies) {
            isGameOver = enemy.update(GetFrameTime(), player.getPos()); 
            enemy.draw();
            if (isGameOver) break; 
        }

    }else {
        monitor.toggleOpen();
    }
    

    isCompleted = monitor.update(GetFrameTime());
    monitor.draw();

    
}