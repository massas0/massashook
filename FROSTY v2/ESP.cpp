
#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "Autowall.h"
#include "Hooks.h"
#include <sstream>




visuals::visuals()
{
	BombCarrier = nullptr;
}

int width = 0;
int height = 0;
bool done = false;
bool done1 = false;

float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];

const char *GetIcon(ItemDefinitionIndexx def)
{
	switch (def)
	{
	default:
		return "W";
	case ItemDefinitionIndexx::WEAPON_MP9:
		return "A";
	case ItemDefinitionIndexx::WEAPON_UMP45:
		return "B";
	case ItemDefinitionIndexx::WEAPON_TASER:
		return "C";
	case ItemDefinitionIndexx::WEAPON_NOVA:
		return "D";
	case ItemDefinitionIndexx::WEAPON_KNIFE_HUNTSMAN:
		return "E";
	case ItemDefinitionIndexx::WEAPON_P90:
		return "F";
	case ItemDefinitionIndexx::WEAPON_P2000:
		return "G";
	case ItemDefinitionIndexx::WEAPON_REVOLVER:
		return "H";
	case ItemDefinitionIndexx::WEAPON_MAG7:
		return "I";
	case ItemDefinitionIndexx::WEAPON_SAWEDOFF:
		return "J";
	case ItemDefinitionIndexx::WEAPON_SCAR20:
		return "K";
	case ItemDefinitionIndexx::WEAPON_SG553:
		return "L";
	case ItemDefinitionIndexx::WEAPON_XM1014:
		return "M";
	case ItemDefinitionIndexx::WEAPON_USPS:
		return "N";
	case ItemDefinitionIndexx::WEAPON_MOLOTOV:
		return "O";
	case ItemDefinitionIndexx::WEAPON_MP7:
		return "P";
	case ItemDefinitionIndexx::WEAPON_KNIFE_DAGGER:
		return "Q";
	case ItemDefinitionIndexx::WEAPON_M4A1:
		return "R";
	case ItemDefinitionIndexx::WEAPON_NEGEV:
		return "S";
	case ItemDefinitionIndexx::WEAPON_M4A1S:
		return "T";
	case ItemDefinitionIndexx::WEAPON_MAC10:
		return "U";
	case ItemDefinitionIndexx::WEAPON_TEC9:
		return "V";
	case ItemDefinitionIndexx::WEAPON_KNIFE_T:
		return "W";
	case ItemDefinitionIndexx::WEAPON_SCOUT:
		return "X";
	case ItemDefinitionIndexx::WEAPON_M249:
		return "Y";
	case ItemDefinitionIndexx::WEAPON_SMOKE:
		return "Z";
	case ItemDefinitionIndexx::WEAPON_FAMAS:
		return "a";
	case ItemDefinitionIndexx::WEAPON_KNIFE_GUT:
		return "b";
	case ItemDefinitionIndexx::WEAPON_KNIFE_FALCHION:
		return "c";
	case ItemDefinitionIndexx::WEAPON_FLASH:
		return "d";
	case ItemDefinitionIndexx::WEAPON_AWP:
		return "e";
	case ItemDefinitionIndexx::WEAPON_G3SG1:
		return "f";
	case ItemDefinitionIndexx::WEAPON_GALIL:
		return "g";
	case ItemDefinitionIndexx::WEAPON_GLOCK:
		return "i";
	case ItemDefinitionIndexx::WEAPON_DEAGLE:
		return "i";
	case ItemDefinitionIndexx::WEAPON_HE:
		return "j";
	case ItemDefinitionIndexx::WEAPON_P250:
		return "k";
	case ItemDefinitionIndexx::WEAPON_INC:
		return "l";
	case ItemDefinitionIndexx::WEAPON_KNIFE_M9BAYONET:
		return "m";
	case ItemDefinitionIndexx::WEAPON_KNIFE_KARAMBIT:
		return "n";
	case ItemDefinitionIndexx::WEAPON_DECOY:
		return "o";
	case ItemDefinitionIndexx::WEAPON_DUALS:
		return "p";
	case ItemDefinitionIndexx::WEAPON_AK47:
		return "q";
	case ItemDefinitionIndexx::WEAPON_KNIFE_BAYONET:
		return "r";
	case ItemDefinitionIndexx::WEAPON_FIVE7:
		return "s";
	case ItemDefinitionIndexx::WEAPON_BIZON:
		return "t";
	case ItemDefinitionIndexx::WEAPON_CZ75:
		return "u";
	case ItemDefinitionIndexx::WEAPON_KNIFE_FLIP:
		return "v";
	case ItemDefinitionIndexx::WEAPON_AUG:
		return "w";
	case ItemDefinitionIndexx::WEAPON_KNIFE_BUTTERFLY:
		return "x";
	case ItemDefinitionIndexx::WEAPON_C4:
		return "y";
	case ItemDefinitionIndexx::WEAPON_KNIFE_CT:
		return "z";
	}
};

