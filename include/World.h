#pragma once
#include <vector>
#include <memory>
#include "StaticBody.h"
#include "DynamicBody.h"
#include "Unit.h"

class World {
public:
	void Init();
	void Update(float dt, Camera3D& cam);
	void Draw();

private:
	std::vector<std::unique_ptr<StaticBody>> staticObjects;
	std::vector<std::unique_ptr<DynamicBody>> dynamicObjects;
	std::vector<std::unique_ptr<Unit>> allUnits;
};