#pragma once
#include "Body.h"

class DynamicBody : public Body {
public:
	float vVelocity = 0.0f;
	bool useGravity = true;
	Color debugColor = YELLOW;

	DynamicBody(Vector3 pos, Vector3 s, Color col);

	void Update(float dt) override;
	void ApplyGravity(float dt);
	void ResolveCollision(const Body& other);

	void Draw() const override;
};
