#include <stdlib.h> // EXIT_SUCCESS
#include "raylib.h"

#include "assets.h"
#include "sprites.h"
#include "game.h"
#include "scene.h"
#include "gameplay.h"
#include "player.h"
#include "ui.h"

void DrawBackground(void)
{
    float bgWidth = MAX_X - MIN_X;
    float bgHeight = MAX_Y - MIN_Y;

    DrawRectangleGradientV(MIN_X, MIN_Y, bgWidth, bgHeight,
        BACKGROUND_COLOR_ONE, BACKGROUND_COLOR_TWO);

    int     linesCount = bgHeight / BACKGROUND_GRID_SPACING;
    int     colCount = bgWidth / BACKGROUND_GRID_SPACING;
    float   curPos;

    for (int i = 0; i < linesCount; i++)
    {
        curPos = MIN_Y + (i + 1) * bgHeight / linesCount;
        DrawLine(MIN_X, curPos, MAX_X, curPos, BACKGROUND_GRID_COLOR);
    }

    for (int i = 0; i < colCount; i++)
    {
        curPos = MIN_X + (i + 1) * bgWidth / colCount;
        DrawLine(curPos, MIN_Y, curPos, MAX_Y, BACKGROUND_GRID_COLOR);
    }
}

void DrawOverlay(GameData const* gameData)
{
    if (gameData->exitCode != EXIT_SUCCESS)
        return;

    Rectangle   playableRect = { MIN_X, MIN_Y,
        MAX_X - MIN_X, MAX_Y + BOTTOM_AREA_HEIGHT - MIN_Y };

    // Draw the border
    DrawRectangleRoundedLines(playableRect, BORDER_ROUNDNESS, 0, BORDER_SIZE,
        BORDER_COLOR);

    DrawRectangleRoundedLines(playableRect, BORDER_ROUNDNESS, 0, INNER_BORDER_SIZE,
        INNER_BORDER_COLOR);

    // Draw the bottom part of the overlay
    Rectangle   bottomRect = { MIN_X - INNER_BORDER_SIZE, MAX_Y,
        playableRect.width + INNER_BORDER_SIZE * 2,
        BOTTOM_AREA_HEIGHT + INNER_BORDER_SIZE};

    DrawRectangleRounded(bottomRect, BOTTOM_AREA_ROUNDNESS, 0, BOTTOM_AREA_COLOR);

    // Trick to make the rectangle rounded only on the bottom
    bottomRect.height /= 2;
    DrawRectangleRec(bottomRect, BOTTOM_AREA_COLOR);

    bottomRect.height = BOTTOM_LINE_HEIGHT;
    DrawRectangleRec(bottomRect, BOTTOM_LINE_COLOR);

    DrawHUD(gameData);

    Sprite* logoSprite = GetSprite(gameData->sprites, TEXTURE_LOGO);

    if (logoSprite != NULL)
    {
        float   logoPosX = WINDOW_WIDTH / 2 - logoSprite->texture.width / 2;
        DrawTexture(logoSprite->texture, logoPosX,
            MAX_Y - logoSprite->texture.height, Fade(WHITE, OVERLAY_ALPHA));
    }
}

void DrawHUD(GameData const* gameData)
{
    if (gameData == NULL)
        return;

    GameplayData*   gameplayData = NULL;
    Sprite*         playerSprite;
    Texture2D       playerTexture = (Texture) {.id = 0};

    playerSprite = GetSprite(gameData->sprites, TEXTURE_PLAYER);
    
    GameplayScene* gameplayScene = (GameplayScene*) FindScene(gameData->scenes, E_GAMEPLAY);

    if (playerSprite != NULL)
        playerTexture = playerSprite->texture;

    if (gameplayScene != NULL && gameplayScene->isActive)
        DrawGameplayInfo((gameplayData = GetGameplayData(gameplayScene)),
            gameData->playerCount, playerTexture);

    if (gameplayData == NULL)
    {
        DrawPlayerInfo( PLAYER_ONE_INFO_X, playerTexture, 1,
            PLAYER_HEALTH, &PLAYER_ONE_KEYS);

        DrawPlayerInfo( PLAYER_TWO_INFO_X, playerTexture, 2,
            PLAYER_HEALTH, &PLAYER_TWO_KEYS);
    }
}

