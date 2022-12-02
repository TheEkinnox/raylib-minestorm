#pragma once
#include <raylib.h>
#define OFFSET 25
#define SMALL_MINE_SPEED 105
#define MEDIUM_MINE_SPEED 85
#define TALL_MINE_SPEED 65
#define GIANT_MINE_SPEED 45

#define FLOATING_MINE_COL_PTS (Vector2[]){(Vector2) { 0, 1},\
	(Vector2) { 9 / 39.0f, 0},(Vector2) { 1, 30/39.0f}}
#define FLOATING_MINE_COL_PTS_COUNT 3

#define FIREBALL_MINE_COL_PTS (Vector2[]){(Vector2) { 0, 7/39.0f},\
	(Vector2) { 30/39.0f, 0}, (Vector2) { 1, 30/39.0f},(Vector2) { 9/39.0f, 1}}
#define FIREBALL_MINE_COL_PTS_COUNT 4

#define MAGNETIC_MINE_COL_PTS (Vector2[]){(Vector2) { 0, 0}, (Vector2) { 1, 0},\
	(Vector2) { 1, 1}, (Vector2) { 0, 1}}
#define MAGNETIC_MINE_COL_PTS_COUNT 4

#define ALL_MINE_COL_PTS (Vector2[]){(Vector2) { 0, 31/39.0f}, (Vector2) { 7/39.0f, 0},\
	(Vector2) { 1, 7/39.0f}, (Vector2) { 31/39.0f, 1}}
#define ALL_MINE_COL_PTS_COUNT 4

#define BOSS_MINE_COL_PTS (Vector2[]){(Vector2){3/39.0f, 1},\
	(Vector2){3/39.0f, 20/39.0f},(Vector2){19/39.0f, 0},\
	(Vector2){35/39.0f, 20/39.0f}, (Vector2){35/39.0f, 1}}
#define BOSS_MINE_COL_PTS_COUNT 5

typedef enum
{
	E_FLOATING,
	E_FIREBALL,
	E_MAGNETIC,
	E_ALL,
	E_BOSS,
	E_SPAWNER,
	E_DESTROYED
} Minestate;

typedef enum
{
	E_SMALL,
	E_MEDIUM,
	E_TALL,
	E_GIANT
}Minesize;

typedef struct Scene GameplayScene;
typedef struct GameplayData GameplayData;


typedef struct Mine
{
	Vector2		pos;
	Vector2		dir;
	Minestate	state;
	Minesize	size;
	Texture2D	texture;
	Sound		deathSound;
}Mine;

/// @brief Initialize a mine with random positions and direction
/// @param mine the mine to initialize
/// @param texture the textures to add on the mine
/// @param deathSound The sound to play when the mine is destroyed
void InitMine(Mine* mine, Texture2D texture, Sound deathSound);

/// @brief implement to the mine the random direction and the speed
/// @param mine the mine to move
void MoveMine(Mine* mine);

/// @brief change the speed depending on the size of the mine
/// @param mine the mine struct to have the size of the mine
/// @return give a float of the speed we need for each size
float GetMineSpeed(Mine* mine);

/// @brief Make the mine to target the player
/// @param mine the mine that should follow the player
/// @param players list of players
/// @param nbPlayers number of players
void MagneticMine(Mine* mine, Player* players, ubyte nbPlayers);

/// @brief convert the size of the mines into a scale
/// @param size the size we want the scale of
/// @return the float thatwe will use to scale size
float GetMineScale (Minesize size);

/// @brief draw the mines with the right textures and size
/// @param mine the mine we want to draw
void DrawMine(Mine* mine);

/// @brief the function that handle the state of the given mine
/// @param mine the mine that we handle
/// @param scene the currently loaded gameplay scene
/// @return 0 on success, the error code otherwise
int HandleMine(Mine* mine, GameplayScene* scene);

/// @brief creates and initializes the mines in the curently loaded scene
/// @param data the gameplay data we use to create the mines
/// @param texture the texture of the mine that spawned
/// @param deathSound The sound to play when the mine is destroyed
void CreateMines (GameplayData* data, Texture2D texture, Sound deathSound);

/// @brief the function that is handling the destructions of a mine
/// @param mine the mine that is destroyed
/// @param gameplayData the score data and the list of current mine loaded
void KillMine (Mine* mine, GameplayData* gameplayData, bool givePoints);

/// @brief the fuction that is handling the score for each type and size of mines
/// @param mine the type and size of the mines
/// @return an unsigned int of the score we will add
unsigned int GetMineScore(Mine* mine);

/// @brief Gets the collision points for the given mine type
/// @param mine The mine for which we want the collision points
/// @param pointsCount A pointer to output the number of points into
/// @param drawCollisions Whether the collisions should be drawn
/// @return The collision points for the given mine
Vector2* GetMineColPts(Mine* mine, size_t* pointsCount, bool drawCollisions);

/// @brief the function that search fot the spawner positions
/// @param mines the spawner mine we checking
/// @param minesCount the list of the mines actually used in the scene
/// @return return the spawner mine
Mine* GetSpawner(Mine* mines, ubyte minesCount);

/// @brief add a setup per level
/// @param level the current level we modify
/// @return a random valide mine state for the current level
Minestate GetMineStateForLevel(unsigned int level);