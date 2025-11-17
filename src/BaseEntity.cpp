#include "BaseEntity.h"

BaseEntity::BaseEntity(Vector3 pos) {
	position = pos;
	health = 100.0f;
	speed = 5.0f;
	isAlive = true;
}
