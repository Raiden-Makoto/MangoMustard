#include "level1.h"

#include <raylib.h>

namespace
{
constexpr Color kPlatformColor{245, 245, 245, 255};
constexpr Color kHighlightColor{120, 180, 120, 255};
constexpr Color kIceColor{140, 200, 255, 255};
constexpr Color kSpikeColor{220, 40, 40, 255};

// Draws a solid rectangle representing a platform or highlight strip.
void DrawRect(Rectangle rect, Color color)
{
    DrawRectangleRec(rect, color);
}

// Renders a horizontal row of triangular spikes resting on the baseline at `base.y`.
// `count` controls how many spikes, `width` the footprint of each spike, and `height` their peak.
void DrawSpikeRow(Vector2 base, int count, float width, float height)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * width;
        Vector2 p1{base.x + offset, base.y};
        Vector2 p2{base.x + offset + width * 0.5f, base.y - height};
        Vector2 p3{base.x + offset + width, base.y};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
    }
}

// Draws a vertical column of spikes pointing rightward from the left edge of a corridor.
// Useful for the hazard wall along the right edge of the level.
void DrawVerticalSpikes(Vector2 start, int count, float size)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * size;
        Vector2 p1{start.x, start.y + offset};
        Vector2 p2{start.x + size, start.y + offset + size * 0.5f};
        Vector2 p3{start.x, start.y + offset + size};
        DrawTriangle(p1, p2, p3, kSpikeColor);
    }
}

// Draws a vertical column of spikes pointing leftwards, for the right wall hazard.
void DrawSideSpikesLeft(Vector2 start, int count, float size)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * size;
        Vector2 p1{start.x, start.y + offset};
        Vector2 p2{start.x - size, start.y + offset + size * 0.5f};
        Vector2 p3{start.x, start.y + offset + size};
        DrawTriangle(p1, p2, p3, kSpikeColor);
    }
}

} // namespace

void DrawLevel1(Texture2D mango, float scale)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // --- Ground strip at bottom ---
    const float groundHeight = static_cast<float>(screenHeight) * 0.05f; // 720 * 0.05 = 36 px tall
    const Rectangle groundRect{0.0f, static_cast<float>(screenHeight) - groundHeight, static_cast<float>(screenWidth), groundHeight}; // y = 720 - 36 = 684
    DrawRect(groundRect, kPlatformColor);

    // --- Bottom highlight bar (green rectangle) ---
    const Rectangle greenCoreRect{
        static_cast<float>(screenHeight) * 0.600f,  // 720 * 0.6 = 432 px from left
        static_cast<float>(screenHeight) - groundHeight, // 684 px from top
        static_cast<float>(screenWidth) * 0.35f, // 960 * 0.35 = 336 px wide
        groundHeight, // 36 px tall
    };
    DrawRect(greenCoreRect, kHighlightColor);

    
    // --- Floor spike clusters ---
    DrawRectangleLines(249, 504, 200, 180, GREEN);  // outline the area
    DrawSpikeRow(Vector2{250.0f, 684.0f}, 1, 180.0f, 180.0f); // is not shown at all
    
    // --- Player spawn ---
    //const float catHeight = static_cast<float>(mango.height) * scale;
    //const float catX = 40.0f;
    //const float catY = groundRect.y - catHeight;
    //DrawTextureEx(mango, Vector2{catX, catY}, 0.0f, scale, WHITE);
}
