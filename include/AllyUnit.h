#pragma once
#include "Unit.h"

class AllyUnit : public Unit {
public:
	AllyUnit(Vector3 pos, Vector3 s, Color col)
		: Unit(pos, s, col) {}

	Vector3* followPos = nullptr;

	void SetFollowPoint(Vector3* p) { followPos = p; }

protected:
	void UpdateAI(float dt) override {
		dt = dt;
		if (followPos) {
			SetMoveTarget(*followPos);
		}
	}
};
