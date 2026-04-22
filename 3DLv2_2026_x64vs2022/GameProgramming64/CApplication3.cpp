#include "CApplication3.h"
#include "CGameScene.h"

CApplication3::CApplication3()
{
}

CApplication3::~CApplication3()
{
}

void CApplication3::Start()
{
	//タイトルシーンのインスタンスを作成
	mpScene = std::make_unique<CGameScene>();
	mpScene->Load(); //タイトルシーンのロード
}

void CApplication3::Update()
{
	mpScene->Update();//タイトルシーンの更新
}