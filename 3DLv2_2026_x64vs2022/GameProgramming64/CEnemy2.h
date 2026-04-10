#pragma once
#include "CCharacter.h"

class CEnemy2 : public CCharacter
{
public:
	//“G‚Ì”‚ğæ“¾
	static int Num();
	static void Num(int n) { sNum = n;  };
	//Õ“Ëˆ—2
	void Collision();
	//Õ“Ëˆ—4
	void Collision(CCharacter* m, CCharacter* o);

	CEnemy2(float x, float y, float w, float h, CTexture* pt);
	void Update();
private:
	static int sNum;	//“G‚Ì”
};