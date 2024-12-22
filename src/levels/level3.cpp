#include "level3.h"


Level3::Level3(int width, int height) : 
    WIDTH(width),
    HEIGHT(height),
    isOver(false),
    board([&]{
        std::ifstream file("./assets/levels/level3.csv");
        if (!file) {
            std::cerr << "Failed to open level3.csv!" << std::endl;
            throw std::runtime_error("Failed to load level file");
        }
        Board b(file);
        file.close();
        return b;
    }()),
    player({static_cast<float>(96), 
            static_cast<float>(96)}, 
           board.boardData),
    monitor("$$ Level 3 $$", 
            {static_cast<float>(width) / 2, 
             static_cast<float>(height) / 2})
{

    // Initialize enemies
    enemies.emplace_back(Vector2{448, 448}, 0, board.boardData);
    enemies.emplace_back(Vector2{448, 832}, 0, board.boardData);
    enemies.emplace_back(Vector2{1152, 160}, 1, board.boardData);

}

void Level3::run() {
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