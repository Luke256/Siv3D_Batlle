#include <Siv3D.hpp>
#include "Title.h"

// ������
Title::Title(const InitData& init) : IScene(init) {

}

// �X�V
void Title::update() {
	getData().PlayerNum = 50;
	changeScene(U"Game");
}

// �`��
void Title::draw() const {

}