void visuals::OnPaintTraverse(C_BaseEntity* local)
{
	NightMode();

	if (g_Options.Visuals.EventLog)
	{
		for (size_t i = 0; i < events.size(); i++) {
			float time = g_Globals->curtime - events[i].time;

			if (time < 6)
			{
				Color color = Color(g_Options.Colors.MenuTheme[0] * 255.f, g_Options.Colors.MenuTheme[1] * 255.f, g_Options.Colors.MenuTheme[2] * 255.f);

				g_Render->Textf(5, 5 + i * 17, color, g_Render->font.Defuse, "[MassasHook]");
				g_Render->Textf(95, 5 + i * 17, events[i].color, g_Render->font.Defuse, events[i].text.c_str());
			}
		}
	}

	grenade_prediction::instance().Paint();
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());


	if (g_Options.Visuals.Crosshair)
		Crosshair(local);

	if (g_Options.Visuals.Hitmarker)
		Hitmarker();

	if (g_Options.Visuals.angleLines && pLocal->IsAlive())
		DrawAngles();


	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{

		C_BaseEntity *entity = g_EntityList->GetClientEntity(i);
		if (entity == nullptr)
			continue;
		if (entity == local)
			continue;

		if (entity && entity != local && !entity->IsDormant())
		{
			if (g_Options.Visuals.Radar.Enabled)
			{
				*(char*)((DWORD)(entity)+offsetz.DT_BaseEntity.m_bSpotted) = 1;
			}
		}

		if (entity->IsDormant())
			continue;



		player_info_t pinfo;
		Vector pos, pos3D;
		pos3D = entity->GetOrigin();

		int owner = 0;
		ClientClass* cClass = (ClientClass*)entity->GetClientClass();

		if (!g_Render->WorldToScreen(pos3D, pos))
			continue;

		if (strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47 || cClass->m_ClassID == (int)ClassID::CC4 || cClass->m_ClassID == (int)ClassID::CBaseAnimating)
		{
		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)(entity);
		int ClassID = cClass->m_ClassID;

		if (!weapon) return;

			if (g_Options.Visuals.Droppedguns)
			{

				if (strstr(entity->GetClientClass()->m_pNetworkName, XorStr("CWeapon")))
				{
					CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)(entity);
					g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_CENTER, "%s" " " "(" "%i" "/" "%i" ")", std::string(entity->GetClientClass()->m_pNetworkName).substr(7).c_str(), weapon->ammo(), weapon->ammo2());
				}
				if (ClassID == 1)
				{
					CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)(entity);
					g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_CENTER, "%s" " " "(" "%i" "/" "%i" ")", XorStr("AK-47"), weapon->ammo(), weapon->ammo2());
				}
				if (ClassID == 39)
				{
					CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)(entity);
					g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_CENTER, "%s" " " "(" "%i" "/" "%i" ")", XorStr("Desert Eagle"), weapon->ammo(), weapon->ammo2());
				}
				if (ClassID == 29)
				{
					g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_CENTER, "%s", XorStr("Dropped C4"));
				}
				if (ClassID == 2)
				{
					g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, Color(255, 255, 255, 255), FONT_CENTER, "%s", XorStr("Defuse Kit"));
				}
			}
		}


		if (g_Options.Visuals.Bomb && cClass->m_ClassID == (int)ClassID::CPlantedC4)
		{

			Vector vOrig; Vector vScreen;
			vOrig = entity->GetOrigin();
			CCSBomb* Bomb = (CCSBomb*)entity;
			float flBlow = Bomb->GetC4BlowTime();
			float lifetime = flBlow - (g_Globals->interval_per_tick * local->GetTickBase());
			if (g_Render->WorldToScreen(vOrig, vScreen))
			{
				if (local->IsAlive() && !Bomb->IsBombDefused())
				{
					float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
					float a = 450.7f;
					float b = 75.68f;
					float c = 789.2f;
					float d = ((flDistance - b) / c);
					float flDamage = a*exp(-d * d);

					damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));

					sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "Lethal");
					sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Damage: %.0f", damage);
					if (lifetime > -2.f)
					{
						if (damage >= local->GetHealth())
						{
							g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), g_Render->font.ESP, bombdamagestringdead);
						}
						else if (local->GetHealth() > damage)
						{
							g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), g_Render->font.ESP, bombdamagestringalive);
						}
					}
				}
				char buffer[64];
				if (lifetime > 0.01f && !Bomb->IsBombDefused())
				{
					sprintf_s(buffer, "Bomb: %.1f", lifetime);
					g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), g_Render->font.ESP, buffer);
				}

			}

			g_Engine->GetScreenSize(width, height);
			int halfX = width / 2;
			int halfY = height / 2;


			if (Bomb->GetBombDefuser() > 0)
			{
				float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * g_Globals->interval_per_tick);
				if (countdown > 0.01f)
				{
					if (lifetime > countdown)
					{
						char defuseTimeString[24];
						sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
						g_Render->Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), g_Render->font.ESP, defuseTimeString);
					}
					else
					{
						g_Render->Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), g_Render->font.ESP, "No Time");
					}
				}
			}
		}

		/*if (g_Options.Visuals.Grenades)
		{

			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			Color GrenadeColor = Color(0, 0, 0, 0);
			char* szModelName = "";
			if (strstr(cClass->m_pNetworkName, XorStr("Projectile")))
			{
				const model_t* pModel = entity->GetModel();
				if (!pModel)
					return;


				const studiohdr_t* pHdr = g_ModelInfo->GetStudiomodel(pModel);
				if (!pHdr)
					return;

				if (!strstr(pHdr->name, XorStr("thrown")) && !strstr(pHdr->name, XorStr("dropped")))
					return;



				IMaterial* mats[32];
				g_ModelInfo->GetModelMaterials(pModel, pHdr->numtextures, mats);

				for (int i = 0; i < pHdr->numtextures; i++)
				{
					IMaterial* mat = mats[i];
					if (!mat)
						continue;

					if (strstr(mat->GetName(), "flashbang"))
					{
						GrenadeColor = Color(255, 255, 0, 255);
						szModelName = "Flashbang";
					}
					else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
					{
						szModelName = "Grenade";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "smoke"))
					{
						szModelName = "Smoke";
						GrenadeColor = Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "decoy"))
					{
						szModelName = "Decoy";
						GrenadeColor = Color(0, 255, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "incendiary"))
					{
						szModelName = "Incendiary";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
					else if (strstr(mat->GetName(), "molotov"))
					{
						szModelName = "Molotov";
						GrenadeColor = Color(255, 0, 0, 255);
						break;
					}
				}
				g_Render->DrawString2(g_Render->font.ESP, (int)pos.x, (int)pos.y, GrenadeColor, FONT_CENTER, "%s", szModelName);
			}
		}*/

		if (g_Engine->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
		{
			if (g_Options.Legitbot.backtrackD)
			{
				if (local->IsAlive())
				{
					for (int t = 0; t < g_Options.Legitbot.backtrackTicks; ++t)
					{
						Vector screenbacktrack[64][12];

						if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
						{
							if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
							{

								g_Surface->DrawSetColor(Color::Green());
								g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

							}
						}
					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}
			if (g_Options.Ragebot.PosAdjust)
			{
				if (local->IsAlive())
				{
					for (int t = 0; t < 12; ++t)
					{
						Vector screenbacktrack[64][12];

						if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
						{
							if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
							{

								g_Surface->DrawSetColor(Color::Green());
								g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

							}
						}
					}
				}
				else
				{
					memset(&headPositions[0][0], 0, sizeof(headPositions));
				}
			}


			if (!g_Render->WorldToScreen(pos3D, pos))
				continue;

			Color clr = entity->GetTeamNum() == local->GetTeamNum() ?
				Color(g_Options.Colors.TeamESP[0] * 255, g_Options.Colors.TeamESP[1] * 255, g_Options.Colors.TeamESP[2] * 255, 255) :
				Color(g_Options.Colors.EnemyESP[0] * 255, g_Options.Colors.EnemyESP[1] * 255, g_Options.Colors.EnemyESP[2] * 255, 255);
			if (!entity->IsAlive())
				continue;
			bool PVS = false;
			RECT rect = DynamicBox(entity, PVS, local);
			if (entity->GetTeamNum() == local->GetTeamNum() && g_Options.Visuals.TeamESP)
			{
				DrawInfo(rect, entity, local);
				if (g_Options.Visuals.AimLine)
					DrawSnapLine(Vector(rect.left + ((rect.right - rect.left) / 2), rect.bottom, 0), clr);
				if (g_Options.Visuals.Skeleton)
					Skeleton(entity, clr);
				if (g_Options.Visuals.health)
					DrawHealth(rect, entity);
				if (g_Options.Visuals.barrel)
					BulletTrace(entity, clr);

				switch (g_Options.Visuals.BoxType)
				{
				case 0:
					break;
				case 1:
					DrawBox(rect, clr);
					break;
				case 2:
					DrawCorners(rect, clr);
					break;
				case 3:
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), clr);
					break;
				}
			}

			if (entity->GetTeamNum() != local->GetTeamNum() && g_Options.Visuals.Box)
			{
				DrawInfo(rect, entity, local);
				if (g_Options.Visuals.AimLine)
					DrawSnapLine(Vector(rect.left + ((rect.right - rect.left) / 2), rect.bottom, 0), clr);
				if (g_Options.Visuals.Skeleton)
					Skeleton(entity, clr);
				if (g_Options.Visuals.health)
					DrawHealth(rect, entity);
				if (g_Options.Visuals.barrel)
					BulletTrace(entity, clr);

				switch (g_Options.Visuals.BoxType)
				{
				case 0:
					break;
				case 1:
					DrawBox(rect, clr);
					break;
				case 2:
					DrawCorners(rect, clr);
					break;
				case 3:
					ThreeDBox(entity->GetCollideable()->OBBMins(), entity->GetCollideable()->OBBMaxs(), entity->GetOrigin(), clr);
					break;
				}
			}
		}


	}
}

