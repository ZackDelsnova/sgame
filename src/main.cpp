#include "CameraController.h"
#include "DynamicBody.h"
#include "World.h"

#include <raylib.h>
#include <raymath.h>
#include <vector>

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Survior Game");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	CameraController cameraCtrl;

	World world;
	world.Init();

	while (!WindowShouldClose()) {

		float dt = GetFrameTime();
		
		if (IsKeyPressed(KEY_ESCAPE)) {
			cameraCtrl.ToggleMouse();
		}

		cameraCtrl.Update(dt);

		BeginDrawing();
		ClearBackground(SKYBLUE);
		
		BeginMode3D(cameraCtrl.camera);

		world.Update(dt);
		world.Draw();

		EndMode3D();

		DrawText(cameraCtrl.mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);
		DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 70, 20, DARKGRAY);
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
