#pragma once
#include "SDK.h"

class CWalkbot
{
public:
	void Run(CInput::CUserCmd* cmd);
	int marker = 0;

private:
	bool WalkInDirection(C_BaseEntity* pLocal, Vector point, CInput::CUserCmd *ppCmd);
	bool MoveToPoint(const std::vector<Vector> points, CInput::CUserCmd *pCmd, C_BaseEntity * pLocal, int pints);

};

extern CWalkbot* Walkbot;