void visuals::DrawBox(RECT rect, Color Col)
{
	g_Render->DrawOutlinedRect(rect.left - 1, rect.top - 1, rect.right - rect.left + 2, rect.bottom - rect.top + 2, Col);
	g_Render->DrawOutlinedRect(rect.left + 1, rect.top + 1, rect.right - rect.left - 2, rect.bottom - rect.top - 2, Col);
	g_Render->DrawOutlinedRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, Col);
}

void visuals::DrawCorners(RECT rect, Color Col)
{
	int x1, y1, x2, y2, w, h;
	x1 = rect.left;
	y1 = rect.top;
	x2 = rect.right;
	y2 = rect.bottom;

	w = x2;
	h = y2;

	int Line_Size = (y1 - h) / 6;
	int Line_Size2 = (y1 - h) / 6;

	int red = 0;
	int green = 0;
	int blue = 0;
	int alpha = 0;
	Col.GetColor(red, green, blue, alpha);
	g_Surface->DrawSetColor(red, green, blue, alpha);

	//top inwards
	g_Surface->DrawLine(w, y1, w + Line_Size, y1);
	g_Surface->DrawLine(x1, y1, x1 - Line_Size, y1);

	//top downwards
	g_Surface->DrawLine(x1, y1, x1, y1 - Line_Size);
	g_Surface->DrawLine(w, y1, w, y1 - Line_Size);

	//bottom inwards
	g_Surface->DrawLine(x1, h, x1 - Line_Size, h);
	g_Surface->DrawLine(w, h, w + Line_Size, h);

	//bottom upwards
	g_Surface->DrawLine(x1, h, x1, h + Line_Size);
	g_Surface->DrawLine(w, h, w, h + Line_Size);

	//outlines

	g_Surface->DrawSetColor(red, green, blue, alpha);

	//top inwards
	g_Surface->DrawLine(w, y1 - 1, w + Line_Size, y1 - 1);
	g_Surface->DrawLine(x1, y1 - 1, x1 - Line_Size, y1 - 1);
	//inlines
	g_Surface->DrawLine(w - 1, y1 + 1, w + Line_Size, y1 + 1);
	g_Surface->DrawLine(x1 + 1, y1 + 1, x1 - Line_Size, y1 + 1);

	// top downwards
	g_Surface->DrawLine(x1 - 1, y1 - 1, x1 - 1, y1 - Line_Size);
	g_Surface->DrawLine(w + 1, y1 - 1, w + 1, y1 - Line_Size);
	//inlines
	g_Surface->DrawLine(x1 + 1, y1, x1 + 1, y1 - Line_Size);
	g_Surface->DrawLine(w - 1, y1, w - 1, y1 - Line_Size);

	//bottom inwards
	g_Surface->DrawLine(x1, h + 1, x1 - Line_Size, h + 1);
	g_Surface->DrawLine(w, h + 1, w + Line_Size, h + 1);
	//inlines
	g_Surface->DrawLine(x1 + 1, h - 1, x1 - Line_Size, h - 1);
	g_Surface->DrawLine(w - 1, h - 1, w + Line_Size, h - 1);

	//bottom upwards
	g_Surface->DrawLine(x1 - 1, h + 1, x1 - 1, h + Line_Size);
	g_Surface->DrawLine(w + 1, h + 1, w + 1, h + Line_Size);
	//inlines
	g_Surface->DrawLine(x1 + 1, h, x1 + 1, h + Line_Size);
	g_Surface->DrawLine(w - 1, h, w - 1, h + Line_Size);
}

