#pragma once
#include<Siv3D.hpp>

struct GameData {
	int32 PlayerNum=2;
};

using MyApp = SceneManager<String, GameData>;

namespace GameInfo
{
	const int32 Width = 800;
	const int32 Height = 800;
	const String Title = U"Bomber_Blocking";
	const ColorF BackgroundColor = Palette::Darkblue;
	const ColorF FadeInColor = Palette::Darkblue;
}