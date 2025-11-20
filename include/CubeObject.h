#pragma once
#include <raylib.h>
#include <raymath.h>

class CubeObject {
public:
	Vector3 position;
	Vector3 size;
	Color color;

	BoundingBox box;

	float vVelocity = 0.0f; // vertical velocity for gravity

	CubeObject(Vector3 pos, Color col, Vector3 s);
	
	void UpdateBoundingBox();
	void Move(Vector3 delta);
	void ApplyGravity(float dt);
	void Update(float dt);
	void Draw() const;
	void ResolveCollision(const CubeObject& other);
};