void visuals::ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col)
{
	Vector min = minin + pos;
	Vector max = maxin + pos;

	Vector corners[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(min.x, min.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(max.x, max.y, max.z),
		Vector(max.x, min.y, max.z) };


	int edges[12][2] = { { 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }, };

	for (const auto edge : edges)
	{
		Vector p1, p2;
		if (!g_Render->WorldToScreen(corners[edge[0]], p1) || !g_Render->WorldToScreen(corners[edge[1]], p2))
			return;
		int red = 0;
		int green = 0;
		int blue = 0;
		int alpha = 0;
		Col.GetColor(red, green, blue, alpha);
		g_Surface->DrawSetColor(red, green, blue, alpha);
		g_Surface->DrawLine(p1.x, p1.y, p2.x, p2.y);
	}
}

void visuals::DrawInfo(RECT rect, C_BaseEntity* pPlayer, C_BaseEntity* local)
{
	player_info_t info;

	static class Text
	{
	public:
		std::string text;
		int side;
		int Font;
		Color color;

		Text(std::string text, int side, int Font, Color color) : text(text), side(side), Font(Font), color(color)
		{
		}
	};
	std::vector< Text > texts;

	if (pPlayer->GetClientClass()->m_ClassID == 83 || pPlayer->GetClientClass()->m_ClassID == 84)
	{
		if (g_Options.Visuals.Name)
			texts.push_back(Text("Hostage", 0, g_Render->font.ESP, Color(255, 255, 255, 255)));
	}
	else if (g_Engine->GetPlayerInfo(pPlayer->GetIndex(), &info))
	{
		//if (g_Options.Visuals.Info && g_PlayerResource->GetPlayerC4() == pPlayer->GetIndex())
		//	texts.push_back(Text("Bomb Carrier", 0, g_Render->font.ESP, Color(255, 100, 100, 255)));

		if (g_Options.Visuals.Info && pPlayer->IsFlashed())
			texts.push_back(Text("Flashed", 1, g_Render->font.ESP, Color(255, 255, 183, 255)));
		if (g_Options.Visuals.Name)
			texts.push_back(Text(info.name, 0, g_Render->font.ESP, Color(255, 255, 255, 255)));

		if (g_Options.Visuals.Distance)
			texts.push_back(Text(std::to_string(flGetDistance(local->GetOrigin(), pPlayer->GetOrigin())) + std::string("FT"), 2, g_Render->font.ESPMini, Color(255, 255, 255, 255)));
		if (g_Options.Visuals.Callout)
			texts.push_back(Text(pPlayer->GetCallout(), 1, g_Render->font.ESP, Color(255, 220, 220, 255)));

		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pPlayer->GetActiveWeaponHandle());
		if (weapon)
		{
			if (g_Options.Visuals.Weapon == 1 && weapon)
				texts.push_back(Text(weapon->GetGunName() + std::string("[") + std::to_string(weapon->ammo()) + std::string("|") + std::to_string(weapon->ammo2()) + std::string("]"), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));
			else if (g_Options.Visuals.Weapon == 2 && weapon)
			{
				ItemDefinitionIndexx iWeap = (ItemDefinitionIndexx)weapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
				char buffer[12];
				sprintf(buffer, "%s", GetIcon(iWeap));
				texts.push_back(Text(buffer, 2, g_Render->font.Icon, Color(255, 255, 255, 240)));
				texts.push_back(Text(std::string("[") + std::to_string(weapon->ammo()) + std::string("|") + std::to_string(weapon->ammo2()) + std::string("]"), 2, g_Render->font.ESP, Color(255, 255, 255, 255)));

			}

			if (g_Options.Visuals.Info && pPlayer->IsScoped())
				texts.push_back(Text("Scoped", 1, g_Render->font.ESP, Color(121, 252, 250, 255)));
			if (g_Options.Visuals.Info)
				texts.push_back(Text(pPlayer->GetArmorName(), 1, g_Render->font.ESP, Color(255, 255, 255, 255)));
			if (g_Options.Visuals.Money)
				texts.push_back(Text(std::string("$") + std::to_string(pPlayer->iAccount()), 1, g_Render->font.ESP, Color(71, 132, 60, 255)));


			float HealthValue = pPlayer->GetHealth();

			float HealthValue2 = pPlayer->GetHealth();
			if (HealthValue2 > 100)
				HealthValue2 = 100;

			char hp[256];
			sprintf(hp, "%.0f", HealthValue);
			int Red = 255 - (HealthValue2 * 2.55);
			int Green = HealthValue2 * 2.55;
			std::stringstream hptext;
			hptext << "HP: " << hp;
			switch (g_Options.Visuals.health)
			{
			case 2:
				texts.push_back(Text(hptext.str(), 1, g_Render->font.ESP, Color(Red, Green, 0, 255)));
				break;
			case 3:
				texts.push_back(Text(hptext.str(), 1, g_Render->font.ESP, Color(Red, Green, 0, 255)));
				break;
			}
		}
	}

	int middle = ((rect.right - rect.left) / 2) + rect.left;
	int Top[3] = { rect.top,rect.top, rect.bottom };
	for (auto text : texts)
	{
		RECT nameSize = g_Render->GetTextSize(text.Font, (char*)text.text.c_str());
		switch (text.side)
		{
		case 0:
			Top[0] -= nameSize.bottom + 1;
			g_Render->DrawString2(text.Font, middle, Top[0] + 6, text.color, FONT_CENTER, (char*)text.text.c_str());
			break;
		case 1:
			g_Render->DrawString2(text.Font, rect.right + 2, Top[1] + 6, text.color, FONT_LEFT, (char*)text.text.c_str());
			Top[1] += nameSize.bottom - 4;
			break;
		case 2:
			g_Render->DrawString2(text.Font, middle, Top[2] + 6, text.color, FONT_CENTER, (char*)text.text.c_str());
			Top[2] += nameSize.bottom - 4;
			break;
		}
	}
}

