#include <stdlib.h>
#include "raylib.h"

#include "errors.h"
#include "assets.h"
#include "sprites.h"
#include "audio.h"
#include "scene.h"
#include "game.h"
#include "fireball.h"
#include "player.h"
#include "mines.h"
#include "minelayer.h"
#include "collide.h"
#include "gameplay.h"

GameplayData* CreateGameplayData()
{
    GameplayData* gameplayData = malloc(sizeof(GameplayData));

    if (gameplayData == NULL)
        return NULL;

    gameplayData->level = 1;
    gameplayData->score = 0;
    gameplayData->isPaused = false;
    gameplayData->drawCollisions = false;

    return gameplayData;
}

GameplayScene* CreateGameplayScene()
{
    GameplayScene*  scene = malloc(sizeof(Scene));

    if (!scene)
        return NULL;

    scene->id = E_GAMEPLAY;

    scene->init = InitGameplayScene;
    scene->update = UpdateGameplayScene;
    scene->unload = UnloadGameplayScene;

    return scene;
}

void InitGameplayScene(GameplayScene* scene)
{
    if (scene == NULL)
        return;

    if (scene->gameData->playerCount > MAX_PLAYER_COUNT)
    {
        scene->gameData->exitCode = INVALID_PLAYER_COUNT;
        return;
    }

    Sprite* playerSprite = GetSprite(scene->gameData->sprites, TEXTURE_PLAYER);
    Sprite* pauseSprite = GetSprite(scene->gameData->sprites, TEXTURE_PAUSE);
    Sprite* playSprite = GetSprite(scene->gameData->sprites, TEXTURE_PLAY);
    Sprite* enemySprite = GetSprite(scene->gameData->sprites, TEXTURE_MINES);
    Sprite* mineLayerSprite = GetSprite(scene->gameData->sprites, TEXTURE_MINE_LAYER);

    if (playerSprite == NULL || enemySprite == NULL || pauseSprite == NULL
        || playSprite == NULL || mineLayerSprite == NULL)
    {
        scene->gameData->exitCode = SPRITE_LOAD_FAILED;
        return;
    }

    Sfx*    shootSfx = GetSfx(scene->gameData->sounds, SFX_SHOOT);
    Sfx*    explosionSfx = GetSfx(scene->gameData->sounds, SFX_EXPLOSION);
    Sfx*    tpSfx = GetSfx(scene->gameData->sounds, SFX_TP);

    if (shootSfx == NULL || explosionSfx == NULL || tpSfx == NULL)
    {
        scene->gameData->exitCode = SFX_LOADING_FAILED;
        return;
    }

    GameplayData*   gameplayData = CreateGameplayData();

    if (gameplayData == NULL)
    {
        scene->gameData->exitCode = GAMEPLAY_DATA_CREATION_FAILED;
        return;
    }

    scene->sceneData = gameplayData;

    CreatePlayers(gameplayData, scene->gameData->playerCount,
        playerSprite->texture, tpSfx->sound, explosionSfx->sound);

    if (gameplayData->players == NULL)
    {
        scene->gameData->exitCode = PLAYERS_CREATION_FAILED;
        return;
    }

    CreateMines(gameplayData, enemySprite->texture, explosionSfx->sound);

    if (gameplayData->mines == NULL)
    {
        scene->gameData->exitCode = MINES_CREATION_FAILED;
        return;
    }

    gameplayData->fireballPool = CreateFireballPool(FIREBALL_POOL_SIZE,
        shootSfx->sound);

    CreateMineLayer(gameplayData, mineLayerSprite->texture, explosionSfx->sound);

    gameplayData->pauseTexture = pauseSprite->texture;
    gameplayData->playTexture = playSprite->texture;
    gameplayData->playersCount = scene->gameData->playerCount;
}

GameplayData* GetGameplayData(GameplayScene const* gameplayScene)
{
    if (gameplayScene != NULL)
        return (GameplayData*)(gameplayScene->sceneData);

    return NULL;
}

GameData* GetGameData(GameplayScene const* gameplayScene)
{
    if (gameplayScene != NULL)
        return gameplayScene->gameData;

    return NULL;
}

