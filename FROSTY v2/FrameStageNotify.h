#pragma once
#include "SkinChanger.h"
#include "Resolver.h"
#include "HookIncludes.h"
#include "GloveChanger.h"
#include "LagComp.h"
typedef void(__stdcall *fsn_t)(ClientFrameStage_t);


void  __stdcall hkFrameStageNotify(ClientFrameStage_t curStage)
{
	try {
		static auto ofunc = hooks::client.get_original<fsn_t>(36);
		if (g_Engine->IsConnected() && g_Engine->IsInGame())
		{
			if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
				for (int i = 1; i < g_Engine->GetMaxClients(); i++)
				{

					auto pEntity = (C_BaseEntity*)g_EntityList->GetClientEntity(i);
					auto local = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

					if (!pEntity
						|| pEntity == local
						|| pEntity->IsDormant()
						|| !pEntity->IsAlive())
						continue;

					resolver->Resolve(pEntity, curStage);
				}
				if (g_Options.Skinchanger.Enabled)
				{
					if (g_Options.Skinchanger.glove)
					{
						GloveChanger();
					}
					SkinChanger();
				}
				backtracking->Update(g_Globals->tickcount);

			}
			if (curStage == FRAME_RENDER_START)
			{
				auto pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
				auto dwDeadFlag = NetVarManager->GetOffset("DT_CSPlayer", "deadflag"); // int
				if (pLocal)
				{
					*(int*)((uintptr_t)pLocal + 0xA30) = g_Globals->framecount; //we'll skip occlusion checks now
					*(int*)((uintptr_t)pLocal + 0xA28) = 0; //clear occlusion flags

					if (pLocal->IsAlive() && g_Input->m_fCameraInThirdPerson)
					{
						if (!g_Options.Visuals.TPAng)
							*(QAngle*)((DWORD)pLocal + dwDeadFlag + 0x4) = Globals::RealAngle;
						else
							*(QAngle*)((DWORD)pLocal + dwDeadFlag + 0x4) = Globals::FakeAngle;

					}
				}
			}

		}
		ofunc(curStage);
	}
	catch (std::exception(PSN))
	{
		char buf[100];
		sprintf(buf, "FrameStageNotify: %s", PSN.what());
		MessageBox(NULL, buf, "Whoops, MassasHook crashed", MB_OK);
	}
}