void visuals::DrawAngles()
{
	C_BaseEntity *pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = pLocal;

	/*AngleVectors(QAngle(0, pLocal->GetLowerBodyYaw(), 0), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 35.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
	return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 165, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(255, 165.f, 0, 255), g_Render->font.ESP, "lby");*/

	AngleVectors(QAngle(Globals::RealAngle), &forward);
	src3D = pLocal->GetOrigin();
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(0, 255, 0, 255), g_Render->font.ESP, "REAL");

	AngleVectors(QAngle(Globals::FakeAngle), &forward);
	dst3D = src3D + (forward * 45.f);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;

	g_Render->Line(src.x, src.y, dst.x, dst.y, Color(255, 0, 0, 255));
	g_Render->Text(dst.x, dst.y, Color(255, 0, 0, 255), g_Render->font.ESP, "FAKE");
}

RECT visuals::DynamicBox(C_BaseEntity* pPlayer, bool& PVS, C_BaseEntity* local)
{
	Vector trans = pPlayer->GetOrigin();

	Vector min;
	Vector max;

	min = pPlayer->GetCollideable()->OBBMins();
	max = pPlayer->GetCollideable()->OBBMaxs();

	Vector pointList[] = {
		Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z)
	};

	Vector Distance = pointList[0] - pointList[1];
	int dst = Distance.Length();
	dst /= 1.3f;
	Vector angs;
	CalcAngle(trans, local->GetEyePosition(), angs);

	Vector all[8];
	angs.y += 45;
	for (int i = 0; i < 4; i++)
	{
		AngleVectors(angs, &all[i]);
		all[i] *= dst;
		all[i + 4] = all[i];
		all[i].z = max.z;
		all[i + 4].z = min.z;
		VectorAdd(all[i], trans, all[i]);
		VectorAdd(all[i + 4], trans, all[i + 4]);
		angs.y += 90;
	}

	Vector flb, brt, blb, frt, frb, brb, blt, flt;
	PVS = true;

	if (!g_DebugOverlay->ScreenPosition(all[3], flb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[0], blb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[2], frb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[6], blt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[5], brt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[4], frt))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[1], brb))
		PVS = false;
	if (!g_DebugOverlay->ScreenPosition(all[7], flt))
		PVS = false;

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
	}
	RECT rect;
	rect.left = left;
	rect.bottom = bottom;
	rect.right = right;
	rect.top = top;
	return rect;
}

