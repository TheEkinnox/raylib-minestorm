#include <stdlib.h>
#include "raylib.h"

#include "list.h"
#include "string_duplicate.h"
#include "string_compare.h"
#include "sprites.h"

SpriteList* CreateSpriteList()
{
	SpriteList*	list = list_create();

	list_init(list, NULL, &DestroySprite);

	return list;
}

Sprite* AddSprite(SpriteList* spriteList, char const* path)
{
	if (spriteList == NULL || path == NULL)
		return NULL;

	Sprite*	sprite = FindSprite(spriteList, path);

	if (sprite != NULL)
		return NULL;

	sprite = malloc(sizeof(Sprite));

	if (sprite == NULL)
		return NULL;

	sprite->path = string_duplicate(path);

	if (sprite->path == NULL)
	{
		free(sprite);
		return NULL;
	}

	sprite->texture = LoadTexture(path);

	if (sprite->texture.id <= 0)
	{
		free(sprite->path);
		free(sprite);
		return NULL;
	}

	list_insert_next(spriteList, NULL, sprite);

	return sprite;
}

Sprite* FindSprite(SpriteList* spriteList, char const* path)
{
	if (spriteList == NULL || path == NULL || list_size(spriteList) == 0)
		return NULL;

	t_list_node*	node = list_head(spriteList);
	Sprite*			sprite = (Sprite*) list_node_data(node);

	while (node != NULL)
	{
		if (string_compare(path, sprite->path) == 0)
			return sprite;

		node = list_node_next(node);
		sprite = (Sprite*) list_node_data(node);
	}

	return NULL;
}

Sprite* GetSprite(SpriteList* spriteList, char const* path)
{
	if (spriteList == NULL || path == NULL)
		return NULL;

	Sprite*	sprite = FindSprite(spriteList, path);

	if (sprite == NULL)
		sprite = AddSprite(spriteList, path);

	return sprite;
}

int RemoveSprite(SpriteList* spriteList, char const* path)
{
	if (spriteList == NULL || path == NULL || list_size(spriteList) == 0)
		return 0;

	t_list_node*	prevNode = NULL;
	t_list_node*	node = list_head(spriteList);
	Sprite*			sprite = (Sprite*) list_node_data(node);

	while (node != NULL)
	{
		if (string_compare(path, sprite->path) == 0)
		{
			prevNode ? list_remove_next(spriteList, prevNode, NULL) :
				list_remove_next(spriteList, list_head(spriteList), NULL);

			return 1;
		}

		prevNode = node;
		node = list_node_next(node);
		sprite = (Sprite*) list_node_data(node);
	}

	return 0;
}

void DestroySprite(void* sprite)
{
	if (sprite == NULL)
		return;

	Sprite*	castSprite = (Sprite*) sprite;

	free(castSprite->path);
	UnloadTexture(castSprite->texture);
	free(sprite);
}

void ClearSprites(SpriteList** spriteList)
{
	if (spriteList == NULL || *spriteList == NULL)
		return;

	list_destroy(spriteList);
}