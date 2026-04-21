#include "CTitleScene.h"
#include "CCamera.h"
#include "glut.h"
#include "CApplication.h"

CTitleScene::CTitleScene()
	: CSceneBase(EScene::eTitle)
{
#ifdef _DEBUG
	printf("CTielScene()\n");
#endif
}

CTitleScene::~CTitleScene()
{
#ifdef _DEBUG
	printf("~CTielScene()\n");
#endif
}

void CTitleScene::Load()
{
	mFont.Load("FontWhite.png", 1, 64);
}

void CTitleScene::Update()
{
	CCamera::Start(0, 800, 0, 600); //2D•`‰æŠJn
	//•`‰æF‚Ìİ’è(ÔF)
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//•¶š—ñ‚Ì•`‰æ
	mFont.Draw(400, 300, 18, 26, "CLICK");
	CCamera::End(); //2D•`‰æI—¹}
}