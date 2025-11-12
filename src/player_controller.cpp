#include "player_controller.h"

#include <algorithm>

void PlayerControllerReset(PlayerControllerState& state, Vector2 spawn)
{
    state.position = spawn;
    state.velocity = {0.0f, 0.0f};
    state.grounded = true;
    state.jumpsUsed = 0;
    state.horizontalVelocity = 0.0f;
    state.onIce = false;
    state.inputLockTimer = 0.0;
}

void PlayerControllerUpdate(PlayerControllerState& state,
                            float deltaTime,
                            const std::vector<Rectangle>& platforms,
                            float spriteWidth,
                            float spriteHeight,
                            const PlayerControllerConfig& config)
{
    if (state.inputLockTimer > 0.0f) {
        state.inputLockTimer -= deltaTime;
        if (state.inputLockTimer < 0.0f) {
            state.inputLockTimer = 0.0f;
        }
    }

    const bool jumpRequested = (state.inputLockTimer <= 0.0f) && IsKeyPressed(KEY_UP);

    float moveInput = 0.0f;
    if (state.inputLockTimer <= 0.0f) {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            moveInput += 1.0f;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            moveInput -= 1.0f;
        }
    }

    const bool noInput = (moveInput == 0.0f);
    if (!noInput) {
        state.horizontalVelocity = moveInput * config.moveSpeed;
    } else {
        if (state.onIce || !state.grounded) {
            // retain momentum on ice or while airborne
        } else {
            state.horizontalVelocity = 0.0f;
        }
    }

    float desiredX = state.position.x + state.horizontalVelocity * deltaTime;

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
            if (!(state.onIce || state.grounded == false)) {
                state.horizontalVelocity = 0.0f;
            }
        }
    }

    state.position.x = std::clamp(desiredX, 0.0f, config.screenWidth - spriteWidth);

    state.velocity.y += config.gravity * deltaTime;

    bool wasGrounded = state.grounded;
    bool wasOnIce = state.onIce;
    bool canJump = (state.inputLockTimer <= 0.0f) &&
                   (state.jumpsUsed < config.maxJumps) &&
                   (!wasGrounded || !wasOnIce);

    if (jumpRequested && canJump) {
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
    state.onIce = false;
    const std::vector<Rectangle>* icy = config.icyPlatforms;

    for (const Rectangle& platform : platforms) {
        if (CheckCollisionRecs(verticalRect, platform)) {
            if (desiredY > state.position.y) {
                desiredY = platform.y - spriteHeight + config.collisionPadding;
                state.velocity.y = 0.0f;
                state.grounded = true;
                state.jumpsUsed = 0;
                if (icy != nullptr) {
                    for (const Rectangle& icyRect : *icy) {
                        if (CheckCollisionRecs(verticalRect, icyRect)) {
                            state.onIce = true;
                            break;
                        }
                    }
                }
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
        state.onIce = false;
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

