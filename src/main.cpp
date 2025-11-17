#include <raylib.h>
#include "CameraController.h"

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Survior Game");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	CameraController cameraCtrl;

	bool toggleGrid = false;

	// moveable object
	Vector3 objectPos = { 0, 1, 0 };

	while (!WindowShouldClose()) {

		float dt = GetFrameTime();
		
		if (IsKeyPressed(KEY_ESCAPE)) {
			cameraCtrl.ToggleMouse();
		}

		cameraCtrl.Update(dt);

		// move objects with arrow key
		if (IsKeyDown(KEY_UP)) objectPos.z -= 5 * dt;
		if (IsKeyDown(KEY_DOWN)) objectPos.z += 5 * dt;
		if (IsKeyDown(KEY_LEFT)) objectPos.x -= 5 * dt;
		if (IsKeyDown(KEY_RIGHT)) objectPos.x += 5 * dt;

		if (IsKeyPressed(KEY_G)) toggleGrid = !toggleGrid;

		BeginDrawing();
		ClearBackground(SKYBLUE);
		
		BeginMode3D(cameraCtrl.camera);

		// semi infinite ground plane
		if (toggleGrid) DrawGrid(2000, 1.0f);
		DrawCube({ 0, -0.5f, 0 }, 2000, 1, 2000, DARKGREEN);

		// simple sky (fake sky)
		DrawSphere({ 0, -1000, 0 }, 980.0f, Fade(SKYBLUE, 0.4f)); // horizon glow

		// debug gizmo
		DrawLine3D({ 0, 0, 0 }, { 2, 0, 0 }, RED);   // X axis)
		DrawLine3D({ 0, 0, 0 }, { 0, 2, 0 }, GREEN); // Y axis
		DrawLine3D({ 0, 0, 0 }, { 0, 0, 2 }, BLUE);  // Z axis

		// moveable cube
		DrawCube(objectPos, 1, 1, 1, ORANGE);
		DrawCubeWires(objectPos, 1, 1, 1, BROWN);

		EndMode3D();

		DrawText(cameraCtrl.mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);
		DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 70, 20, DARKGRAY);
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
