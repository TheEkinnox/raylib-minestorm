#include <stdio.h>

#include "raylib.h"
#include "game.h"

int main(void)
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE);
	InitAudioDevice();

	// Set the current working directory to the executable's dir
	// to avoid problems with relative paths
	ChangeDirectory(GetApplicationDirectory());

	int result = StartGame();

	CloseAudioDevice();
	CloseWindow();

	if (result == EXIT_GAME_END)
		result = 0;

	if (result != 0)
		fprintf(stderr, "\nAn unexpected error occured. Code number : %d\n\n", result);

	return result;
}