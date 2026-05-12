#pragma once
#ifndef CXPLAYER_H
#define CXPLAYER_H
#include "CXCharacter.h"
#include "CColliderLine.h"
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
	CColliderLine mColliderLine;
};
#endif