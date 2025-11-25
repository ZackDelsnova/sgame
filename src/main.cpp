#include "CameraController.h"
#include "World.h"

#include <raylib.h>
#include <raymath.h>
#include <vector>

typedef enum {
	STATE_MENU,
	STATE_GAME,
	STATE_PAUSE
} GameState;

bool Button(Rectangle bounds, const char* text) {
	Vector2 mouse = GetMousePosition();

	bool hovered = CheckCollisionPointRec(mouse, bounds);
	bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	// background color
	Color bg = hovered ? DARKGRAY : GRAY;

	// draw rect
	DrawRectangleRec(bounds, bg);

	// outline
	DrawRectangleLines(
		static_cast<int>(bounds.x), 
		static_cast<int>(bounds.y), 
		static_cast<int>(bounds.width), 
		static_cast<int>(bounds.height),
		BLACK
	);

	// center text
	int fontSize = 20;
	int textWidth = MeasureText(text, fontSize);
	DrawText(
		text,
		static_cast<int>(bounds.x + (bounds.width - textWidth) / 2),
		static_cast<int>(bounds.y + (bounds.height - fontSize) / 2),
		fontSize,
		WHITE
	);

	return clicked;
}

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Survior Game");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	GameState currentState = STATE_MENU;

	CameraController cameraCtrl;

	World world;
	world.Init();

	while (!WindowShouldClose()) {

		// input for ui
		switch (currentState) {
		case STATE_GAME:
			if (IsKeyPressed(KEY_ESCAPE)) {
				cameraCtrl.UnlockMouse();
				currentState = STATE_PAUSE;
			}
			break;

		case STATE_PAUSE:
			if (IsKeyPressed(KEY_ESCAPE)) {
				cameraCtrl.LockMouse();
				currentState = STATE_GAME;
			}
			break;

		default:
			break;
		}

		float dt = GetFrameTime();

		BeginDrawing();

		switch (currentState) {
		case STATE_MENU: {
			ClearBackground(BLACK);
			DrawText("Survival Game", 330, 80, 40, RAYWHITE);

			if (Button(Rectangle{ 300, 250, 200, 50 }, "Start")) {
				cameraCtrl.LockMouse();
				currentState = STATE_GAME;
			}

			if (Button(Rectangle{ 300, 330, 200, 50 }, "Exit")) {
				EndDrawing();
				CloseWindow();
				return 0;
			}
		} break;

		case STATE_GAME: {
			int cx = GetScreenWidth() / 2;
			int cy = GetScreenHeight() / 2;

			ClearBackground(SKYBLUE);

			cameraCtrl.Update(dt);

			BeginMode3D(cameraCtrl.camera);

			if (IsKeyPressed(KEY_ONE)) {
				world.SpawnAlly(cameraCtrl.camera);
			}
			if (IsKeyPressed(KEY_TWO)) {
				world.SpawnEnemy(cameraCtrl.camera);
			}
			if (IsKeyPressed(KEY_THREE)) {
				world.KillUnitInFront(cameraCtrl.camera);
			}
		

			world.Update(dt, cameraCtrl.camera);
			world.Draw();

			EndMode3D();

			// cross air
			DrawLine(cx - 10, cy, cx + 10, cy, BLACK);
			DrawLine(cx, cy - 10, cx, cy + 10, BLACK);

			DrawFPS(10, 10);
			DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 40, 20, DARKGRAY);
			DrawText(world.GetAllyUnitCount().c_str(), 10, 70, 20, BLUE);
			DrawText(world.GetEnemyUnitCount().c_str(), 10, 90, 20, RED);

		} break;
			
		case STATE_PAUSE: {
			DrawText("Paused", 350, 100, 30, RAYWHITE);

			if (Button(Rectangle{ 300, 250, 200, 50 }, "Resume")) {
				cameraCtrl.LockMouse();
				currentState = STATE_GAME;
			}

			if (Button(Rectangle{ 300, 330, 200, 50 }, "Bact to Menu")) {
				currentState = STATE_MENU;
			}

			DrawText("1 - Spawn Ally (Blue cube)", 550, 250, 30, RAYWHITE);
			DrawText("2 - Spawn Enemy (Red cube)", 550, 350, 30, RAYWHITE);
			DrawText("3 - Kill Units (any ally and enemy)", 550, 450, 30, RAYWHITE);
		} break;
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
