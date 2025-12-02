#pragma once
#include <vector>
#include <memory>
#include <string>
#include "StaticBody.h"
#include "DynamicBody.h"
#include "Unit.h"
#include "AllyUnit.h"
#include "EnemyUnit.h"
#include "Player.h"

#include <iostream>

class World {
public:
	Vector3 floorPos = { 0.0f, -0.5f,0.0f };

	bool followCamera = false;

	float spawnInterval = 10.0f;
	float spawnTimer = spawnInterval;
	float spawnRadius = 20.0f;
	int minSpawn = 2;
	int maxSpawn = 5;

	Player* player = nullptr;

	void Init();
	void Update(float dt, Camera3D& cam);
	void Draw();

	void SpawnAlly(Camera3D& cam);
	void SpawnEnemy(Camera3D& cam);
	void KillUnit(Unit* target);
	void RefreshAllUnits();
	bool IsSpaceFree(Vector3 pos, Vector3 size);

	std::string GetAllyUnitCount() {
		return std::to_string(allies.size());
	}

	bool GameOver() {
		return allies.size() <= 0;
	}

	std::string GetEnemyUnitCount() {
		return std::to_string(enemies.size());
	}

	std::string GetSpawnTimer() {
		return std::to_string(spawnTimer);
	}

	void Reset() {
		allies.clear();
		enemies.clear();
		dynamicObjects.clear();
		staticObjects.clear();
		allUnits.clear();

		followCamera = false;
		cameraFollowPoint = { 0,0,0 };

		Init();

		RefreshAllUnits();
	}

	void SetPlayer(Player* p) {
		player = p;
	}

private:
	std::vector<std::unique_ptr<StaticBody>> staticObjects;
	std::vector<std::unique_ptr<DynamicBody>> dynamicObjects;
	std::vector<Unit*> allUnits;
	std::vector<std::unique_ptr<AllyUnit>> allies;
	std::vector<std::unique_ptr<EnemyUnit>> enemies;

	Vector3 cameraFollowPoint = Vector3{ 0,0,0 };
};