#pragma once

#include "CState.h"
#include "CInput.h"

class CPlayerAttack : public CState
{
public:
	void Start(CXCharacter* parent) override;
	void Update() override;
private:
	CInput mInput;
};