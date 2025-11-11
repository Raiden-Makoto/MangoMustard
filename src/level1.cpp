#include "level1.h"

#include <raylib.h>
#include <algorithm>
#include <vector>

#include "player_controller.h"

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
void DrawSpikeRow(Vector2 base, int count, float width, float height, std::vector<Rectangle>* hazardRects = nullptr)
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
void DrawSideSpikesLeft(Vector2 start, int count, float size, std::vector<Rectangle>* hazardRects = nullptr)
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

void DrawSideSpikesRight(Vector2 start, int count, float size, std::vector<Rectangle>* hazardRects = nullptr)
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
void DrawSpikesDown(Vector2 start, int count, float width, float height, std::vector<Rectangle>* hazardRects = nullptr)
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

} // namespace

namespace
{
struct Collectible {
    Vector2 position{};
    bool collected = false;
};

struct LevelState {
    PlayerControllerState cat;
    std::vector<Collectible> mangoes;
    Vector2 spawn{};
    bool initialized = false;
    bool failed = false;
};

LevelState& GetLevelState()
{
    static LevelState state;
    return state;
}

void ResetMangoes(LevelState& state, const std::vector<Vector2>& positions)
{
    state.mangoes.clear();
    state.mangoes.reserve(positions.size());
    for (const Vector2& pos : positions) {
        state.mangoes.push_back({pos, false});
    }
}
} // namespace

