#include "Player.h"
#include "AllyUnit.h"
#include "World.h"
#include <raylib.h>

void Player::GainXpOnly(float amt) {
	xp += amt;
}

bool Player::CheckLevelUp() {
	if (xp >= xpNeeded) {
		xp -= xpNeeded;
		level++;
		xpNeeded *= 1.30f;

		// Update ally pointers BEFORE generating upgrades
		GenerateUpgradeChoices(alliesPointers);
		upgradePending = true;
		return true;
	}
	return false;
}


void Player::AddXp(float amt) {
	xp += amt;
	if (xp >= xpNeeded) {
		LevelUp();
	}
}

void Player::SetAllyList(const std::vector<std::unique_ptr<AllyUnit>>& allies) {
	alliesPointers.clear();
	alliesPointers.reserve(allies.size());

	for (auto& a : allies) {
		alliesPointers.push_back(a.get());
	}
}

void Player::LevelUp() {
	xp -= xpNeeded;
	level++;
	xpNeeded *= 1.30f;
	GenerateUpgradeChoices(alliesPointers);
	upgradePending = true;
}

void Player::GenerateUpgradeChoices(const std::vector<AllyUnit*>& allies) {
	offeredUpgrades.clear();

	std::vector<UpgradeOption> pool;

	pool.push_back({ UpgradeOption::GET_NEW_ALLY, nullptr });

	for (AllyUnit* a : allies) {
		pool.push_back({ UpgradeOption::INCREASE_ATTACK, a });
		pool.push_back({ UpgradeOption::DECREASE_ATTACK_SPEED, a });
		pool.push_back({ UpgradeOption::INCREASE_RANGE, a });
		pool.push_back({ UpgradeOption::INCREASE_CRIT_CHANCE, a });
		pool.push_back({ UpgradeOption::INCREASE_CRIT_DAMAGE, a });
	}

	for (int i = 0; i < 3; i++) {
		int idx = GetRandomValue(0, static_cast<int>(pool.size()) - 1);
		offeredUpgrades.push_back(pool[idx]);
		pool.erase(pool.begin() + idx);
	}
}

void Player::ApplyUpgrade(const UpgradeOption& u, World* world, Camera3D& cam) {
	if (u.type == UpgradeOption::GET_NEW_ALLY) {
		world->SpawnAlly(cam);
	}
	else {
		AllyUnit* a = u.target;
		switch (u.type) {
		case UpgradeOption::INCREASE_ATTACK:
			a->attack += 1.0f;
			break;
		case UpgradeOption::DECREASE_ATTACK_SPEED:
			a->attackCooldown *= 0.85f;
			break;
		case UpgradeOption::INCREASE_RANGE:
			a->attackRange += 0.8f;
			break;
		case UpgradeOption::INCREASE_CRIT_CHANCE:
			a->critChance += 0.02f;
			break;
		case UpgradeOption::INCREASE_CRIT_DAMAGE:
			a->critMultiplier += 0.2f;
			break;
		}
	}
	upgradePending = false;
}

