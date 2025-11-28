#include "AllyUnit.h"

AllyUnit::AllyUnit(Vector3 pos, Vector3 s, Color col)
	: Unit(pos, s, col) {}

void AllyUnit::SetFollowPoint(Vector3* p) {
	if (followPos == nullptr) {
		prevState = state;
		prevTargetPoint = targetPoint;
		prevTargetBody = targetBody;
	}
	followPos = p;
}

void AllyUnit::ClearFollowPoint() {
	if (followPos == nullptr) return;
	followPos = nullptr;

	if (prevState == UnitState::ChasingTarget && prevTargetBody) {
		targetBody = prevTargetBody;
		state = UnitState::ChasingTarget;
	}
	else if (prevState == UnitState::MovingToPoint) {
		targetPoint = prevTargetPoint;
		targetBody = nullptr;
		state = UnitState::MovingToPoint;
	}
	else {
		targetBody = nullptr;
		state = UnitState::Idle;
	}
}

void AllyUnit::UpdateAI(float dt) {
	dt = dt;
	if (followPos) {
		SetMoveTarget(*followPos);
	}
}
