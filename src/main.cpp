#include "CameraController.h"
#include <raylib.h>
#include <vector>

struct CubeObject {
	Vector3 position;
	Color color;
	float size;
};

bool CheckCollision(CubeObject obj1, CubeObject obj2);

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
	CubeObject cube1 = { { 0, 1, 0 }, RED, 1 };
	CubeObject cube2 = { { 2, 1, 0 }, BLUE, 1 };

	while (!WindowShouldClose()) {

		float dt = GetFrameTime();
		
		if (IsKeyPressed(KEY_ESCAPE)) {
			cameraCtrl.ToggleMouse();
		}

		cameraCtrl.Update(dt);

		// move objects with arrow key
		if (IsKeyDown(KEY_UP)) cube1.position.z -= 5 * dt;
		if (IsKeyDown(KEY_DOWN)) cube1.position.z += 5 * dt;
		if (IsKeyDown(KEY_LEFT)) cube1.position.x -= 5 * dt;
		if (IsKeyDown(KEY_RIGHT)) cube1.position.x += 5 * dt;

		if (IsKeyDown(KEY_ONE)) cube2.position.z -= 5 * dt;
		if (IsKeyDown(KEY_TWO)) cube2.position.z += 5 * dt;
		if (IsKeyDown(KEY_THREE)) cube2.position.x -= 5 * dt;
		if (IsKeyDown(KEY_FOUR)) cube2.position.x += 5 * dt;

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
		DrawCube(cube1.position, cube1.size, cube1.size, cube1.size, cube1.color);
		DrawCube(cube2.position, cube2.size, cube2.size, cube2.size, cube2.color);

		EndMode3D();

		DrawText(cameraCtrl.mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);
		DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 70, 20, DARKGRAY);
		if (CheckCollision(cube1, cube2)) {
			DrawText("COLLISION!", 10, 100, 20, RED);
		}
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

bool CheckCollision(CubeObject obj1, CubeObject obj2) {
	BoundingBox box1 = { { obj1.position.x - obj1.size / 2, obj1.position.y - obj1.size / 2, obj1.position.z - obj1.size / 2 },
						 { obj1.position.x + obj1.size / 2, obj1.position.y + obj1.size / 2, obj1.position.z + obj1.size / 2 } };
	BoundingBox box2 = { { obj2.position.x - obj2.size / 2, obj2.position.y - obj2.size / 2, obj2.position.z - obj2.size / 2 },
						 { obj2.position.x + obj2.size / 2, obj2.position.y + obj2.size / 2, obj2.position.z + obj2.size / 2 } };
	if (CheckCollisionBoxes(box1, box2)) {
		return true;
	}
	return false;
}