#include "CameraController.h"
#include "DynamicBody.h"
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
		
		/*
		if (IsKeyPressed(KEY_ESCAPE)) {
			cameraCtrl.ToggleMouse();
		}
		*/

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
			ClearBackground(SKYBLUE);

			cameraCtrl.Update(dt);

			BeginMode3D(cameraCtrl.camera);

			world.Update(dt);
			world.Draw();

			EndMode3D();

			DrawText(cameraCtrl.mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);
			DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 70, 20, DARKGRAY);
			DrawFPS(10, 10);
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
		} break;
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
