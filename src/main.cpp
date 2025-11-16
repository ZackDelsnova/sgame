#include "raylib.h"

int main() {
	InitWindow(800, 600, "Basic Raylib Window");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("Hello, Raylib!", 350, 280, 20, RAYWHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}