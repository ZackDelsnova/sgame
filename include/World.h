#pragma once
#include "StaticBody.h"
#include <vector>

class World {
public:
	std::vector<StaticBody> worldObjects;

	void Init();
	void UpdateAndDraw(float dt);
};