#pragma once
#include <Siv3D.hpp>
#define elif else if

struct Player {
	Player(String MainWeapon_, Vec2 Pos_, String Type_, String Name_) {
		if (MainWeapon_ != U"M1" && MainWeapon_ != U"M2" && MainWeapon_ != U"S1" && MainWeapon_ != U"S2") {
			if (MainWeapon_ == U"R") {
				Array<String>a = { U"M1",U"M2" };
				MainWeapon = a[int32(Random() * a.size())];
			}
			else {
				throw Error(U"Visable Weapon Type '" + MainWeapon_ + U"'");
			}
		}
		else {
			MainWeapon = MainWeapon_;
		}

		Cool = 0;
		if (MainWeapon == U"M1") {
			CoolMax = 10;
			Err = 0;
			MoveSpeed = 8;
		}
		elif(MainWeapon == U"M2") {
			CoolMax = 5;
			Err = 0;
			MoveSpeed = 8;
		}
		elif(MainWeapon == U"S1") {
			CoolMax = 50;
			Err = 100;
			MoveSpeed = 5;
		}
		elif(MainWeapon == U"S2") {
			CoolMax = 100;
			Err = 100;
			MoveSpeed = 5;
		}

		Pos = Pos_;
		HP = 100;
		Type = Type_;
		WeaponNow = U"Main";
		Name = Name_;
		Arc = 0_deg;
	}
	void SwitchWeapon() {
		if (WeaponNow == U"Main") {
			WeaponNow = U"Sub";
		}
		else {
			WeaponNow = U"Main";
		}
	}

	void SetBody(P2World &world) {
		Body = world.createDynamicCircle(Pos, 50);
		Body.setPos(Pos);
		//throw Error(Format(Body.getPos()));
	}

	String MainWeapon; //���C������(�}�V���K��1,2(M1,M2):�X�i�C�p�[1,2(S1,S2))
	/*
	M1:���x���A�_���[�W��
	M2:���x��A�_���[�W��
	S1:�A�ˍ��A�_���[�W��
	S2:�A�˒�A�_���[�W��
	*/

	Vec2 Pos;
	int32 HP;
	String Type; //�N�����삷��̂�
	String WeaponNow;
	int32 Cool; //���݂̃N�[������
	int32 CoolMax; //���˒���̃N�[������
	int32 Err; //��U��
	String Name; //�v���C���[��
	int32 MoveSpeed;
	double Arc;
	double SwitchArc; //CPU�p
	P2Body Body;
};


struct Bullet {
	Bullet(Player Player_,double Arc_) {
		Pos = Player_.Pos;
		Arc = Arc_;
		String Weapon = Player_.WeaponNow;
		if (Weapon == U"Main" && Player_.MainWeapon == U"M1") {
			Damage = 10;
			Life = 30;
		}
		elif(Weapon == U"Main" && Player_.MainWeapon == U"M2") {
			Damage = 8;
			Life = 30;
		}
		elif(Weapon == U"Main" && Player_.MainWeapon == U"S1") {
			Damage = 35;
			Life = 50;
		}
		elif(Weapon == U"Main" && Player_.MainWeapon == U"S2") {
			Damage = 50;
			Life = 50;
		}
		else {
			Damage = 15;
			Life = 15;
		}
		Speed = 50;
		Master = Player_.Name;
	}
	Vec2 Pos;
	double Arc;
	int32 Damage;//�����������ɗ^������_���[�W
	int32 Life; //��яI���܂ł̃t���[����
	int32 Speed; //��ԑ���
	String Master; //�e���������v���C���[��
};

struct KillLog {
	KillLog(String str_) {
		str = str_;
		life = 500;
	}

	String str;
	int32 life;
};

struct Object {

	Object(Vec2 Pos, Vec2 Scale) {
		Body = Rect(Pos.x, Pos.y, Scale.x, Scale.y);
	}

	Rect Body;
};