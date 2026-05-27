#include "CPlayerWalk.h"
#include "CXPlayer.h"

//移動速度
#define VELOCITY 0.1f
#define ROTATIONSPEED 2.0f

void CPlayerWalk::Start(CXCharacter* parent)
{
	//親のポインタを保存
	mpParent = parent;
	//アニメーションの変更
	mpParent->ChangeAnimation(1, true, 60);
	mState = EState::EWALK; //状態の種類を歩くにする
}

void CPlayerWalk::Update()
{
	if (mInput.Key('W'))
	{
		CVector p = mpParent->Position();
		mpParent->Position(p + mpParent->MatrixRotate().VectorZ() * VELOCITY);
	}
	else
	{
		//Wキーが押されていないときは待機状態にする
		mState = EState::EIDLE;
	}
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
	if (mInput.Key('I'))
	{
		mState = EState::EATTACK;
	}
	if (mInput.Key(VK_SPACE))
	{
		mState = EState::EJUMP;
	}
}