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

    Texture2D mango = LoadTexture("src/assets/mangocat.png");
    if (mango.id == 0) {
        CloseWindow();
        return 1;
    }

    const float scale = 0.15f; // 85% smaller than original size
    const Vector2 mangoSize = {
        static_cast<float>(mango.width) * scale,
        static_cast<float>(mango.height) * scale
    };

    Vector2 titleMangoPos = {
        (screenWidth - mangoSize.x) / 2.0f,
        (screenHeight - mangoSize.y) / 2.0f + 120.0f
    };

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
                DrawTextureEx(mango, titleMangoPos, 0.0f, scale, WHITE);

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
                        state = GameState::Level1;
                        levelStartTime = GetTime();
                    }
                }
                break;
            }
            case GameState::Level1: {
                BeginDrawing();
                ClearBackground(BLACK);

                DrawLevel1(mango, scale);
                DrawLevelLabel(1);
                int elapsedSeconds = static_cast<int>(GetTime() - levelStartTime);
                DrawTimerLabel(elapsedSeconds);

                EndDrawing();
                break;
            }
        }
    }

    UnloadTexture(mango);
    CloseWindow();
    return 0;
}
