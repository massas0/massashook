#include "Walkbot.h"

const std::vector<Vector> ct_dust2 = {

	Vector(279.11f,2175.06f,-128.18f),
	Vector(1380.f, 2131.6f, -5.48f), Vector(1398.04f, 928.93f, -8.78f), Vector(841.17f, 950.18f, 0.14f),
	Vector(581.58f, 674.48f, 1.50f) , Vector(690.59f, 345.20f, 1.54f), Vector(446.82f, 70.25f, -3.26f),
	Vector(-5.52f, -829.16f, -3.58f), Vector(-503.56f, -739.92f, 114.16f), Vector(-1913.50f,-735.72f, 121.20f),
	Vector(-1676.94f, 726.70f, 31.00f), Vector(-1655.66f,1053.87f,31.57f), Vector(-1989.64f,1330.22f,28.93f),
	Vector(-1926.35f,2197.58f,-3.22f), Vector(-1331.98f,2231.99f,2.05f), Vector(-1286.65f,2164.06f,4.25f),
	Vector(-606.69f,2370.25f,-105.67f),Vector(-429.61f, 2196.21f, -123.86)

};

const std::vector<Vector> t_dust2 = { 
	Vector(-1399.53, -844.30, 180.50), // 0
	Vector(-1903.95, -757.09, 184.14), // 1
	Vector(-1748.27, 395.31, 66.25), // 2
	Vector(-1623.74, 1080.75, 97.27), // 3
	Vector(-1971.85, 1252.58, 94.98), // 4
	Vector(-1952.23, 2206.21, 61.32), // 5
	Vector(-1357.44, 2243.62, 65.17), // 6
	Vector(-1283.54, 2147.95, 69.66), // 7
	Vector(-524.830017, 2244.989990, - 54.438431), // 8
	Vector(-62.45, 2131.61, -62.74), // 9
	Vector(1448.97, 2171.56, 54.65), // 10
	Vector(1416.15, 3013.41, 184.71), // 11
	Vector(666.89, 2491.53, 160.40), // 12
	Vector(351.03, 2470.60, 160.63), // 13
	Vector(342.20, 1505.46, 65.88), // 14
	Vector(-189.66, 1340.55, 64.15), // 15
	Vector(-192.74, 689.97, 65.77), // 16
	Vector(-435.500092, 513.376465, 61.485100), // 17
	Vector(334.822510, 260.660095, 64.603569), // 18
	Vector(250.02, -831.84, 53.88) // 19
};

inline float fl_GetDistance(Vector from, Vector to)
{
	Vector angle;
	angle.x = to.x - from.x;	angle.y = to.y - from.y;	angle.z = to.z - from.z;

	return floor(sqrt(angle.x*angle.x + angle.y*angle.y + angle.z*angle.z) * 0.0254f);
}

void CWalkbot::Run(CInput::CUserCmd* cmd)
{
	if (!g_Options.Misc.WalkRobot) return;

	auto local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (!local) return;

	int team = local->GetTeamNum();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());

	if (!pWeapon) return;

	C_BaseEntity* weaponEnt = (C_BaseEntity*)pWeapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (!g_Engine->IsInGame())
	{
		marker = 0;
		return;
	}

	if (team == 0) // unassigned
	{
		g_Engine->ClientCmd("teammenu"); // Start the Auto-Select team menu.
		return;
	}

	if ((pWeaponClass->m_ClassID == 29)) //drop bomb
	{
		int droptick;
		droptick++;
		if (droptick > 32)
		{
			g_Engine->ClientCmd("drop");
		}
	}


	if (g_Options.Misc.WalkRobot == 1)
	{
		MoveToPoint(t_dust2, cmd, local, 20);
		Globals::walkbot = true;
	}
	


}

bool CWalkbot::MoveToPoint(const std::vector<Vector> points, CInput::CUserCmd *pCmd, C_BaseEntity * pLocal, int pints)
{

	if (Globals::legitbotting)
		return false;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (!pWeapon) return false;
	else if ((pWeapon->ammo() <= 5)) pCmd->buttons |= IN_RELOAD;

	if (pCmd->buttons & IN_ATTACK) return false;


	if (marker == pints)
	{
		marker = 0;
		g_CVar->ConsoleColorPrintf(Color(225, 225, 10), "[Walkbot] Looped all points.\n");
	}

	C_BaseEntity* weaponEnt = (C_BaseEntity*)pWeapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if ((fl_GetDistance(pLocal->GetOrigin(), points[marker]) <= 1.f))
	{
		g_CVar->ConsoleColorPrintf(Color(225, 225, 10), "[Walkbot] Reached point.\n");
		marker++;
	}
	else
		WalkInDirection(pLocal, points[marker], pCmd);
	
	return false;
}

bool CWalkbot::WalkInDirection(C_BaseEntity* pLocal, Vector point, CInput::CUserCmd *ppCmd)
{
	bool ReturnValue = false;
	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();
	VectorAngles(point - src, angles);

	ReturnValue = true;

	sanitize_angles(angles);
	ppCmd->forwardmove = 449.f;
	movementfix(angles, ppCmd);
	return ReturnValue;
}

CWalkbot* Walkbot = new CWalkbot();;