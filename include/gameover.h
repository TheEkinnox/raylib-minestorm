#pragma once

typedef struct Scene GameOverScene;

GameOverScene* CreateGameOverScene ();

/// @brief Create and initialize the Game Over scene 
/// @param gameOverScene the scene we are creating 
void InitGameOverScene (GameOverScene* gameOverScene);

/// @brief draw the GameOver scene
/// @param gameOverScene the scene GameOver to draw
void UpdateGameOverScene (GameOverScene* gameOverScene);
