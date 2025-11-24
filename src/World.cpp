#include "World.h"

void World::Init() {
	
	staticObjects.push_back(
		std::make_unique<StaticBody>(
			Vector3{ 0, -0.5f, 0 },
			Vector3{ 2000, 1, 2000},
			DARKGREEN
		) // floor
	);
	staticObjects.back()->isGround = true;

	dynamicObjects.push_back(
		std::make_unique<DynamicBody>(
			Vector3{ 0, 10, 0},
			Vector3{ 1, 1, 1},
			ORANGE
		) // test object
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
		for (auto& other : allUnits) {
			if (other.get() == u.get()) continue;
			u->ResolveCollision(*other);
		}
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

void World::SpawnAlly(Camera3D& cam) {
	Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));

	Vector3 spawnPos = Vector3Add(cam.position, Vector3Scale(forward, 5.0f));

	if (IsSpaceFree(spawnPos, { 1,1,1 })) {
		allUnits.push_back(std::make_unique<Unit>(spawnPos, Vector3{ 1,1,1 }, BLUE));
	}
	else {
		for (int i = 0; i < 10; i++) {
			spawnPos.z += 0.5f;
			if (IsSpaceFree(spawnPos, { 1,1,1 })) {
				allUnits.push_back(std::make_unique<Unit>(spawnPos, Vector3{ 1,1,1 }, BLUE));
				break;
			}
		}
	}
}

void World::SpawnEnemy(Camera3D& cam) {
	Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));

	Vector3 spawnPos = Vector3Add(cam.position, Vector3Scale(forward, 5.0f));

	if (IsSpaceFree(spawnPos, { 1,1,1 })) {
		allUnits.push_back(std::make_unique<Unit>(spawnPos, Vector3{ 1,1,1 }, RED));
	}
	else {
		for (int i = 0; i < 10; i++) {
			spawnPos.z += 0.5f;
			if (IsSpaceFree(spawnPos, { 1,1,1 })) {
				allUnits.push_back(std::make_unique<Unit>(spawnPos, Vector3{ 1,1,1 }, RED));
				break;
			}
		}
	}
}

void World::KillUnitInFront(Camera3D& cam) {
	Ray ray = { cam.position, Vector3Normalize(Vector3Subtract(cam.target, cam.position)) };

	int indexToDelete = -1;
	float closest = 99999.0f;

	for (int i = 0; i < allUnits.size(); i++) {
		RayCollision hit = GetRayCollisionBox(ray, allUnits[i]->box);
		if (hit.hit && hit.distance < closest) {
			closest = hit.distance;
			indexToDelete = i;
		}
	}

	if (indexToDelete != -1) {
		allUnits.erase(allUnits.begin() + indexToDelete);
	}
}

bool World::IsSpaceFree(Vector3 pos, Vector3 size) {
	BoundingBox box = {
	{ pos.x - size.x / 2, pos.y - size.y / 2, pos.z - size.z / 2 },
	{ pos.x + size.x / 2, pos.y + size.y / 2, pos.z + size.z / 2 }
	};

	for (auto& u : allUnits) {
		if (CheckCollisionBoxes(box, u->box))
			return false;
	}

	return true;
}

std::string World::GetUnitCount() {
	return std::to_string(allUnits.size());
}
