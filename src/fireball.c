#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include "errors.h"
#include "assets.h"
#include "sprites.h"
#include "audio.h"
#include "gameplay.h"
#include "fireball.h"

FireballPool* CreateFireballPool(unsigned int size, Sound shootSound)
{
	if (size == 0)
		return NULL;

	FireballPool*	pool = malloc(sizeof(FireballPool));

	if (pool == NULL)
		return NULL;

	pool->fireballs = (Fireball*) malloc(size * sizeof(Fireball));

	if (pool->fireballs == NULL)
	{
		free(pool);
		return NULL;
	}

	for (unsigned int i = 0; i < size; i++)
	{
		pool->fireballs[i].isActive = false;
		pool->fireballs[i].shootSound = shootSound;
	}

	pool->size = size;
	pool->count = 0;

	return pool;
}

Fireball* ShootFireball(FireballPool* pool, Color color, Vector2 velocity,
					Vector2 startPos, bool isPlayer)
{
	if (pool == NULL || pool->size == 0)
		return NULL;

	Fireball* fireball = NULL;

	for (unsigned int i = 0; i < pool->size; i++)
	{
		if (i + 1 > pool->count)
			pool->count = i + 1;

		if (!pool->fireballs[i].isActive)
		{
			fireball = &pool->fireballs[i];
			fireball->isActive = true;
			fireball->pos = startPos;
			fireball->velocity = velocity;
			fireball->color = color;
			fireball->isPlayer = isPlayer;
			fireball->radius = isPlayer ? PLAYER_FIREBALL_RADIUS
				: MINE_FIREBALL_RADIUS;

			fireball->lifetime = isPlayer ? PLAYER_FIREBALL_RANGE
				: MINE_FIREBALL_RANGE;

			fireball->lifetime /= Vector2Length(velocity);

			PlaySound(fireball->shootSound);

			return fireball;
		}
	}

	return NULL;
}

int HandleFireballs(GameplayData* gameplayData)
{
	if (gameplayData->fireballPool == NULL)
		return NULL_FIREBALL_POOL;

	int	result = EXIT_SUCCESS;

	for (unsigned int i = 0; i < gameplayData->fireballPool->count; i++)
	{
		Fireball fireball = gameplayData->fireballPool->fireballs[i];

		if (!fireball.isActive)
			continue;

		if ((result = DrawFireball(&fireball)) != EXIT_SUCCESS)
			return result;

		if (gameplayData->isPaused)
			continue;

		fireball.pos.x += (fireball.velocity.x * GetFrameTime());
		fireball.pos.y += (fireball.velocity.y * GetFrameTime());

		fireball.pos.x = Wrap(fireball.pos.x, MIN_X, MAX_X);
		fireball.pos.y = Wrap(fireball.pos.y, MIN_Y, MAX_Y);

		if (fireball.lifetime > 0)
			fireball.lifetime -= GetFrameTime();
		else
			DisableFireball(&fireball);

		gameplayData->fireballPool->fireballs[i] = fireball;
	}

	return result;
}

int DisableAllFireballs(FireballPool* pool)
{
	if (pool == NULL)
		return NULL_FIREBALL_POOL;

	for (unsigned int i = 0; i < pool->count; i++)
		DisableFireball(&pool->fireballs[i]);

	return EXIT_SUCCESS;
}

void DestroyFireballPool(FireballPool** pool)
{
	if (pool == NULL || *pool == NULL || (*pool)->fireballs == NULL)
		return;

	free((*pool)->fireballs);

	free(*pool);
	*pool = NULL;
}

int DrawFireball(Fireball const* fireball)
{
	if (fireball == NULL)
		return NULL_BULLET;

	if (!fireball->isActive)
		return EXIT_SUCCESS;

	int result = EXIT_SUCCESS;

	DrawCircleV(fireball->pos, fireball->radius, fireball->color);
	return result;
}

int DisableFireball(Fireball* fireball)
{
	if (fireball == NULL)
		return NULL_BULLET;

	fireball->isActive = false;

	return EXIT_SUCCESS;
}