void UpdateGameplayScene(GameplayScene* scene)
{
    if (scene == NULL || scene->sceneData == NULL
        || scene->gameData->exitCode != EXIT_SUCCESS)
        return;

    GameplayData*   gameplayData = GetGameplayData(scene);

    if (IsKeyPressed(PAUSE_KEY))
    {
        gameplayData->isPaused = !gameplayData->isPaused;

        if (gameplayData->isPaused)
            LoadScene(scene->gameData, E_PAUSE, false);
        else
            UnloadScene(scene->gameData, E_PAUSE);
    }

    if (IsKeyPressed(SHOW_COL_KEY))
        gameplayData->drawCollisions = !gameplayData->drawCollisions;

#ifdef DEBUG
    HandleCheats(scene);
#endif

    if (scene->gameData->exitCode == EXIT_SUCCESS && scene->isActive)
        UpdatePlayers(scene);

    if (scene->gameData->exitCode == EXIT_SUCCESS && scene->isActive)
        UpdateMines(scene);

    if (scene->gameData->exitCode == EXIT_SUCCESS && scene->isActive)
        scene->gameData->exitCode = HandleFireballs(gameplayData);

    if (scene->gameData->exitCode == EXIT_SUCCESS && scene->isActive)
        scene->gameData->exitCode = HandleMineLayer(gameplayData);

    if (scene->gameData->exitCode == EXIT_SUCCESS && scene->isActive)
        HandleCollision(gameplayData);
}

#ifdef DEBUG
void HandleCheats(GameplayScene* scene)
{
    if (scene == NULL || scene->sceneData == NULL
        || scene->gameData->exitCode != EXIT_SUCCESS)
        return;

    GameplayData*   gameplayData = GetGameplayData(scene);

    if (gameplayData == NULL)
        return; 

    if (IsKeyDown(KEY_LEFT_SHIFT))
    {
        if (IsKeyPressed(KEY_M))
        {
            for (ubyte i = 0; i < gameplayData->minesCount; i++)
            {
                if (gameplayData->mines[i].state != E_DESTROYED
                    && gameplayData->mines[i].state != E_SPAWNER)
                {
                    KillMine(&gameplayData->mines[i], gameplayData, true);
                    break;
                }
            }
        }

        if (IsKeyPressed(KEY_P))
        {
            for (ubyte i = 0; i < gameplayData->playersCount; i++)
            {
                if (gameplayData->players[i].lives > 0)
                {
                    KillPlayer(&gameplayData->players[i]);
                    break;
                }
            }
        }

        if (IsKeyPressed(KEY_H))
            for (ubyte i = 0; i < gameplayData->playersCount; i++)
                gameplayData->players[i].lives = PLAYER_HEALTH;
    }
    else if (IsKeyDown(KEY_LEFT_ALT))
    {
        if (IsKeyPressed(KEY_M))
            for (ubyte i = 0; i < gameplayData->minesCount; i++)
                if (gameplayData->mines[i].state != E_DESTROYED
                    && gameplayData->mines[i].state != E_SPAWNER)
                    KillMine(&gameplayData->mines[i], gameplayData, true);

        if (IsKeyPressed(KEY_P))
            for (ubyte i = 0; i < gameplayData->playersCount; i++)
                if (gameplayData->players[i].lives > 0)
                    KillPlayer(&gameplayData->players[i]);
    }
}
#endif

void UpdatePlayers(GameplayScene* scene)
{
    if (scene == NULL || scene->sceneData == NULL
        || scene->gameData->exitCode != EXIT_SUCCESS)
        return;

    GameplayData*   gameplayData = GetGameplayData(scene);

    ubyte   deadPlayersCount = 0;

    for (ubyte i = 0; i < scene->gameData->playerCount; i++)
    {
        if (scene->gameData->exitCode != EXIT_SUCCESS)
            break;

        if (gameplayData->isPaused)
            scene->gameData->exitCode = DrawPlayer(&gameplayData->players[i]);
        else
            scene->gameData->exitCode = HandlePlayer(&gameplayData->players[i], gameplayData->fireballPool);

        if (gameplayData->players[i].lives == 0)
            deadPlayersCount++;
    }

    if (deadPlayersCount == scene->gameData->playerCount)
        LoadScene(scene->gameData, E_GAME_OVER, true);
}

