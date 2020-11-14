#include <Siv3D.hpp>
#include "Game.h"
#include "GameClass.h"

// 初期化
Game::Game(const InitData& init) : IScene(init) {
	Scene::SetBackground(Palette::Gray);

	Player tmp(U"M1", Vec2(50, 50), U"Human", U"Luke");
	Players << tmp;
	for (auto i : step(getData().PlayerNum-1)) {
		tmp = Player(U"R", Vec2(Random() * (FieldSize.x - 100), Random() * (FieldSize.y - 100)), U"CPU", U"CPU" + ToString(i + 1));
		Players << tmp;
	}

	camera = Camera2D(Players[0].Pos, 0.5);
	Frame = 0;
	LogFont = Font(20);
	Alive = true;
}

// 更新
void Game::update() {

	ClearPrint();

	Print << Players.size();
	Print << Players[0].WeaponNow;
	Print << Players[0].HP;
	Print << Frame;

	camera.update();
	camera.setScale(.5);
	camera.setCenter(Players[0].Pos);
	for (int32 i = 0; i < Players.size(); ++i) {
		if (Players[i].Type == U"Human") {
			const auto t = camera.createTransformer();
			double Arc = Players[i].Arc = atan2(Cursor::Pos().y - Players[i].Pos.y, Cursor::Pos().x - Players[i].Pos.x);
			Print << Players[i].Cool;
			if (KeyW.pressed()) {
				Players[i].Pos.y += Players[i].MoveSpeed * sin(Arc);
				Players[i].Pos.x += Players[i].MoveSpeed * cos(Arc);
			}
			if (KeyS.pressed()) {
				Players[i].Pos.y -= Players[i].MoveSpeed * sin(Arc);
				Players[i].Pos.x -= Players[i].MoveSpeed * cos(Arc);
			}
			if (KeyA.pressed()) {
				Players[i].Pos.y -= Players[i].MoveSpeed * cos(Arc);
				Players[i].Pos.x += Players[i].MoveSpeed * sin(Arc);
			}
			if (KeyD.pressed()) {
				Players[i].Pos.y += Players[i].MoveSpeed * cos(Arc);
				Players[i].Pos.x -= Players[i].MoveSpeed * sin(Arc);
			}

			if (MouseL.pressed() && Players[i].Cool <= 0 && Players[i].WeaponNow == U"Main") {
				double a;
				if (Players[i].WeaponNow == U"Main") {
					if (Players[i].MainWeapon[0] == 'M') {
						Players[i].Err = Min(20 * (Players[i].MainWeapon[1] - '0'), Players[i].Err + (Players[i].MainWeapon[1] - '0'));
						if (Players[i].Cool == -1) Players[i].Err = 0;
						a = Arc + ToRadians(Random() * Players[i].Err * 2 - Players[i].Err);
					}
					elif(Players[i].MainWeapon[0] == 'S') {
						a = Arc + ToRadians(Random() * Players[i].Err / 2 - Players[i].Err / 4);
					}
				}

				Players[i].Cool = Players[i].CoolMax;


				Bullet tmp(Players[i], a);
				//自滅回避
				tmp.Pos.x += (PlayerSize * 2 + 10) * cos(a);
				tmp.Pos.y += (PlayerSize * 2 + 10) * sin(a);

				Bullets << tmp;
				if (Players[i].MainWeapon[0] == 'S') {
					Players[i].Err = 50 + (Players[i].MainWeapon[1] - '0' - 1) * 50;
				}
			}

			if (MouseL.pressed() && Players[i].WeaponNow == U"Sub") {
				Bullet tmp(Players[i], Arc);
				//自滅回避
				tmp.Pos.x += (PlayerSize * 2 + 10) * cos(Arc);
				tmp.Pos.y += (PlayerSize * 2 + 10) * sin(Arc);

				Bullets << tmp;
			}
			Players[i].Cool = Max(-1, Players[i].Cool - 1);

			if (KeyShift.pressed() && (Players[i].MainWeapon[0] == 'S' && Players[i].WeaponNow==U"Main")) {//スコープ
				camera.setScale(.4);
				camera.setCenter(Players[i].Pos + Vec2(1000 * cos(Arc), 1000 * sin(Arc)));
				Players[i].Err = Max(0, Players[i].Err - 1);
			}
			if (KeyShift.up() && Players[i].MainWeapon[0] == 'S') {
				Players[i].Err = 50 + (Players[i].MainWeapon[1] - '0' - 1) * 50;
			}

			if (KeyZ.down()) {
				Players[i].SwitchWeapon();
			}
		}

		else { //NPCの動作
			act(i);
		}


		for (int32 j = 0; j < Bullets.size(); ++j) {
			if (Circle(Players[i].Pos, PlayerSize).intersects(Line(Bullets[j].Pos, Bullets[j].Pos - Vec2(Bullets[j].Speed * cos(Bullets[j].Arc), Bullets[j].Speed * sin(Bullets[j].Arc))))) {
				Players[i].HP -= Bullets[j].Damage;
				if (Players[i].HP <= 0) {
					Logs << KillLog(Bullets[j].Master + U" killed " + Players[i].Name);
				}
				Bullets.remove_at(j);
				--j;
			}
		}
		if (Players[i].HP <= 0) {
			if (i == 0) {
				Alive = false;
			}
			Players.remove_at(i);
			--i;
			continue;
		}

		Players[i].Pos.x = Min(FieldSize.x - PlayerSize, Players[i].Pos.x);
		Players[i].Pos.x = Max(PlayerSize, Players[i].Pos.x);
		Players[i].Pos.y = Min(FieldSize.x - PlayerSize, Players[i].Pos.y);
		Players[i].Pos.y = Max(PlayerSize, Players[i].Pos.y);
	}

	for (int32 i = 0; i < Bullets.size(); ++i) {
		if (Bullets[i].Life <= 0) {
			Bullets.remove_at(i);
			--i;
			continue;
		}
		--Bullets[i].Life;
		Bullets[i].Pos.x += Bullets[i].Speed * cos(Bullets[i].Arc);
		Bullets[i].Pos.y += Bullets[i].Speed * sin(Bullets[i].Arc);
	}

	for (int32 i = 0; i < Logs.size(); ++i) {
		Logs[i].life--;
		if (Logs[i].life <= 0) {
			Logs.remove_at(i);
			--i;
			continue;
		}
	}

	while (Logs.size() > 10) {
		Logs.remove_at(0);
	}

	++Frame;
}

