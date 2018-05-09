#pragma once
#include "SDK.h"
#include "Misc.h"

class AutoBlock
{
public:
	void Tick(CInput::CUserCmd *cmd)
	{
		if (!g_Options.Misc.AutoBlockKey)
			return;

		if (!GetAsyncKeyState(g_Options.Misc.AutoBlockKey))
			return;

		auto pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		C_BaseEntity *pTarget = NULL;

		float bestDistance = 50.0f;

		int max = g_Engine->GetMaxClients();

		for (int i = 0; i < max; i++)
		{
			C_BaseEntity *pEntity = g_EntityList->GetClientEntity(i);

			if (pEntity == NULL) continue;
			if (pEntity == pLocal) continue;
			if (pEntity->IsDormant()) continue;
			if (pEntity->GetLifeState() != LIFE_ALIVE) continue;
			if (pEntity->GetHealth() <= 0) continue;

			float distance = (pEntity->GetOrigin() - pLocal->GetOrigin()).Length();

			if (distance < bestDistance)
			{
				bestDistance = distance;
				pTarget = pEntity;
			}
		}

		if (pTarget != NULL)
		{
			QAngle angles = CalcAngle(pTarget->GetOrigin(), pLocal->GetOrigin());
			angles.y -= pLocal->GetEyeAngles1().y;
			angles.Normalized();

			cmd->sidemove = sin(DEG2RAD(angles.y)) * 10000.0f;// 450.0f;

			if (cmd->sidemove > 450.0f)
				cmd->sidemove = 450.0f;

			if (cmd->sidemove < -450.0f)
				cmd->sidemove = -450.0f;
		}
	}
};

extern AutoBlock *autoBlock;
