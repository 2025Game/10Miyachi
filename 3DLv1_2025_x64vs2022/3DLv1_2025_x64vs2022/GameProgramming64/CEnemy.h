#pragma once
#include "CCharacter.h"

class CEnemy : public CCharacter
{
public:
	//衝突処理２
	void Collision();
	//衝突処理４
	//Collision(自分のポインタ, 衝突相手のポインタ)
	void Collision(CCharacter* m, CCharacter* o);

	//CEnemy(X座標,Y座標,幅,高さ,画像左,画像右,画像下,画像上,テクスチャのポインタ)
	CEnemy(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt);

	bool Collision(CRectangle* rect);
	void Update();
};