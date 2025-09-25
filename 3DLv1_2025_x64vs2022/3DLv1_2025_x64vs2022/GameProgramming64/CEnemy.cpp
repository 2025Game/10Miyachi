#include "CEnemy.h"
#include "CApplication.h"

void CEnemy::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CEnemy::Collision(CCharacter* m, CCharacter* o)
{
	switch (o->Tag())
	{
	case ETag::EENEMY:
		break;
	default:
		if (CRectangle::Collision(o))
		{
			mState = EState::ESTOP;
			mEnabled = false;
		}
	}
}

CEnemy::CEnemy(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, l, r, b, t);
	mState = EState::EMOVE;
	mTag = CCharacter::ETag::EENEMY;
}

bool CEnemy::Collision(CRectangle* rect)
{
	if (CRectangle::Collision(rect))
	{
		Texture(Texture(), 1946, 2172, 920, 664);
		mState = EState::ESTOP;
		return true;
	}
	return false;
}

void CEnemy::Update()
{
	if (mState == EState::EMOVE)
	{

		float x = X() + 8;
		if (x > 820.0f)
		{
			x = 0.0f;
		}

		X(x);
	}
}
