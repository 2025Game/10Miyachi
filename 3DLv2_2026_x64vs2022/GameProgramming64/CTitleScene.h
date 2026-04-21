#pragma once
#ifndef CTITLESCENE_H
#define CTITLESCENE_H
#include "CSceneBase.h"
#include "CFont.h"
//タイトルシーン
class CTitleScene :public CSceneBase
{
public:
	//コンストラクタ
	CTitleScene();
	//デストラクタ
	~CTitleScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();
private:
	CFont mFont; //フォントクラスのインスタンス
};
#endif