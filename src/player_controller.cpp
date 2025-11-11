#include "player_controller.h"

#include <algorithm>

void PlayerControllerReset(PlayerControllerState& state, Vector2 spawn)
{
    state.position = spawn;
    state.velocity = {0.0f, 0.0f};
    state.grounded = true;
    state.jumpsUsed = 0;
}

void PlayerControllerUpdate(PlayerControllerState& state,
                            float deltaTime,
                            const std::vector<Rectangle>& platforms,
                            float spriteWidth,
                            float spriteHeight,
                            const PlayerControllerConfig& config)
{
    float desiredX = state.position.x;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        desiredX += config.moveSpeed * deltaTime;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        desiredX -= config.moveSpeed * deltaTime;
    }

    Rectangle horizontalRect{desiredX + config.collisionPadding,
                             state.position.y + config.collisionPadding,
                             spriteWidth - config.collisionPadding * 2.0f,
                             spriteHeight - config.collisionPadding * 2.0f};

    for (const Rectangle& platform : platforms) {
        if (CheckCollisionRecs(horizontalRect, platform)) {
            if (desiredX > state.position.x) {
                desiredX = platform.x - spriteWidth + config.collisionPadding;
            } else {
                desiredX = platform.x + platform.width - config.collisionPadding;
            }
            horizontalRect.x = desiredX + config.collisionPadding;
        }
    }

    state.position.x = std::clamp(desiredX, 0.0f, config.screenWidth - spriteWidth);

    state.velocity.y += config.gravity * deltaTime;

    if (IsKeyPressed(KEY_UP) && state.jumpsUsed < config.maxJumps) {
        state.velocity.y = config.jumpVelocity;
        state.grounded = false;
        state.jumpsUsed++;
    }

    float desiredY = state.position.y + state.velocity.y * deltaTime;
    Rectangle verticalRect{state.position.x + config.collisionPadding,
                            desiredY + config.collisionPadding,
                            spriteWidth - config.collisionPadding * 2.0f,
                            spriteHeight - config.collisionPadding * 2.0f};

    state.grounded = false;

    for (const Rectangle& platform : platforms) {
        if (CheckCollisionRecs(verticalRect, platform)) {
            if (desiredY > state.position.y) {
                desiredY = platform.y - spriteHeight + config.collisionPadding;
                state.velocity.y = 0.0f;
                state.grounded = true;
                state.jumpsUsed = 0;
            } else {
                desiredY = platform.y + platform.height - config.collisionPadding;
                state.velocity.y = 0.0f;
                state.jumpsUsed = std::max(state.jumpsUsed, config.maxJumps - 1);
            }
            verticalRect.y = desiredY + config.collisionPadding;
        }
    }

    state.position.y = std::clamp(desiredY, -spriteHeight, config.screenHeight - spriteHeight);

    if (state.position.y >= config.screenHeight - spriteHeight - 0.1f) {
        state.position.y = config.screenHeight - spriteHeight;
        state.velocity.y = 0.0f;
        state.grounded = true;
        state.jumpsUsed = 0;
    }
}

Rectangle PlayerControllerCollisionRect(const PlayerControllerState& state,
                                        float spriteWidth,
                                        float spriteHeight,
                                        float padding)
{
    return Rectangle{
        state.position.x + padding,
        state.position.y + padding,
        spriteWidth - padding * 2.0f,
        spriteHeight - padding * 2.0f};
}

