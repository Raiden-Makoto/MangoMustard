#include "levelselect.h"

#include <raylib.h>

int UpdateLevelSelect(int selectedLevel, LevelData levels[], int levelCount)
{
    const int minLevel = 1;
    const int maxLevel = levelCount;

    int move = 0;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN)) {
        move = 1;
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP)) {
        move = -1;
    }

    if (move != 0) {
        int nextLevel = selectedLevel;
        for (int attempts = 0; attempts < levelCount; ++attempts) {
            nextLevel += move;
            if (nextLevel > maxLevel)
                nextLevel = minLevel;
            if (nextLevel < minLevel)
                nextLevel = maxLevel;
            if (levels[nextLevel - 1].unlocked) {
                selectedLevel = nextLevel;
                break;
            }
        }
    }

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    const char *titleText = "Level Select";
    const int titleFontSize = 48;
    const int levelFontSize = 32;
    const int instructionFontSize = 24;

    BeginDrawing();
    ClearBackground(BLACK);

    const int titleWidth = MeasureText(titleText, titleFontSize);
    DrawText(titleText, (screenWidth - titleWidth) / 2, screenHeight * 0.1f, titleFontSize, RAYWHITE);

    const char *instructionText = "Use arrow keys, ENTER to confirm";
    const int instructionWidth = MeasureText(instructionText, instructionFontSize);
    DrawText(instructionText,
             (screenWidth - instructionWidth) / 2,
             screenHeight * 0.2f + titleFontSize,
             instructionFontSize,
             RAYWHITE);

    const int columns = 5;
    const int rows = (levelCount + columns - 1) / columns;
    const float gridWidth = columns * 140.0f;
    const float gridHeight = rows * 100.0f;
    const float originX = (screenWidth - gridWidth) / 2.0f;
    const float originY = screenHeight * 0.35f;

    int level = 1;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (level > maxLevel)
                break;

            const float cellX = originX + col * 140.0f;
            const float cellY = originY + row * 100.0f;

            const Rectangle cellRect = {cellX, cellY, 120.0f, 80.0f};
            const bool unlocked = levels[level - 1].unlocked;
            const bool isSelected = (level == selectedLevel) && unlocked;

            DrawRectangleRec(cellRect,
                             unlocked ? (isSelected ? GOLD : Fade(LIGHTGRAY, 0.4f)) : Fade(GRAY, 0.3f));
            DrawRectangleLinesEx(cellRect, 2.0f, unlocked ? RAYWHITE : Fade(RAYWHITE, 0.3f));

            const char *levelText = TextFormat("%d", level);
            const int textWidth = MeasureText(levelText, levelFontSize);
            DrawText(levelText,
                     static_cast<int>(cellRect.x + (cellRect.width - textWidth) / 2),
                     static_cast<int>(cellRect.y + (cellRect.height - levelFontSize) / 2),
                     levelFontSize,
                     unlocked ? (isSelected ? RAYWHITE : WHITE) : Fade(WHITE, 0.4f));

            if (!unlocked) {
                const char *lockText = "LOCKED";
                const int lockWidth = MeasureText(lockText, 18);
                DrawText(lockText,
                         static_cast<int>(cellRect.x + (cellRect.width - lockWidth) / 2),
                         static_cast<int>(cellRect.y + cellRect.height - 24),
                         18,
                         Fade(WHITE, 0.6f));
            }

            level++;
        }
    }

    EndDrawing();

    return selectedLevel;
}
