#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

#include "errors.h"
#include "game.h"
#include "gameplay.h"
#include "mines.h"
#include "minelayer.h"

void InitMineLayer(MineLayer* mineLayer)
{
    if (mineLayer == NULL)
        return;

    int spawnX = GetRandomValue(0 , OFFSET);

    if (GetRandomValue (0, 1) == 0)
        spawnX = MIN_X - spawnX;
    else
        spawnX = MAX_X + spawnX;

    int spawnY = GetRandomValue(0 , OFFSET);

    if (GetRandomValue (0, 1) == 0)
        spawnY = MIN_Y - spawnY;
    else
        spawnY = MAX_Y + spawnY;

    mineLayer->pos = (Vector2) {spawnX, spawnY};
    mineLayer->isActive = true;
    mineLayer->canSpawn = false;
    mineLayer->remainingMines = MINE_LAYER_MINES_COUNT;

    SetMineLayerTarget(mineLayer);
}

void MoveMineLayer(MineLayer* mineLayer)
{
    if (mineLayer != NULL)
    {
        // Vector2 speed = Vector2Scale(mineLayer->dir, 180 * GetFrameTime());
        // mineLayer->pos.x += speed.x;
        // mineLayer->pos.y += speed.y;
        mineLayer->pos = Vector2MoveTowards(mineLayer->pos, mineLayer->target, 180 * GetFrameTime());
        // mineLayer->pos.x = Wrap (mineLayer->pos.x, MIN_X, MAX_X);
        // mineLayer->pos.y = Wrap (mineLayer->pos.y, MIN_Y, MAX_Y);
    }
}

void SetMineLayerTarget(MineLayer* mineLayer)
{
    if (mineLayer != NULL)
    {
        mineLayer->target.x = GetRandomValue(MIN_X + OFFSET, MAX_X - OFFSET);
        mineLayer->target.y = GetRandomValue(MIN_Y + OFFSET, MAX_Y - OFFSET);

        mineLayer->dir = Vector2Normalize(Vector2Subtract (mineLayer->target, mineLayer->pos));
    }
}

void LayMine(GameplayData* gameplayData)
{
    if ( gameplayData != NULL && gameplayData->mineLayer != NULL
        && gameplayData->mineLayer->isActive && gameplayData->mineLayer->remainingMines > 0)
    {
        gameplayData->minesCount++;
        gameplayData->mines = realloc(gameplayData->mines,
            gameplayData->minesCount * sizeof(Mine));

        InitMine(&gameplayData->mines[gameplayData->minesCount - 1],
            gameplayData->mines[0].texture, gameplayData->mines[0].deathSound);

        gameplayData->mines[gameplayData->minesCount - 1].pos =
            gameplayData->mineLayer->pos;

        gameplayData->mineLayer->remainingMines--;

        if (gameplayData->mineLayer->remainingMines > 0)
            SetMineLayerTarget(gameplayData->mineLayer);
        else
            gameplayData->mineLayer->isActive = false;
    }
}

int HandleMineLayer (GameplayData* gameplayData)
{
    if ( gameplayData->mineLayer == NULL)
        return NULL_MINE_LAYER;

    if (!gameplayData->mineLayer->isActive)
        return EXIT_SUCCESS;

    if (!gameplayData->isPaused)
    {
        gameplayData->mineLayer->dir = Vector2Normalize(Vector2Subtract (
            gameplayData->mineLayer->target, gameplayData->mineLayer->pos));

        MoveMineLayer(gameplayData->mineLayer);

        if (FloatEquals(gameplayData->mineLayer->pos.x, gameplayData->mineLayer->target.x)
        && FloatEquals(gameplayData->mineLayer->pos.y, gameplayData->mineLayer->target.y))
            LayMine(gameplayData);
    }

    DrawTextureEx(gameplayData->mineLayer->texture, gameplayData->mineLayer->pos, 0, MINE_LAYER_SCALE, WHITE);

    return EXIT_SUCCESS;
}

void CreateMineLayer (GameplayData* gameplayData, Texture2D texture,
    Sound deathSound)
{
    if (gameplayData == NULL || texture.id <= 0 || deathSound.stream.buffer == NULL)
        return;

    gameplayData->mineLayer = malloc(sizeof(MineLayer));

    if (gameplayData->mineLayer == NULL)
        return;

    gameplayData->mineLayer->isActive = false;
    gameplayData->mineLayer->canSpawn = true;
    gameplayData->mineLayer->texture = texture;
    gameplayData->mineLayer->deathSound = deathSound;
    gameplayData->mineLayer->remainingMines = MINE_LAYER_MINES_COUNT;
}

Vector2* GetMineLayerColPts (MineLayer* mineLayer, size_t* pointsCount, bool drawCollisions)
{
  if (mineLayer == NULL)
    {
        *pointsCount = 0;
        return NULL;
    }

    Vector2* colPoints = MINE_LAYER_COL_PTS;

    if (colPoints)
    {
        *pointsCount = MINE_LAYER_COL_PTS_COUNT;

        Vector2* res = malloc(*pointsCount * sizeof(Vector2));

        if (res == NULL)
            return NULL;

        for (size_t i = 0; i < *pointsCount; i++)
        {
            res[i].x = mineLayer->pos.x + colPoints[i].x
                * mineLayer->texture.width * MINE_LAYER_SCALE;
            res[i].y = mineLayer->pos.y + colPoints[i].y
                * mineLayer->texture.height * MINE_LAYER_SCALE;
        }
        if (drawCollisions)
            for (size_t i = 0; i < *pointsCount; i++)
                DrawLineV(res[i], res[(i+1)%*pointsCount], PINK);

        return res;
    }
    return colPoints;
}

void KillMineLayer(MineLayer* mineLayer)
{
    if (mineLayer == NULL || !mineLayer->isActive)
        return;

    mineLayer->isActive = false;

    PlaySound(mineLayer->deathSound);
}