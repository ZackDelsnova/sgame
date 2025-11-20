#include "CameraController.h"
#include "StaticBody.h"
#include "DynamicBody.h"

#include <raylib.h>
#include <raymath.h>
#include <vector>

Vector3 NormalizeOrZero(Vector3 v) {
	float l = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	if (l > 0.00001f) {
		return Vector3Scale(v, 1.0f / l);
	}
	return { 0, 0, 0 };
}

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Survior Game");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	CameraController cameraCtrl;

	StaticBody floorBody({ 0.0f, -0.5f, 0.0f }, { 2000, 1, 2000 }, DARKGREEN);
	DynamicBody boxBody({ 0.0f, 10.0f, 0.0f }, { 1, 1, 1 }, ORANGE);

	bool toggleGrid = false;

	while (!WindowShouldClose()) {

		float dt = GetFrameTime();
		
		if (IsKeyPressed(KEY_ESCAPE)) {
			cameraCtrl.ToggleMouse();
		}

		cameraCtrl.Update(dt);

		if (IsKeyPressed(KEY_G)) toggleGrid = !toggleGrid;

		BeginDrawing();
		ClearBackground(SKYBLUE);
		
		BeginMode3D(cameraCtrl.camera);

		// semi infinite ground plane
		if (toggleGrid) DrawGrid(2000, 1.0f);

		// simple sky (fake sky)
		DrawSphere({ 0, -1000, 0 }, 980.0f, Fade(SKYBLUE, 0.4f)); // horizon glow

		// debug xyz axis
		DrawLine3D({ 0, 0, 0 }, { 2, 0, 0 }, RED);   // X axis)
		DrawLine3D({ 0, 0, 0 }, { 0, 2, 0 }, GREEN); // Y axis
		DrawLine3D({ 0, 0, 0 }, { 0, 0, 2 }, BLUE);  // Z axis

		floorBody.Draw();
		boxBody.Update(dt);

		EndMode3D();

		DrawText(cameraCtrl.mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);
		DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 70, 20, DARKGRAY);
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
