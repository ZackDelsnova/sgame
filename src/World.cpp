#include "World.h"

void World::Init() {
	
	// ground
	auto floor = std::make_unique<StaticBody>(
		Vector3{ 0, -0.5f, 0 }, Vector3{ 2000, 1, 2000 }, DARKGREEN
	);
	floor->isGround = true;

	staticObjects.push_back(std::move(floor));

	// test dynamic object
	dynamicObjects.push_back(std::make_unique<DynamicBody>(
		Vector3{ 0, 10, 0 }, Vector3{ 1, 1, 1 }, ORANGE));

}

void World::Update(float dt, Camera3D& cam) {

	// forward point from camera
	Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
	cameraFollowPoint = Vector3Add(cam.position, Vector3Scale(forward, 5.0f));
	cameraFollowPoint.y = -0.5f;

	// allies update
	for (auto& a : allies) {
		a->SetFollowPoint(&cameraFollowPoint);
		a->Update(dt);

		// collide with world
		for (auto& st : staticObjects) {
			a->ResolveCollision(*st);
		}
		for (auto& dyn : dynamicObjects) {
			a->ResolveCollision(*dyn);
		}	
	}

	// enemies update
	for (auto& e : enemies) {
		// assign target aferwards
		e->Update(dt);

		// collide with world
		for (auto& st : staticObjects) {
			e->ResolveCollision(*st);
		}
		for (auto& dyn : dynamicObjects) {
			e->ResolveCollision(*dyn);
		}
	}

	// update dynamic bodies
	for (auto& d : dynamicObjects) {
		d->Update(dt);

		for (auto& st : staticObjects) {
			d->ResolveCollision(*st);
		}
	}

	// ally vs ally
	for (size_t i = 0; i < allies.size(); i++) {
		for (size_t j = i + 1; j < allies.size(); j++) {
			allies[i]->ResolveCollision(*allies[j]);
		}
	}

	// enemy vs enemy
	for (size_t i = 0; i < enemies.size(); i++) {
		for (size_t j = i + 1; j < enemies.size(); j++) {
			enemies[i]->ResolveCollision(*enemies[j]);
		}
	}

	// ally vs enemy
	for (auto& a : allies) {
		for (auto& e : enemies) {
			a->ResolveCollision(*e);
			e->ResolveCollision(*a); // optional, but safer
		}
	}
}

void World::Draw() {

	// static
	for (auto& st : staticObjects) {
		st->Draw();
	}

	// dynamic
	for (auto& dyn : dynamicObjects) {
		dyn->Draw();
	}

	// allies
	for (auto& a : allies) {
		a->Draw();
	}

	// enemies
	for (auto& e : enemies) {
		e->Draw();
	}
}

void World::SpawnAlly(Camera3D& cam) {
	Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));

	Vector3 spawnPos = Vector3Add(cam.position, Vector3Scale(forward, 5.0f));

	if (!IsSpaceFree(spawnPos, { 1,1,1 })) {
		for (int i = 0; i < 10; i++) {
			spawnPos.z += 0.5f;
			if (IsSpaceFree(spawnPos, { 1,1,1 })) break;
		}
	}

	auto u = std::make_unique<AllyUnit>(spawnPos, Vector3{ 1,1,1 }, BLUE);
	allUnits.push_back(u.get());
	allies.push_back(std::move(u));
}

void World::SpawnEnemy(Camera3D& cam) {
	Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));

	Vector3 spawnPos = Vector3Add(cam.position, Vector3Scale(forward, 5.0f));

	if (!IsSpaceFree(spawnPos, { 1,1,1 })) {
		for (int i = 0; i < 10; i++) {
			spawnPos.z += 0.5f;
			if (IsSpaceFree(spawnPos, { 1,1,1 })) break;
		}
	}

	auto u = std::make_unique<EnemyUnit>(spawnPos, Vector3{ 1,1,1 }, RED);
	allUnits.push_back(u.get());
	enemies.push_back(std::move(u));
}

void World::KillUnitInFront(Camera3D& cam) {
	Ray ray = { cam.position, Vector3Normalize(Vector3Subtract(cam.target, cam.position)) };

	Unit* toDelete = nullptr;
	float closest = 99999.0f;

	for (Unit* u : allUnits) {
		RayCollision hit = GetRayCollisionBox(ray, u->box);
		if (hit.hit && hit.distance < closest) {
			closest = hit.distance;
			toDelete = u;
		}
	}

	if (!toDelete) return;

	// remove from allUnits
	allUnits.erase(
		std::remove(allUnits.begin(), allUnits.end(), toDelete),
		allUnits.end()
	);

	// remove from allies (owner)
	for (size_t i = 0; i < allies.size(); i++) {
		if (allies[i].get() == toDelete) {
			allies.erase(allies.begin() + i);
			return;
		}
	}

	// remove from enemies (owner)
	for (size_t i = 0; i < enemies.size(); i++) {
		if (enemies[i].get() == toDelete) {
			enemies.erase(enemies.begin() + i);
			return;
		}
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
