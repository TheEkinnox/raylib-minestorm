#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

#include "errors.h"
#include "game.h"
#include "gameplay.h"
#include "fireball.h"
#include "player.h"

void CreatePlayers(GameplayData* gameplayData, ubyte playerCount,
    Texture2D playerTexture, Sound tpSound, Sound deathSound)
{
    gameplayData->players = malloc(playerCount * sizeof(Player));

    if (gameplayData->players == NULL)
        return;

    float   spawnX;

    for (ubyte i = 0; i < playerCount; i++)
    {
        InitPlayer(gameplayData->players + i, playerTexture, i, tpSound,
            deathSound);

        spawnX = MIN_X + (i + 1) * (MAX_X - MIN_X) / (playerCount + 1);
        gameplayData->players[i].position.x = spawnX;
    }
}

int InitPlayer(Player* player, Texture2D texture, ubyte playerIndex,
    Sound tpSound, Sound deathSound)
{
    if (player == NULL)
        return NULL_PLAYER;

    if (texture.id <= 0)
        return NULL_TEXTURE;

    player->texture = texture;
    player->color = playerIndex == 0 ? PLAYER_ONE_COLOR : PLAYER_TWO_COLOR;
    player->keyMap = playerIndex == 0 ? PLAYER_ONE_KEYS : PLAYER_TWO_KEYS;

    player->position = (Vector2) {
        .x = WINDOW_WIDTH / 2 - player->texture.width / 2,
        .y = WINDOW_HEIGHT / 2 - player->texture.height / 2
    };

    player->velocity = (Vector2) { .x = 0, .y = 0 };

    // direction and rotation are separated to avoid the
    // extra operations needed to get one from the other
    player->direction = (Vector2) { .x = 0, .y = -1 };
    player->rotation = -90;
    player->lives = PLAYER_HEALTH;
    player->tpCooldown = 0;

    player->tpSound = tpSound;
    player->deathSound = deathSound;

    return EXIT_SUCCESS;
}

int HandlePlayer(Player* player, FireballPool* fireballs)
{
    if (player == NULL)
        return NULL_PLAYER;

    if (player->lives == 0)
        return EXIT_SUCCESS;

    int result = HandlePlayerInput(player, fireballs);

    if (result == EXIT_SUCCESS)
        result = MovePlayer(player);

    if (result == EXIT_SUCCESS)
        result = DrawPlayer(player);

    return EXIT_SUCCESS;
}

int HandlePlayerInput(Player* player, FireballPool* fireballs)
{
    if (player == NULL)
        return NULL_PLAYER;

    float frameTime = GetFrameTime();

    if (player->tpCooldown > 0)
    {
        player->tpCooldown -= frameTime;
        return EXIT_SUCCESS;
    }

    player->velocity.x -= player->velocity.x * frameTime;
    player->velocity.y -= player->velocity.y * frameTime;

    bool rotateLeft, rotateRight;
    if ((rotateLeft = IsKeyDown(player->keyMap.left))
        || (rotateRight = IsKeyDown(player->keyMap.right)))
    {
        float rotationSpeed = PLAYER_ROTATION_SPEED * frameTime;
        rotationSpeed *= rotateLeft ? -1 : 1;

        player->rotation = fmodf(player->rotation + rotationSpeed, 360);
        player->direction.x = cosf(player->rotation * DEG2RAD);
        player->direction.y = sinf(player->rotation * DEG2RAD);
    }
    else if (IsKeyDown(player->keyMap.forward))
    {
        // Get the velocity magnitude for easier velocity manipulation
        float   velocityMagnitude = Vector2Length(player->velocity);

        // Add the acceleration to the magnitude and limit it to PLAYER_MAX_SPEED
        velocityMagnitude = MIN(velocityMagnitude + PLAYER_ACCELERATION * frameTime, PLAYER_MAX_SPEED);

        // Apply the new magnitude to the direction to have a per axis velocity
        player->velocity.x = player->direction.x * velocityMagnitude;
        player->velocity.y = player->direction.y * velocityMagnitude;
    }

    if (player->tpCooldown <= 0
        && (IsKeyPressed(player->keyMap.primaryTeleport)
        || IsKeyPressed(player->keyMap.secondaryTeleport)))
    {
        player->position.x = GetRandomValue(MIN_X, MAX_X);
        player->position.y = GetRandomValue(MIN_Y, MAX_Y);
        player->tpCooldown = PLAYER_TP_COOLDOWN;
        player->velocity = Vector2Zero();

        PlaySound(player->tpSound);
    }

    if (IsKeyPressed(player->keyMap.shoot))
    {
        Vector2 fireballVelocity = (Vector2) {
            .x = player->direction.x * PLAYER_FIREBALL_SPEED + player->velocity.x,
            .y = player->direction.y * PLAYER_FIREBALL_SPEED + player->velocity.y
        };
        ShootFireball(fireballs, player->color, fireballVelocity, player->position, true);
    }

    return EXIT_SUCCESS;
}

