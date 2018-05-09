#pragma once
#include "HookIncludes.h"
#include "Interfaces.h"
#include <ctime>
#include "ESP.h"
#include "MovementRecorder.h"

typedef void(__thiscall* paint_traverse_t)(PVOID, unsigned int, bool, bool);


bool once = true;
bool once1 = false;
int width1 = 0;
int height1 = 0;

void __fastcall hkPaintTraverse(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	try {
		static auto ofunc = hooks::panel.get_original<paint_traverse_t>(41);
		C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		if (local != nullptr && local->IsAlive() && g_Options.Visuals.noscopeborder && !strcmp("HudZoom", g_Panel->GetName(vguiPanel)))
		{
			return;
		}
		ofunc(pPanels, vguiPanel, forceRepaint, allowForce);
		static unsigned int FocusOverlayPanel = 0;
		static bool FoundPanel = false;
		if (!FoundPanel)
		{
			PCHAR szPanelName = (PCHAR)g_Panel->GetName(vguiPanel);
			if (strstr(szPanelName, "FocusOverlayPanel"))
			{
				FocusOverlayPanel = vguiPanel;
				FoundPanel = true;
			}
		}
		else if (FocusOverlayPanel == vguiPanel)
		{
			if (g_Engine->IsConnected() && g_Engine->IsInGame())
			{
				static auto linegoesthrusmoke = U::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
				static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
				if (g_Options.Visuals.NoSmoke) *(int*)(smokecout) = 0;
				

				visuals::instance().OnPaintTraverse(local);

				g_Options.Visuals.ThirdPerson = true;

				

				auto m_flFlashDuration = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashDuration");
				auto m_flFlashMaxAlpha = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
				if (local != nullptr)
				{
					if (g_Options.Misc.WalkRobot) {
						
						g_Render->Textf(50, 50, Color::White(), g_Render->font.ESP, "Walkbotting...");

					}

					CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
					if (g_Options.Visuals.NoFlash)
					{
						*MakePtr(float*, local, m_flFlashDuration) = 0.f;
						*MakePtr(float*, local, m_flFlashMaxAlpha) = 0.f;
					}
					if (local && local->IsScoped() && g_Options.Visuals.noscopeborder && MiscFunctions::IsSniper(pWeapon))
					{
						int width = 0;
						int height = 0;
						g_Engine->GetScreenSize(width, height);

						int centerX = static_cast<int>(width * 0.5f);
						int centerY = static_cast<int>(height * 0.5f);
						g_Render->Line(0, centerY, width, centerY, Color(0, 0, 0, 255));
						g_Render->Line(centerX, 0, centerX, height, Color(0, 0, 0, 255));
					}
					if (g_Options.Visuals.SniperCrosshair && MiscFunctions::IsSniper(pWeapon) && !local->IsScoped())
					{
						int width = 0;
						int height = 0;

						g_Engine->GetScreenSize(width, height);
						if (local  && local->IsAlive())
						{
							static Vector ViewAngles;
							g_Engine->GetViewAngles(ViewAngles);
							ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

							static Vector fowardVec;
							AngleVectors(ViewAngles, &fowardVec);
							fowardVec *= 10000;

							// Get ray start / end
							Vector start = local->GetOrigin() + local->GetViewOffset();
							Vector end = start + fowardVec, endScreen;

							CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
							float cone = 0.01f;
							if (cone > 0.0f)
							{
								if (cone < 0.01f) cone = 0.01f;
								float size = (cone * height) * 0.7f;
								Color color(255, 255, 255, 200);

								if (g_Render->WorldToScreen(end, endScreen))
								{
									g_Render->OutlineCircle(endScreen.x, endScreen.y, (int)size, 100, color);
								}
							}
						}
					}
					if (g_Options.Visuals.spread)
					{
						int width = 0;
						int height = 0;

						g_Engine->GetScreenSize(width, height);
						if (local  && local->IsAlive())
						{
							static Vector ViewAngles;
							g_Engine->GetViewAngles(ViewAngles);
							ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

							static Vector fowardVec;
							AngleVectors(ViewAngles, &fowardVec);
							fowardVec *= 10000;

							// Get ray start / end
							Vector start = local->GetOrigin() + local->GetViewOffset();
							Vector end = start + fowardVec, endScreen;

							CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
							float cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
							if (cone > 0.0f)
							{
								if (cone < 0.01f) cone = 0.01f;
								float size = (cone * height) * 0.7f;
								
								Color color;
								if (cone < 0.1f)
									color = Color(0, 255, 40);
								else if (cone < 0.5f)
									color = Color(239, 199, 21);
								else if (cone < 1.f)
									color = Color(255, 30, 0);


								if (g_Render->WorldToScreen(end, endScreen))
								{
									g_Render->OutlineCircle(endScreen.x, endScreen.y, (int)size, 100, color);
								}
							}
						}
					}
				}
			}
			if (!g_Engine->IsInGame()) {
				g_Options.Misc.NameChangerFix = false;
				g_Options.Misc.ChatSpam = 0;
				g_Options.Misc.NameSpammer = 0;
				g_Options.Misc.NoName = false;
				g_Options.Misc.silentstealer = false;
				g_Options.Misc.ChatSpam = false;
				g_Options.Misc.ClantagChanger = 0;
			}
			if (g_Options.Ragebot.MainSwitch && !once)
			{
				g_Options.Legitbot.MainSwitch = false;
				once = !once;
			}
			if (g_Options.Legitbot.MainSwitch && once)
			{
				g_Options.Ragebot.MainSwitch = false;
				once = !once;
			}

			DrawMovementRecorder();
		}
	}
	catch (std::exception(PT))
	{
		char buf[100];
		sprintf(buf, "PaintTraverse: %s", PT.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}
