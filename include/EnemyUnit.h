#pragma once
#include "BaseEntity.h"

class EnemyUnit : public BaseEntity {
public:
	EnemyUnit(Vector3 pos);
	
	void Update(float dt) override;
	void Draw() override;
};
