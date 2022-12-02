#pragma once
#include "raylib.h"
#include "stdbool.h"

#define MINE_LAYER_COL_PTS (Vector2[]){(Vector2) { 0, 0}, (Vector2) { 1, 0},\
	(Vector2) { 1, 1}, (Vector2) { 0, 1}}
#define MINE_LAYER_COL_PTS_COUNT 4

#define MINE_LAYER_SCALE 0.5f
#define MINE_LAYER_SCORE 1000

#define MIN_LAYER_DESTROYED_MINES 10
#define MINE_LAYER_MINES_COUNT 7

typedef Vector2 Vector2;

typedef struct MineLayer
{
	Vector2		pos;
	Vector2		dir;
	Texture2D	texture;
	Sound		deathSound;
    Vector2     target;
	ubyte		remainingMines;
    bool        isActive;
    bool        canSpawn;
}MineLayer;

/// @brief Initialize the mineLayer to a position offset
/// @param mineLayer the mineLayer to initialize
void InitMineLayer(MineLayer* mineLayer);

/// @brief Set the movement of the mine to a first direction 
/// @param mineLayer the minelayer to move 
void MoveMineLayer(MineLayer* mineLayer);

/// @brief put a spawner to the position of the mineLayer
/// @param gameplayData the gameplayData in which the spawner should be 
void LayMine (GameplayData* gameplayData);

/// @brief Get a random position and set it to target for the mine layer
/// @param mineLayer the mineLayer we are setting a target 
void SetMineLayerTarget(MineLayer* mineLayer);

/// @brief the function that handle the state of the given mineLayer
/// @param gameplaData the data of the scene in which the mineLayer is evolving
/// @return 0 on success, the error code otherwise
int HandleMineLayer (GameplayData* gameplaData);

/// @brief Creates a new mine layer instance
/// @param gameplayData The gameplay data in which the mine layer should be created
/// @param texture The texture to use for the mine layer
/// @param deathSound The sound the mine layer should play when it's destroyed
void CreateMineLayer (GameplayData* gameplayData, Texture2D texture,
    Sound deathSound);

/// @brief Gets the collision points for the given mine layer
/// @param mineLayer The mine layer for which we want the collision points
/// @param pointsCount A pointer to output the number of points into
/// @param drawCollisions Whether the collisions should be drawn
/// @return The collision points for the given mine layer
Vector2* GetMineLayerColPts (MineLayer* mineLayer, size_t* pointsCount, bool drawCollisions);

/// @brief handle the state of the mineLayer when it is destroyed
/// @param mineLayer the minelayer we handle
void KillMineLayer(MineLayer* mineLayer);