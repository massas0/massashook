#pragma once
#include "HookIncludes.h"
#include "SDK.h"

typedef void(__thiscall *override_view_t)(void* _this, CViewSetup* setup);
typedef float(__thiscall *get_fov_t)(void*);



float __fastcall hkGetViewModelFOV(void* ecx, void* edx)
{
	try {
		C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		static auto ofunc = hooks::clientmode.get_original<get_fov_t>(35);
		float viewmodelFOV = ofunc(ecx);
		if (!g_Options.Visuals.viewmodelChanger || !pLocal)
			return viewmodelFOV;
		else
		{
			if (!pLocal->IsScoped())
				return g_Options.Visuals.viewmodelChanger;
			else
				return viewmodelFOV;
		}
	}
	catch (std::exception(GVF))
	{
		char buf[100];
		sprintf(buf, "GetViewModelFOV: %s", GVF.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}


void __fastcall hkOverrideView(void* _this, void* _edx, CViewSetup* setup)
{
	try {
		static auto ofunc = hooks::clientmode.get_original<override_view_t>(18);
		C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		if (pLocal && g_Engine->IsInGame())
		{
			if (g_Options.Visuals.NoVisualRecoil)
			{
				setup->angles.x -= ((pLocal->localPlayerExclusive()->GetViewPunchAngle().x * 2) * 0.45);
				setup->angles.y -= ((pLocal->localPlayerExclusive()->GetViewPunchAngle().y * 2) * 0.45);
			}
			if (!pLocal->IsScoped() && g_Options.Visuals.FOVChanger)
				setup->fov = g_Options.Visuals.FOVChanger;

			static bool enabledtp = false;
			static bool check = false;
			if (GetAsyncKeyState(g_Options.Visuals.TPKey))
			{
				if (!check)
					enabledtp = !enabledtp;
				check = true;
			}
			else
				check = false;

			static Vector vecAngles;
			g_Engine->GetViewAngles(vecAngles);
			if (g_Options.Visuals.ThirdPerson && pLocal->IsAlive() && enabledtp)
			{
				if (!g_Input->m_fCameraInThirdPerson)
				{
					g_Input->m_fCameraInThirdPerson = true;
					g_Input->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 150);
				}
			}
			else
			{
				g_Input->m_fCameraInThirdPerson = false;
				g_Input->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
			}
		}
		grenade_prediction::instance().View(setup);
		ofunc(_this, setup);
	}
	catch (std::exception(OV))
	{
		char buf[100];
		sprintf(buf, "OverrideView: %s", OV.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}