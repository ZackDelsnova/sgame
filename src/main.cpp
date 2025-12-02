#include "CameraController.h"
#include "World.h"

#include <raylib.h>
#include <raymath.h>
#include <vector>

typedef enum {
	STATE_MENU,
	STATE_GAME,
	STATE_PAUSE,
	STATE_UPGRADE_MENU,
	STATE_GAMEOVER
} GameState;

bool Button(Rectangle bounds, const char* text) {
	Vector2 mouse = GetMousePosition();

	bool hovered = CheckCollisionPointRec(mouse, bounds);
	bool clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

	// background color
	Color bg = hovered ? DARKGRAY : GRAY;

	// draw rect
	DrawRectangleRec(bounds, bg);

	// outline
	DrawRectangleLines(
		static_cast<int>(bounds.x), 
		static_cast<int>(bounds.y), 
		static_cast<int>(bounds.width), 
		static_cast<int>(bounds.height),
		BLACK
	);

	// center text
	int fontSize = 20;
	int textWidth = MeasureText(text, fontSize);
	DrawText(
		text,
		static_cast<int>(bounds.x + (bounds.width - textWidth) / 2),
		static_cast<int>(bounds.y + (bounds.height - fontSize) / 2),
		fontSize,
		WHITE
	);

	return clicked;
}

int main() {
	const int screenWidth = 1280;
	const int screenHeight = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Survior Game");
	SetExitKey(KEY_NULL);
	SetTargetFPS(120);

	GameState currentState = STATE_MENU;

	CameraController cameraCtrl;
	Player player;

	World world;
	world.Init();
	world.SetPlayer(&player);

	while (!WindowShouldClose()) {

		// input for ui
		switch (currentState) {
		case STATE_GAME:
			if (IsKeyPressed(KEY_ESCAPE)) {
				cameraCtrl.UnlockMouse();
				currentState = STATE_PAUSE;
			}
			break;

		case STATE_PAUSE:
			if (IsKeyPressed(KEY_ESCAPE)) {
				cameraCtrl.LockMouse();
				currentState = STATE_GAME;
			}
			break;

		default:
			break;
		}

		float dt = GetFrameTime();

		BeginDrawing();

		switch (currentState) {
		case STATE_MENU: {
			ClearBackground(BLACK);
			DrawText("Survival Game", 330, 80, 40, RAYWHITE);

			if (Button(Rectangle{ 300, 250, 200, 50 }, "Start")) {
				cameraCtrl.LockMouse();
				currentState = STATE_GAME;
			}

			if (Button(Rectangle{ 300, 330, 200, 50 }, "Exit")) {
				EndDrawing();
				CloseWindow();
				return 0;
			}
		} break;

		case STATE_GAME: {
			int cx = GetScreenWidth() / 2;
			int cy = GetScreenHeight() / 2;

			ClearBackground(SKYBLUE);

			if (player.upgradePending) {
				cameraCtrl.UnlockMouse();
				currentState = STATE_UPGRADE_MENU;
				break;
			}

			cameraCtrl.Update(dt);

			BeginMode3D(cameraCtrl.camera);

			world.Update(dt, cameraCtrl.camera);
			world.Draw();

			EndMode3D();

			// cross air
			DrawLine(cx - 10, cy, cx + 10, cy, BLACK);
			DrawLine(cx, cy - 10, cx, cy + 10, BLACK);

			DrawFPS(10, 10);
			DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 40, 20, DARKGRAY);
			DrawText(world.GetAllyUnitCount().c_str(), 10, 70, 20, BLUE);
			DrawText(world.GetEnemyUnitCount().c_str(), 10, 90, 20, RED);
			DrawText(world.GetSpawnTimer().c_str(), 10, 110, 20, BLACK);
			DrawText(player.GetLevel().c_str(), 10, 130, 20, GREEN);

			if (world.GameOver()) {
				cameraCtrl.UnlockMouse();
				currentState = STATE_GAMEOVER;
			}
		} break;

		case STATE_PAUSE: {
			DrawText("Paused", 350, 100, 30, RAYWHITE);

			if (Button(Rectangle{ 300, 250, 200, 50 }, "Resume")) {
				cameraCtrl.LockMouse();
				currentState = STATE_GAME;
			}

			if (Button(Rectangle{ 300, 330, 200, 50 }, "Bact to Menu")) {
				currentState = STATE_MENU;
			}
		} break;

		case STATE_GAMEOVER: {
			DrawText("Paused", 350, 100, 30, RED);

			if (Button(Rectangle{ 300, 250, 200, 50 }, "Try Again")) {
				world.Reset();
				cameraCtrl.Reset();
				cameraCtrl.LockMouse();
				currentState = STATE_GAME;
			}

			if (Button(Rectangle{ 600, 250, 200, 50 }, "Quit")) {
				currentState = STATE_MENU;
			}
			
		} break;
		case STATE_UPGRADE_MENU: {
			ClearBackground(SKYBLUE);

			cameraCtrl.Update(0); // no movement
			BeginMode3D(cameraCtrl.camera);
			world.Draw();         // draw static world (no Update)
			EndMode3D();

			// Draw crosshair and HUD dimmed
			DrawText(cameraCtrl.GetCompassDirection().c_str(), 10, 40, 20, DARKGRAY);
			DrawText(world.GetAllyUnitCount().c_str(), 10, 70, 20, BLUE);
			DrawText(world.GetEnemyUnitCount().c_str(), 10, 90, 20, RED);
			DrawText(world.GetSpawnTimer().c_str(), 10, 110, 20, BLACK);
			DrawText(player.GetLevel().c_str(), 10, 130, 20, GREEN);

			// Dark overlay
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

			// Upgrade cards
			int y = 200;

			for (int i = 0; i < player.offeredUpgrades.size(); i++) {
				auto& opt = player.offeredUpgrades[i];

				Rectangle r{ 200, (float)y, 400, 60 };
				std::string label;

				if (opt.type == Player::UpgradeOption::GET_NEW_ALLY)
					label = "Summon New Ally";
				else {
					switch (opt.type) {
					case Player::UpgradeOption::INCREASE_ATTACK: label = "Increase Attack"; break;
					case Player::UpgradeOption::DECREASE_ATTACK_SPEED: label = "Faster Attack Speed"; break;
					case Player::UpgradeOption::INCREASE_RANGE: label = "Increase Range"; break;
					case Player::UpgradeOption::INCREASE_CRIT_CHANCE: label = "Increase Crit Chance"; break;
					case Player::UpgradeOption::INCREASE_CRIT_DAMAGE: label = "Increase Crit Damage"; break;
					}
					label += " (Unit)";
				}

				if (Button(r, label.c_str())) {
					player.ApplyUpgrade(opt, &world, cameraCtrl.camera);

					cameraCtrl.LockMouse();
					currentState = STATE_GAME;
				}

				y += 90;
			}
		} break;
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
