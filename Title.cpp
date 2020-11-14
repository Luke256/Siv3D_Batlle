#include <Siv3D.hpp>
#include "Title.h"

// 初期化
Title::Title(const InitData& init) : IScene(init) {

}

// 更新
void Title::update() {
	getData().PlayerNum = 20;
	changeScene(U"Game");
}

// 描画
void Title::draw() const {

}