void UpdateMines(GameplayScene* scene)
{
    if (scene->gameData->exitCode != EXIT_SUCCESS)
        return;

    GameplayData*   gameplayData = GetGameplayData(scene);

    ubyte   destroyedMinesCount = 0;
    bool    shouldForceSpawn = true;

    for (ubyte i = 0; i < gameplayData->minesCount; i++)
    {
        if (scene->gameData->exitCode != EXIT_SUCCESS)
            break;

        scene->gameData->exitCode = HandleMine(&gameplayData->mines[i], scene);

        if (gameplayData->mines[i].state == E_DESTROYED)
            destroyedMinesCount++;

        if (shouldForceSpawn && gameplayData->mines[i].state != E_DESTROYED
            && gameplayData->mines[i].state != E_SPAWNER)
            shouldForceSpawn = false;
    }

    if (shouldForceSpawn)
    {
        Mine* spawner = GetSpawner(gameplayData->mines, gameplayData->minesCount);

        if (spawner)
            spawner->state = GetMineStateForLevel(gameplayData->level);
    }

    if (destroyedMinesCount == gameplayData->minesCount
        && (gameplayData->mineLayer == NULL || !gameplayData->mineLayer->isActive))
        StartNextLevel(scene);
    else if (gameplayData->mineLayer->canSpawn && !gameplayData->mineLayer->isActive
        && destroyedMinesCount > MIN_LAYER_DESTROYED_MINES)
        InitMineLayer(gameplayData->mineLayer);
}

void StartNextLevel(GameplayScene* scene)
{
    GameplayData* gameplayData = GetGameplayData(scene);

    if (gameplayData == NULL)
        return;

    gameplayData->level++;

    DisableAllFireballs(gameplayData->fireballPool);

    Sound       tpSound = gameplayData->players->tpSound;
    Sound       explosionSound = gameplayData->players->deathSound;

    float spawnX;
    for (ubyte i = 0; i < gameplayData->playersCount; i++)
    {
        InitPlayer(gameplayData->players + i, gameplayData->players[i].texture, i, tpSound,
            gameplayData->players[i].deathSound);

        spawnX = MIN_X + (i + 1) * (MAX_X - MIN_X) / (gameplayData->playersCount + 1);
        gameplayData->players[i].position.x = spawnX;
    }

    Texture2D   minesTexture = gameplayData->mines->texture;

    free(gameplayData->mines);
    CreateMines(gameplayData, minesTexture, explosionSound);

    gameplayData->mineLayer->isActive = false;
    gameplayData->mineLayer->canSpawn = true;
}

void HandleCollision(GameplayData* gameplayData)
{
    if (gameplayData == NULL)
        return;

    HandlePlayersCollisions(gameplayData);
    HandleMinesCollisions(gameplayData);
    HandleFireballsCollisions(gameplayData);
}

void HandlePlayersCollisions(GameplayData* gameplayData)
{
    for (ubyte i = 0; i < gameplayData->playersCount; i++)
    {
        if (gameplayData->players[i].lives == 0)
            continue;

        size_t    playerColPtsCount = 0;
        Vector2*  playerColPts =
            GetPlayerColPts(&gameplayData->players[i], &playerColPtsCount, gameplayData->drawCollisions);

        if (playerColPts == NULL)
            continue;

        size_t    otherColPtsCount = 0;
        Vector2*  otherColPts = NULL;

        for (ubyte j = 0; j < gameplayData->minesCount; j++)
        {
            if (gameplayData->mines[j].state == E_SPAWNER || gameplayData->mines[j].state == E_DESTROYED)
                continue;

            otherColPts = GetMineColPts(&gameplayData->mines[j], &otherColPtsCount, gameplayData->drawCollisions);

            if (otherColPts != NULL && CheckCollisionPolygons(playerColPts,
                playerColPtsCount, otherColPts, otherColPtsCount))
            {
                KillPlayer(&gameplayData->players[i]);
                KillMine(&gameplayData->mines[j], gameplayData, false);
            }
        }

        for (unsigned int j = 0; j < gameplayData->fireballPool->count; j++)
        {
            if (gameplayData->players[i].lives == 0)
                break;

            if (gameplayData->fireballPool->fireballs[j].isActive
                && !gameplayData->fireballPool->fireballs[j].isPlayer
                && CheckCollisionCirclePolygon(gameplayData->fireballPool->fireballs[j].pos,
                gameplayData->fireballPool->fireballs[j].radius, playerColPts,
                playerColPtsCount))
            {
                KillPlayer(&gameplayData->players[i]);
                DisableFireball(&gameplayData->fireballPool->fireballs[j]);
            }
        }

        if (gameplayData->players[i].lives > 0 && gameplayData->mineLayer != NULL
            && gameplayData->mineLayer->isActive)
        {
            otherColPts = GetMineLayerColPts(gameplayData->mineLayer, &otherColPtsCount, gameplayData->drawCollisions);

            if (otherColPts != NULL && CheckCollisionPolygons(playerColPts,
                playerColPtsCount, otherColPts, otherColPtsCount))
            {
                KillPlayer(&gameplayData->players[i]);
                KillMineLayer(gameplayData->mineLayer);
            }
        }

        free(playerColPts);
        free(otherColPts);
    }
}

