#pragma once

#define PAUSE_BACKGROUND_COLOR Fade(BLACK, .6f)
#define UNPAUSE_TEXT "%s: Resume"

typedef struct Scene PauseScene;

/// @brief Create and initialize the pause scene 
/// @return the created pause scene
PauseScene* CreatePauseScene();

/// @brief Draw the pause scene
/// @param scene the scene to draw
void UpdatePauseScene(PauseScene* scene);