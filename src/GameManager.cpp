#include "GameManager.h"

GameManager::GameManager(float width, float height)
    : WIDTH(width), HEIGHT(height), currentState(GameState::START_SCREEN),
      glowTimer(0), textGlowIntensity(0), currentLevelIndex(0)
{
    // Initialize buttons
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonSpacing = 70; // Space between buttons
    playButton = {WIDTH / 2 - buttonWidth / 2, HEIGHT / 2, buttonWidth, buttonHeight};
    quitButton = {WIDTH / 2 - buttonWidth / 2, HEIGHT / 2 + buttonSpacing, buttonWidth, buttonHeight};
    retryButton = {WIDTH / 2 - buttonWidth / 2, HEIGHT / 2 + 100, buttonWidth, buttonHeight};
    nextLevelButton = {WIDTH / 2 - buttonWidth / 2, HEIGHT / 2, buttonWidth, buttonHeight};
    returnToMenuButton = {WIDTH / 2 - buttonWidth / 2, HEIGHT / 2 + 100, buttonWidth, buttonHeight};
    returnToMenuButton2 = {WIDTH / 2 - buttonWidth / 2, HEIGHT / 2 + 100 + buttonSpacing, buttonWidth, buttonHeight};

    levels.push_back(new Level1(WIDTH, HEIGHT));
    levels.push_back(new Level2(WIDTH, HEIGHT));
    levels.push_back(new Level3(WIDTH, HEIGHT));
}

void GameManager::resetCurrentLevel() {
    if (currentLevelIndex < levels.size()) {
        delete levels[currentLevelIndex];
        // Recreate the current level based on index
        switch(currentLevelIndex) {
            case 0:
                levels[currentLevelIndex] = new Level1(WIDTH, HEIGHT);
                break;
            case 1:
                levels[currentLevelIndex] = new Level2(WIDTH, HEIGHT);
                break;
            case 2:
                levels[currentLevelIndex] = new Level3(WIDTH, HEIGHT);
                break;
        }
    }
}

void GameManager::update(float deltaTime)
{
    glowTimer += GetFrameTime();
    textGlowIntensity = (sinf(glowTimer * 2) + 1.0f) * 0.5f;

    switch (currentState)
    {
    case GameState::START_SCREEN:
        if (CheckCollisionPointRec(GetMousePosition(), playButton) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentLevelIndex = 0;
            resetCurrentLevel();
            setState(GameState::PLAYING);
        }
        else if (CheckCollisionPointRec(GetMousePosition(), quitButton) &&
                 IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Close the window and exit the game
            CloseWindow();
            exit(0);
        }
        break;

    case GameState::PLAYING:
    if (currentLevelIndex < levels.size())
    {
        Level *currentLevel = levels[currentLevelIndex];
        currentLevel->run();

        if (currentLevel->isCompleted)
        {
            setState(GameState::LEVEL_COMPLETE);
        }
        else if (currentLevel->isGameOver)
        {
            static float gameOverDelay = 0.0f;
            
            if (!isGameOverDelayStarted) {
                gameOverDelay = 0.0f;
                isGameOverDelayStarted = true;
            }
            
            gameOverDelay += GetFrameTime();
            
            if (gameOverDelay >= 0.5f) {
                setState(GameState::GAME_OVER);
                isGameOverDelayStarted = false;
            }
        }
        else {
            // Reset the delay flag when not in game over
            isGameOverDelayStarted = false;
        }
    }
    break;

    case GameState::GAME_OVER:
        if (CheckCollisionPointRec(GetMousePosition(), retryButton) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            resetCurrentLevel();
            setState(GameState::PLAYING);
        }
        else if (CheckCollisionPointRec(GetMousePosition(), returnToMenuButton2) &&
                 IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            setState(GameState::START_SCREEN);
        }
        break;

    case GameState::LEVEL_COMPLETE:
        if (CheckCollisionPointRec(GetMousePosition(), nextLevelButton) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentLevelIndex++;
            if (currentLevelIndex < levels.size())
            {
                resetCurrentLevel();
                setState(GameState::PLAYING);
            }
            else
            {
                setState(GameState::GAME_COMPLETE);
            }
        }
        break;

    case GameState::GAME_COMPLETE:
        if (CheckCollisionPointRec(GetMousePosition(), returnToMenuButton) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            setState(GameState::START_SCREEN);
        }
        break;
    }
}

