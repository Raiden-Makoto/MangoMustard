#include "level1.h"

#include <raylib.h>
#include <algorithm>
#include <vector>

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

// Draws a vertical column of spikes pointing leftwards, for the right wall hazard.
void DrawSideSpikesLeft(Vector2 start, int count, float size)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * size;
        Vector2 p1{start.x, start.y + offset};
        Vector2 p2{start.x - size, start.y + offset + size * 0.5f};
        Vector2 p3{start.x, start.y + offset + size};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
    }
}

void DrawSideSpikesRight(Vector2 start, int count, float size)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * size;
        Vector2 p1{start.x, start.y + offset};
        Vector2 p2{start.x + size, start.y + offset + size * 0.5f};
        Vector2 p3{start.x, start.y + offset + size};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
    }
}

// Draws a row of downward-pointing spikes along a platform ceiling.
void DrawSpikesDown(Vector2 start, int count, float width, float height)
{
    for (int i = 0; i < count; ++i) {
        const float offset = i * width;
        Vector2 p1{start.x + offset, start.y};
        Vector2 p2{start.x + offset + width, start.y};
        Vector2 p3{start.x + offset + width * 0.5f, start.y + height};
        DrawTriangleLines(p1, p2, p3, kSpikeColor);
    }
}

} // namespace

namespace
{
struct CatState {
    Vector2 position{};
    Vector2 velocity{};
    bool grounded = false;
};

CatState& GetCatState()
{
    static CatState state;
    return state;
}
} // namespace

