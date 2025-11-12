#pragma once

#include <raylib.h>
#include <vector>

struct PlayerControllerState {
    Vector2 position{};
    Vector2 velocity{};
    bool grounded = false;
    int jumpsUsed = 0;
    float horizontalVelocity = 0.0f;
    bool onIce = false;
    double inputLockTimer = 0.0;
};

struct PlayerControllerConfig {
    float moveSpeed;
    float gravity;
    float jumpVelocity;
    float collisionPadding;
    int maxJumps;
    float screenWidth;
    float screenHeight;
    const std::vector<Rectangle>* icyPlatforms = nullptr;
};

void PlayerControllerReset(PlayerControllerState& state, Vector2 spawn);

void PlayerControllerUpdate(PlayerControllerState& state,
                            float deltaTime,
                            const std::vector<Rectangle>& platforms,
                            float spriteWidth,
                            float spriteHeight,
                            const PlayerControllerConfig& config);

Rectangle PlayerControllerCollisionRect(const PlayerControllerState& state,
                                        float spriteWidth,
                                        float spriteHeight,
                                        float padding);