void visuals::BulletTrace(C_BaseEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	Vector eyes = *pEntity->GetEyeAngles();

	AngleVectors(eyes, &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(6) - Vector(0, 0, 0);
	dst3D = src3D + (forward * g_Options.Visuals.barrelL);

	ray.Init(src3D, dst3D);

	g_EngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!g_Render->WorldToScreen(src3D, src) || !g_Render->WorldToScreen(tr.endpos, dst))
		return;


	g_Render->Line(src.x, src.y, dst.x, dst.y, color);
	g_Render->DrawOutlinedRect(dst.x - 3, dst.y - 3, 6, 6, color);
};

void visuals::Skeleton(C_BaseEntity* pEntity, Color Col)
{

	studiohdr_t* pStudioHdr = g_ModelInfo->GetStudiomodel(pEntity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);
			g_DebugOverlay->ScreenPosition(vParent, sParent);
			g_DebugOverlay->ScreenPosition(vChild, sChild);
			g_Render->Line(sParent[0], sParent[1], sChild[0], sChild[1], Col);
		}
	}

}

void visuals::DrawSnapLine(Vector to, Color clr)
{
	int width = 0;
	int height = 0;
	g_Engine->GetScreenSize(width, height);
	Vector From((width / 2), height - 1, 0);
	g_Render->Line(From.x, From.y, to.x, to.y, clr);
}