void DrawLevel1(Texture2D cat, float catScale, Texture2D mango, float mangoScale, float deltaTime)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // --- Ground strip at bottom ---
    const float groundHeight = static_cast<float>(screenHeight) * 0.05f; // 720 * 0.05 = 36 px tall
    const Rectangle groundRect{0.0f, static_cast<float>(screenHeight) - groundHeight, static_cast<float>(screenWidth), groundHeight}; // y = 720 - 36 = 684
    DrawRect(groundRect, kPlatformColor);
    std::vector<Rectangle> platforms;
    platforms.reserve(12);
    platforms.push_back(groundRect);

    const float mangoHover = 18.0f;
    const float mangoWidth = static_cast<float>(mango.width) * mangoScale;
    const float mangoHeight = static_cast<float>(mango.height) * mangoScale;
    float mangoX = static_cast<float>(screenWidth) * 0.88f;
    float mangoY = groundRect.y - mangoHeight - mangoHover;

    // --- Mango ---
    DrawTextureEx(mango, Vector2{mangoX, mangoY}, 0.0f, mangoScale, WHITE);

    // --- Bottom highlight bar (green rectangle) ---
    const Rectangle greenCoreRect{
        static_cast<float>(screenHeight) * 0.600f,  // 720 * 0.6 = 432 px from left
        static_cast<float>(screenHeight) - groundHeight, // 684 px from top
        static_cast<float>(screenWidth) * 0.35f, // 960 * 0.35 = 336 px wide
        groundHeight, // 36 px tall
    };
    DrawRect(greenCoreRect, kHighlightColor);

    
    // --- Floor spike clusters ---
    DrawSpikeRow(Vector2{groundRect.x + 140.0f, groundRect.y}, 3, 28.0f, 30.0f);

    // -- Second Row Platforms --
    const float platformHeight = static_cast<float>(screenHeight) * 0.025f; // 720 * 0.025 = 18 px tall
    float distFromTop = static_cast<float>(screenHeight) * 0.65f; // 720 * 0.6 = 432 px from top
    const Rectangle platformRect1{0.0f, distFromTop + platformHeight, static_cast<float>(screenWidth) * 0.25f, platformHeight};
    DrawRect(platformRect1, kPlatformColor);
    platforms.push_back(platformRect1);

    const Rectangle platformRect2{static_cast<float>(screenWidth) * 0.25f, distFromTop + platformHeight, static_cast<float>(screenWidth) * 0.16f, platformHeight};
    DrawRect(platformRect2,kIceColor);
    platforms.push_back(platformRect2);

    // -- Under 2nd Row Spikes --
    DrawSpikesDown(Vector2{groundRect.x + 140.0f, distFromTop + platformHeight * 2}, 3, 28.0f, 15.0f);

    // -- Second Row Platform Above Grass --
    const float hoverHeight = static_cast<float>(screenHeight) * 0.20f; 
    const Rectangle platformRect3{static_cast<float>(screenWidth) * 0.53f, distFromTop + hoverHeight, static_cast<float>(screenWidth) * 0.18f, platformHeight};
    DrawRect(platformRect3,kPlatformColor);
    platforms.push_back(platformRect3);

    // -- Another Mango --
    mangoX = static_cast<float>(screenWidth) * 0.60f;
    mangoY = platformRect3.y - 2 * platformRect3.height - mangoHover;
    DrawTextureEx(mango, Vector2{mangoX, mangoY}, 0.0f, mangoScale, WHITE);

    // -- Bottom Vertical Spikes --
    DrawSideSpikesLeft(Vector2{static_cast<float>(screenWidth), groundRect.y - 11*30.0f}, 11, 30.0f); // match horizontal spike size

    // -- Third Row Platforms --
    distFromTop -= static_cast<float>(screenHeight) * 0.15f; // 720 * 0.05 = 36 px
    const Rectangle platformRect4{0, distFromTop, static_cast<float>(screenWidth) * 0.85f, platformHeight};
    DrawRect(platformRect4,kPlatformColor);
    platforms.push_back(platformRect4);

    // -- Third Row Left-Side Spikes --
    DrawSideSpikesRight(Vector2{0.0f, distFromTop - 6*30.0f}, 6, 30.0f);
    
    // -- Third Row Right-Side Spikes --
    DrawSideSpikesLeft(Vector2{static_cast<float>(screenWidth), distFromTop - 3*30.0f}, 3, 30.0f); 

    // -- Third Row Vertical Spikes --
    DrawSpikeRow(Vector2{static_cast<float>(screenWidth) * 0.30f, distFromTop}, 3, 30.0f, 30.0f);
    DrawSpikeRow(Vector2{static_cast<float>(screenWidth) * 0.50f, distFromTop}, 3, 30.0f, 30.0f);

    // Fourth Row Platforms --
    distFromTop -= static_cast<float>(screenHeight) * 0.15f; // 720 * 0.05 = 36 px
    const Rectangle platformRect5{0, distFromTop - platformHeight - 2 * 30.0f, static_cast<float>(screenWidth) * 0.28f, platformHeight};
    DrawRect(platformRect5,kPlatformColor);
    platforms.push_back(platformRect5);

    // 3.5fth Row Platform -- 
    const Rectangle platformRect6{
        static_cast<float>(screenWidth) * 0.37f,
        distFromTop + platformHeight,
        static_cast<float>(screenWidth) * 0.15f,
        platformHeight
    };
    DrawRect(platformRect6,kPlatformColor);
    platforms.push_back(platformRect6);

    // --- Player spawn ---
    CatState& catState = GetCatState();
    const float catWidth = static_cast<float>(cat.width) * catScale;
    const float catHeight = static_cast<float>(cat.height) * catScale;
    const Vector2 catSpawn{40.0f, groundRect.y - catHeight};
    if (IsKeyPressed(KEY_R)) {
        catState.position = catSpawn;
        catState.velocity = {0.0f, 0.0f};
        catState.grounded = true;
    }
    if (catState.position.y == 0.0f && catState.position.x == 0.0f) {
        catState.position = catSpawn;
        catState.grounded = true;
    }

    const float moveSpeed = 200.0f;
    float desiredX = catState.position.x;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        desiredX += moveSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        desiredX -= moveSpeed * deltaTime;
    }
    const float collisionPadding = 10.0f;
    Rectangle horizontalRect{desiredX + collisionPadding,
                             catState.position.y + collisionPadding,
                             catWidth - collisionPadding * 2.0f,
                             catHeight - collisionPadding * 2.0f};
    for (const auto& platform : platforms) {
        if (CheckCollisionRecs(horizontalRect, platform)) {
            if (desiredX > catState.position.x) {
                desiredX = platform.x - catWidth + collisionPadding;
                horizontalRect.x = desiredX + collisionPadding;
            } else {
                desiredX = platform.x + platform.width - collisionPadding;
                horizontalRect.x = desiredX + collisionPadding;
            }
        }
    }
    catState.position.x = std::clamp(desiredX, 0.0f, static_cast<float>(screenWidth) - catWidth);

    const float gravity = 900.0f;
    const float jumpVelocity = -420.0f;
    catState.velocity.y += gravity * deltaTime;
    if (catState.grounded && IsKeyPressed(KEY_UP)) {
        catState.velocity.y = jumpVelocity;
        catState.grounded = false;
    }
    float desiredY = catState.position.y + catState.velocity.y * deltaTime;
    Rectangle verticalRect{catState.position.x + collisionPadding,
                            desiredY + collisionPadding,
                            catWidth - collisionPadding * 2.0f,
                            catHeight - collisionPadding * 2.0f};
    catState.grounded = false;
    for (const auto& platform : platforms) {
        if (CheckCollisionRecs(verticalRect, platform)) {
            if (desiredY > catState.position.y) {
                desiredY = platform.y - catHeight + collisionPadding;
                verticalRect.y = desiredY + collisionPadding;
                catState.velocity.y = 0.0f;
                catState.grounded = true;
            } else {
                desiredY = platform.y + platform.height - collisionPadding;
                verticalRect.y = desiredY + collisionPadding;
                catState.velocity.y = 0.0f;
            }
        }
    }
    catState.position.y = desiredY;

    DrawTextureEx(cat, catState.position, 0.0f, catScale, WHITE);
}
