#include <stdlib.h>
#include "raylib.h"

#include "title.h"
#include "game.h"
#include "scene.h"
#include "ui.h"

TitleScene* CreateTitleScene ()
{
TitleScene* titleScene = malloc (sizeof(TitleScene));

if (titleScene == NULL)
    return NULL;

titleScene->id = E_TITLE;

titleScene->init = NULL;
titleScene->update = UpdateTitleScene;
titleScene->unload = NULL;
return titleScene;
}

void UpdateTitleScene (TitleScene* titleScene)
{
    if (IsKeyPressed(KEY_F))
    {
        titleScene->gameData->playerCount = 1;
        LoadScene (titleScene->gameData, E_GAMEPLAY, true);
    }
    else if (IsKeyPressed(KEY_K))
    {
        titleScene->gameData->playerCount = 2;
        LoadScene (titleScene->gameData, E_GAMEPLAY, true);
    }
    else if (IsKeyPressed(KEY_ESCAPE))
        titleScene->gameData->exitCode = EXIT_GAME_END;
    
    DrawTitleScene (titleScene);
}

void DrawTitleScene ()
{
    char const* text = "MAIN MENU";
    float       posY = WINDOW_HEIGHT /3 - FONT_SIZE_TITLE / 2;


    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_TITLE) / 2, posY,
        FONT_SIZE_TITLE, WHITE);

    text = LAUNCH_SOLO_GAME;
    posY += 3 * FONT_SIZE_TITLE;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_NORMAL) / 2, posY,
        FONT_SIZE_NORMAL, BLUE);

    text = LAUNCH_MULTIPLAYER_GAME;
    posY += 2 * FONT_SIZE_NORMAL;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_NORMAL) / 2, posY,
        FONT_SIZE_NORMAL, GREEN);
    
    text = TextFormat(EXIT_GAME, EXIT_KEY_TEXT);
    posY += 2 * FONT_SIZE_NORMAL;

    DrawText(text, WINDOW_WIDTH / 2 - MeasureText(text, FONT_SIZE_NORMAL) / 2, posY,
        FONT_SIZE_NORMAL, RED);
}