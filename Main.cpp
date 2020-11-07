#include<Siv3D.hpp>
#include"Main.h"
#include"Title.h"
#include"Game.h"

void Main(void) {
	Window::Resize(GameInfo::Width, GameInfo::Height);
	Scene::Resize(GameInfo::Width, GameInfo::Height);
	Window::SetTitle(GameInfo::Title);
	Scene::SetBackground(GameInfo::BackgroundColor);

	MyApp manager;
	manager.setFadeColor(GameInfo::FadeInColor);
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");

	// メインループで更新と描画をする
	while (System::Update())
	{
		if (!manager.update()) break;
	}
}