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

class CApplication
{
public:
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