void visuals::DrawHealth(RECT rect, C_BaseEntity* pPlayer)
{
	float HealthValue = pPlayer->GetHealth();

	float HealthValue2 = pPlayer->GetHealth();
	if (HealthValue2 > 100)
		HealthValue2 = 100;

	char hp[256];
	sprintf(hp, "%.0f", HealthValue);
	int Red = 255 - (HealthValue2 * 2.55);
	int Green = HealthValue2 * 2.55;
	float height = (rect.bottom - rect.top) * (HealthValue2 / 100);
	// bar text both
	switch (g_Options.Visuals.health)
	{
	case 1:
		g_Render->GradientH(rect.left - 5, rect.bottom - height, 2, height, Color(Red, Green, 0, 255), Color(Red, Green, 0, 255));
		break;
	case 2:
		break;
	case 3:
		g_Render->GradientH(rect.left - 5, rect.bottom - height, 2, height, Color(Red, Green, 0, 255), Color(Red, Green, 0, 255));
		break;
	}
}


void visuals::Hitmarker()
{
	if (G::hitmarkeralpha < 0.f)
		G::hitmarkeralpha = 0.f;
	else if (G::hitmarkeralpha > 0.f)
		G::hitmarkeralpha -= 0.01f;

	int W, H;
	g_Engine->GetScreenSize(W, H);

	if (G::hitmarkeralpha > 0.f)
	{
		g_Render->Line(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, Color(240, 240, 240, (G::hitmarkeralpha * 255.f)));

	}
}

