#pragma once
#ifndef CSCENEBASE_H
#define CSCENEBASE_H
#include "EScene.h"
class CTask;
//シーンのベースクラス
class CSceneBase
{
public:
	//コンストラクタ
	CSceneBase(EScene scene);
	//デストラクタ
	virtual ~CSceneBase() {};
	//シーン読み込み処理(継承先で実装)
	virtual void Load() = 0;
	//シーン更新処理(継承先で実装)
	virtual void Update() = 0;
	//シーンの種類を取得
	EScene GetSceneType() const;
private:
	EScene mSceneType;//シーンの種類
};
#endif