#include "CXPlayer.h"

#define GRAVITY 0.0625f // 重力

void CXPlayer::Update()
{
	//課題4.2 GRAVITYの大きさだけ、下方向へ移動させる
	Position(Position() - CVector(0.0f, GRAVITY, 0.0f));
	//親クラスの更新
	CXCharacter::Update();
}