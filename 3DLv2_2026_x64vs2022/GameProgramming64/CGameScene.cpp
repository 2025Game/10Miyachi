#include "CGameScene.h"
#include "CCharacter3.h"
#include "CTaskManager.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CCollisionManager.h"
#include "CCube.h"
#include "CCamera.h"
#include "CPaladin.h"

//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\sky.obj", "res\\sky.mtl"

CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
{
}

void CGameScene::Load()
{
	//課題 背景モデルデータの読み込み
	mBackGround.Load(MODEL_BACKGROUND);
	//キャラクタのインスタンス作成
	CCharacter3* character = new CCharacter3();
	//キャラクタのモデルの設定
	character->Model(&mBackGround);
	mPlayer.Load(MODEL_FILE);
	CXPlayer* player = new CXPlayer();
	player->Init(&mPlayer);
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
	CPaladin* paladin = new CPaladin(CVector(0.0f, 1.0f, -4.0f));
	CCharacter3* cube = new CCube();
	cube->Position(CVector(0.0f, 0.0f, -9.0f));
	cube->Scale(CVector(10.0f, 0.5f, 10.0f));
	//カメラ位置の設定
	CCamera::Instance()->Scale(CVector(0.0f, 1.0f, -7.0f));
}

void CGameScene::Update()
{
	//カメラの設定
	//gluLookAt(1.0f, 2.0f, 10.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	//全キャラクタの更新
	CTaskManager::Instance()->Update();
	//衝突処理の呼び出し
	CTaskManager::Instance()->Collision();
	//カメラの更新
	CCamera::Instance()->Update();
	//課題 全キャラクタの描画
	CTaskManager::Instance()->Render();
	//コライダの描画
	CCollisionManager::Instance()->Render();
}