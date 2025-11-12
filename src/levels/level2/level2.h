#pragma once

#include <raylib.h>

void DrawLevel2(Texture2D cat,
                float catScale,
                Texture2D mango,
                float mangoScale,
                float deltaTime,
                int& collectedMangoes,
                int& livesRemaining,
                bool& levelFailed,
                bool& quitToMenu,
                bool& levelRestarted,
                bool& levelCompleted);
int GetLevel2TotalMangoCount();
void ResetLevel2State();
