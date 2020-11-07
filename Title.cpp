#include <Siv3D.hpp>
#include "Title.h"

// ‰Šú‰»
Title::Title(const InitData& init) : IScene(init) {

}

// XV
void Title::update() {
	getData().PlayerNum = 10;
	changeScene(U"Game");
}

// •`‰æ
void Title::draw() const {

}
