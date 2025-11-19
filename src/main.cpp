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
	
	float vVelocity = 0.0f;

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

	void ApplyGrayity(float dt) {
		const float gravity = -9.81f;
		
		vVelocity += gravity * dt;
		position.y += vVelocity * dt;

		if (position.y < size.y / 2) {
			position.y = size.y / 2;
			vVelocity = 0.0f;
		}

		UpdateBoundingBox();
	}

	void ResolveCollision(CubeObject& other) {
		if (!CheckCollisionBoxes(box, other.box)) {
			return;
		}

		float dx1 = other.box.max.x - box.min.x; // overlap on x from other to this
		float dx2 = box.max.x - other.box.min.x; // overlap on x from this to other
		float dy1 = other.box.max.y - box.min.y; // overlap on y from other to this
		float dy2 = box.max.y - other.box.min.y; // overlap on y from this to other
		float dz1 = other.box.max.z - box.min.z; // overlap on z from other to this
		float dz2 = box.max.z - other.box.min.z; // overlap on z from this to other

		float minX = (dx1 < dx2) ? dx1 : -dx2;
		float minY = (dy1 < dy2) ? dy1 : -dy2;
		float minZ = (dz1 < dz2) ? dz1 : -dz2;

		if (fabs(minX) < fabs(minY) && fabs(minX) < fabs(minZ)) {
			move({ minX, 0, 0 });
		}
		else if (fabs(minY) < fabs(minX) && fabs(minY) < fabs(minZ)) {
			move({ 0, minY, 0 });
			vVelocity = 0.0f; // reset vertical velocity on Y collision
		}
		else {
			move({ 0, 0, minZ });
		}

		UpdateBoundingBox();
	}
};

bool CheckCollision(CubeObject cube1, CubeObject cube2) {
	if (CheckCollisionBoxes(cube1.box, cube2.box)) {
		return true;
	}
	return false;
}

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

	bool toggleGrid = false;

	CubeObject floorCube({ 0, -0.5f, 0 }, DARKGREEN, { 2000, 1, 2000 });

	// moveable object
	CubeObject cube1({ 0, 10, 0 }, RED, { 1, 1, 1 });
	CubeObject cube2({ 0, 10, 2 }, BLUE, { 1, 1, 1});

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
		input1 = NormalizeOrZero(input1);
		cube1.move(Vector3Scale(input1, 5 * dt));

		Vector3 input2 = { 0, 0, 0 };
		if (IsKeyDown(KEY_ONE)) input2.z -= 1;
		if (IsKeyDown(KEY_TWO)) input2.z += 1;
		if (IsKeyDown(KEY_THREE)) input2.x -= 1;
		if (IsKeyDown(KEY_FOUR)) input2.x += 1;
		input2 = NormalizeOrZero(input2);
		cube2.move(Vector3Scale(input2, 5 * dt));

		if (IsKeyPressed(KEY_G)) toggleGrid = !toggleGrid;

		BeginDrawing();
		ClearBackground(SKYBLUE);
		
		BeginMode3D(cameraCtrl.camera);

		// semi infinite ground plane
		if (toggleGrid) DrawGrid(2000, 1.0f);
		floorCube.Draw();

		// simple sky (fake sky)
		DrawSphere({ 0, -1000, 0 }, 980.0f, Fade(SKYBLUE, 0.4f)); // horizon glow

		// debug gizmo
		DrawLine3D({ 0, 0, 0 }, { 2, 0, 0 }, RED);   // X axis)
		DrawLine3D({ 0, 0, 0 }, { 0, 2, 0 }, GREEN); // Y axis
		DrawLine3D({ 0, 0, 0 }, { 0, 0, 2 }, BLUE);  // Z axis

		// moveable cube
		cube1.Draw();
		cube1.ApplyGrayity(dt);
		cube1.ResolveCollision(cube2);
		DrawBoundingBox(cube1.box, collisionColor);
		cube2.Draw();
		cube2.ApplyGrayity(dt);
		cube2.ResolveCollision(cube1);
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