void HandleMinesCollisions(GameplayData* gameplayData)
{
    for (ubyte i = 0; i < gameplayData->minesCount; i++)
    {
        if (gameplayData->mines[i].state == E_SPAWNER || gameplayData->mines[i].state == E_DESTROYED)
            continue;

        size_t    mineColPtsCount = 0;
        Vector2*  mineColPts =
            GetMineColPts(&gameplayData->mines[i], &mineColPtsCount, gameplayData->drawCollisions);

        if (mineColPts == NULL)
            continue;

        for (unsigned int j = 0; j < gameplayData->fireballPool->count; j++)
        {
            if (gameplayData->fireballPool->fireballs[j].isActive
                && gameplayData->fireballPool->fireballs[j].isPlayer
                && CheckCollisionCirclePolygon(gameplayData->fireballPool->fireballs[j].pos,
                gameplayData->fireballPool->fireballs[j].radius, mineColPts,
                mineColPtsCount))
            {
                if (gameplayData->mines[i].state == E_BOSS
                    && gameplayData->mines[i].size == E_TALL)
                {
                    for (ubyte k = 0; k < gameplayData->playersCount; k++)
                    {
                        gameplayData->players[k].lives =
                            MIN(gameplayData->players[k].lives + 1, PLAYER_HEALTH);
                    }
                }

                KillMine(&gameplayData->mines[i], gameplayData, true);
                DisableFireball(&gameplayData->fireballPool->fireballs[j]);
            }
        }

        free(mineColPts);
    }
}

void HandleFireballsCollisions(GameplayData* gameplayData)
{
    if (gameplayData == NULL || gameplayData->fireballPool == NULL || gameplayData->fireballPool->count == 0)
        return;

    for (unsigned int i = 0; i < gameplayData->fireballPool->count; i++)
    {
        if (!gameplayData->fireballPool->fireballs[i].isActive)
            continue;

        for (unsigned int j = 0; j < gameplayData->fireballPool->count; j++)
        {
            if (gameplayData->fireballPool->fireballs[j].isActive
                && gameplayData->fireballPool->fireballs[i].isPlayer !=
                gameplayData->fireballPool->fireballs[j].isPlayer
                && CheckColCircles(gameplayData->fireballPool->fireballs[i].pos,
                gameplayData->fireballPool->fireballs[i].radius,
                gameplayData->fireballPool->fireballs[j].pos,
                gameplayData->fireballPool->fireballs[j].radius))
            {
                gameplayData->score += FIREBALL_SCORE;
                DisableFireball(&gameplayData->fireballPool->fireballs[i]);
                DisableFireball(&gameplayData->fireballPool->fireballs[j]);
            }
        }

        if (gameplayData->mineLayer != NULL && gameplayData->mineLayer->isActive
            && gameplayData->fireballPool->fireballs[i].isPlayer)
        {
            size_t      otherColPtsCount = 0;
            Vector2*    otherColPts = GetMineLayerColPts(gameplayData->mineLayer,
                                &otherColPtsCount, gameplayData->drawCollisions);

            if (otherColPts != NULL && CheckCollisionCirclePolygon(
                gameplayData->fireballPool->fireballs[i].pos,
                gameplayData->fireballPool->fireballs[i].radius,
                otherColPts, otherColPtsCount))
            {
                gameplayData->score += MINE_LAYER_SCORE;
                DisableFireball(&gameplayData->fireballPool->fireballs[i]);
                KillMineLayer(gameplayData->mineLayer);
            }

            free(otherColPts);
        }
    }
}

void UnloadGameplayScene(GameplayScene* scene)
{
    GameplayData*   gameplayData = GetGameplayData(scene);

    if (gameplayData != NULL)
    {
        if (gameplayData->players)
            free(gameplayData->players);
        
        if (gameplayData->mines)
            free(gameplayData->mines);

        if (gameplayData->mineLayer)
            free(gameplayData->mineLayer);

        if (gameplayData->fireballPool)
            DestroyFireballPool(&gameplayData->fireballPool);

        free(gameplayData);
    }
}