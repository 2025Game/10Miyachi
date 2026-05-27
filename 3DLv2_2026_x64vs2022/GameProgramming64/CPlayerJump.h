#pragma once

#include "CState.h"
#include "CInput.h"
#include "CXCharacter.h"

class CPlayerJump : public CState
{
public:
	//衝突処理
    //Collision(コライダ1, コライダ2)
	void Collision(CCollider* m, CCollider* o) override;
	void Start(CXCharacter* parent) override;
	void Update() override;
private:
	CVector mJumpV; //ジャンプの速度
	CInput mInput;
};