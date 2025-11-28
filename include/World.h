#pragma once
#include <vector>
#include <memory>
#include <string>
#include "StaticBody.h"
#include "DynamicBody.h"
#include "Unit.h"
#include "AllyUnit.h"
#include "EnemyUnit.h"

class World {
public:
	Vector3 floorPos = { 0.0f, -0.5f,0.0f };

	bool followCamera = false;

	void Init();
	void Update(float dt, Camera3D& cam);
	void Draw();

	void SpawnAlly(Camera3D& cam);
	void SpawnEnemy(Camera3D& cam);
	void KillUnitInFront(Camera3D& cam);
	void KillUnit(Unit* target);
	void RefreshAllUnits();
	bool IsSpaceFree(Vector3 pos, Vector3 size);

	std::string GetAllyUnitCount() {
		return std::to_string(allies.size());
	}

	std::string GetEnemyUnitCount() {
		return std::to_string(enemies.size());
	}

private:
	std::vector<std::unique_ptr<StaticBody>> staticObjects;
	std::vector<std::unique_ptr<DynamicBody>> dynamicObjects;
	std::vector<Unit*> allUnits;
	std::vector<std::unique_ptr<AllyUnit>> allies;
	std::vector<std::unique_ptr<EnemyUnit>> enemies;

	Vector3 cameraFollowPoint = Vector3{ 0,0,0 };
};