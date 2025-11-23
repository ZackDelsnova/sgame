#pragma once
#include <vector>
#include <memory>
#include "StaticBody.h"
#include "DynamicBody.h"

class World {
public:
	void Init();
	void Update(float dt);
	void Draw();

private:
	std::vector<std::unique_ptr<StaticBody>> staticObjects;
	std::vector<std::unique_ptr<DynamicBody>> dynamicObjects;
};