#pragma once

#include <raylib.h>
#include <vector>

// Color constants used across all levels
extern const Color kPlatformColor;
extern const Color kHighlightColor; // mustard-yellow
extern const Color kIceColor;
extern const Color kSpikeColor;
extern const Color kEndGoalColor;

// Draws a solid rectangle representing a platform or highlight strip.
void DrawRect(Rectangle rect, Color color);

// Renders a horizontal row of triangular spikes resting on the baseline at `base.y`.
// `count` controls how many spikes, `width` the footprint of each spike, and `height` their peak.
void DrawSpikeRow(Vector2 base, int count, float width, float height, std::vector<Rectangle>* hazardRects = nullptr);

// Draws a vertical column of spikes pointing leftwards, for the right wall hazard.
void DrawSideSpikesLeft(Vector2 start, int count, float size, std::vector<Rectangle>* hazardRects = nullptr);

// Draws a vertical column of spikes pointing rightwards, for the left wall hazard.
void DrawSideSpikesRight(Vector2 start, int count, float size, std::vector<Rectangle>* hazardRects = nullptr);

// Draws a row of downward-pointing spikes along a platform ceiling.
void DrawSpikesDown(Vector2 start, int count, float width, float height, std::vector<Rectangle>* hazardRects = nullptr);

// Utility functions for rectangle overlap calculations
float OverlapWidth(const Rectangle& a, const Rectangle& b);
float OverlapHeight(const Rectangle& a, const Rectangle& b);

// Collectible struct for mangoes and other collectibles
struct Collectible {
    Vector2 position{};
    bool collected = false;
};

// Helper function to reset collectibles from a list of positions
void ResetCollectibles(std::vector<Collectible>& collectibles, const std::vector<Vector2>& positions);

