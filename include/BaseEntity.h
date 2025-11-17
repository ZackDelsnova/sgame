#pragma once
#include <raylib.h>
#include <raymath.h>

class BaseEntity {
public:
	Vector3 position;
	float health;
	float speed;
	bool isAlive;

	BaseEntity(Vector3 pos);
	virtual ~BaseEntity() {}

	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
};
