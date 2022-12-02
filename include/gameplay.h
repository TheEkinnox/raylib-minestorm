#pragma once

#define MAX_PLAYER_COUNT 2
#define FIREBALL_POOL_SIZE 30

#define PAUSE_KEY KEY_SPACE
#define SHOW_COL_KEY KEY_C

typedef unsigned char ubyte; // a 1 byte integer (0 to 255)

typedef struct Scene GameplayScene;
typedef struct GameData GameData;
typedef struct Player Player;
typedef struct Mine Mine;
typedef struct MineLayer MineLayer;
typedef struct FireballPool FireballPool;

typedef struct GameplayData
{
    FireballPool*   fireballPool;
    Player*         players;
    Mine*           mines;
    MineLayer*      mineLayer;
    unsigned int    level;
    size_t          score;
    Texture2D       pauseTexture;
    Texture2D       playTexture;
    ubyte           playersCount;
    ubyte           minesCount;
    bool            isPaused;
    bool            drawCollisions;
} GameplayData;

/// @brief Creates and initializes a new GameplayData instance
/// @param playerCount The number of players to initialize
/// @param playerTexture The texture of a player
/// @param minesTexture The sprite sheet of the mine
/// @return A pointer to the created gameplay data on success, NULL otherwise.
GameplayData* CreateGameplayData();

/// @brief Creates and initializes a new Gameplay Scene
/// @return A pointer to the created gameplay scene on success, NULL otherwise.
GameplayScene*  CreateGameplayScene();

/// @brief Initializes the given gameplay scene
/// @param scene A pointer to the scene to initialize
void InitGameplayScene(GameplayScene* scene);

/// @brief Gets the casted gameplay data from the given gameplay scene
/// @param gameplayScene The scene of which gameplayData to fetch
/// @return The casted scene data of the given gameplay scene
GameplayData*   GetGameplayData(GameplayScene const* gameplayScene);

/// @brief Gets the game data from the given gameplay scene
/// @param gameplayScene The scene of which gameData to fetch
/// @return The game data of the given gameplay scene
GameData* GetGameData(GameplayScene const* gameplayScene);

/// @brief Updates the given gameplay scene
/// @param scene A pointer to the scene to update
void UpdateGameplayScene(GameplayScene* scene);

/// @brief Handles the developper cheats (enabled only in DEBUG)
/// @param scene The scene for which the cheats should be handled
#ifdef DEBUG
void HandleCheats(GameplayScene* scene);
#endif

/// @brief Updates the players data for the given gameplay scene
/// @param scene A pointer to the scene of which players should be updated
void UpdatePlayers(GameplayScene* scene);

/// @brief Updates the mine data for the given gameplay scene
/// @param scene A pointer to the scene of which mine should be updated
void UpdateMines(GameplayScene* scene);

void StartNextLevel(GameplayScene* scene);

void HandleCollision (GameplayData* gameplayData);

void HandlePlayersCollisions(GameplayData* gameplayData);

void HandleMinesCollisions(GameplayData* gameplayData);

void HandleFireballsCollisions(GameplayData* gameplayData);

/// @brief Unloads the given gameplay scene
/// @param scene A pointer to the scene to update
void UnloadGameplayScene(GameplayScene* scene);