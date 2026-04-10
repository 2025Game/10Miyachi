#pragma once
#include "CFont.h"
/*
* CUi
* ユーザーインタフェースクラス
*/
class CUi
{
public:
	void PosY(float f);	//高度の設定
	void RotX(float f);	//ピッチの設定
	void RotY(float f);	//ヨ―の設定

	//ゲームクリア表示
	void Clear();

	//ゲームオーバー表示
	void Over();

	//スタート画面
	void Start();

	//Enemy(敵の数)
	void Enemy(int enemy);

	void Hp(int hp);
	CUi();
	//Time(時間)
	void Time(int time);
	void Render();
private:
	float mPosY;	//高度　Y座標
	float mRotX;	//ピッチ　X回転値
	float mRotY;	//ヨー　Y回転値

	int mEnemy;	//敵の数
	int mHp;
	int mTime;	//経過時間
	CFont mFont;
};
