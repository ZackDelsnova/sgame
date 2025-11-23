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
}

void World::Update(float dt) {
	// update dynamic bodies
	for (auto& dyn : dynamicObjects) {
		dyn->Update(dt);

		// resolve collision
		for (auto& st : staticObjects) {
			dyn->ResolveCollision(*st);
		}
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
}
