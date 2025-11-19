#include "CameraController.h"
#include <raylib.h>
#include <raymath.h>
#include <vector>

class CubeObject {
public:
	Vector3 position = { 0, 0.5f, 0 };
	Color color = RAYWHITE;
	Vector3 size = { 1, 1, 1 };
	BoundingBox box;
	
	CubeObject(Vector3 pos, Color col, Vector3 s) {
		position = pos;
		color = col;
		size = s;
		UpdateBoundingBox();
	}
	
	void UpdateBoundingBox() {
		box.max = { position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2 };
		box.min = { position.x - size.x / 2, position.y - size.y / 2, position.z - size.z / 2 };
	}

	void move(Vector3 delta) {
		position = Vector3Add(position, delta);
		UpdateBoundingBox();
	}

	void Draw() {
		DrawCubeV(position, size, color);
	}
};

bool CheckCollision(CubeObject cube1, CubeObject cube2) {
	if (CheckCollisionBoxes(cube1.box, cube2.box)) {
		return true;
	}
	return false;
}

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
	CubeObject cube1({ 0, 0.5f, 0 }, RED, { 1, 1, 1 });
	CubeObject cube2({ 0, 0.5f, 2 }, BLUE, { 1, 1, 1});

	Color collisionColor = YELLOW;

	

	while (!WindowShouldClose()) {

		float dt = GetFrameTime();
		
		if (IsKeyPressed(KEY_ESCAPE)) {
			cameraCtrl.ToggleMouse();
		}

		cameraCtrl.Update(dt);

		// move objects with arrow key
		Vector3 input1 = { 0, 0, 0 };
		if (IsKeyDown(KEY_UP)) input1.z -= 1 ;
		if (IsKeyDown(KEY_DOWN)) input1.z += 1;
		if (IsKeyDown(KEY_LEFT)) input1.x -= 1;
		if (IsKeyDown(KEY_RIGHT)) input1.x += 1;
		cube1.move(Vector3Scale(input1, 5 * dt));

		Vector3 input2 = { 0, 0, 0 };
		if (IsKeyDown(KEY_ONE)) input2.z -= 1;
		if (IsKeyDown(KEY_TWO)) input2.z += 1;
		if (IsKeyDown(KEY_THREE)) input2.x -= 1;
		if (IsKeyDown(KEY_FOUR)) input2.x += 1;
		cube2.move(Vector3Scale(input2, 5 * dt));

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
		cube1.Draw();
		DrawBoundingBox(cube1.box, collisionColor);
		cube2.Draw();
		DrawBoundingBox(cube2.box, collisionColor);

		EndMode3D();

		DrawText(cameraCtrl.mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);
		DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 70, 20, DARKGRAY);
		if (CheckCollision(cube1, cube2)) {
			DrawText("COLLISION!", 10, 100, 20, RED);
			collisionColor = RED;
		}
		else {
			collisionColor = YELLOW;
		}
		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
