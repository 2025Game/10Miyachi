#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CPlayer.h"
#include "CFont.h"
#include "CMiss.h"
#include "CCharacterManager.h"
#include "CGame.h"
#include "CVector.h"
#include "CModel.h"
#include "CCharacter3.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"

class CApplication
{
public:
	~CApplication();

	static CUi* Ui();	//UIクラスのインスタンスを取得

	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();

	//static CTaskManager* TaskManager();

	static CTexture* Texture();
	static CCharacterManager* CharacterManager();
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};

	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
private:
	static CUi* spUi;	//UIクラスのポインタ

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;

	//三角コライダの作成
	//CColliderTriangle mColliderTriangle;
	//CColliderTriangle mColliderTriangle2;

	//モデルビューの逆行列
	static CMatrix mModelViewInverse;
	//C5モデル
	CModel mModelC5;

	//static CTaskManager mTaskManager;
	CPlayer mPlayer;
	//CCharacter3 mCharacter;

	CModel mBackGround; //背景モデル
	//モデルクラスのインスタンス作成
	CModel mModel;

	CVector mEye;
	CSound mSoundBgm;
	CSound mSoundOver;

	CGame* mpGame;
	static CCharacterManager mCharacterManager;
	EState mState;
	CMiss* mpMiss;
	CInput mInput;
	CFont mFont;
	CPlayer* mpPlayer;
	CBullet* mpBullet;
	static CTexture mTexture;
	CEnemy* mpEnemy;
};