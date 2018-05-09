#pragma once
#include "HookIncludes.h"
#include <vcruntime_exception.h>
typedef void(__thiscall* dme_t)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);

float flColor2[3];
float flVColor2[3];
float GlowCol[3];

std::vector<const char*> vistasmoke_mats =
{
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};


void __fastcall hkDrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	try {
		static bool DontDraw = false;
		static auto ofunc = hooks::modelrender.get_original<dme_t>(21);

		static IMaterial* norm2 = CreateMaterial(false, true, false);
		static IMaterial* flat2 = CreateMaterial(false, false, false);
		static IMaterial* wire2 = CreateMaterial(false, false, true);

		static IMaterial* znorm2 = CreateMaterial(true, true, false);
		static IMaterial* zflat2 = CreateMaterial(true, false, false);
		static IMaterial* zwire2 = CreateMaterial(true, false, true);

		const char * ModelName = g_ModelInfo->GetModelName((model_t*)pInfo.pModel);
		C_BaseEntity* pModelEntity = (C_BaseEntity*)g_EntityList->GetClientEntity(pInfo.entity_index);
		C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());


		if (g_Options.Visuals.matierial)
		{

			if (ModelName && strstr(ModelName, "models/player"))
			{
				if (pModelEntity && pLocal)
				{

					if ((g_Options.Visuals.Box && pModelEntity->GetTeamNum() != pLocal->GetTeamNum()) || (g_Options.Visuals.TeamESP && pModelEntity->GetTeamNum() == pLocal->GetTeamNum()))
					{

						if (pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
						{
							flColor2[0] = g_Options.Colors.EnemyChamsNVis[0];
							flColor2[1] = g_Options.Colors.EnemyChamsNVis[1];
							flColor2[2] = g_Options.Colors.EnemyChamsNVis[2];
							flVColor2[0] = g_Options.Colors.EnemyChamsVis[0];
							flVColor2[1] = g_Options.Colors.EnemyChamsVis[1];
							flVColor2[2] = g_Options.Colors.EnemyChamsVis[2];
							GlowCol[0] = g_Options.Colors.EnemyGlow[0];
							GlowCol[1] = g_Options.Colors.EnemyGlow[1];
							GlowCol[2] = g_Options.Colors.EnemyGlow[2];
						}
						if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
						{
							flColor2[0] = g_Options.Colors.TeamChamsNVis[0];
							flColor2[1] = g_Options.Colors.TeamChamsNVis[1];
							flColor2[2] = g_Options.Colors.TeamChamsNVis[2];
							flVColor2[0] = g_Options.Colors.TeamChamsVis[0];
							flVColor2[1] = g_Options.Colors.TeamChamsVis[1];
							flVColor2[2] = g_Options.Colors.TeamChamsVis[2];
							GlowCol[0] = g_Options.Colors.TeamGlow[0];
							GlowCol[1] = g_Options.Colors.TeamGlow[1];
							GlowCol[2] = g_Options.Colors.TeamGlow[2];
						}
						if (pModelEntity)
						{
							if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0)
							{


								if (pModelEntity->HasGunGameImmunity())
								{
									norm2->AlphaModulate(0.5f);
									znorm2->AlphaModulate(0.5f);
									flat2->AlphaModulate(0.5f);
									zflat2->AlphaModulate(0.5f);
									wire2->AlphaModulate(0.5f);
									zwire2->AlphaModulate(0.5f);
								}
								else
								{
									norm2->AlphaModulate(1.f);
									znorm2->AlphaModulate(1.f);
									flat2->AlphaModulate(1.f);
									zflat2->AlphaModulate(1.f);
									wire2->AlphaModulate(1.f);
									zwire2->AlphaModulate(1.f);
								}



								if (g_Options.Visuals.matierial == 1)
								{
									if (g_Options.Visuals.XQZ2)
									{

										znorm2->ColorModulate(flColor2[0], flColor2[1], flColor2[2]);
										g_ModelRender->ForcedMaterialOverride(znorm2);

										ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
									}
									norm2->ColorModulate(flVColor2[0], flVColor2[1], flVColor2[2]);
									g_ModelRender->ForcedMaterialOverride(norm2);
									ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

								}
								else if (g_Options.Visuals.matierial == 2)
								{
									if (g_Options.Visuals.XQZ2)
									{

										zflat2->ColorModulate(flColor2[0], flColor2[1], flColor2[2]);
										g_ModelRender->ForcedMaterialOverride(zflat2);
										ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
									}
									flat2->ColorModulate(flVColor2[0], flVColor2[1], flVColor2[2]);
									g_ModelRender->ForcedMaterialOverride(flat2);
									ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
								}
							}

						}
					}

				}

			}
		}





		if (!DontDraw)
			ofunc(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
		g_ModelRender->ForcedMaterialOverride(NULL);
	}
	catch (std::exception(DME))
	{
		char buf[100];
		sprintf(buf, "DrawModelExecute: %s", DME.what());
		MessageBox(NULL, buf, "Whoops, MassasHook crashed", MB_OK);
	}
}