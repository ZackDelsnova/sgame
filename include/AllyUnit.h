#pragma once
#include "Unit.h"

class AllyUnit : public Unit {
public:
	AllyUnit(Vector3 pos, Vector3 s, Color col);

	Vector3* followPos = nullptr;

	UnitState prevState = UnitState::Idle;
	Vector3 prevTargetPoint = { 0,0,0 };
	Unit* prevTargetBody = nullptr;

	void SetFollowPoint(Vector3* p);
	void ClearFollowPoint();

protected:
	void UpdateAI(float dt) override;
};
