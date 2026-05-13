#pragma once
#ifndef CXPLAYER_H
#define CXPLAYER_H
#include "CXCharacter.h"
#include "CColliderLine.h"
#include "CPlayerIdle.h"
class CXPlayer : public CXCharacter
{
public:
	//衝突処理
    //Collision(コライダ1, コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//衝突処理
	void Collision();
	CXPlayer();
	void Update() override;
private:
	EState mState; //状態の保持
	CState* mpState; //状態処理
	std::unique_ptr<CPlayerIdle> mpIdle; //待機状態
	CColliderLine mColliderLine;
};
#endif