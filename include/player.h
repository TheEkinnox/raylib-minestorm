 #pragma once

#define PLAYER_ACCELERATION 375     // The acceleration of the player in pixels/second/second
#define PLAYER_MAX_SPEED 425        // The maximum speed of the player in pixels/second
#define PLAYER_ROTATION_SPEED 240   // The rotation speed of the player in degrees per second
#define PLAYER_HEALTH 3             // The initial amount of health of the player
#define PLAYER_TP_COOLDOWN .5f      // The time in seconds to wait between teleportations

#define PLAYER_ONE_COLOR SKYBLUE // The tint of the first player's texture
#define PLAYER_TWO_COLOR LIME    // The tint of the second player's texture

#define PLAYER_ONE_KEYS (PlayerKeyMap) { KEY_R, KEY_D, KEY_G, KEY_F, KEY_E, KEY_T }
#define PLAYER_TWO_KEYS (PlayerKeyMap) { KEY_I, KEY_J, KEY_L, KEY_K, KEY_U, KEY_O }

#define PLAYER_COL_PTS (Vector2[]){(Vector2) {0, 0}, (Vector2) {1, 0.5f}, (Vector2) {0, 1}}
#define PLAYER_COL_PTS_COUNT 3

typedef struct GameplayData GameplayData;
typedef struct Texture Texture2D;
typedef struct Vector2 Vector2;

typedef struct PlayerKeyMap
{
    KeyboardKey forward;            // The key to press to move forward
    KeyboardKey left;               // The key to press to rotate to the left
    KeyboardKey right;              // The key to press to rotate to the right
    KeyboardKey shoot;              // The key to press to shoot a fireball
    KeyboardKey primaryTeleport;    // The primary key to press to teleport to a random position
    KeyboardKey secondaryTeleport;  // The secondary key to press to teleport to a random position
} PlayerKeyMap;

typedef struct Player
{
    Texture2D    texture;    // The texture of the player
    Color        color;      // The tint to apply to the player's texture
    Vector2      position;   // The current position of the player
    Vector2      direction;  // The normalized last non nul velocity of the player
    Vector2      velocity;   // The current velocity of the player
    Sound        tpSound;    // The sound to play when the player teleports
    Sound        deathSound; // The sound to play when the player dies
    PlayerKeyMap keyMap;     // The key bindings of the player
    float        rotation;   // The player's rotation in degrees
    float        tpCooldown; // The remaining time before the player can teleport again
    ubyte        lives;      // The current number of lives of the player
} Player;

/// @brief Creates and initializes an array of players
/// @param gameplayData The gameplay data for which the players should be created
/// @param playerCount The size of the players array to create
/// @param playerTexture The texture to use for the players
/// @param tpSound The sound to play when the player teleports
/// @param deathSound The sound to play when the player dies
void CreatePlayers(GameplayData* gameplayData, ubyte playerCount, Texture2D playerTexture,
    Sound tpSound, Sound deathSound);

/// @brief Initializes the given player
/// @param player A pointer to the player to initialize
/// @param texture The texture of the player
/// @param playerIndex The index of the player (0 for p1, 1 for p2)
/// @param tpSound The sound to play when the player teleports
/// @param deathSound The sound to play when the player dies
/// @return 0 on success, the error code otherwise
int InitPlayer(Player* player, Texture2D texture, ubyte playerIndex,
    Sound tpSound, Sound deathSound);

/// @brief Wrapper for all the player related management function
/// @param player A pointer to the player to handle
/// @param fireballs A pointer to the fireballs pool to use
/// @return 0 on success, the error code otherwise
int HandlePlayer(Player* player, FireballPool* fireballs);

/// @brief Handles the input for the given player
/// @param player A pointer to the player of which inputs should be handled
/// @param fireballs A pointer to the fireballs pool to use
/// @return 0 on success, the error code otherwise
int HandlePlayerInput(Player* player, FireballPool* fireballs);

/// @brief Moves the given player based on it's current velocity and direction
/// @param player The player to move
/// @return 0 on success, the error code otherwise
int MovePlayer(Player* player);

/// @brief Draws the given player on the screen
/// @param player A pointer to the player to draw
/// @return 0 on success, the error code otherwise
int DrawPlayer(Player const* player);

/// @brief Gets the collision points for the given player
/// @param player The player for which we want the collision points
/// @param pointsCount A pointer to output the number of points into
/// @param drawCollisions Whether the collisions should be drawn
/// @return The collision points for the given player
Vector2* GetPlayerColPts(Player* player, size_t* pointsCount, bool drawCollisions);

/// @brief Remove one life to the player
/// @param player the player we removed the lifes
void KillPlayer(Player* player);