void GameManager::drawStartScreen()
{
    // Draw title with glowing effect
    const char *title = "CYBER ESCAPE";
    float fontSize = 60;
    float titleWidth = MeasureText(title, fontSize);

    // Draw glow
    Color glowColor = {CYBER_BLUE.r, CYBER_BLUE.g, CYBER_BLUE.b,
                       (unsigned char)(255 * textGlowIntensity)};
    DrawText(title, WIDTH / 2 - titleWidth / 2 + 2, HEIGHT / 3 + 2, fontSize, glowColor);

    // Draw main text
    DrawText(title, WIDTH / 2 - titleWidth / 2, HEIGHT / 3, fontSize, CYBER_BLUE);

    // Draw START button with hover effect
    Color playButtonColor = CheckCollisionPointRec(GetMousePosition(), playButton) ? CYBER_RED : CYBER_BLUE;
    DrawRectangleLinesEx(playButton, 2, playButtonColor);
    const char *playText = "START MISSION";
    float playWidth = MeasureText(playText, 20);
    DrawText(playText,
             playButton.x + playButton.width / 2 - playWidth / 2,
             playButton.y + 15,
             20,
             playButtonColor);

    // Draw QUIT button with hover effect
    Color quitButtonColor = CheckCollisionPointRec(GetMousePosition(), quitButton) ? CYBER_RED : CYBER_BLUE;
    DrawRectangleLinesEx(quitButton, 2, quitButtonColor);
    const char *quitText = "QUIT";
    float quitWidth = MeasureText(quitText, 20);
    DrawText(quitText,
             quitButton.x + quitButton.width / 2 - quitWidth / 2,
             quitButton.y + 15,
             20,
             quitButtonColor);

    // Draw atmospheric effects
    for (int i = 0; i < 10; i++)
    {
        float lineY = fmodf(GetTime() * (50.0f + i * 20) + i * 100, HEIGHT);
        DrawLineEx({0, lineY},
                   {WIDTH, lineY},
                   1,
                   Fade(CYBER_BLUE, 0.3f));
    }
}

void GameManager::drawGameOverScreen()
{
    const char *gameOverText = "SYSTEM FAILURE";
    float fontSize = 60;
    float textWidth = MeasureText(gameOverText, fontSize);

    // Draw glitchy game over text
    for (int i = 0; i < 3; i++)
    {
        float offset = sinf(GetTime() * 10 + i) * 5;
        Color glitchColor = {CYBER_RED.r, CYBER_RED.g, CYBER_RED.b,
                             (unsigned char)(255 * 0.5f)};
        DrawText(gameOverText,
                 WIDTH / 2 - textWidth / 2 + offset,
                 HEIGHT / 3 + offset,
                 fontSize,
                 glitchColor);
    }

    DrawText(gameOverText,
             WIDTH / 2 - textWidth / 2,
             HEIGHT / 3,
             fontSize,
             CYBER_RED);

    // Draw retry button
    Color RetrybuttonColor = CheckCollisionPointRec(GetMousePosition(), retryButton) ? CYBER_BLUE : CYBER_RED;
    DrawRectangleLinesEx(retryButton, 2, RetrybuttonColor);
    const char *retryText = "REBOOT SYSTEM";
    float retryWidth = MeasureText(retryText, 20);
    DrawText(retryText,
             retryButton.x + retryButton.width / 2 - retryWidth / 2,
             retryButton.y + 15,
             20,
             RetrybuttonColor);


    Color MenuButtonColor = CheckCollisionPointRec(GetMousePosition(), returnToMenuButton2) ? CYBER_RED : CYBER_BLUE;
    DrawRectangleLinesEx(returnToMenuButton2, 2, MenuButtonColor);
    const char *menuText = "Back To Base";
    float menuWidth = MeasureText(menuText, 20);
    DrawText(menuText,
             returnToMenuButton2.x + returnToMenuButton2.width / 2 - menuWidth / 2,
             returnToMenuButton2.y + 15,
             20,
             MenuButtonColor);

}

