#pragma once
#include "HookIncludes.h"
#include "Renderer.h"
#include "Interface.h"
#include "Parser.h"
#include "Variables.h"
#include <d3d9.h>
uint8_t* m_present;
uint8_t* m_reset;

IDirect3DDevice9* d3d9_device;
typedef HRESULT(__stdcall *EndScene_t) (IDirect3DDevice9*);

typedef HRESULT(__stdcall *Reset_t) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

EndScene_t	oEndScene = nullptr;
Reset_t		oReset = nullptr;

std::list<int> GetObservervators(int playerId)
{
	std::list<int> list;

	if (!g_Engine->IsInGame())
		return list;

	C_BaseEntity* player = g_EntityList->GetClientEntity(playerId);
	if (!player)
		return list;

	if (!player->IsAlive())
	{
		C_BaseEntity* observerTarget = g_EntityList->GetClientEntityFromHandle(player->GetObserverTargetHandle());
		if (!observerTarget)
			return list;

		player = observerTarget;
	}

	for (int i = 1; i < g_Engine->GetMaxClients(); i++)
	{
		C_BaseEntity* pPlayer = g_EntityList->GetClientEntity(i);
		if (!pPlayer)
			continue;

		if (pPlayer->IsDormant() || pPlayer->IsAlive())
			continue;

		C_BaseEntity* target = g_EntityList->GetClientEntityFromHandle(pPlayer->GetObserverTargetHandle());
		if (player != target)
			continue;

		list.push_back(i);
	}

	return list;
}

Vector2D RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians = false)
{
	float r_1, r_2;
	float x_1, y_1;

	r_1 = -(EntityPos.y - LocalPlayerPos.y);
	r_2 = EntityPos.x - LocalPlayerPos.x;
	float Yaw = angle - 90.0f;

	float yawToRadian = Yaw * (float)(M_PI / 180.0F);
	x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
	y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

	*viewCheck = y_1 < 0;

	x_1 *= zoom;
	y_1 *= zoom;

	int sizX = sizeX / 2;
	int sizY = sizeY / 2;

	x_1 += sizX;
	y_1 += sizY;

	if (x_1 < 5)
		x_1 = 5;

	if (x_1 > sizeX - 5)
		x_1 = sizeX - 5;

	if (y_1 < 5)
		y_1 = 5;

	if (y_1 > sizeY - 5)
		y_1 = sizeY - 5;


	x_1 += posX;
	y_1 += posY;


	return Vector2D(x_1, y_1);

}

void SpecList()
{
	if (ImGui::Begin(XorStr("Spectators"), &g_Options.Visuals.speclist, ImVec2(300, 300), 0.2f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders))
	{
		//ImGui::Separator();

		for (int playerId : GetObservervators(g_Engine->GetLocalPlayer()))
		{
			C_BaseEntity* player = g_EntityList->GetClientEntity(playerId);

			player_info_t entityInformation;
			g_Engine->GetPlayerInfo(playerId, &entityInformation);

			//ImGuiStyle& style = ImGui::GetStyle();
			//style.Colors[ImGuiCol_Text] = player->GetTeam() == 2 ? ImVec4(240, 70, 80, 255) : ImVec4(0, 0, 170, 255);

			char* bufix;
			switch (player->GetTeamNum())
			{
			case 0:
				bufix = XorStr("[NO TEAM]");
				//col = { 100,100,100 };
				break;
			case 1:
				bufix = XorStr("[SPEC]");
				break;
			case 2:
				bufix = XorStr("[T]");
				break;
			case 3:
				bufix = XorStr("[CT]");
				break;
			}

			char buf[255]; sprintf_s(buf, u8"%s | %s", entityInformation.name, bufix);
			//D::DrawString(F::ESP, scrn.right - TextSize.right - 4, (scrn.bottom / 2) + (16 * ayy) + 9, pTarget->GetIndex() == G::LocalPlayer->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 255), FONT_LEFT, buf);
			ImGui::TextColored(ImVec4(1, 1, 1, 1),XorStr(u8"%s"), buf);
		}

		//ImGui::Separator();

		ImGui::End();
	}
}

int ParseSkins()
{

	return 0;
}

HRESULT __stdcall hkReset(IDirect3DDevice9* thisptr, D3DPRESENT_PARAMETERS* params) {

	try {
		if (!renderer->IsReady())
			return oReset(thisptr, params);

		RenderDefault(thisptr);
		ImGui_ImplDX9_InvalidateDeviceObjects();
		renderer->~Renderer();

		HRESULT result = oReset(thisptr, params);

		ImGui_ImplDX9_CreateDeviceObjects();

		renderer->Initialize(FindWindowA("Valve001", NULL), thisptr);
		g_Render->SetupFonts();
		return result;
	}
	catch (std::exception(Reset))
	{
		char buf[100];
		sprintf(buf, "Reset: %s", Reset.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}

typedef HRESULT(_stdcall *Present_T)(void*, const RECT*, RECT*, HWND, RGNDATA*);
Present_T oPresent;
HRESULT _stdcall hkPresent(LPDIRECT3DDEVICE9 pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
	try {
		if (!renderer->IsReady())
		{
			ParseSkins();
			RenderDefault(pDevice);
			renderer->Initialize(FindWindowA("Valve001", NULL), pDevice);
		}
		static bool mouse_enabled = false;
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
		bool is_renderer_active = renderer->IsActive();

		if (is_renderer_active) {
			if (mouse_enabled) {
				g_Engine->ClientCmd_Unrestricted("cl_mouseenable 0");
				mouse_enabled = false;
				G::menuon = true;
				g_Options.Menu.iTab = 0;
			}
		}
		else {
			if (!mouse_enabled) {
				g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
				mouse_enabled = true;
				G::menuon = false;
			}
		}

		ImGui::GetIO().MouseDrawCursor = is_renderer_active;

		ImGui_ImplDX9_NewFrame();

		if (is_renderer_active)
		{
			RenderInterface();
		}
		if (g_Options.Visuals.EventLog)
		{
			purchase.on_fire_event(nullptr, true);
		}

		auto pstyle = ImGui::GetStyle();
		static bool once;

		if (g_Options.Visuals.speclist && g_Engine->IsInGame())
		{
			if (!once) {
				ImGui::SetNextWindowPos(ImVec2(5, 269));
				ImGui::SetNextWindowSize(ImVec2(119, 186));
				once = true;
			}
			SpecList();
		}

		ImGui::Render();

		return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}
	catch (std::exception(Pre))
	{
		char buf[100];
		sprintf(buf, "Preset: %s", Pre.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}
