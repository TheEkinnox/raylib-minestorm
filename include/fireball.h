#pragma once

#define PLAYER_FIREBALL_RADIUS 3
#define PLAYER_FIREBALL_SPEED 300
#define PLAYER_FIREBALL_RANGE (MAX_X - MIN_X) * 0.75f

#define MINE_FIREBALL_SPEED MEDIUM_MINE_SPEED
#define MINE_FIREBALL_RADIUS 3
#define MINE_FIREBALL_RANGE (MAX_X - MIN_X) / 2
#define MINE_FIREBALL_COL_PTS {(Vector2) {0, 0}, (Vector2) {1, 0}, (Vector2) {1, 1}, (Vector2) {0, 1}}
#define MINE_FIREBALL_COL_PTS_COUNT 4

#define FIREBALL_SCORE 110

typedef struct Color Color;
typedef struct Vector2 Vector2;
typedef struct Sound Sound;

typedef struct GameplayData GameplayData;
typedef struct Player Player;

typedef struct Fireball
{
	Color		color;
	Vector2		pos;
	Vector2		velocity;
	Sound		shootSound;
	float		radius;
	float		lifetime;
	bool		isActive;
	bool		isPlayer;
} Fireball;

typedef struct FireballPool
{
	Fireball*		fireballs;
	unsigned int	size;
	unsigned int	count;
} FireballPool;

/// @brief Creates and initializes a new pool of fireballs
/// @param size The size of the pool to create
/// @param shootSound The sound to play when a fireball is shot
/// @return The created fireball pool
FireballPool*	CreateFireballPool(unsigned int size, Sound shootSound);

/// @brief Handle the display and movement of the fireballs
/// @param gameplayData The gameplay data of which fireballs should be handled
/// @return 0 on success, the error code otherwise
int	HandleFireballs(GameplayData* gameplayData);

/// @brief Adds or find an inactive fireball in the given pool,
/// enables it and shoots it.
/// @param pool The fireballs pool from which the fireball should be shot
/// @param color The color of the fireball to draw
/// @param velocity The unscaled velocity of the fireball
/// @param startPos The position from which the fireball should be shot
/// @param isPlayer Whether the fireball is shot by a player or not
/// @return A pointer to the shot fireball on success, NULL otherwise
Fireball*	ShootFireball(FireballPool* pool, Color color, Vector2 velocity,
						Vector2 startPos, bool isPlayer);

/// @brief Disables all the fireballs in the given pool
/// @param pool The fireball pool of which we want to disable the elements
/// @return 0 on success, the error code otherwise
int	DisableAllFireballs(FireballPool* pool);

/// @brief Destroys the given fireball pool and all of it's elements
/// @param pool A pointer to the fireball pool to destroy
void	DestroyFireballPool(FireballPool** pool);

/// @brief Draws a fireball on the screen
/// @param fireball The fireball to draw
/// @return 0 on success, the error code otherwise
int	DrawFireball(Fireball const* fireball);

/// @brief Disables a fireball
/// @param fireball The fireball to disable
/// @param playSound Whether the fireball should play a sound when disabled
/// @return 0 on success, the error code otherwise
int	DisableFireball(Fireball* fireball);

/// @brief Frees all the ressources of the given fireball
/// @param fireball A pointer to the fireball to free
void	DestroyFireball(Fireball** fireball);