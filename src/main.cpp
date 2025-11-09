#include <raylib.h>

int main(void) {
    const int screenWidth = 960;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "MangoMustard");

    Texture2D mango = LoadTexture("src/assets/mangocat.png");
    if (mango.id == 0) {
        CloseWindow();
        return 1;
    }

    Vector2 mangoPos = {
        (screenWidth - static_cast<float>(mango.width)) / 2.0f,
        (screenHeight - static_cast<float>(mango.height)) / 2.0f
    };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextureV(mango, mangoPos, WHITE);
        EndDrawing();
    }

    UnloadTexture(mango);
    CloseWindow();
    return 0;
}