int MovePlayer(Player* player)
{
    if (player == NULL)
        return NULL_PLAYER;

    float   targetX = player->position.x + player->velocity.x * GetFrameTime();
    float   targetY = player->position.y + player->velocity.y * GetFrameTime();

    player->position.x = Wrap(targetX, MAX_X, MIN_X);
    player->position.y = Wrap(targetY, MAX_Y, MIN_Y);

    return EXIT_SUCCESS;
}

int DrawPlayer(Player const* player)
{
    if (player == NULL)
        return NULL_PLAYER;

    if (player->lives == 0)
        return EXIT_SUCCESS;

    // TODO: If the player is in tp cooldown, draw a tp vfx

    Rectangle   sourceRect = { 0, 0, player->texture.width, player->texture.height };
    Rectangle   destRect = { player->position.x, player->position.y,
        sourceRect.width, sourceRect.height };
    Vector2     origin = { player->texture.width / 2, player->texture.height / 2 };

    DrawTexturePro(player->texture, sourceRect, destRect, origin, player->rotation,
                  player->color);

    return EXIT_SUCCESS;
}

Vector2* GetPlayerColPts(Player* player, size_t* pointsCount, bool drawCollisions)
{
    if (player == NULL)
    {
        *pointsCount = 0;
        return NULL;
    }

    Vector2* colPoints = PLAYER_COL_PTS;

    if (colPoints)
    {
        *pointsCount = PLAYER_COL_PTS_COUNT;

        Vector2* res = malloc(*pointsCount * sizeof(Vector2));

        if (res == NULL)
            return NULL;

        for (size_t i = 0; i < *pointsCount; i++)
        {
            colPoints[i].x = player->position.x + colPoints[i].x
                * player->texture.width;
            colPoints[i].y = player->position.y + colPoints[i].y
                * player->texture.height;

            // Move back to the origin
            colPoints[i] = (Vector2) {
                colPoints[i].x - player->position.x,
                colPoints[i].y - player->position.y
            };

            float cosRotation = cosf(player->rotation * DEG2RAD);
            float sinRotation = sinf(player->rotation * DEG2RAD);

            // Rotate our vector
            colPoints[i] = (Vector2) {
                colPoints[i].x * cosRotation - colPoints[i].y * sinRotation,
                colPoints[i].x * sinRotation + colPoints[i].y * cosRotation
            };

            // Move back towards the original position
            colPoints[i] = (Vector2) {
                colPoints[i].x + player->position.x,
                colPoints[i].y + player->position.y
            };

            res[i] = Vector2Subtract(colPoints[i],
                Vector2Rotate((Vector2){player->texture.width / 2,
                player->texture.height / 2}, player->rotation * DEG2RAD));
        }

        if (drawCollisions)
            for (size_t i = 0; i < *pointsCount; i++)
                DrawLineV(res[i], res[(i+1)%*pointsCount], PINK);

        return res;
    }

    return colPoints;
}

void KillPlayer(Player* player)
{
    if (player == NULL || player->lives == 0)
        return;

    player->lives--;
    PlaySound(player->deathSound);
}