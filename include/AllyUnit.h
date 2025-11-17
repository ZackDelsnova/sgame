#pragma once
#include "BaseEntity.h"

class AllyUnit : public BaseEntity {
public:
	AllyUnit(Vector3 pos);
	
	void Update(float dt) override;
	void Draw() override;
};
