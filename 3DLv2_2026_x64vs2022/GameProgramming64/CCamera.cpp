#include "CCamera.h"
#include "glut.h"

void CCamera::Start(double left, double right , double bottom, double top)
{
	//モデルビュー行列の退避
	glPushMatrix();
	//モデルビュー行列の初期化
	glLoadIdentity();
	//Depthテストオフ
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);

	//プロジェクション行列への切り替え
	glMatrixMode(GL_PROJECTION);
	//プロジェクション行列の退避
	glPushMatrix();
	//プロジェクション行列の初期化
	glLoadIdentity();
	//表示エリアの設定
	gluOrtho2D(left, right, bottom, top);
}

void CCamera::End()
{
	//プロジェクション行列を戻す
	glPopMatrix();
	//モデルビューモードへ切り替え
	glMatrixMode(GL_MODELVIEW);

	//モデルビュー行列を戻す
	glPopMatrix();
	//Depthテストオン
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CCamera::Update()
{
	//課題 Jキーで左回転、Lキーで右回転
	if (mInput.Key('J'))
	{
		mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
	}
	CTransform::Update();
	//カメラの位置、注視点、上方向を計算する
	CVector mCenter = CVector() * mMatrix;
	CVector mEye = CVector(1.0f, 1.0f, 1.0f) * mMatrix;
	CVector mUp = CVector(0.0f, 1.0f, 0.0f);
	//カメラの位置、注視点、上方向を設定する
	gluLookAt(mEye.X(), mEye.Y(), mEye.Z() , mCenter.X(), mCenter.Y(), mCenter.Z() , mUp.X(), mUp.Y(), mUp.Z());
}

CCamera* CCamera::spInstance = nullptr;

CCamera* CCamera::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CCamera();
	}
	return spInstance;
}