void DrawGameplayInfo(GameplayData* gameplayData, ubyte playersCount,
    Texture2D playerTexture)
{
    if (gameplayData == NULL)
        return;

    float player1PosX = WINDOW_WIDTH / 2 - PLAYER_INFO_WIDTH / 2;

    if (playersCount > 1)
    {
        player1PosX = PLAYER_ONE_INFO_X;

        DrawPlayerInfo( PLAYER_TWO_INFO_X, playerTexture, 2,
            gameplayData->players[1].lives,
            gameplayData->isPaused ? &PLAYER_TWO_KEYS : NULL);
    }

    DrawPlayerInfo(player1PosX, playerTexture, 1,
        gameplayData->players[0].lives,
        gameplayData->isPaused ? &PLAYER_ONE_KEYS : NULL);

    char const* scoreText = TextFormat(SCORE_TEXT, gameplayData->score);
    float       elemPosX = (WINDOW_WIDTH - MeasureText(scoreText, FONT_SIZE_NORMAL)) / 2;
    float       elemPosY = MAX_Y + FONT_SIZE_NORMAL / 2;

    scoreText = TextToUpper(scoreText);
    DrawText(scoreText, elemPosX, elemPosY, FONT_SIZE_NORMAL, RAYWHITE);

    Texture2D pausePlayTexture = gameplayData->isPaused ?
        gameplayData->playTexture : gameplayData->pauseTexture;

    elemPosX = MAX_X - pausePlayTexture.width
        - MeasureText(PAUSE_KEY_TEXT, FONT_SIZE_SMALL)
        - FONT_SIZE_SMALL * 3/2;

    elemPosY = MAX_Y + BOTTOM_LINE_HEIGHT
        + (BOTTOM_AREA_HEIGHT - BOTTOM_LINE_HEIGHT) / 2
        - pausePlayTexture.height / 2;

    DrawTexture(pausePlayTexture, elemPosX, elemPosY, PAUSE_KEY_TEXT_COLOR);

    elemPosY = MAX_Y + FONT_SIZE_SMALL / 2;
    
    elemPosX += pausePlayTexture.width + FONT_SIZE_SMALL / 2;
    DrawText(PAUSE_KEY_TEXT, elemPosX, elemPosY, FONT_SIZE_SMALL,
        PAUSE_KEY_TEXT_COLOR);
}

void DrawPlayerInfo(float posX, Texture2D texture, ubyte playerNum,
    ubyte lives, PlayerKeyMap const* keyMap)
{
    float posY = PLAYER_INFO_Y;

    Rectangle   playerInfoRect = { posX, posY,
        PLAYER_INFO_WIDTH, PLAYER_INFO_HEIGHT};

    Color color = playerNum == 1 ? PLAYER_ONE_COLOR : PLAYER_TWO_COLOR;

    DrawRectangleRounded(playerInfoRect, PLAYER_INFO_ROUNDNESS, 0,
        Fade(color, OVERLAY_ALPHA));

    posY += FONT_SIZE_NORMAL / 2;
    char const* text = TextFormat("Player %d", playerNum);
    float       elemPosX = posX + PLAYER_INFO_WIDTH / 2
        - MeasureText(text, FONT_SIZE_NORMAL) / 2;

    DrawText(text, elemPosX, posY, FONT_SIZE_NORMAL, PLAYER_INFO_TEXT_COLOR);

    posY += FONT_SIZE_NORMAL * 3 / 2;

    DrawLives((Vector2) {posX, posY}, texture, lives);

    if (keyMap != NULL)
        DrawKeyMap(*keyMap, (Vector2){ posX, PLAYER_KEYS_TEXT_Y });
}

void DrawLives(Vector2 pos, Texture2D texture, ubyte lives)
{
    if (texture.id > 0)
        pos.y += FONT_SIZE_NORMAL;

    for (ubyte i = 0; i < lives; i++)
    {
        if (texture.id > 0)
        {
            float textureScale = FONT_SIZE_NORMAL / (float) texture.width;

            float elemPosX = pos.x + (i + 1) * PLAYER_INFO_WIDTH / (lives + 1)
                - texture.height * textureScale / 2;

            DrawTextureEx(texture, (Vector2) {elemPosX, pos.y}, -90, textureScale,
                PLAYER_INFO_TEXT_COLOR);
        }
        else
        {
            float elemPosX = pos.x + (i + 1) * PLAYER_INFO_WIDTH / (lives + 1)
                - MeasureText("A", FONT_SIZE_NORMAL) / 2;

            DrawText("A", elemPosX, pos.y, FONT_SIZE_NORMAL,
                PLAYER_INFO_TEXT_COLOR);
        }
    }
}

void DrawKeyMap(PlayerKeyMap keyMap, Vector2 pos)
{
    char const*   keyMapText = TextFormat(PLAYER_KEYS_TEXT, keyMap.left, keyMap.right,
        keyMap.forward, keyMap.shoot, keyMap.primaryTeleport,
        keyMap.secondaryTeleport);

    pos.x += PLAYER_INFO_WIDTH / 2 - MeasureText(keyMapText, FONT_SIZE_SMALL) / 2;

    DrawText(keyMapText, pos.x, pos.y,
        FONT_SIZE_SMALL, PLAYER_KEYS_TEXT_COLOR);
}