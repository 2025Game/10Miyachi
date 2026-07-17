#pragma once

#ifndef CCOLLIDERCAPSULE_H
#define CCOLLIDERCAPSULE_H
#include "CCollider.h"

//カプセルコライダクラス
class CColliderCapsule : public CCollider
{
public:
	//コンストラクタ(カプセルコライダ)
	//CColliderCapsule(親, 親行列, 外頂点1, 外頂点2, 半径)
	CColliderCapsule(CCharacter3* parent, const CMatrix* matrix, const CVector& v0, const CVector& v1, float radius);
	//カプセルコライダの設定
	//Set(親, 親行列, 外頂点1, 外頂点2, 半径)
	void Set(CCharacter3* parent, const CMatrix* matrix, const CVector& v0, const CVector& v1, float radius);
	void Render(); //コライダの描画
	void Update(); //座標の更新
	void ChangePriority(); //優先順位の更新
private:
	CVector mSp; //始点内
	CVector mEp; //終点内
};
#endif