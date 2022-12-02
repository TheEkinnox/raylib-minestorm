#pragma once

#define BACKGROUND_COLOR_ONE (Color) { 18, 10, 30, 255 }
#define BACKGROUND_COLOR_TWO BLACK
#define BACKGROUND_GRID_COLOR (Color) { 0, 184, 214, 26 }
#define BACKGROUND_GRID_SPACING 58

#define BORDER_ROUNDNESS .01f
#define BORDER_COLOR (Color) { 255, 251, 10, 255 }
#define INNER_BORDER_SIZE 3
#define INNER_BORDER_COLOR WHITE

#define FONT_SIZE_SMALL 18
#define FONT_SIZE_NORMAL 20
#define FONT_SIZE_TITLE 32

#define OVERLAY_ALPHA .65f

#define PLAYER_INFO_ROUNDNESS .2f
#define PLAYER_INFO_WIDTH 160
#define PLAYER_INFO_HEIGHT FONT_SIZE_NORMAL * 3.5f
#define PLAYER_INFO_Y MIN_Y + FONT_SIZE_NORMAL
#define PLAYER_ONE_INFO_X MIN_X + FONT_SIZE_NORMAL
#define PLAYER_TWO_INFO_X MAX_X - FONT_SIZE_NORMAL - PLAYER_INFO_WIDTH
#define PLAYER_INFO_TEXT_COLOR Fade(BLACK, OVERLAY_ALPHA)

#define PLAYER_KEYS_TEXT "Rotate: '%c' and '%c'\nMove: '%c'\nShoot: '%c'\nTeleport: '%c' or '%c'"
#define PLAYER_KEYS_TEXT_COLOR Fade(WHITE, OVERLAY_ALPHA)
#define PLAYER_KEYS_TEXT_Y PLAYER_INFO_Y + PLAYER_INFO_HEIGHT + FONT_SIZE_NORMAL / 2

#define BOTTOM_AREA_ROUNDNESS .35f
#define BOTTOM_AREA_COLOR (Color) { 0, 50, 155, 255 }
#define BOTTOM_LINE_HEIGHT 2
#define BOTTOM_LINE_COLOR Fade(RED, OVERLAY_ALPHA)
#define SCORE_TEXT "Score: %04d"

#define PAUSE_KEY_TEXT "Press 'SPACE'"
#define PAUSE_KEY_TEXT_COLOR Fade(BORDER_COLOR, OVERLAY_ALPHA)

#define EXIT_KEY_TEXT "Press 'ESC'"
#define EXIT_TEXT "%s: Back to main menu"
#define EXIT_GAME "%s: to quit Game"

typedef unsigned char ubyte; // a 1 byte integer (0 to 255)

typedef struct PlayerKeyMap PlayerKeyMap;
typedef struct GameData GameData;
typedef struct GameplayData GameplayData;
typedef struct Scene GameplayScene;

/// @brief Draws the background of the game
void    DrawBackground(void);

/// @brief Draws the overlay of the game
/// @param gameData A pointer to the current game data
void    DrawOverlay(GameData const* gameData);

/// @brief Draws the Heads-Up display on the screen
/// @param gameData A pointer to the current game data
/// or NULL to not draw gameplay scene specific infos
void    DrawHUD(GameData const* gameData);

/// @brief Draws the gameplay related info on the overlay
/// @param gameplayData The currently loaded gameplay scene's data
/// @param playersCount The number of players in the scene
/// @param livesTexture The texture to use to display the lives of a player
void DrawGameplayInfo(GameplayData* gameplayData, ubyte playersCount,
    Texture2D livesTexture);

/// @brief Draws a player's info on screen
/// @param posX The horizontal position at which the info should be drawn
/// @param texture The texture to use for the lives display
/// @param playerNum The number of the player (1 = player one, 2 = player two, ...)
/// @param lives The number of lives of the player
/// @param keyMap The key map to draw (NULL to not draw)
void    DrawPlayerInfo(float posX, Texture2D texture, ubyte playerNum,
    ubyte lives, PlayerKeyMap const* keyMap);

/// @brief Draws the lives of the player at the given position
/// @param pos The position at which the player's lives should be drawn
/// @param texture The texture to use to represent the player lives.
/// If texture.id == 0 uses 'A'
/// @param lives The number of lives to draw
void    DrawLives(Vector2 pos, Texture2D texture, ubyte lives);

/// @brief Draws the specified player key map on the screen at the given position
/// @param keyMap The set of keys to draw
/// @param pos The position at which the key map should be drawn
void    DrawKeyMap(PlayerKeyMap keyMap, Vector2 pos);