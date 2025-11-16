#include "raylib.h"
#include "raymath.h"

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Survior Game");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	// camera
	float yaw = 0.0f;
	float pitch = 0.0f;
	float sensitivity = 0.003f;
	float defaultFOV = 70.0f;

	Camera3D camera = { 0 };
	camera.position = { 0.0f, 2.0f, 6.0f };
	camera.target = { 0.0f, 1.5f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = defaultFOV;
	camera.projection = CAMERA_PERSPECTIVE;

	bool mouseCaptured = true;
	DisableCursor();

	// movement
	Vector3 velocity = { 0 };
	float accel = 20.0f;
	// float friction = 8.0f;
	float baseSpeed = 7.0f;
	float sprintMultiplier = 1.5f;

	// moveable object
	Vector3 objectPos = { 0, 1, 0 };
	
	// bool noClip = true;

	while (!WindowShouldClose()) {

		float dt = GetFrameTime();

		if (IsKeyPressed(KEY_ESCAPE)) {
			mouseCaptured = !mouseCaptured;
			if (mouseCaptured) DisableCursor();
			else EnableCursor();
		}

		// camera rotation
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

		Vector3 right = { 
			sinf(yaw - PI / 2.0f),
			0,
			cosf(yaw - PI / 2.0f)
		};
		Vector3 up = { 0, 1 , 0 };

		float currentSpeed = baseSpeed;
		if (IsKeyDown(KEY_LEFT_SHIFT)) currentSpeed *= sprintMultiplier;

		Vector3 targetVel = { 0 };

		if (IsKeyDown(KEY_W)) targetVel = Vector3Add(targetVel, forward);
		if (IsKeyDown(KEY_S)) targetVel = Vector3Add(targetVel, Vector3Negate(forward));
		if (IsKeyDown(KEY_A)) targetVel = Vector3Add(targetVel, Vector3Negate(right));
		if (IsKeyDown(KEY_D)) targetVel = Vector3Add(targetVel, right);
		if (IsKeyDown(KEY_Q)) targetVel = Vector3Add(targetVel, Vector3Negate(up));
		if (IsKeyDown(KEY_E)) targetVel = Vector3Add(targetVel, up);

		if (Vector3Length(targetVel) > 0.01f) {
			targetVel = Vector3Normalize(targetVel);
		}

		targetVel = Vector3Scale(targetVel, currentSpeed);
		
		// smooth acceleration
		velocity = Vector3Lerp(velocity, targetVel, accel * dt);

		// apply movement
		camera.position = Vector3Add(camera.position, Vector3Scale(velocity, dt));
		camera.target = Vector3Add(camera.position, forward);

		// fov change
		camera.fovy -= GetMouseWheelMove() * 3.0f;
		camera.fovy = Clamp(camera.fovy, 30.0f, 120.0f);
		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_MIDDLE)) camera.fovy = defaultFOV;

		// move objects with arrow key
		if (IsKeyDown(KEY_UP)) objectPos.z -= 5 * dt;
		if (IsKeyDown(KEY_DOWN)) objectPos.z += 5 * dt;
		if (IsKeyDown(KEY_LEFT)) objectPos.x -= 5 * dt;
		if (IsKeyDown(KEY_RIGHT)) objectPos.x += 5 * dt;

		BeginDrawing();
		ClearBackground(SKYBLUE);
		
		BeginMode3D(camera);

		// infinite grid
		DrawGrid(200, 1.0f);

		// simple sky (fake sky)
		DrawSphere({ 0, 1000, 0 }, 980.0f, Fade(SKYBLUE, 0.4f)); // horizon glow

		// debug gizmo
		DrawLine3D({ 0, 0, 0 }, { 2, 0, 0 }, RED);   // X axis)
		DrawLine3D({ 0, 0, 0 }, { 0, 2, 0 }, GREEN); // Y axis
		DrawLine3D({ 0, 0, 0 }, { 0, 0, 2 }, BLUE);  // Z axis

		// moveable cube
		DrawCube(objectPos, 1, 1, 1, ORANGE);
		DrawCubeWires(objectPos, 1, 1, 1, BROWN);

		EndMode3D();

		DrawText(mouseCaptured ? "Mouse: LOCKED (ESC to unlock)" : "Mouse: FREE (ESC to lock)", 10, 40, 20, DARKGRAY);

		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
