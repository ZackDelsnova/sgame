#include "Unit.h"

Unit::Unit(Vector3 pos, Vector3 s, Color col) : DynamicBody(pos, s, col) {
	useGravity = true;
}

void Unit::SetMoveTarget(Vector3 point) {
	targetPoint = point;
	state = UnitState::MovingToPoint;
	targetBody = nullptr;
}

void Unit::SetChaseTarget(Unit* target) {
	targetBody = target;
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

	if (attackTimer > 0.0f) {
		attackTimer -= dt;
	}

	float pct = hp / maxHP;
	pct = Clamp(pct, 0.0f, 1.0f);

	renderColor = {
		(unsigned char)(baseColor.r * (0.4f + 0.6f * pct)),
		(unsigned char)(baseColor.g * (0.4f + 0.6f * pct)),
		(unsigned char)(baseColor.b * (0.4f + 0.6f * pct)),
		255
	};

	UpdateAI(dt);

	switch (state)
	{
	case UnitState::Idle:
		break;
	case UnitState::MovingToPoint:
		MoveTowards(targetPoint, dt);
		break;
	case UnitState::ChasingTarget:
		if (targetBody && targetBody->isAlive()) {
			MoveTowards(targetBody->position, dt);
		}
		else {
			targetBody = nullptr;
			state = UnitState::Idle;
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

bool Unit::TryAttack(Unit* other) {
	
	float distSq = GetDistance(other->position);

	if (distSq + 5.0f < (attackRange * attackRange)) {
		targetBody = nullptr;
		Attack(other);
		return true;
	}
	return false;
}

float Unit::GetDistance(Vector3 pos) {
	float dx = position.x - pos.x;
	float dy = position.y - pos.y;
	float dz = position.z - pos.z;

	return dx * dx + dy * dy + dz * dz;
}
