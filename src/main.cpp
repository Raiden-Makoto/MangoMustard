#include <raylib.h>

void DrawTitleScreen();

int main(void) {
    const int screenWidth = 960;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "MangoMustard");

    Texture2D mango = LoadTexture("src/assets/mangocat.png");
    if (mango.id == 0) {
        CloseWindow();
        return 1;
    }

    const float scale = 0.15f; // 85% smaller than original size
    const Vector2 mangoSize = {
        static_cast<float>(mango.width) * scale,
        static_cast<float>(mango.height) * scale
    };

    Vector2 mangoPos = {
        (screenWidth - mangoSize.x) / 2.0f,
        (screenHeight - mangoSize.y) / 2.0f + 120.0f
    };

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTitleScreen();
        DrawTextureEx(mango, mangoPos, 0.0f, scale, WHITE);

        EndDrawing();
    }

    UnloadTexture(mango);
    CloseWindow();
    return 0;
}
