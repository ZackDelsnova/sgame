#include "World.h"

void World::Init() {
	
	// ground
	auto floor = std::make_unique<StaticBody>(
		floorPos, Vector3{ 2000, 1, 2000 }, DARKGREEN
	);
	floor->isGround = true;

	staticObjects.push_back(std::move(floor));

	// test dynamic object
	dynamicObjects.push_back(std::make_unique<DynamicBody>(
		Vector3{ 0, 10, 0 }, Vector3{ 1, 1, 1 }, ORANGE));

	auto u = std::make_unique<AllyUnit>(Vector3{2, 0, 2}, Vector3{1,1,1}, BLUE);
	u->team = 0;
	allUnits.push_back(u.get());
	allies.push_back(std::move(u));

	spawnRadius = 20.0f;
	minSpawn = 2;
	maxSpawn = 5;
	spawnInterval = 10.0f;
	spawnTimer = spawnInterval;
}

void World::Update(float dt, Camera3D& cam) {

	if (spawnTimer <= 0.0f) {
		SpawnEnemy(cam);
		spawnTimer = spawnInterval;
	}
	else {
		spawnTimer -= dt;
	}

	if (IsKeyPressed(KEY_TAB)) {
		followCamera = !followCamera;

		if (!followCamera) {
			for (auto& a : allies) {
				a->ClearFollowPoint();	
			}
		}
	}

	// allies update
	for (auto& a : allies) {
		if (followCamera) {
			// forward point from camera
			Vector3 forward = Vector3Normalize(Vector3Subtract(cam.target, cam.position));
			cameraFollowPoint = Vector3Add(cam.position, Vector3Scale(forward, 5.0f));
			cameraFollowPoint.y = -0.5f;
			a->SetFollowPoint(&cameraFollowPoint);
		}
		a->Update(dt);
		if (a->position.y <= floorPos.y) KillUnit(a.get());

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
		if (e->position.y <= floorPos.y) KillUnit(e.get());

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

	// ally attack enemy
	for (auto& a : allies) {
		if (!a->isAlive()) continue;

		for (auto& e : enemies) {
			if (!e->isAlive()) continue;

			if (!a->TryAttack(e.get())) {
				if (!a->targetBody && a->GetDistance(e.get()->position) < a->chaseRange)
					a->SetChaseTarget(e.get());
			}
		}
	}

	// enemy attack ally
	for (auto& e : enemies) {
		if (!e->isAlive()) continue;

		for (auto& a : allies) {
			if (!a->isAlive()) continue;

			if (!e->TryAttack(a.get())) {
				if (!e->targetBody && e->GetDistance(a.get()->position) < e->chaseRange)  
					e->SetChaseTarget(a.get());
			}
		}
	}

	// remove if dead
	allies.erase(
		std::remove_if(allies.begin(), allies.end(),
			[](const auto& u) { return !u->isAlive(); }),
		allies.end()
	);

	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(),
			[](const auto& u) { return !u->isAlive(); }),
		enemies.end()
	);

	RefreshAllUnits();

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
	u->team = 0;
	allUnits.push_back(u.get());
	allies.push_back(std::move(u));
}

void World::SpawnEnemy(Camera3D& cam) {

	int count = minSpawn + (rand() % (maxSpawn - minSpawn + 1));

	for (int i = 0; i < count; i++)
	{
		float angle = static_cast<float>(rand()) / RAND_MAX * 6.283185f; // 0 - 2pi
		float dist = static_cast<float>(rand()) / RAND_MAX * spawnRadius;

		Vector3 spawnPos = cam.position;
		spawnPos.x += static_cast<float>(cos(angle)) * dist;
		spawnPos.z += static_cast<float>(sin(angle)) * dist;

		if (!IsSpaceFree(spawnPos, { 1,1,1 })) {
			for (int j = 0; j < 10; j++) {
				spawnPos.z += 0.5f;
				if (IsSpaceFree(spawnPos, { 1,1,1 })) break;
			}
		}

		auto u = std::make_unique<EnemyUnit>(spawnPos, Vector3{ 1,1,1 }, RED);
		u->chaseRange = 2000.0f;
		u->team = 1;
		allUnits.push_back(u.get());
		enemies.push_back(std::move(u));
	}
}

void World::KillUnitInFront(Camera3D& cam) {
	Ray ray = { cam.position, Vector3Normalize(Vector3Subtract(cam.target, cam.position)) };

	Unit* target = nullptr;
	float closest = 1e9f;

	for (Unit* u : allUnits) {
		RayCollision hit = GetRayCollisionBox(ray, u->box);
		if (hit.hit && hit.distance < closest) {
			closest = hit.distance;
			target = u;
		}
	}

	KillUnit(target);
}

void World::KillUnit(Unit* target) {
	if (!target) return;

	// remove from allies (owner)
	auto aIt = std::find_if(allies.begin(), allies.end(),
		[&](const std::unique_ptr<AllyUnit>& p) { return p.get() == target; });

	if (aIt != allies.end()) {
		allies.erase(aIt);
		RefreshAllUnits();
		return;
	}

	// remove from enemies (owner)
	auto eIt = std::find_if(enemies.begin(), enemies.end(),
		[&](const std::unique_ptr<EnemyUnit>& p) { return p.get() == target; });

	if (eIt != enemies.end()) {
		enemies.erase(eIt);
		RefreshAllUnits();
		return;
	}
}

void World::RefreshAllUnits()
{
	allUnits.clear();
	allUnits.reserve(allies.size() + enemies.size());
	for (auto& a : allies) allUnits.push_back(a.get());
	for (auto& e : enemies) allUnits.push_back(e.get());

	for (Unit* u : allUnits) {
		if (!u->isAlive()) {
			for (Unit* other : allUnits) {
				if (other->targetBody == u) {
					other->targetBody = nullptr;
				}
			}
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

