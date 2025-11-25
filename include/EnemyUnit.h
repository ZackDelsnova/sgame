#pragma once
#include "Unit.h"

class EnemyUnit : public Unit {
public:
	EnemyUnit(Vector3 pos, Vector3 s, Color col)
		: Unit(pos, s, col) {}

	Unit* target = nullptr;

protected:
	void UpdateAI(float dt) override {
		dt = dt;
		if (target) {
			SetChaseTarget(target);
		}
	}
};
