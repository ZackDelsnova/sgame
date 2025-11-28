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
	float maxHP = 100.0f;
	float hp = 100.0f;

	float moveSpeed = 5.0f;
	
	float attack = 10.0f;
	float attackRange = 3.0f;
	float attackCooldown = 0.6f;
	float attackTimer = 0.0f;

	float chaseRange = 100.0f;
	
	int team = 0; // temp team allocation 0 - ally, 1 - enemy

	// movement
	Vector3 targetPoint = { 0.0f,0.0f, 0.0f };
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
	bool TryAttack(Unit* other);
	void TakeDamage(float amt);

	float GetDistance(Vector3 pos);

	bool isAlive() const { return hp > 0;  }

};