// 描画
void Game::draw() const {
	{
		ScopedRenderStates2D sampler(SamplerState::ClampNearest);
		const auto t = camera.createTransformer();

		Rect(0, 0, FieldSize.x, FieldSize.y).draw(Palette::Burlywood);

		double Arc = atan2(Cursor::Pos().y - Players[0].Pos.y, Cursor::Pos().x - Players[0].Pos.x);
		Bullet tmp(Players[0], Arc);
		if (Alive) {
			Line(Players[0].Pos, Players[0].Pos + Vec2(tmp.Life * tmp.Speed * cos(Arc), tmp.Life * tmp.Speed * sin(Arc))).draw(LineStyle::RoundDot, 12, Palette::Orange);

			if (Players[0].MainWeapon[0] == 'M' && (MouseL.pressed() || Players[0].Cool != -1) && Players[0].WeaponNow == U"Main") {
				Circle(Players[0].Pos, tmp.Life * tmp.Speed).drawPie(Arc + 90_deg - ToRadians(Players[0].Err), ToRadians(Players[0].Err * 2), ColorF(255, 0, 0, 0.3));
			}
			elif(KeyShift.pressed() && Players[0].MainWeapon[0] == 'S' && Players[0].WeaponNow == U"Main") {
				Circle(Players[0].Pos, tmp.Life * tmp.Speed).drawPie(Arc + 90_deg - ToRadians(Players[0].Err / 4), ToRadians(Players[0].Err / 2), ColorF(255, 0, 0, 0.3));
				Circle(Players[0].Pos, 2500).drawPie(Arc + 120_deg, 300_deg, ColorF(0, 0, 0, 0.95));
			}
		}

		for (auto i : Players) {
			Circle(i.Pos, PlayerSize).draw(Palette::Brown);
			double Arc = i.Arc;
			Circle(i.Pos, PlayerSize).drawArc(Arc + 45_deg, 90_deg, 10, 2, Palette::Aqua);
		}
		for (auto i : Bullets) {
			Line(i.Pos, i.Pos - Vec2(i.Speed * 2 * cos(i.Arc), i.Speed * 2 * sin(i.Arc))).draw(10, ColorF(1, 0, 0, 0.5));
			Circle(i.Pos, 5).draw(Palette::Black);
		}


	}

	for (int32 i = 0; i < Logs.size(); ++i) {
		LogFont(Logs[i].str).draw(5, 5 + i * LogFont.fontSize());
	}

}


