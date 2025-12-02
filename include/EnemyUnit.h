#pragma once
#include "Unit.h"

class EnemyUnit : public Unit {
public:
	EnemyUnit(Vector3 pos, Vector3 s, Color col);

	Unit* target = nullptr;

	float xpWorth = 5.0f;

	float GetXpWorth() const override { return xpWorth; }

protected:
	void UpdateAI(float dt) override;
};
