#pragma once
#ifndef CGAMESCENE_H
#define CGAMESCENE_H
#include "CSceneBase.h"
#include "CModel.h"
//ゲームシーン
class CGameScene :public CSceneBase
{
public:
	CGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();
private:
	CModel mBackGround; //背景モデル
};
#endif