void DrawLevel1(Texture2D cat,
                float catScale,
                Texture2D mango,
                float mangoScale,
                float deltaTime,
                int& collectedMangoes,
                bool& levelFailed,
                bool& quitToMenu,
                bool& levelRestarted)
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

    std::vector<Rectangle> spikeHazards;
    spikeHazards.reserve(64);

    std::vector<Vector2> mangoPositions;
    mangoPositions.reserve(4);

    const float mangoHover = 18.0f;
    const float mangoWidth = static_cast<float>(mango.width) * mangoScale;
    const float mangoHeight = static_cast<float>(mango.height) * mangoScale;
    mangoPositions.push_back(Vector2{static_cast<float>(screenWidth) * 0.88f,
                                     groundRect.y - mangoHeight - mangoHover});

    // --- Bottom highlight bar (green rectangle) ---
    const Rectangle greenCoreRect{
        static_cast<float>(screenHeight) * 0.600f,  // 720 * 0.6 = 432 px from left
        static_cast<float>(screenHeight) - groundHeight, // 684 px from top
        static_cast<float>(screenWidth) * 0.35f, // 960 * 0.35 = 336 px wide
        groundHeight, // 36 px tall
    };
    DrawRect(greenCoreRect, kHighlightColor);

    
    // --- Floor spike clusters ---
    DrawSpikeRow(Vector2{groundRect.x + 140.0f, groundRect.y}, 3, 28.0f, 30.0f, &spikeHazards);

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
    DrawSpikesDown(Vector2{groundRect.x + 140.0f, distFromTop + platformHeight * 2}, 3, 28.0f, 15.0f, &spikeHazards);

    // -- Second Row Platform Above Grass --
    const float hoverHeight = static_cast<float>(screenHeight) * 0.20f; 
    const Rectangle platformRect3{static_cast<float>(screenWidth) * 0.53f, distFromTop + hoverHeight, static_cast<float>(screenWidth) * 0.18f, platformHeight};
    DrawRect(platformRect3,kPlatformColor);
    platforms.push_back(platformRect3);

    // -- Another Mango --
    mangoPositions.push_back(Vector2{
        static_cast<float>(screenWidth) * 0.60f,
        platformRect3.y - 2 * platformRect3.height - mangoHover});

    // -- Bottom Vertical Spikes --
    DrawSideSpikesLeft(Vector2{static_cast<float>(screenWidth), groundRect.y - 11*30.0f}, 11, 30.0f, &spikeHazards); // match horizontal spike size

    // -- Third Row Platforms --
    distFromTop -= static_cast<float>(screenHeight) * 0.15f; // 720 * 0.05 = 36 px
    const Rectangle platformRect4{0, distFromTop, static_cast<float>(screenWidth) * 0.85f, platformHeight};
    DrawRect(platformRect4,kPlatformColor);
    platforms.push_back(platformRect4);

    // -- Third Row Left-Side Spikes --
    DrawSideSpikesRight(Vector2{0.0f, distFromTop - 6*30.0f}, 6, 30.0f, &spikeHazards);
    
    // -- Third Row Right-Side Spikes --
    DrawSideSpikesLeft(Vector2{static_cast<float>(screenWidth), distFromTop - 3*30.0f}, 3, 30.0f, &spikeHazards); 

    // -- Third Row Vertical Spikes --
    DrawSpikeRow(Vector2{static_cast<float>(screenWidth) * 0.30f, distFromTop}, 3, 30.0f, 30.0f, &spikeHazards);
    DrawSpikeRow(Vector2{static_cast<float>(screenWidth) * 0.50f, distFromTop}, 3, 30.0f, 30.0f, &spikeHazards);

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
    LevelState& levelState = GetLevelState();
    PlayerControllerState& catState = levelState.cat;
    const float catWidth = static_cast<float>(cat.width) * catScale;
    const float catHeight = static_cast<float>(cat.height) * catScale;
    const Vector2 catSpawn{40.0f, groundRect.y - catHeight};
    levelFailed = levelState.failed;
    quitToMenu = false;
    levelRestarted = false;

    const bool resetPressed = IsKeyPressed(KEY_R);
    if (resetPressed || !levelState.initialized) {
        PlayerControllerReset(catState, catSpawn);
        levelState.spawn = catSpawn;
        levelState.initialized = true;
        levelState.failed = false;
    }
    if (levelState.mangoes.size() != mangoPositions.size() || resetPressed || !levelState.initialized) {
        ResetMangoes(levelState, mangoPositions);
    } else {
        for (size_t i = 0; i < mangoPositions.size() && i < levelState.mangoes.size(); ++i) {
            if (!levelState.mangoes[i].collected) {
                levelState.mangoes[i].position = mangoPositions[i];
            }
        }
    }

    const PlayerControllerConfig controllerConfig{
        200.0f,   // moveSpeed
        900.0f,   // gravity
        -420.0f,  // jumpVelocity
        10.0f,     // collisionPadding
        2,        // maxJumps
        static_cast<float>(screenWidth),
        static_cast<float>(screenHeight)
    };

    if (!levelState.failed) {
        PlayerControllerUpdate(catState, deltaTime, platforms, catWidth, catHeight, controllerConfig);
    } else {
        catState.velocity = {0.0f, 0.0f};
    }

    Rectangle catCollisionRect = PlayerControllerCollisionRect(catState, catWidth, catHeight, controllerConfig.collisionPadding);

    bool hitHazard = false;
    if (!levelState.failed) {
        for (const Rectangle& hazard : spikeHazards) {
            if (CheckCollisionRecs(catCollisionRect, hazard)) {
                hitHazard = true;
                break;
            }
        }
    }

    int collectedCount = 0;
    if (hitHazard) {
        levelState.failed = true;
        PlayerControllerReset(catState, levelState.spawn);
        ResetMangoes(levelState, mangoPositions);
        catCollisionRect = PlayerControllerCollisionRect(catState, catWidth, catHeight, controllerConfig.collisionPadding);
    }

    for (auto& mangoState : levelState.mangoes) {
        if (mangoState.collected) {
            collectedCount++;
            continue;
        }
        Rectangle mangoRect{mangoState.position.x, mangoState.position.y, mangoWidth, mangoHeight};
        if (!levelState.failed && CheckCollisionRecs(catCollisionRect, mangoRect)) {
            mangoState.collected = true;
            collectedCount++;
            continue;
        }
        DrawTextureEx(mango, mangoState.position, 0.0f, mangoScale, WHITE);
    }

    DrawTextureEx(cat, catState.position, 0.0f, catScale, WHITE);

    if (levelState.failed) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
        const char *failText = "Level Failed";
        const int titleFont = 48;
        const int titleWidth = MeasureText(failText, titleFont);
        DrawText(failText, (screenWidth - titleWidth) / 2, screenHeight / 2 - 80, titleFont, RED);

        const char *retryText = "Press R to retry";
        const char *quitText = "Press Q to return to level select";
        const int infoFont = 24;
        const int retryWidth = MeasureText(retryText, infoFont);
        const int quitWidth = MeasureText(quitText, infoFont);
        DrawText(retryText, (screenWidth - retryWidth) / 2, screenHeight / 2, infoFont, RAYWHITE);
        DrawText(quitText, (screenWidth - quitWidth) / 2, screenHeight / 2 + 40, infoFont, RAYWHITE);

        if (IsKeyPressed(KEY_R)) {
            PlayerControllerReset(catState, levelState.spawn);
            ResetMangoes(levelState, mangoPositions);
            collectedCount = 0;
            levelState.failed = false;
            levelRestarted = true;
        } else if (IsKeyPressed(KEY_Q)) {
            quitToMenu = true;
            collectedMangoes = collectedCount;
            levelFailed = true;
            return;
        }

        if (levelState.failed) {
            collectedMangoes = 0;
            levelFailed = true;
            return;
        }
    }

    levelFailed = levelState.failed;
    collectedMangoes = collectedCount;
}

int GetLevel1TotalMangoCount()
{
    const LevelState& state = GetLevelState();
    return static_cast<int>(state.mangoes.size());
}

void ResetLevel1State()
{
    LevelState& state = GetLevelState();
    state = LevelState{};
}
