#pragma once

typedef struct Texture Texture2D;
typedef struct s_list SpriteList; // Syntactic sugar for t_list

typedef struct Sprite
{
	char*		path;
	Texture2D	texture;
} Sprite;

/// @brief Creates a list of sprites and initializes it
/// @return A pointer to the created SpriteList on success, NULL otherwise
SpriteList*	CreateSpriteList();

/// @brief Adds a sprite with the given path to a given list if it isn't in it
/// @param spriteList The list in which the sprite should be added
/// @param path The path of the sprite to add
/// @return A pointer to the added sprite on success, NULL otherwise
Sprite*	AddSprite(SpriteList* spriteList, char const* path);

/// @brief Finds or loads the sprite with the given path from a given list
/// @param spriteList The list in which the sprite should be found or added
/// @param path The path of the sprite to find or load
/// @return A pointer to the found or new sprite on success, NULL otherwise
Sprite*	FindSprite(SpriteList* spriteList, char const* path);

/// @brief Finds or loads the sprite with the given path from a given list
/// @param spriteList The list in which the sprite should be found or added
/// @param path The path of the sprite to find or load
/// @return A pointer to the found or new sprite on success, NULL otherwise
Sprite*	GetSprite(SpriteList* spriteList, char const* path);

/// @brief Removes the sprite with the given path from a given list
/// @param spriteList The list from which the sprite should be removed
/// @param path The path of the sprite to remove
/// @return 1 on success, 0 otherwise
int	RemoveSprite(SpriteList* spriteList, char const* path);

/// @brief Unloads the texture and frees the memory allocated for the sprite's data.
/// This method takes in a void* to be usable as a list callback
/// @param sprite A pointer to the sprite to destroy
void	DestroySprite(void* sprite);

/// @brief Destroys a given sprites list and all of it's elements
/// @param spriteList The sprite list to destroy
void	ClearSprites(SpriteList** spriteList);