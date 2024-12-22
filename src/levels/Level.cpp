#ifndef LEVEL_H
#define LEVEL_H

class Level {
public:
    virtual ~Level() = default;
    virtual void run() = 0;
    bool isCompleted = false;
    bool isGameOver = false;
};

#endif // LEVEL_H