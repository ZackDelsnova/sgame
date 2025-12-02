#include "CameraController.h"

CameraController::CameraController() {
	yaw = 0.0f;
	pitch = 0.0f;
	sensitivity = 0.003f;

	baseSpeed = 7.0f;
	sprintMultiplier = 1.5f;
	accel = 20.0f;

	velocity = { 0 };

	defaultFOV = 70.0f;

	camera = { 0 };
	camera.position = { 0.0f, 2.0f, 6.0f };
	camera.target = { 0.0f, 1.5f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = defaultFOV;
	camera.projection = CAMERA_PERSPECTIVE;

	mouseCaptured = true;
}

void CameraController::Reset() {
	camera = { 0 };
	camera.position = { 0.0f, 2.0f, 6.0f };
	camera.target = { 0.0f, 1.5f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = defaultFOV;
}

void CameraController::LockMouse() {
	mouseCaptured = true;
	DisableCursor();
}

void CameraController::UnlockMouse() {
	mouseCaptured = false;
	EnableCursor();
}

void CameraController::Update(float dt) {
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

		// speed 
		float currentSpeed = baseSpeed;
		if (IsKeyDown(KEY_LEFT_SHIFT)) currentSpeed *= sprintMultiplier;

		Vector3 targetVel = { 0 };

		if (IsKeyDown(KEY_W)) targetVel = Vector3Add(targetVel, forward);
		if (IsKeyDown(KEY_S)) targetVel = Vector3Add(targetVel, Vector3Negate(forward));
		if (IsKeyDown(KEY_A)) targetVel = Vector3Add(targetVel, Vector3Negate(right));
		if (IsKeyDown(KEY_D)) targetVel = Vector3Add(targetVel, right);
		if (IsKeyDown(KEY_LEFT_CONTROL)) targetVel = Vector3Add(targetVel, Vector3Negate(up));
		if (IsKeyDown(KEY_SPACE)) targetVel = Vector3Add(targetVel, up);
		
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
		if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) camera.fovy = defaultFOV;
}

std::string CameraController::GetCompassDirection() const {
	float angle = yaw;

	// rad to deg and clamp to 0-360
	float degree = fmodf(angle * RAD2DEG + 360.0f, 360.0f);

	if (degree >= 337.5f || degree < 22.5f) return "N";
	else if (degree < 67.5f) return "NE";
	else if (degree < 112.5f) return "E";
	else if (degree < 157.5f) return "SE";
	else if (degree < 202.5f) return "S";
	else if (degree < 247.5f) return "SW";
	else if (degree < 292.5f) return "W";
	else return "NW";
}
