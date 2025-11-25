#pragma once
#include "DynamicBody.h"
#include <vector>
#include <memory>
#include <raymath.h>

enum class UnitState {
	Idle,
	MovingToPoint,
	ChasingTarget
};

class Unit : public DynamicBody {
public:
	// stats
	float hp = 100.0f;
	float moveSpeed = 5.0f;
	float attack = 10.0f;
	float attackRange = 2.0f;
	float attackCooldown = 1.0f;
	float attackTimer = 0.0f;
	int team = 0; // temp team allocation 0 - ally, 1 - enemy

	// movement
	Vector3 targetPoint = Vector3{ 0,0,0 };
	Body* targetBody = nullptr;
	UnitState state = UnitState::Idle;

	Unit(Vector3 pos, Vector3 s, Color col);

	// stuff needed in future for child classes
	void SetMoveTarget(Vector3 point);
	void SetChaseTarget(Body* body);

	void Update(float dt) override;
	void MoveTowards(Vector3 point, float dt);

	virtual void UpdateAI(float dt);

	void Attack(Unit* other);

	void TakeDamage(float amt);

	bool isAlive() const { return hp > 0;  }

};
