#ifndef LEVEL1_H
#define LEVEL1_H

#include <fstream>
#include <iostream>
#include "../raylib.h"
#include "../Board.h"
#include "../Player.h"
#include "../Monitor.h"
#include "../Enemy.h"
#include "Level.cpp"


class Level1 : public Level {
private:
    float WIDTH;
    float HEIGHT;
    bool isOver;
    Player player;
    Monitor monitor;
    Board board;
    std::vector<Enemy> enemies;

public:
    bool isGoal = false;
    Level1(int width, int height);
    void run();
    ~Level1() = default;
};

#endif