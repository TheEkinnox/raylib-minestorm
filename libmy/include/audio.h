#pragma once

typedef struct Sound Sound;
typedef struct s_list SfxList; // Syntactic sugar for t_list

typedef struct Sfx
{
	char*	path;
	Sound	sound;
} Sfx;

/// @brief Creates a list of sfxs and initializes it
/// @return A pointer to the created SfxList on success, NULL otherwise
SfxList*	CreateSfxList();

/// @brief Adds a sound effect with the given path to a given list if it isn't in it
/// @param sfxList The list in which the sfx should be added
/// @param path The path of the sfx to add
/// @return A pointer to the added sfx on success, NULL otherwise
Sfx*	AddSfx(SfxList* sfxList, char const* path);

/// @brief Finds or loads the sound effect with the given path from a given list
/// @param sfxList The list in which the sfx should be found or added
/// @param path The path of the sfx to find or load
/// @return A pointer to the found or new sfx on success, NULL otherwise
Sfx*	FindSfx(SfxList* sfxList, char const* path);

/// @brief Finds or loads the sound effect with the given path from a given list
/// @param sfxList The list in which the sfx should be found or added
/// @param path The path of the sfx to find or load
/// @return A pointer to the found or new sfx on success, NULL otherwise
Sfx*	GetSfx(SfxList* sfxList, char const* path);

/// @brief Removes the sound effect with the given path from a given list
/// @param sfxList The list from which the sfx should be removed
/// @param path The path of the sfx to remove
/// @return 1 on success, 0 otherwise
int	RemoveSfx(SfxList* sfxList, char const* path);

/// @brief Unloads the sound and frees the memory allocated for the sfx's data.
/// This method takes in a void* to be usable as a list callback
/// @param sfx A pointer to the sfx to destroy
void	DestroySfx(void* sfx);

/// @brief Destroys a given sound effects list and all of it's elements
/// @param sfxList The sfx list to destroy
void	ClearSounds(SfxList** sfxList);