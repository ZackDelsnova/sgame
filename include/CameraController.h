#pragma once
#include <raylib.h>
#include <raymath.h>
#include <string>

class CameraController {
public:
	Camera3D camera;

	float yaw;
	float pitch;
	float sensitivity;

	float baseSpeed;
	float sprintMultiplier;
	float accel;

	Vector3 velocity;

	bool mouseCaptured;

	float defaultFOV;

	CameraController();

	void Update(float dt);
	void LockMouse();
	void UnlockMouse();
	std::string GetCompassDirection() const;
};