void visuals::Crosshair(C_BaseEntity* local)
{
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
			Color color(255, 255, 255, 200);

			if (g_Render->WorldToScreen(end, endScreen))
			{
				g_Render->OutlineCircle(endScreen.x, endScreen.y, (int)size, 100, color);
			}
		}
	}
}

void visuals::DLight(C_BaseEntity *local, C_BaseEntity* entity)
{
	player_info_t pinfo;
	if (local && entity && entity != local)
	{
		if (g_Engine->GetPlayerInfo(entity->GetIndex(), &pinfo) && entity->IsAlive() && !entity->IsDormant())
		{
			if (local->GetTeamNum() != entity->GetTeamNum())
			{
				dlight_t* pElight = g_Dlight->CL_AllocElight(entity->GetIndex());
				pElight->origin = entity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
				pElight->radius = 300.0f;
				pElight->color.b = 36;
				pElight->color.g = 224;
				pElight->color.r = 41;
				pElight->die = g_Globals->curtime + 0.05f;
				pElight->decay = pElight->radius / 5.0f;
				pElight->key = entity->GetIndex();

				dlight_t* pDlight = g_Dlight->CL_AllocDlight(entity->GetIndex());
				pDlight->origin = entity->GetOrigin();
				pDlight->radius = 300.0f;
				pDlight->color.b = 36;
				pDlight->color.g = 224;
				pDlight->color.r = 41;
				pDlight->die = g_Globals->curtime + 0.05f;
				pDlight->decay = pDlight->radius; // / 5.0f;
				pDlight->key = entity->GetIndex();
			}
		}
	}
}

void visuals::NightMode()
{
	if (g_Options.Misc.nightMode)
	{
		if (!done)
		{


			static auto sv_skyname = g_CVar->FindVar("sv_skyname");
			static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
			r_DrawSpecificStaticProp->SetValue(1);
			sv_skyname->SetValue("sky_csgo_night02");

			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(0.10, 0.10, 0.10);
				}
				if (strstr(group, "StaticProp"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{
					pMaterial->ColorModulate(0.30, 0.30, 0.30);
				}

				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
				}
				done = true;
			}

		}
	}
	else
	{
		if (done)
		{
			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "StaticProp"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(name, "models/props/de_dust/palace_bigdome"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
				if (strstr(name, "models/props/de_dust/palace_pillars"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
				if (strstr(group, "Particle textures"))
				{
					pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
			}
			done = false;
		}
	}

	if (g_Options.Misc.colored_walls)
	{
		if (!done1)
		{

			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{
					pMaterial->ColorModulate(g_Options.Colors.ColWalls[0], g_Options.Colors.ColWalls[1], g_Options.Colors.ColWalls[2]);
				}
				done1 = true;
			}

		}
	}
	else
	{
		if (done1)
		{
			for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
			{
				IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

				if (!pMaterial)
					continue;

				const char* group = pMaterial->GetTextureGroupName();
				const char* name = pMaterial->GetName();

				if (strstr(group, "World textures"))
				{

					pMaterial->ColorModulate(1, 1, 1);
				}
			}
			done1 = false;
		}
	}
}
