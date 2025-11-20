#include "CameraController.h"
#include "Body.h"

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

	Body testBody({ 0, 1, 0 }, { 1, 1, 1 }, RED);

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

		testBody.Update(dt);
		testBody.Draw();

		EndMode3D();

		DrawText(cameraCtrl.mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);
		DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 70, 20, DARKGRAY);
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
