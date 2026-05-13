#pragma once

class CCollider;
class CXCharacter;

//状態の種類
enum class EState
{
	ENONE, //状態なし
	EIDLE, //待機
	EWALK, //歩き
};
class CState
{
public:
	virtual ~CState() {};
	//状態の開始
	virtual void Start(CXCharacter* parent) {};
	//状態の更新
	virtual void Update() {};
	//衝突処理
	//Collision(コライダ1, コライダ2)
	virtual void Collision(CCollider* m, CCollider* o) {};
	//状態の取得
	EState State() { return mState; }
protected:
	EState mState; //状態の種類
	CXCharacter* mpParent; //親のポインタ
};