#pragma once
#ifndef CAPPLICATION3_H
#define CAPPLICATION3_H
#include "CTitleScene.h"
#include <memory> //std::shared_ptr
class CApplication3
{
public:
	CApplication3();
	~CApplication3();
	void Start();
	void Update();
private:
	//シーンのインスタンスを保持するスマートポインタ
	std::unique_ptr<CSceneBase> mpScene;
};
#endif