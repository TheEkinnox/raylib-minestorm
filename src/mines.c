#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

#include "errors.h"
#include "gameplay.h"
#include "game.h"
#include "player.h"
#include "fireball.h"
#include "mines.h"

void InitMine(Mine* mine, Texture2D texture, Sound deathSound)
{
    if (mine == NULL || texture.id <= 0)
        return;

    int spawnX = GetRandomValue(MIN_X + OFFSET ,MAX_X - OFFSET);
    int spawnY = GetRandomValue(MIN_Y + OFFSET ,MAX_Y - OFFSET);
    int dirX = GetRandomValue(0, 1);
    int dirY = GetRandomValue(0, 1);

    if (dirX == 0)
        dirX = -1;

    if(dirY == 0)
        dirY = -1;

    mine->dir = Vector2Normalize((Vector2) {dirX, dirY}); // Random direction
    mine->pos = (Vector2) {spawnX, spawnY}; // Random positions
    mine->state = E_SPAWNER;
    mine->size = E_TALL;
    mine->texture = texture;
    mine->deathSound = deathSound;
}


void MoveMine(Mine* mine)
{
    if (mine != NULL && mine->state != E_SPAWNER && mine->state != E_DESTROYED)
    {
        Vector2 speed = Vector2Scale(mine->dir,GetMineSpeed(mine) * GetFrameTime());
        mine->pos.x += speed.x;
        mine->pos.y += speed.y;
        mine->pos.x = Wrap (mine->pos.x, MIN_X, MAX_X);
        mine->pos.y = Wrap (mine->pos.y, MIN_Y, MAX_Y);
    }
}

float GetMineSpeed(Mine* mine)
{
    if (mine->size == E_SMALL)
        return SMALL_MINE_SPEED;
    else if (mine->size == E_MEDIUM)
        return MEDIUM_MINE_SPEED;
    else if (mine->size == E_TALL)
        return TALL_MINE_SPEED;
    else if (mine->state == E_BOSS)
        return GIANT_MINE_SPEED;

    return 0;
}

void MagneticMine(Mine* mine, Player* players, ubyte nbPlayers)
{
    if (mine != NULL && players != NULL)
    {
        Player* closestPlayer = NULL;
        float playerDistance, minDistance;

        for (ubyte i = 0; i < nbPlayers; i++)
        {
            if (players[i].lives == 0)
                continue;

            playerDistance = Vector2DistanceSqr (mine->pos, players[i].position);

            if ( closestPlayer == NULL || playerDistance < minDistance )
            {
                minDistance = playerDistance;
                closestPlayer = players + i;
            }
        }

        if (closestPlayer != NULL)
        {
            mine->dir = Vector2Subtract(closestPlayer->position, mine->pos);
            mine->dir = Vector2Normalize(mine->dir);
        }
    }
}

float GetMineScale (Minesize size)
{
    switch (size)
    {
    case E_GIANT:
        return 1.5f;
    case E_TALL:
        return 1;
    case E_MEDIUM:
        return 0.8f;
    case E_SMALL:
        return 0.55f;
    default:
        return 0;
    }
}

void DrawMine(Mine* mine)
{
    if (mine->state == E_SPAWNER)
    {
        DrawCircle(mine->pos.x, mine->pos.y, 2 ,WHITE);
    }
    else if (mine->state != E_DESTROYED)
    {
        Rectangle textureRec =
        {
            .x = mine->state * mine->texture.width / E_SPAWNER,
            .y = 0,
            .width = mine->texture.width / E_SPAWNER,
            .height = mine->texture.height
        };
        Rectangle scaledRec =
        {
            .x = mine->pos.x,
            .y = mine->pos.y,
            .width =textureRec.width * GetMineScale (mine->size),
            .height = textureRec.height * GetMineScale (mine->size)
        };
        DrawTexturePro (mine->texture, textureRec, scaledRec, Vector2Zero()
            , 0, WHITE);
    }
}

int HandleMine(Mine* mine, GameplayScene* scene)
{
    if (mine == NULL)
        return NULL_MINES;

    GameplayData* sceneData = GetGameplayData(scene);
    GameData* gameData = GetGameData(scene);

    if (!sceneData->isPaused)
    {
        if ( mine->state == E_MAGNETIC || mine->state == E_ALL
            || mine->state == E_BOSS)
            MagneticMine(mine, sceneData->players, gameData->playerCount);

        MoveMine(mine);
    }
    DrawMine(mine);
    return EXIT_SUCCESS;
}

void CreateMines (GameplayData* gameplayData, Texture2D texture, Sound deathSound)
{
    // ubyte minesCount = GetRandomValue (2 , 4);
    ubyte minesCount = 1;
    gameplayData->minesCount = (int)minesCount * 7;
    gameplayData->mines = malloc(gameplayData->minesCount * sizeof(Mine));

    if (gameplayData->mines == NULL)
        return;

    for (ubyte i = 0; i < gameplayData->minesCount; i++)
    {
        InitMine (gameplayData->mines + i, texture, deathSound);

        if (i < minesCount)
        {
            gameplayData->mines[i].state = GetMineStateForLevel(gameplayData->level);

            if (gameplayData->mines[i].state == E_BOSS)
            {
                gameplayData->minesCount += 8;
                gameplayData->mines = realloc(gameplayData->mines,
                    gameplayData->minesCount * sizeof(Mine));
                gameplayData->mines[i].size = E_GIANT;
            }
        }
    }
}

