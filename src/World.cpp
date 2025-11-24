#include "World.h"

void World::Init() {
	
	staticObjects.push_back(
		std::make_unique<StaticBody>(
			Vector3{ 0, -0.5f, 0 },
			Vector3{ 2000, 1, 2000},
			DARKGREEN
		) // floor
	);

	dynamicObjects.push_back(
		std::make_unique<DynamicBody>(
			Vector3{ 0, 10, 0},
			Vector3{ 1, 1, 1},
			ORANGE
		) // test object
	);

	allUnits.push_back(
		std::make_unique<Unit>(
			Vector3{ 0, 10, -5 },
			Vector3{ 1, 1, 1 },
			PINK
		) // test unit
	);

}

void World::Update(float dt, Camera3D& cam) {

	// update ai
	for (auto& u : allUnits) {
		if (!u) continue;

		Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
		Vector3 followPoint = Vector3Add(cam.position, Vector3Scale(forward, 5.0f));
		followPoint.y = -0.5f;
		u->SetMoveTarget(followPoint);

		u->Update(dt);
	}

	// update dynamic body
	for (auto& dyn : dynamicObjects) {
		dyn->Update(dt);
	}

	// static collosion
	for (auto& st : staticObjects) {
		for (auto& dyn : dynamicObjects)
			dyn->ResolveCollision(*st);

		for (auto& u : allUnits)
			u->ResolveCollision(*st);
	}

	// dynamic collision
	for (auto& dyn : dynamicObjects) {
		for (auto& u : allUnits)
			u->ResolveCollision(*dyn);
	}
}

void World::Draw() {

	// static
	for (auto& st : staticObjects) {
		st->Draw();
	}

	for (auto& dyn : dynamicObjects) {
		dyn->Draw();
	}

	for (auto& u : allUnits) {
		u->Draw();
	}
}
