#include "level_helpers.h"

#include <algorithm>

// Color constants used across all levels
const Color kPlatformColor{245, 245, 245, 255};
const Color kHighlightColor{204, 173, 14, 255}; // mustard-yellow
const Color kIceColor{140, 200, 255, 255};
const Color kSpikeColor{220, 40, 40, 255};
const Color kEndGoalColor{0, 255, 128, 255};

// Draws a solid rectangle representing a platform or highlight strip.
void DrawRect(Rectangle rect, Color color)
{
    DrawRectangleRec(rect, color);
}

// Renders a horizontal row of triangular spikes resting on the baseline at `base.y`.
// `count` controls how many spikes, `width` the footprint of each spike, and `height` their peak.
void DrawSpikeRow(Vector2 base, int count, float width, float height, std::vector<Rectangle>* hazardRects)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * width;
        Vector2 p1{base.x + offset, base.y};
        Vector2 p2{base.x + offset + width * 0.5f, base.y - height};
        Vector2 p3{base.x + offset + width, base.y};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
        if (hazardRects != nullptr) {
            hazardRects->push_back(Rectangle{base.x + offset, base.y - height, width, height});
        }
    }
}

// Draws a vertical column of spikes pointing leftwards, for the right wall hazard.
void DrawSideSpikesLeft(Vector2 start, int count, float size, std::vector<Rectangle>* hazardRects)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * size;
        Vector2 p1{start.x, start.y + offset};
        Vector2 p2{start.x - size, start.y + offset + size * 0.5f};
        Vector2 p3{start.x, start.y + offset + size};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
        if (hazardRects != nullptr) {
            hazardRects->push_back(Rectangle{start.x - size, start.y + offset, size, size});
        }
    }
}

// Draws a vertical column of spikes pointing rightwards, for the left wall hazard.
void DrawSideSpikesRight(Vector2 start, int count, float size, std::vector<Rectangle>* hazardRects)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * size;
        Vector2 p1{start.x, start.y + offset};
        Vector2 p2{start.x + size, start.y + offset + size * 0.5f};
        Vector2 p3{start.x, start.y + offset + size};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
        if (hazardRects != nullptr) {
            hazardRects->push_back(Rectangle{start.x, start.y + offset, size, size});
        }
    }
}

// Draws a row of downward-pointing spikes along a platform ceiling.
void DrawSpikesDown(Vector2 start, int count, float width, float height, std::vector<Rectangle>* hazardRects)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * width;
        Vector2 p1{start.x + offset, start.y};
        Vector2 p2{start.x + offset + width, start.y};
        Vector2 p3{start.x + offset + width * 0.5f, start.y + height};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
        if (hazardRects != nullptr) {
            hazardRects->push_back(Rectangle{start.x + offset, start.y, width, height});
        }
    }
}

// Utility functions for rectangle overlap calculations
float OverlapWidth(const Rectangle& a, const Rectangle& b)
{
    float left = std::max(a.x, b.x);
    float right = std::min(a.x + a.width, b.x + b.width);
    return right - left;
}

float OverlapHeight(const Rectangle& a, const Rectangle& b)
{
    float top = std::max(a.y, b.y);
    float bottom = std::min(a.y + a.height, b.y + b.height);
    return bottom - top;
}

// Helper function to reset collectibles from a list of positions
void ResetCollectibles(std::vector<Collectible>& collectibles, const std::vector<Vector2>& positions)
{
    collectibles.clear();
    collectibles.reserve(positions.size());
    for (const Vector2& pos : positions) {
        collectibles.push_back({pos, false});
    }
}

