#pragma once

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 800
#define TARGET_FRAME_RATE 60
#define GAME_TITLE "Mine Storm"
#define EXIT_GAME_END 2 

#define BORDER_SIZE 26
#define BOTTOM_AREA_HEIGHT 32

#define MIN_X BORDER_SIZE
#define MIN_Y BORDER_SIZE
#define MAX_X (WINDOW_WIDTH - BORDER_SIZE)
#define MAX_Y (WINDOW_HEIGHT - BORDER_SIZE - BOTTOM_AREA_HEIGHT)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(val, min, max) MAX(MIN(val, max), min)
#define ARRAY_LENGTH(array) (array ? sizeof(array) / sizeof(array[0]) : 0)

typedef unsigned char ubyte; // a 1 byte integer (0 to 255)

typedef struct s_list SpriteList;
typedef struct s_list SfxList;
typedef struct s_list SceneList;

typedef struct GameData
{
    SpriteList* sprites;
    SfxList*    sounds;
    SceneList*  scenes;
    int         exitCode;
    ubyte       playerCount;
} GameData;

/// @brief Creates and initializes the game's data at the title screen
/// @return A pointer to the created GameData structure or NULL if it failed
GameData*	CreateGameData(int* result);

/// @brief Initializes the game data then starts the main loop.
/// Once the game is done, destroys the game data
/// @returns 0 on success, the error code otherwise.
int	StartGame(void);

/// @brief Runs the main loop of the game with the given gameData
/// @param gameData The game data instance to use in the main loop
void RunMainLoop(GameData* gameData);

/// @brief Frees the allocated memory for a given GameData struct and it's fields
/// @param gameData A pointer to the GameData struct pointer to destroy
void	DestroyGameData(GameData** gameData);