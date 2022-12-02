#include <stdlib.h>
#include "raylib.h"

#include "list.h"
#include "string_duplicate.h"
#include "string_compare.h"
#include "audio.h"

SfxList* CreateSfxList()
{
	SfxList*	list = list_create();

	list_init(list, NULL, &DestroySfx);

	return list;
}

Sfx* AddSfx(SfxList* sfxList, char const* path)
{
	if (sfxList == NULL || path == NULL)
		return NULL;

	Sfx*	sfx = FindSfx(sfxList, path);

	if (sfx != NULL)
		return NULL;

	sfx = malloc(sizeof(Sfx));

	if (sfx == NULL)
		return NULL;

	sfx->path = string_duplicate(path);

	if (sfx->path == NULL)
	{
		free(sfx);
		return NULL;
	}

	sfx->sound = LoadSound(path);

	if (sfx->sound.stream.buffer == NULL)
	{
		free(sfx->path);
		free(sfx);
		return NULL;
	}

	list_insert_next(sfxList, NULL, sfx);

	return sfx;
}

Sfx* FindSfx(SfxList* sfxList, char const* path)
{
	if (sfxList == NULL || path == NULL || list_size(sfxList) == 0)
		return NULL;

	t_list_node*	node = list_head(sfxList);
	Sfx*			sfx = (Sfx*) list_node_data(node);

	while (node != NULL)
	{
		if (string_compare(path, sfx->path) == 0)
			return sfx;

		node = list_node_next(node);
		sfx = (Sfx*) list_node_data(node);
	}

	return NULL;
}

Sfx* GetSfx(SfxList* sfxList, char const* path)
{
	if (sfxList == NULL || path == NULL)
		return NULL;

	Sfx*	sfx = FindSfx(sfxList, path);

	if (sfx == NULL)
		sfx = AddSfx(sfxList, path);

	return sfx;
}

int RemoveSfx(SfxList* sfxList, char const* path)
{
	if (sfxList == NULL || path == NULL || list_size(sfxList) == 0)
		return 0;

	t_list_node*	prevNode = NULL;
	t_list_node*	node = list_head(sfxList);
	Sfx*			sfx = (Sfx*) list_node_data(node);

	while (node != NULL)
	{
		if (string_compare(path, sfx->path) == 0)
		{
			prevNode ? list_remove_next(sfxList, prevNode, NULL) :
				list_remove_next(sfxList, list_head(sfxList), NULL);

			return 1;
		}

		prevNode = node;
		node = list_node_next(node);
		sfx = (Sfx*) list_node_data(node);
	}

	return 0;
}

void DestroySfx(void* sfx)
{
	if (sfx == NULL)
		return;

	Sfx*	castSfx = (Sfx*) sfx;

	free(castSfx->path);
	UnloadSound(castSfx->sound);
	free(sfx);
}

void ClearSounds(SfxList** sfxList)
{
	if (sfxList == NULL || *sfxList == NULL)
		return;

	list_destroy(sfxList);
}