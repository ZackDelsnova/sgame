#pragma once
#include "Body.h"

class StaticBody : public Body {
public:
	StaticBody(Vector3 pos, Vector3 s, Color col);

	void Update(float dt) override;
};
