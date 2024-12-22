#include "level2.h"


Level2::Level2(int width, int height) : 
    WIDTH(width),
    HEIGHT(height),
    isOver(false),
    board([&]{
        std::ifstream file("./assets/levels/level2.csv");
        if (!file) {
            std::cerr << "Failed to open level2.csv!" << std::endl;
            throw std::runtime_error("Failed to load level file");
        }
        Board b(file);
        file.close();
        return b;
    }()),
    player({static_cast<float>(width - 96), 
            static_cast<float>(height - 96)}, 
           board.boardData),
    monitor("$$ Level 2 $$", 
            {static_cast<float>(width) / 2, 
             static_cast<float>(height) / 2})
{

    // Initialize enemies
    enemies.emplace_back(Vector2{608, 352}, 0, board.boardData);
    enemies.emplace_back(Vector2{105, 704}, 0, board.boardData);
}

void Level2::run() {
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