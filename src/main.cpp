#include "raylib.h"
#include "raymath.h"

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Basic Raylib Window");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	// camera
	Camera3D camera;
	camera.position = { 0, 5, 10 };
	camera.target = { 0, 0, 0 };
	camera.up = { 0, 1, 0 };
	camera.fovy = 70.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	float speed = 5.0f;
	float sensitivity = 0.003f;

	bool mouseCaptured = true;
	DisableCursor();

	float yaw = 0.0f;
	float pitch = 0.0f;

	while (!WindowShouldClose()) {

		float dt = GetFrameTime();

		if (IsKeyPressed(KEY_ESCAPE)) {
			mouseCaptured = !mouseCaptured;
			if (mouseCaptured) DisableCursor();
			else EnableCursor();
		}

		if (mouseCaptured) {
			Vector2 mouseDelta = GetMouseDelta();
			yaw -= mouseDelta.x * sensitivity;
			pitch -= mouseDelta.y * sensitivity;

			pitch = Clamp(pitch, -1.5f, 1.5f); // limit vertical look
		}

		Vector3 forward = {
			cosf(pitch) * sinf(yaw),
			sinf(pitch),
			cosf(pitch) * cosf(yaw)
		};

		Vector3 right = Vector3CrossProduct(forward, { 0, 1, 0 });
		right = Vector3Normalize(right);

		if (IsKeyDown(KEY_W)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, speed * dt));
		if (IsKeyDown(KEY_S)) camera.position = Vector3Add(camera.position, Vector3Scale(forward, -speed * dt));
		if (IsKeyDown(KEY_A)) camera.position = Vector3Add(camera.position, Vector3Scale(right, -speed * dt));
		if (IsKeyDown(KEY_D)) camera.position = Vector3Add(camera.position, Vector3Scale(right, speed * dt));

		if (IsKeyDown(KEY_Q)) camera.position.y -= speed * dt;
		if (IsKeyDown(KEY_E)) camera.position.y += speed * dt;

		camera.target = Vector3Add(camera.position, forward);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		BeginMode3D(camera);

		DrawPlane({ 0, 0, 0 }, { 20, 20 }, LIGHTGRAY); // ground

		DrawCube({ 0, 1, 0 }, 1, 1, 1, RED); // cube 
		DrawCubeWires({ 0, 1, 0 }, 1, 1, 1, MAROON);

		EndMode3D();

		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
