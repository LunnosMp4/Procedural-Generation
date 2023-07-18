#include <iostream>
#include <raylib.h>
#include <libnoise/module/perlin.h>

const int WIDTH = 128;   // Width of the terrain grid
const int HEIGHT = 128;  // Height of the terrain grid

const int SCREEN_WIDTH = 512;   // Width of the window
const int SCREEN_HEIGHT = 512;  // Height of the window

const float SCALE_X = static_cast<float>(SCREEN_WIDTH) / WIDTH;
const float SCALE_Y = static_cast<float>(SCREEN_HEIGHT) / HEIGHT;

const int CHUNK_SIZE = 256;

void generateTerrain(double terrain[WIDTH][HEIGHT], int offsetX, int offsetY)
{
    noise::module::Perlin perlin;

    perlin.SetSeed(12345);

    perlin.SetFrequency(1.0);
    perlin.SetPersistence(0.2);

    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
            double nx = static_cast<double>(x + offsetX) / WIDTH;
            double ny = static_cast<double>(y + offsetY) / HEIGHT;

            double noiseValue = perlin.GetValue(nx, ny, 0.0);
            terrain[x][y] = noiseValue;
        }
    }
}

void renderTerrain(double terrain[WIDTH][HEIGHT]) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Map the terrain heights to the range [0, 255]
            double height = (terrain[x][y] + 1.0) / 2.0 * 255;

            Color color = { static_cast<u_char>(height), static_cast<u_char>(height), static_cast<u_char>(height), 255 };
            DrawRectangle(x * SCALE_X, y * SCALE_Y, SCALE_X, SCALE_Y, color);
        }
    }

    EndDrawing();
}
    

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Procedural Terrain");
    double terrain[WIDTH][HEIGHT];

    generateTerrain(terrain, 0, 0);

    int playerX = 0;
    int playerY = 0;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W))
            playerY -= 5;
        if (IsKeyDown(KEY_S))
            playerY += 5;
        if (IsKeyDown(KEY_A))
            playerX -= 5;
        if (IsKeyDown(KEY_D))
            playerX += 5;

        int offsetX = playerX / 2;
        int offsetY = playerY / 2;

        generateTerrain(terrain, offsetX, offsetY);
        renderTerrain(terrain);

        std::cout << "Player position: (" << playerX << ", " << playerY << ")" << std::endl;
    }

    CloseWindow();

    return 0;
}