#include "CubeObject.h"

CubeObject::CubeObject(Vector3 pos, Color col, Vector3 s) {
	position = pos;
	color = col;
	size = s;
	UpdateBoundingBox();
}

void CubeObject::UpdateBoundingBox() {
	box.max = { position.x + size.x / 2, position.y + size.y / 2, position.z + size.z / 2 };
	box.min = { position.x - size.x / 2, position.y - size.y / 2, position.z - size.z / 2 };
}

void CubeObject::Move(Vector3 delta) {
	position = Vector3Add(position, delta);
	UpdateBoundingBox();
}

void CubeObject::ApplyGravity(float dt) {
	if (!useGravity) return;

	const float gravity = -9.81f;
	
	vVelocity += gravity * dt;
	position.y += vVelocity * dt;

	if (position.y < size.y / 2) {
		position.y = size.y / 2;
		vVelocity = 0.0f;
	}

	UpdateBoundingBox();
}

void CubeObject::Update(float dt) {
	ApplyGravity(dt);
	Draw();
}

void CubeObject::Draw() const {
	DrawCubeV(position, size, color);
	DrawBoundingBox(box, debugColor);
}

void CubeObject::ResolveCollision(const CubeObject& other) {
	if (!CheckCollisionBoxes(box, other.box)) {
		debugColor = YELLOW;
		return;
	}

	debugColor = RED;
	// Calculate overlap on each axis
	float overlapX1 = other.box.max.x - box.min.x;
	float overlapX2 = box.max.x - other.box.min.x;
	float resolveX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;

	float overlapY1 = other.box.max.y - box.min.y;
	float overlapY2 = box.max.y - other.box.min.y;
	float resolveY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;

	float overlapZ1 = other.box.max.z - box.min.z;
	float overlapZ2 = box.max.z - other.box.min.z;
	float resolveZ = (overlapZ1 < overlapZ2) ? overlapZ1 : -overlapZ2;

	// Choose the smallest axis to push out (classic AABB push-back)
	float absX = fabsf(resolveX);
	float absY = fabsf(resolveY);
	float absZ = fabsf(resolveZ);

	if (absX < absY && absX < absZ) {
		// resolve on X
		position.x += resolveX;
	}
	else if (absY < absX && absY < absZ) {
		// resolve on Y
		position.y += resolveY;

		// if resolved vertically upwards, stop gravity
		if (resolveY > 0.0f)
			vVelocity = 0.0f;
	}
	else {
		// resolve on Z
		position.z += resolveZ;
	}

	UpdateBoundingBox();
}
