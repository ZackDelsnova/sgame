#include "Body.h"

Body::Body(Vector3 pos, Vector3 s, Color col) {
	position = pos;
	size = s;
	color = col;
	baseColor = col;
	renderColor = col;
	UpdateBoundingBox();
}

void Body::UpdateBoundingBox() {
	box.min = {
		position.x - size.x / 2.0f,
		position.y - size.y / 2.0f,
		position.z - size.z / 2.0f
	};
	box.max = {
		position.x + size.x / 2.0f,
		position.y + size.y / 2.0f,
		position.z + size.z / 2.0f
	};
}

void Body::Update(float dt) {
	dt = dt;
	// Base class does nothing
}

void Body::Draw() const {
	DrawCubeV(position, size, renderColor);
	DrawBoundingBox(box, borderColor);
}
