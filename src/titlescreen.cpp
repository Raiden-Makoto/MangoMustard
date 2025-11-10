#include <raylib.h>
#include "levelselect.h"

void DrawTitleScreen()
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    const char *mangoText = "Mango";
    const char *mustardText = " Mustard";
    const char *sloganText = "we've got bigger mangos to mustard lil bro";
    const char *promptText = "Press ENTER to play";

    const int titleFontSize = 64;
    const int sloganFontSize = 24;
    const int promptFontSize = 28;

    const int mangoWidth = MeasureText(mangoText, titleFontSize);
    const int mustardWidth = MeasureText(mustardText, titleFontSize);
    const int titleWidth = mangoWidth + mustardWidth;

    const float titleX = (screenWidth - titleWidth) / 2.0f;
    const float titleY = screenHeight * 0.15f;

    DrawText(mangoText, static_cast<int>(titleX), static_cast<int>(titleY), titleFontSize, ORANGE);
    DrawText(mustardText, static_cast<int>(titleX) + mangoWidth, static_cast<int>(titleY), titleFontSize, YELLOW);

    const int sloganWidth = MeasureText(sloganText, sloganFontSize);
    const float sloganX = (screenWidth - sloganWidth) / 2.0f;
    const float sloganY = titleY + titleFontSize + 20.0f;

    DrawText(sloganText, static_cast<int>(sloganX), static_cast<int>(sloganY), sloganFontSize, RAYWHITE);

    const int promptWidth = MeasureText(promptText, promptFontSize);
    const float promptX = (screenWidth - promptWidth) / 2.0f;
    const float promptY = screenHeight * 0.75f;

    DrawText(promptText, static_cast<int>(promptX), static_cast<int>(promptY), promptFontSize, RAYWHITE);
}
