#include "CPlayerIdle.h"
#include "CXCharacter.h"

//回転速度
#define ROTATIONSPEED 2.0f

void CPlayerIdle::Start(CXCharacter* parent)
{
	//親のポインタを保存
	mpParent = parent;
	//アニメーションの変更
	mpParent->ChangeAnimation(0, true, 60);
	mState = EState::EIDLE; //状態の種類を待機にする
}

void CPlayerIdle::Update()
{
	//Aキーで左回転、Dキーで右回転
	if (mInput.Key('D'))
	{
		CVector r = mpParent->Rotation() + CVector(0.0f, -ROTATIONSPEED, 0.0f);
		mpParent->Rotation(r);
	}
	if (mInput.Key('A'))
	{
		CVector r = mpParent->Rotation() + CVector(0.0f, +ROTATIONSPEED, 0.0f);
		mpParent->Rotation(r);
	}
	if (mInput.Key('W'))
	{
		mState = EState::EWALK;
	}
}