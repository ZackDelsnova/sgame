#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "CameraController.h"

class AllyUnit;

class World;

class Player {
public:
	int level = 1;
	float xp = 0.0f;
	float xpNeeded = 20.0f;

	bool upgradePending = false;
	bool spawnNewAlly = false;

	// upgrade menu
	struct UpgradeOption {
		enum Type {
			GET_NEW_ALLY,
			INCREASE_ATTACK,
			DECREASE_ATTACK_SPEED,
			INCREASE_RANGE,
			INCREASE_CRIT_CHANCE,
			INCREASE_CRIT_DAMAGE
		} type;

		AllyUnit* target = nullptr;
	};

	std::vector<UpgradeOption> offeredUpgrades;

	std::vector<AllyUnit*> alliesPointers;

	void SetAllyList(const std::vector<std::unique_ptr<AllyUnit>>& allies);

	void GainXpOnly(float amt);
	void AddXp(float amt);
	bool CheckLevelUp();
	void LevelUp();
	void GenerateUpgradeChoices(const std::vector<AllyUnit*>& allies);
	void ApplyUpgrade(const UpgradeOption& u, World* world, Camera3D& cam);

	std::string GetLevel() {
		std::stringstream ss;
		ss << xp << " / " << xpNeeded;
		return ss.str();
	}
};