void GameManager::drawLevelCompleteScreen()
{
    const char *completeText = "SECTOR CLEARED";
    float fontSize = 60;
    float textWidth = MeasureText(completeText, fontSize);

    DrawText(completeText,
             WIDTH / 2 - textWidth / 2,
             HEIGHT / 3,
             fontSize,
             CYBER_BLUE);

    // Draw next level button
    Color buttonColor = CheckCollisionPointRec(GetMousePosition(), nextLevelButton) ? CYBER_RED : CYBER_BLUE;
    DrawRectangleLinesEx(nextLevelButton, 2, buttonColor);
    const char *nextText = "PROCEED";
    float nextWidth = MeasureText(nextText, 20);
    DrawText(nextText,
             nextLevelButton.x + nextLevelButton.width / 2 - nextWidth / 2,
             nextLevelButton.y + 15,
             20,
             buttonColor);
}

void GameManager::drawGameCompleteScreen()
{
    // Draw victory text with holographic effect
    const char* victoryText = "SYSTEM SECURED";
    const char* subText = "All sectors successfully Completed";
    float fontSize = 60;
    float subFontSize = 30;
    float textWidth = MeasureText(victoryText, fontSize);
    float subTextWidth = MeasureText(subText, subFontSize);

    // Draw holographic layers
    for (int i = 0; i < 3; i++)
    {
        float offset = sinf(GetTime() * 5 + i) * 3;
        Color holoColor = {
            CYBER_BLUE.r,
            CYBER_BLUE.g,
            CYBER_BLUE.b,
            (unsigned char)(255 * (0.3f - i * 0.1f))
        };
        
        DrawText(victoryText,
                 WIDTH / 2 - textWidth / 2 + offset,
                 HEIGHT / 3 + offset,
                 fontSize,
                 holoColor);
    }

    // Draw main victory text
    DrawText(victoryText,
             WIDTH / 2 - textWidth / 2,
             HEIGHT / 3,
             fontSize,
             CYBER_BLUE);

    // Draw subtitle with glow effect
    Color glowColor = {
        CYBER_BLUE.r,
        CYBER_BLUE.g,
        CYBER_BLUE.b,
        (unsigned char)(255 * textGlowIntensity)
    };
    
    DrawText(subText,
             WIDTH / 2 - subTextWidth / 2,
             HEIGHT / 3 + fontSize + 20,
             subFontSize,
             glowColor);

    // Draw statistics
    char statsText[100];
    sprintf(statsText, "SECTORS CLEARED: %ld", levels.size());
    float statsWidth = MeasureText(statsText, 20);
    DrawText(statsText,
             WIDTH / 2 - statsWidth / 2,
             HEIGHT / 2,
             20,
             CYBER_BLUE);

    // Draw return to menu button
    Color buttonColor = CheckCollisionPointRec(GetMousePosition(), returnToMenuButton) ? CYBER_RED : CYBER_BLUE;
    DrawRectangleLinesEx(returnToMenuButton, 2, buttonColor);
    const char* menuText = "RETURN TO BASE";
    float menuWidth = MeasureText(menuText, 20);
    DrawText(menuText,
             returnToMenuButton.x + returnToMenuButton.width / 2 - menuWidth / 2,
             returnToMenuButton.y + 15,
             20,
             buttonColor);

    // Draw decorative grid effect
    for (int i = 0; i < 20; i++)
    {
        float lineY = fmodf(GetTime() * 30 + i * 40, HEIGHT);
        DrawLineEx({0, lineY},
                   {WIDTH, lineY},
                   1,
                   Fade(CYBER_BLUE, 0.1f));
    }

    // Draw circular pulse effect
    float pulseRadius = (sinf(GetTime() * 2) + 1.0f) * 100.0f + 50.0f;
    DrawCircleLines(WIDTH / 2, HEIGHT / 2, pulseRadius, Fade(CYBER_BLUE, 0.2f));
    DrawCircleLines(WIDTH / 2, HEIGHT / 2, pulseRadius * 0.7f, Fade(CYBER_BLUE, 0.15f));
}

void GameManager::draw()
{
    switch (currentState)
    {
    case GameState::START_SCREEN:
        drawStartScreen();
        break;
    case GameState::GAME_OVER:
        drawGameOverScreen();
        break;
    case GameState::LEVEL_COMPLETE:
        drawLevelCompleteScreen();
        break;
    case GameState::GAME_COMPLETE:
        drawGameCompleteScreen();
        break;
    }
}

void GameManager::setState(GameState newState)
{
    currentState = newState;
}

GameManager::~GameManager() {
    for (Level* level : levels) {
        delete level;
    }
    levels.clear();
}