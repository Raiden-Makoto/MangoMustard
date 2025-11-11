#include "hud.h"

#include <raylib.h>

void DrawLevelLabel(int levelNumber)
{
    const char *levelLabel = TextFormat("Level %d", levelNumber);
    DrawText(levelLabel, 16, 16, 32, ORANGE);
}

void DrawTimerLabel(int elapsedSeconds)
{
    const int minutes = elapsedSeconds / 60;
    const int seconds = elapsedSeconds % 60;
    const char *timerLabel = TextFormat("%02d:%02d", minutes, seconds);
    const int fontSize = 32;
    const int textWidth = MeasureText(timerLabel, fontSize);
    const int padding = 16;
    DrawText(timerLabel, GetScreenWidth() - textWidth - padding, padding, fontSize, YELLOW);
}

void DrawMangoCounter(int collected, int total)
{
    const char *counterLabel = TextFormat("Mangoes %d/%d", collected, total);
    const int fontSize = 22;
    const int padding = 16;
    const int timerSpacing = 32;
    const int verticalOffset = padding + timerSpacing + 8;
    const int textWidth = MeasureText(counterLabel, fontSize);
    DrawText(counterLabel, GetScreenWidth() - textWidth - padding, verticalOffset, fontSize, YELLOW);
}