void Game::act(int32 i) {
	Print << Players[i].Err;
	if (Players[i].Type == U"Human")return;
	elif(Players[i].Type == U"CPU") {
		Circle Vision(Players[i].Pos, 800);
		bool Found = false;
		Vec2 To;

		Array<int32>array;
		for (int32 j : step(Players.size())) {
			array << j;
		}

		Shuffle(array.begin(), array.end());
		for (auto j : array) {
			if (Players[j].Pos == Players[i].Pos)continue;
			if (Vision.intersects(Circle(Players[j].Pos, PlayerSize))) {

				Players[i].Arc = atan2(Players[j].Pos.y - Players[i].Pos.y, Players[j].Pos.x - Players[i].Pos.x);
				Found = true;
				To = Players[j].Pos;
				break;
			}
		}

		Bullet tmp(Players[i], Players[i].Arc);
		if (Frame % 50 == 0) {
			Players[i].Arc = ToRadians(Random() * 90 - 45 + Players[i].Arc);
		}
		elif(Found) {
			if (Players[i].Cool <= 0 && ((Players[i].WeaponNow == U"Main" && Players[i].MainWeapon[0] == 'M' && (Players[i].Err < 5 || Players[i].Cool == -1)) || Players[i].MainWeapon[0] == 'S')) {
				Players[i].Cool = Players[i].CoolMax;
				double a = Players[i].Arc;
				if (Players[i].WeaponNow == U"Main") {
					if (Players[i].MainWeapon[0] == 'M') {
						Players[i].Err = Min(20 * (Players[i].MainWeapon[1] - '0'), Players[i].Err + (Players[i].MainWeapon[1] - '0'));
						if (Players[i].Cool == -1) Players[i].Err = 0;
						a = Players[i].Arc + ToRadians(Random() * Players[i].Err * 2 - Players[i].Err);
					}
					elif(Players[i].MainWeapon[0] == 'S') {
						a = Players[i].Arc + ToRadians(Random() * Players[i].Err / 2 - Players[i].Err / 4);
					}
				}
				tmp = Bullet(Players[i], a);
				tmp.Pos.x += (PlayerSize * 2 + 10) * cos(Players[i].Arc);
				tmp.Pos.y += (PlayerSize * 2 + 10) * sin(Players[i].Arc);
				Bullets << tmp;
			}
		}

		if (!Found || (Found && To.distanceFrom(Players[i].Pos) > tmp.Life * tmp.Speed)) {
			Players[i].Pos.y += Players[i].MoveSpeed * sin(Players[i].Arc);
			Players[i].Pos.x += Players[i].MoveSpeed * cos(Players[i].Arc);
		}
		Players[i].Cool = Max(-1, Players[i].Cool - 1);

	}
}

/*
*/