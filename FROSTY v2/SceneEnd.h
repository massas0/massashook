#pragma once
#include "HookIncludes.h"
#include "Hooks.h"
#include "SDK.h"
#include "Chams.h"
typedef void(__thiscall *SceneEnd_t)(void *pEcx);

void __fastcall hookSceneEnd(void *pEcx, void *pEdx)
{
	auto ofunc = hooks::renderview.get_original<SceneEnd_t>(9);

	if (g_Options.Visuals.fakeangle && g_Input->m_fCameraInThirdPerson)
	{
		C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		if (pLocal)
		{
			static  IMaterial* ghost_mat = CreateMaterial2("VertexLitGeneric", "vgui/white_additive", false, true, true, true, true);
			if (ghost_mat)
			{
				Vector OrigAng;
				OrigAng = *pLocal->GetEyeAngles();
				pLocal->SetAngle2(Vector(0, Globals::FakeAngle.y, 0));

				float NormalColor[3] = { 1, 1, 1 };
				g_RenderView->SetColorModulation(NormalColor);
				g_RenderView->SetBlend(0.3f);
				g_ModelRender->ForcedMaterialOverride(ghost_mat);
				pLocal->DrawModel(0x1, 200);
				g_ModelRender->ForcedMaterialOverride(nullptr);
				pLocal->SetAngle2(OrigAng);
			}
		}
	}
	
	ofunc(pEcx);
}
	