void KillMine (Mine* mine, GameplayData* gameplayData, bool givePoints)
{
    if (mine->state == E_DESTROYED || mine->state == E_SPAWNER)
        return;
        
    if (givePoints)
        gameplayData->score += GetMineScore(mine);

    if (mine->size > 0)
    {
        mine->size --;
        for (ubyte i = 0; i < 2; i++)
        {
            Mine* spawner = GetSpawner(gameplayData->mines,
                gameplayData->minesCount);

            if (spawner == NULL)
                break;

            spawner->state = mine->state;
            spawner->size = mine->size;
        }
    }

    for (ubyte i = 0; i < 2; i++)
    {
        if (mine->state == E_FIREBALL || mine->state == E_ALL
            || mine->state == E_BOSS)
        {
            MagneticMine(mine, gameplayData->players,
                gameplayData->playersCount);
            Vector2 fireballVelocity = (Vector2)
            {
            .x = mine->dir.x * MINE_FIREBALL_SPEED,
            .y = mine->dir.y * MINE_FIREBALL_SPEED
            };
            ShootFireball(gameplayData->fireballPool, RED, fireballVelocity,
                mine->pos, false);
        }
    }

    mine->state = E_DESTROYED;

    PlaySound(mine->deathSound);
}

unsigned int GetMineScore(Mine* mine)
{
    switch (mine->state)
    {
    case E_FLOATING:
        if (mine->size == E_SMALL)
            return 200;
        else if (mine->size == E_MEDIUM)
            return 135;
        else if (mine->size == E_TALL)
            return 100;
        break;
    case E_FIREBALL:
        if (mine->size == E_SMALL)
            return 425;
        else if (mine->size == E_MEDIUM)
            return 360;
        else if (mine->size == E_TALL)
            return 325;
        break;
    case E_MAGNETIC:
        if (mine->size == E_SMALL)
            return 600;
        else if (mine->size == E_MEDIUM)
            return 535;
        else if (mine->size == E_TALL)
            return 500;
        break;
    case E_ALL:
        if (mine->size == E_SMALL)
            return 850;
        else if (mine->size == E_MEDIUM)
            return 585;
        else if (mine->size == E_TALL)
            return 750;
        break;
        case E_BOSS:
    if (mine->size == E_SMALL)
        return 1000;
    else if (mine->size == E_MEDIUM)
        return 800;
    else if (mine->size == E_TALL)
        return 700;
    else if (mine->size == E_GIANT)
        return 750;
    break;
    default:
        break;
    }
    return 0;
}

Vector2* GetMineColPts(Mine* mine, size_t* pointsCount, bool drawCollisions)
{
    if (mine == NULL)
    {
        *pointsCount = 0;
        return NULL;
    }

    Vector2* colPoints = NULL;

    switch (mine->state)
    {
    case E_FLOATING:
        colPoints = FLOATING_MINE_COL_PTS;
        *pointsCount = FLOATING_MINE_COL_PTS_COUNT;
        break;
    case E_FIREBALL:
        colPoints = FIREBALL_MINE_COL_PTS;
        *pointsCount = FIREBALL_MINE_COL_PTS_COUNT;
        break;
    case E_MAGNETIC:
        colPoints = MAGNETIC_MINE_COL_PTS;
        *pointsCount = MAGNETIC_MINE_COL_PTS_COUNT;
        break;
    case E_ALL:
        colPoints = ALL_MINE_COL_PTS;
        *pointsCount = ALL_MINE_COL_PTS_COUNT;
        break;
    case E_BOSS:
        colPoints = BOSS_MINE_COL_PTS;
        *pointsCount = BOSS_MINE_COL_PTS_COUNT;
        break;
    default:
        break;
    }

    if (colPoints)
    {
        float scale = GetMineScale(mine->size);

        Vector2* res = malloc(*pointsCount * sizeof(Vector2));

        if (res == NULL)
            return NULL;

        for (size_t i = 0; i < *pointsCount; i++)
        {
            res[i].x = mine->pos.x + colPoints[i].x * (mine->texture.width / E_SPAWNER) * scale;
            res[i].y = mine->pos.y + colPoints[i].y * mine->texture.height * scale;
        }

        if (drawCollisions)
            for (size_t i = 0; i < *pointsCount; i++)
                DrawLineV(res[i], res[(i+1)%*pointsCount], PINK);

        return res;
    }

    return NULL;
}

Mine* GetSpawner (Mine* mines, ubyte minesCount)
{
    if (mines == NULL)
        return NULL;

    for (ubyte i = 0; i < minesCount; i++)
        if (mines[i].state == E_SPAWNER)
            return &mines[i];

    return NULL;
}

Minestate GetMineStateForLevel(unsigned int level)
{
    return GetRandomValue(E_FLOATING, MIN(level - 1, E_SPAWNER - 1));
}