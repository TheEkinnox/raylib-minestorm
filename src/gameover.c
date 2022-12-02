#include <stdlib.h>
#include "raylib.h"

#include "game.h"
#include "gameplay.h"
#include "scene.h"
#include "ui.h"
#include "gameover.h"

GameOverScene* CreateGameOverScene ()
{
    GameOverScene* gameOverScene = malloc (sizeof(GameOverScene));

    if (gameOverScene == NULL)
        return NULL;

    gameOverScene->id = E_GAME_OVER;

    gameOverScene->init = InitGameOverScene;
    gameOverScene->update = UpdateGameOverScene;
    gameOverScene->unload = NULL;

    return gameOverScene;
}

void InitGameOverScene (GameOverScene* gameOverScene)
{
    GameplayData* gameplayData =
        GetGameplayData(FindScene(gameOverScene->gameData->scenes, E_GAMEPLAY));

    if (gameplayData == NULL)
        return;

    gameOverScene->sceneData = (void*) gameplayData->score;
}

void UpdateGameOverScene (GameOverScene* gameOverScene)
{
    char const* text = "GAME OVER";
    float       posY = WINDOW_HEIGHT / 3 - FONT_SIZE_TITLE / 2;
    
    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_TITLE) / 2, posY,
        FONT_SIZE_TITLE, WHITE);

    text = TextFormat(SCORE_TEXT, (size_t) gameOverScene->sceneData);
    posY += FONT_SIZE_TITLE + FONT_SIZE_NORMAL;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_NORMAL) / 2, posY,
        FONT_SIZE_NORMAL, GREEN);

    text = TextFormat(EXIT_TEXT, EXIT_KEY_TEXT);
    posY += 2 * FONT_SIZE_TITLE;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_NORMAL) / 2, posY,
        FONT_SIZE_NORMAL, SKYBLUE);

    if (IsKeyPressed(KEY_ESCAPE))
        LoadScene(gameOverScene->gameData, E_TITLE, true);
}