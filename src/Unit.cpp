#include "Unit.h"

Unit::Unit(Vector3 pos, Vector3 s, Color col) : DynamicBody(pos, s, col) {
	useGravity = true;
}

void Unit::SetMoveTarget(Vector3 point) {
	targetPoint = point;
	state = UnitState::MovingToPoint;
	targetBody = nullptr;
}

void Unit::SetChaseTarget(Body* body) {
	targetBody = body;
	state = UnitState::ChasingTarget;
}

void Unit::MoveTowards(Vector3 point, float dt) {
	Vector3 dir = { point.x - position.x, -0.5f, point.z - position.z };

	float dist = Vector3Length(dir);

	if (dist < 0.1f) {
		state = UnitState::Idle;
		return;
	}

	dir = Vector3Normalize(dir);

	position.x += dir.x * moveSpeed * dt;
	position.z += dir.z * moveSpeed * dt;

	UpdateBoundingBox();
}

void Unit::Update(float dt) {
	// physics
	ApplyGravity(dt);

	UpdateAI(dt);

	switch (state)
	{
	case UnitState::Idle:
		break;
	case UnitState::MovingToPoint:
		MoveTowards(targetPoint, dt);
		break;
	case UnitState::ChasingTarget:
		if (targetBody) {
			MoveTowards(targetBody->position, dt);
		}
		break;
	default:
		break;
	}

	DynamicBody::Update(dt);
}

void Unit::UpdateAI(float dt) {
	dt = dt;
}

void Unit::Attack(Unit* other) {
	if (attackTimer > 0) return;
	if (!other->isAlive()) return;

	attackTimer = attackCooldown;
	other->TakeDamage(attack);
}

void Unit::TakeDamage(float amt) {
	hp -= amt;
	if (hp <= 0) {
		hp = 0;
		color = GRAY;
	}
}
