#include "CPlayerAttack.h"
#include "CXPlayer.h"

void CPlayerAttack::Start(CXCharacter* parent)
{
	mpParent = parent;
	mpParent->ChangeAnimation(3, false, 30);
	mState = EState::EATTACK;
}

void CPlayerAttack::Update()
{
	//アニメーションが終了しているか
	if (mpParent->IsAnimationFinished())
	{
		//アニメーションが終了したら待機状態にする
		mState = EState::EIDLE;
	}
}