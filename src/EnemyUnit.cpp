#include "EnemyUnit.h"

EnemyUnit::EnemyUnit(Vector3 pos, Vector3 s, Color col)
	: Unit(pos, s, col) {
	xpWorth = (float)GetRandomValue(3, 8);
}

void EnemyUnit::UpdateAI(float dt) {
	dt = dt;
	if (target) {
		SetChaseTarget(target);
	}
}
