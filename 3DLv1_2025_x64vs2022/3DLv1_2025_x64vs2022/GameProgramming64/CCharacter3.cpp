#include "CCharacter3.h"
#include "CApplication.h"

void CCharacter3::Model(CModel* m)
{
	mpModel = m;
}

//描画処理
void CCharacter3::Render()
{
	mpModel->Render(mMatrix);
}

CCharacter3::~CCharacter3()
{
	//タスクリストから削除
	CApplication::TaskManager()->Remove(this);
}

CCharacter3::CCharacter3()
	:mpModel(nullptr)
{
	//タスクリストに追加
	CApplication::TaskManager()->Add(this);
}