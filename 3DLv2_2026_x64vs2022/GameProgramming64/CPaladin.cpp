#include "CPaladin.h"

#define PALADIN_MODEL "res\\paladin\\Paladin WProp J Nordstrom@Idle.fbx.x"

CModelX CPaladin::msModel;

CPaladin::CPaladin(const CVector& pos, const CVector& rot, const CVector& scale)
	: mCollider(this, &mCombinedMatrix, CVector(0.0f, 4.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), 0.5f)
{
	//static変数は初期値の状態で1つだけ作成され削除されない。
	//1つ作成されたらその後初期値の代入はされない。
	static bool first = true;
	if (first)
	{
		msModel.Load(PALADIN_MODEL);
		first = false;
	}
	Init(&msModel);
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
}

void CPaladin::Update()
{
	CXCharacter::Update();
	mCollider.Update();
}