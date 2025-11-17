#include "EnemyUnit.h"

EnemyUnit::EnemyUnit(Vector3 pos) : BaseEntity(pos) {}

void EnemyUnit::Update(float dt) {
	// For now, the ally unit will just stand still
	dt = dt; // suppress unused variable warning
}

void EnemyUnit::Draw() {
	// Draw the ally unit as a blue cube
	DrawCube(position, 1.0f, 1.0f, 1.0f, RED);
	DrawCubeWires(position, 1.0f, 1.0f, 1.0f, MAROON);
}
