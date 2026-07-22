#pragma once

#ifndef CPALADIN_H
#define CPALADIN_H
#include "CXCharacter.h"
#include "CColliderCapsule.h"

class CPaladin : public CXCharacter
{
public:
	//CPaladin(位置, 回転, 拡大縮小)
	CPaladin(const CVector& pos, const CVector& rot = CVector(), const CVector& scale = CVector(2.5f, 2.5f, 2.5f));
	void Update() override;
private:
	static CModelX msModel;
	CColliderCapsule mCollider; //カプセルコライダ
};
#endif