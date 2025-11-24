#include "DynamicBody.h"

DynamicBody::DynamicBody(Vector3 pos, Vector3 s, Color col)
	: Body(pos, s, col) {
}

void DynamicBody::ApplyGravity(float dt) {
	if (!useGravity) return;

	const float gravity = -9.81f;
	vVelocity += gravity * dt;
	position.y += vVelocity * dt;
	UpdateBoundingBox();
}

void DynamicBody::Update(float dt) {
	ApplyGravity(dt);
}

void DynamicBody::Draw() const {
	Body::Draw();
}

void DynamicBody::ResolveCollision(const Body& other) {
	if (!CheckCollisionBoxes(box, other.box)) {
		return;
	}

	// calculate overlap on each axis
	float overlapX1 = other.box.max.x - box.min.x;
	float overlapX2 = box.max.x - other.box.min.x;
	float resolveX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;

	float overlapY1 = other.box.max.y - box.min.y;
	float overlapY2 = box.max.y - other.box.min.y;
	float resolveY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;

	float overlapZ1 = other.box.max.z - box.min.z;
	float overlapZ2 = box.max.z - other.box.min.z;
	float resolveZ = (overlapZ1 < overlapZ2) ? overlapZ1 : -overlapZ2;

	// choose smallest axis to push out (classic AABB push-back)
	float absX = fabsf(resolveX);
	float absY = fabsf(resolveY);
	float absZ = fabsf(resolveZ);

	bool isGround_ = other.isGround;

	// for small y axis overlap, prevent stacking
	if (!isGround_ && absY < 0.1f) {
		absY = 99999.0f;
	}

	if (absX < absY && absX < absZ) {
		position.x += resolveX;
	}
	else if (absZ < absX && absZ < absY) {
		position.z += resolveZ;
	}
	else {
		position.y += resolveY;

		if (resolveY > 0) {
			vVelocity = 0.0f; // landed on ground
		}
	}

	UpdateBoundingBox();
}
