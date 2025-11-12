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
        Level1,
        LevelComplete
    };

    constexpr bool kDebugStartInLevel1 = true;
    GameState state = kDebugStartInLevel1 ? GameState::Level1 : GameState::Title;
    double levelStartTime = GetTime();
    int currentLevel = 1;
    int lastLevelElapsedSeconds = 0;
    int lastLevelCollectedMangoes = 0;
    int lastLevelTotalMangoes = 0;

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
                        currentLevel = selectedLevel;
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
                bool levelCompletedFlag = false;
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
                           levelRestarted,
                           levelCompletedFlag);

                if (levelRestarted) {
                    levelStartTime = GetTime();
                }

                DrawLevelLabel(currentLevel);
                int elapsedSeconds = static_cast<int>(GetTime() - levelStartTime);
                if (!levelFailed) {
                    DrawTimerLabel(elapsedSeconds);
                    DrawMangoCounter(collectedMangoes, GetLevel1TotalMangoCount());
                }
                DrawLives(livesRemaining);

                EndDrawing();

                if (quitToMenu) {
                    ResetLevel1State();
                    state = GameState::LevelSelect;
                    break;
                }

                if (levelCompletedFlag) {
                    lastLevelElapsedSeconds = elapsedSeconds;
                    lastLevelCollectedMangoes = collectedMangoes;
                    lastLevelTotalMangoes = GetLevel1TotalMangoCount();
                    if (currentLevel < 10) {
                        levels[currentLevel].unlocked = true;
                    }
                    ResetLevel1State();
                    state = GameState::LevelComplete;
                }
                break;
            }
            case GameState::LevelComplete: {
                BeginDrawing();
                ClearBackground(BLACK);

                const char *title = "Level Complete!";
                const int titleFontSize = 48;
                const int titleWidth = MeasureText(title, titleFontSize);
                const Color goalAccent{0, 255, 128, 255};
                DrawText(title, (screenWidth - titleWidth) / 2, screenHeight / 2 - 140, titleFontSize, goalAccent);

                const int infoFontSize = 32;
                const int minutes = lastLevelElapsedSeconds / 60;
                const int seconds = lastLevelElapsedSeconds % 60;
                const char *timeLabel = TextFormat("Time: %02d:%02d", minutes, seconds);
                const int timeWidth = MeasureText(timeLabel, infoFontSize);
                DrawText(timeLabel, (screenWidth - timeWidth) / 2, screenHeight / 2 - 40, infoFontSize, RAYWHITE);

                const char *mangoLabel = TextFormat("Mangoes: %d/%d", lastLevelCollectedMangoes, lastLevelTotalMangoes);
                const int mangoWidth = MeasureText(mangoLabel, infoFontSize);
                DrawText(mangoLabel, (screenWidth - mangoWidth) / 2, screenHeight / 2 + 10, infoFontSize, RAYWHITE);

                const char *prompt = "Press ENTER to return to level select";
                const int promptFontSize = 24;
                const int promptWidth = MeasureText(prompt, promptFontSize);
                DrawText(prompt, (screenWidth - promptWidth) / 2, screenHeight / 2 + 90, promptFontSize, LIGHTGRAY);

                EndDrawing();

                if (IsKeyPressed(KEY_ENTER)) {
                    state = GameState::LevelSelect;
                }
                break;
            }
        }
    }

    UnloadTexture(mango);
    UnloadTexture(cat);
    CloseWindow();
    return 0;
}
