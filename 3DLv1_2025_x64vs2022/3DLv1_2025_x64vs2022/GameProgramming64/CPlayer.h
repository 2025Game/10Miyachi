#pragma once
#include "CCharacter.h"
#include "CInput.h"
class CPlayer : public CCharacter
{
public:
	void Update();
private:
	CInput mInput;

};
