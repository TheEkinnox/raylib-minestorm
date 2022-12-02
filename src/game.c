#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#include "errors.h"
#include "sprites.h"
#include "audio.h"
#include "list.h"
#include "scene.h"
#include "ui.h"
#include "gameplay.h"
#include "game.h"

GameData* CreateGameData(int* result)
{
	GameData*	gameData = malloc(sizeof(GameData));

	if (gameData == NULL)
	{
		*result = DATA_LOAD_FAILED;
		return NULL;
	}

	gameData->sprites = CreateSpriteList();

	if (gameData->sprites == NULL)
	{
		free(gameData);
		*result = SPRITE_LIST_CREATION_FAILED;
		return NULL;
	}

	gameData->sounds = CreateSfxList();

	if (gameData->sounds == NULL)
	{
		DestroyGameData(&gameData);
		*result = SFX_LIST_CREATION_FAILED;
		return NULL;
	}

	gameData->scenes = CreateSceneList();

	if (gameData->scenes == NULL)
	{
		DestroyGameData(&gameData);
		*result = SCENES_LIST_CREATION_FAILED;
		return NULL;
	}

	gameData->playerCount = 2;
	gameData->exitCode = EXIT_SUCCESS;

	*result = EXIT_SUCCESS;

	return gameData;
}

int StartGame(void)
{
	int result;
	GameData*	gameData = CreateGameData(&result);

	if (gameData == NULL)
		return result;

	SetTargetFPS(TARGET_FRAME_RATE);

	// Disable the "escape to close" behavior since we have our own exit logic
	SetExitKey(KEY_NULL);

	LoadScene(gameData, E_TITLE, false);

	srand(time(NULL));

	RunMainLoop(gameData);

	result = gameData->exitCode;
	DestroyGameData(&gameData);

	return result;
}

void RunMainLoop(GameData* gameData)
{
	bool	shouldExit = gameData->exitCode != EXIT_SUCCESS;

	while (!WindowShouldClose() && !shouldExit)
	{
		BeginDrawing();

		ClearBackground(BLACK);
		DrawBackground();

		list_foreach(gameData->scenes, UpdateScene);

		DrawOverlay(gameData);

		EndDrawing();

		shouldExit = gameData->exitCode != EXIT_SUCCESS ||
			!ContainsActiveScene(gameData->scenes);
	}
}

void DestroyGameData(GameData** gameData)
{
	if (gameData == NULL || *gameData == NULL)
		return;

	ClearSprites(&(*gameData)->sprites);

	ClearSounds(&(*gameData)->sounds);

	ClearScenes(&(*gameData)->scenes);

	free(*gameData);
	*gameData = NULL;
}