#include "World.h"

void World::Init() {
	StaticBody floorBody({ 0.0f, -0.5f, 0.0f }, { 2000, 1, 2000 }, DARKGREEN);
	worldObjects.push_back(floorBody);
}

void World::UpdateAndDraw(float dt) {
	dt = dt;
	// simple sky (fake sky)
	DrawSphere({ 0, -1000, 0 }, 980.0f, Fade(SKYBLUE, 0.4f)); // horizon glow

	// debug xyz axis
	DrawLine3D({ 0, 0, 0 }, { 2, 0, 0 }, RED);   // X axis)
	DrawLine3D({ 0, 0, 0 }, { 0, 2, 0 }, GREEN); // Y axis
	DrawLine3D({ 0, 0, 0 }, { 0, 0, 2 }, BLUE);  // Z axis

	// draw all static bodies/non movable stuff
	for (auto& obj : worldObjects) {
		obj.Draw();
	}
}