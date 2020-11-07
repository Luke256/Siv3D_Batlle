#pragma once
#include <Siv3D.hpp>
#define elif else if

struct Player {
	Player(String MainWeapon_, Vec2 Pos_, String Type_, String Name_) {
		if (MainWeapon_ != U"M1" && MainWeapon_ != U"M2" && MainWeapon_ != U"S1" && MainWeapon_ != U"S2") {
			if (MainWeapon_ == U"R") {
				Array<String>a = { U"M1",U"M2",U"S1",U"S2" };
				MainWeapon = a[int32(Random() * 4)];
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
			CoolMax = 8;
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

	String MainWeapon; //メイン武器(マシンガン1,2(M1,M2):スナイパー1,2(S1,S2))
	/*
	M1:精度高、ダメージ低
	M2:精度低、ダメージ高
	S1:連射高、ダメージ低
	S2:連射低、ダメージ高
	*/

	Vec2 Pos;
	int32 HP;
	String Type; //誰が操作するのか
	String WeaponNow;
	int32 Cool; //現在のクール時間
	int32 CoolMax; //発射直後のクール時間
	int32 Err; //手振れ
	String Name; //プレイヤー名
	int32 MoveSpeed;
	double Arc;
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
			Damage = 10;
			Life = 30;
		}
		elif(Weapon == U"Main" && Player_.MainWeapon == U"S1") {
			Damage = 25;
			Life = 50;
		}
		elif(Weapon == U"Main" && Player_.MainWeapon == U"S2") {
			Damage = 35;
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
	int32 Damage;//当たった時に与えられるダメージ
	int32 Life; //飛び終わるまでのフレーム数
	int32 Speed; //飛ぶ速さ
	String Master; //弾を撃ったプレイヤー名
};