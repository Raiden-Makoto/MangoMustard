#pragma once

#include <raylib.h>

void DrawLevel1(Texture2D cat,
                float catScale,
                Texture2D mango,
                float mangoScale,
                float deltaTime,
                int& collectedMangoes,
                int& livesRemaining,
                bool& levelFailed,
                bool& quitToMenu,
                bool& levelRestarted);
int GetLevel1TotalMangoCount();
void ResetLevel1State();
