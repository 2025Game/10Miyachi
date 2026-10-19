#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include "CTaskManager.h"
#include "CCollider.h"

//衝突処理範囲より離れているコライダは衝突処理しない
#define COLLISIONRANGE 30

class CCollisionManager : public CTaskManager
{
public:
	//衝突処理
	void Collision();

	//インスタンスの取得
	static CCollisionManager* Instance();
	void Collision(CCollider* c, int range);
private:
	//マネージャのインスタンス
	static CCollisionManager* mpInstance;
};

#endif

