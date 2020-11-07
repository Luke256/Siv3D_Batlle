// include
#pragma once
#include "Main.h"
#include "GameClass.h"

class Game : public MyApp::Scene
{
private:
	const Vec2 FieldSize = Vec2(5000, 5000);
	const double PlayerSize = 50.0;

	Array<Player>Players;
	Array<Bullet>Bullets;
	Array<String>Logs;
	Camera2D camera;
	int32 Frame;
	

public:

	Game(const InitData& init);
	void update() override;
	void draw() const override;
	void act(int32 index, int32 action);
};
