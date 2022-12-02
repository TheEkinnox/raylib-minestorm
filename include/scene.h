#pragma once

#include <stdbool.h>

typedef struct s_list SceneList; // syntactic sugar for t_list

typedef struct GameData GameData;
typedef struct Scene Scene;

typedef enum
{
	E_TITLE,
	E_GAMEPLAY,
	E_PAUSE,
	E_GAME_OVER
} SceneId;

typedef struct Scene
{
	GameData*	gameData;			// The data for the whole game
	void*		sceneData;			// The data for the scene
	void		(*init)(Scene*);	// The initialization function of the scene
	void		(*update)(Scene*);	// The per frame update function of the scene
	void		(*unload)(Scene*);	// The function to call to unload the scene
	SceneId		id;					// The identifier of the scene
	bool		isActive;			// Whether the scene is currently active or not
} Scene;

/// @brief Creates and initializes a new scene list
/// @return A pointer to the created scene list on success. NULL otherwise.
SceneList*	CreateSceneList(void);

/// @brief Finds a loaded scene with the given id and set it as inactive.
/// If no matching scene is found, does nothing.
/// This function takes in two void* to be usable as a list callback
/// @param id The identifier of the scene to unload
/// @return 0 on success, the error code otherwise
int	CompareScenes(void* scene1, void* scene2);

/// @brief Unloads all the scenes of the given list of scenes
/// except the one with the given `ignoreId`
/// @param sceneList The list of scenes to unload
/// @param ignoredId The id of the scene to ignore (-1 for none)
void UnloadAllScenes(SceneList* sceneList, SceneId ignoredId);

/// @brief Finds a loaded scene with the given id.
/// @param sceneList The list of scenes in which the scene should be found
/// @param id The identifier of the scene to find
/// @return A pointer to the found scene on success, NULL otherwise.
Scene*	FindScene(SceneList* sceneList, SceneId id);

/// @brief Checks wheter there is any active scene in the given list or not
/// @param sceneList The list of scenes to search for active scenes in
/// @return true if at least one scene is currently active, false otherwise.
bool	ContainsActiveScene(SceneList* sceneList);

/// @brief Creation a new scene with the matching function for the given id.
/// @param id The id of the scene to create
/// @return A pointer to the created scene on success, NULL otherwise.
Scene*	CreateScene(SceneId id);

/// @brief Finds a loaded scene with the given id and set it as active.
/// If no matching scene is found creates a new one with the corresponding function.
/// @param gameData The game data in which the scene should be found
/// @param id The identifier of the scene to load
/// @param isUnique whether or not the scene should be the only loaded scene
void	LoadScene(GameData* gameData, SceneId id, bool isUnique);

/// @brief Runs the per-frame update function of the given scene
/// This function takes in a void* to be usable as a list_foreach callback
/// @param scene a pointer to the scene to update
void	UpdateScene(void* scene);

/// @brief Finds a loaded scene with the given id and set it as inactive.
/// If no matching scene is found, does nothing.
/// @param gameData The game data in which the scene should be found
/// @param id The identifier of the scene to unload
void	UnloadScene(GameData* gameData, SceneId id);

/// @brief Unloads the scene and frees the memory allocated for it's data.
/// This function takes in a void* to be usable as a list callback
/// @param scene A pointer to the scene to destroy
void	DestroyScene(void* scene);

/// @brief Destroys a given scenes list and all of it's elements
/// @param sceneList A pointer to the scene list to clear
void	ClearScenes(SceneList** sceneList);