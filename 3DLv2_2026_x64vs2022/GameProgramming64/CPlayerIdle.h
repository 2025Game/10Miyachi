#pragma once

#include "CState.h"
#include "CInput.h"

class CPlayerIdle : public CState
{
public:
	void Start(CXCharacter* parent) override;
	void Update() override;
private:
	CInput mInput;
};