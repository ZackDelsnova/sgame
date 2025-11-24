#pragma once
#include <vector>
#include <memory>
#include <string>
#include "StaticBody.h"
#include "DynamicBody.h"
#include "Unit.h"

class World {
public:
	void Init();
	void Update(float dt, Camera3D& cam);
	void Draw();

	void SpawnAlly(Camera3D& cam);
	void SpawnEnemy(Camera3D& cam);
	void KillUnitInFront(Camera3D& cam);

	bool IsSpaceFree(Vector3 pos, Vector3 size);

	std::string GetUnitCount();
private:
	std::vector<std::unique_ptr<StaticBody>> staticObjects;
	std::vector<std::unique_ptr<DynamicBody>> dynamicObjects;
	std::vector<std::unique_ptr<Unit>> allUnits;
};