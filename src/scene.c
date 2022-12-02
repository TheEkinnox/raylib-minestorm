#include <stdlib.h>
#include "raylib.h"

#include "errors.h"
#include "list.h"
#include "game.h"
#include "title.h"
#include "gameplay.h"
#include "pause.h"
#include "gameover.h"
#include "scene.h"

SceneList* CreateSceneList(void)
{
    SceneList* list = list_create();

    list_init(list, CompareScenes, DestroyScene);

    return list;
}

int	CompareScenes(void* scene1, void* scene2)
{
    if (scene1 == NULL && scene2 == NULL)
        return 0;
    else if (scene1 == NULL)
        return 1;
    else if (scene2 == NULL)
        return -1;

    Scene*  castScene1 = (Scene*) scene1;
    Scene*  castScene2 = (Scene*) scene2;

    if (castScene1->id == castScene2->id)
        return 0;

    return castScene1 > castScene2 ? -1 : 1;
}

Scene* FindScene(SceneList* sceneList, SceneId id)
{
    if (sceneList == NULL || list_size(sceneList) == 0)
        return NULL;

    t_list_node*    curSceneNode = list_head(sceneList);
    Scene*          curScene;

    while (curSceneNode)
    {
        curScene = (Scene*) list_node_data(curSceneNode);

        if (curScene && curScene->id == id)
            return curScene;

        curSceneNode = list_node_next(curSceneNode);
    }

    return NULL;
}

bool ContainsActiveScene(SceneList* sceneList)
{
    if (sceneList == NULL || list_size(sceneList) == 0)
        return false;

    t_list_node*    curSceneNode = list_head(sceneList);
    Scene*          curScene;

    while (curSceneNode)
    {
        curScene = (Scene*) list_node_data(curSceneNode);

        if (curScene && curScene->isActive)
            return true;

        curSceneNode = list_node_next(curSceneNode);
    }

    return false;
}

void UnloadAllScenes(SceneList* sceneList, SceneId ignoredId)
{
    if (sceneList == NULL || list_size(sceneList) == 0)
        return;

    t_list_node*    curSceneNode = list_head(sceneList);
    Scene*          curScene;

    while (curSceneNode)
    {
        curScene = (Scene*) list_node_data(curSceneNode);

        if (curScene->isActive && (ignoredId == (SceneId) -1
            || curScene->id != ignoredId))
        {
            if (curScene->unload)
                curScene->unload(curScene);

            curScene->isActive = false;
        }

        curSceneNode = list_node_next(curSceneNode);
    }
}

Scene* CreateScene(SceneId id)
{
    switch (id)
    {
    case E_TITLE:
        return CreateTitleScene();
    case E_GAMEPLAY:
        return CreateGameplayScene();
    case E_PAUSE:
        return CreatePauseScene();
    case E_GAME_OVER:
        return CreateGameOverScene();
    default:
        return NULL;
    }

    // This line should not be reached (handled in default) but gcc can't see it...
    return NULL;
}

void LoadScene(GameData* gameData, SceneId id, bool unloadOthers)
{
    if (gameData == NULL || gameData->exitCode != EXIT_SUCCESS)
        return;

    if (gameData->scenes == NULL)
    {
        gameData->exitCode = NULL_SCENES_LIST;
        return;
    }

    Scene*  scene = FindScene(gameData->scenes, id);

    if (scene == NULL)
    {
        scene = CreateScene(id);

        if (scene == NULL)
        {
            gameData->exitCode = SCENE_CREATION_FAILED;
            return;
        }

        list_insert_next(gameData->scenes, NULL, scene);

        // Sort the scenes list to make sure the draw order is preserved
        list_sort(gameData->scenes);
    }

    scene->gameData = gameData;

    if (scene->init)
        scene->init(scene);

    if (gameData->exitCode == EXIT_SUCCESS)
    {
        if (unloadOthers)
            UnloadAllScenes(gameData->scenes, id);

        scene->isActive = true;
    }
}

void UpdateScene(void* scene)
{
    Scene*  castScene = (Scene*) scene;

    // We want to segfault if the scene or the gameData is NULL so no check for this
    if (!castScene->isActive || castScene->gameData->exitCode != EXIT_SUCCESS)
        return;

    castScene->update(castScene);
}

void UnloadScene(GameData* gameData, SceneId id)
{
    if (gameData == NULL || gameData->exitCode != EXIT_SUCCESS)
        return;

    Scene*  scene = FindScene(gameData->scenes, id);

    if (scene == NULL)
        return;

    if (scene->unload)
        scene->unload(scene);

    scene->isActive = false;
}

void DestroyScene(void* scene)
{
    if (scene == NULL)
        return;

    Scene*  castScene = (Scene*) scene;

    if (castScene->unload && castScene->isActive)
        castScene->unload(castScene);

    free(castScene);
}

void ClearScenes(SceneList** sceneList)
{
    if (sceneList == NULL || *sceneList == NULL)
        return;

    list_destroy(sceneList);
}