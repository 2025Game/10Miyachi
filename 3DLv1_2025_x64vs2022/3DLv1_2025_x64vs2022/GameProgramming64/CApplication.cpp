#include "CApplication.h"
#include "CRectangle.h"
#include "CInput.h"
//OpenGL
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"

//敵輸送機モデル
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl"
//モデルデータの指定
#define MODEL_OBJ "res\\f14.obj", "res\\f14.mtl"
#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル
//背景モデルデータの指定
#define MODEL_BACKGROUND  "res\\sky.obj", "res\\sky.mtl"

CCharacterManager CApplication::mCharacterManager;
CTexture CApplication::mTexture;

CTexture* CApplication::Texture()
{
	return &mTexture;
}

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTaskManager CApplication::mTaskManager;
CTaskManager* CApplication::TaskManager()
{
	return &mTaskManager;
}

void CApplication::Start()
{
	//C5モデルの読み込み
	mModelC5.Load(MODEL_C5);
	//敵機のインスタンス作成
	new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy(&mModelC5, CVector(30.0f, 10.0f, -130.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	//モデルファイルの入力
	mModel.Load(MODEL_OBJ);
	mBackGround.Load(MODEL_BACKGROUND);
	mPlayer.Model(&mModel);
	mPlayer.Scale(CVector(0.1f, 0.1f, 0.1f));
	mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));
	mPlayer.Position(CVector(0.0f, 0.0f, -3.0f));
	mEye = CVector(1.0f, 2.0f, 3.0f);
	CMatrix matrix;
	matrix.Print();
}

void CApplication::Update()
{
	//タスクマネージャの更新
	mTaskManager.Update();

	//頂点1､頂点2､頂点3,法線データの作成
	CVector v0, v1, v2, n;
	//法線を上向きで設定する
	n.Set(0.0f, 1.0f, 0.0f);
	//頂点1の座標を設定する
	v0.Set(0.0f, 0.0f, 0.5f);
	//頂点2の座標を設定する
	v1.Set(1.0f, 0.0f, 0.0f);
	//頂点3の座標を設定する
	v2.Set(0.0f, 0.0f, -0.5f);

	if (mInput.Key('J'))
	{
		mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('I'))
	{
		mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
	}
	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	}

	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = mPlayer.Position() + CVector(0.0f, 1.0f, -3.0f) * mPlayer.MatrixRotate();
	//注視点を求める
	c = mPlayer.Position();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
	//カメラの設定
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

	mBackGround.Render();

	//タスクリストの削除
	mTaskManager.Delete();
	//タスクマネージャの描画
	mTaskManager.Render();
}