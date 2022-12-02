#include <stdlib.h>
#include "raylib.h"

#include "game.h"
#include "scene.h"
#include "ui.h"
#include "pause.h"

PauseScene* CreatePauseScene()
{
    PauseScene* pauseScene = malloc(sizeof(PauseScene));

    if (pauseScene == NULL)
        return NULL;

    pauseScene->id = E_PAUSE;

    pauseScene->init = NULL;
    pauseScene->update = UpdatePauseScene;
    pauseScene->unload = NULL;

    return pauseScene;
}

void UpdatePauseScene(PauseScene* pauseScene)
{
    DrawRectangle(MIN_X, MIN_Y, MAX_X-MIN_X, MAX_Y - MIN_Y, PAUSE_BACKGROUND_COLOR);
 
    char const* text = "PAUSE";
    float       posY = WINDOW_HEIGHT / 3 - FONT_SIZE_TITLE / 2;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_TITLE) / 2, posY,
        FONT_SIZE_TITLE, WHITE);

    text = TextFormat(UNPAUSE_TEXT, PAUSE_KEY_TEXT);
    posY += 8 * FONT_SIZE_TITLE;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_NORMAL) / 2, posY,
        FONT_SIZE_NORMAL, SKYBLUE);

    text = TextFormat(EXIT_TEXT, EXIT_KEY_TEXT);
    posY += 2 * FONT_SIZE_NORMAL;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_NORMAL) / 2, posY,
        FONT_SIZE_NORMAL, RED);

    if (IsKeyPressed(KEY_ESCAPE))
        LoadScene(pauseScene->gameData, E_TITLE, true);
}