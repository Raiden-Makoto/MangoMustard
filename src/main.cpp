#include <raylib.h>

#include "level1.h"
#include "levelselect.h"
#include "hud.h"

void DrawTitleScreen();

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "MangoMustard");

    Texture2D cat = LoadTexture("src/assets/mangocat.png");
    if (cat.id == 0) {
        CloseWindow();
        return 1;
    }

    Texture2D mango = LoadTexture("src/assets/mango.png");
    if (mango.id == 0) {
        UnloadTexture(cat);
        CloseWindow();
        return 1;
    }

    const float catScale = 0.12f; // 88% smaller than original size
    const float mangoScale = 0.054f;
    const Vector2 catSize = {
        static_cast<float>(cat.width) * catScale,
        static_cast<float>(cat.height) * catScale
    };

    Vector2 titleMangoPos = {
        (screenWidth - catSize.x) / 2.0f,
        (screenHeight - catSize.y) / 2.0f + 120.0f
    };

    ResetLevel1State();

    enum class GameState {
        Title,
        LevelSelect,
        Level1
    };

    constexpr bool kDebugStartInLevel1 = true;
    GameState state = kDebugStartInLevel1 ? GameState::Level1 : GameState::Title;
    double levelStartTime = GetTime();

    LevelData levels[10] = {};
    for (int i = 0; i < 10; ++i) {
        levels[i].unlocked = (i == 0); // Only level 1 unlocked by default
    }

    int selectedLevel = 1;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        switch (state) {
            case GameState::Title: {
                BeginDrawing();
                ClearBackground(BLACK);

                DrawTitleScreen();
                DrawTextureEx(cat, titleMangoPos, 0.0f, catScale, WHITE);

                bool startGame = IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP);
                EndDrawing();

                if (startGame) {
                    state = GameState::LevelSelect;
                }
                break;
            }
            case GameState::LevelSelect: {
                selectedLevel = UpdateLevelSelect(selectedLevel, levels, 10);
                if (IsKeyPressed(KEY_ENTER) && levels[selectedLevel - 1].unlocked) {
                    if (selectedLevel == 1) {
                        ResetLevel1State();
                        state = GameState::Level1;
                        levelStartTime = GetTime();
                    }
                }
                break;
            }
            case GameState::Level1: {
                BeginDrawing();
                ClearBackground(BLACK);

                int collectedMangoes = 0;
                int livesRemaining = 0;
                bool levelFailed = false;
                bool quitToMenu = false;
                bool levelRestarted = false;
                const float deltaTime = GetFrameTime();
                DrawLevel1(cat,
                           catScale,
                           mango,
                           mangoScale,
                           deltaTime,
                           collectedMangoes,
                           livesRemaining,
                           levelFailed,
                           quitToMenu,
                           levelRestarted);

                if (quitToMenu) {
                    ResetLevel1State();
                    state = GameState::LevelSelect;
                    EndDrawing();
                    break;
                }

                if (levelRestarted) {
                    levelStartTime = GetTime();
                }

                DrawLevelLabel(1);
                int elapsedSeconds = static_cast<int>(GetTime() - levelStartTime);
                if (!levelFailed) {
                    DrawTimerLabel(elapsedSeconds);
                    DrawMangoCounter(collectedMangoes, GetLevel1TotalMangoCount());
                }
                DrawLives(livesRemaining);

                EndDrawing();
                break;
            }
        }
    }

    UnloadTexture(mango);
    UnloadTexture(cat);
    CloseWindow();
    return 0;
}
