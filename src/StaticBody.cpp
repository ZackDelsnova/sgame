#include "StaticBody.h"

// StaticBody does not need any additional implementation
StaticBody::StaticBody(Vector3 pos, Vector3 s, Color col)
	: Body(pos, s, col) {
}

void StaticBody::Update(float dt) {
	dt = dt;
	// StaticBody does not move
}