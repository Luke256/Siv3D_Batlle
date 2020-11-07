// include
#pragma once
#include "Main.h"

class Title : public MyApp::Scene
{
private:


public:

	Title(const InitData& init);
	void update() override;
	void draw() const override;
};
