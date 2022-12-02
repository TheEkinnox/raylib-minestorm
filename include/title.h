#pragma once

#define LAUNCH_SOLO_GAME "Press \"F\":\t\tsolo game"
#define LAUNCH_MULTIPLAYER_GAME "Press \"K\":\t\tTwo player game"

typedef struct Scene TitleScene;

/// @brief Create and initialize theTitle scene 
/// @return the title scene we create
TitleScene* CreateTitleScene ();

/// @brief Handles the keymaps for the title scene
/// @param titleScene The curently loaded title scene  
void UpdateTitleScene (TitleScene* titleScene);

/// @brief Draw the title scene
void DrawTitleScene ();