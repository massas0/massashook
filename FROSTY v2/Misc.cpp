#include "Misc.h"
#include "Interfaces.h"
#include "Render.h"
#include <time.h>
#include "BaseClient.h"
#include "Strafer.h"
#include "CircleS.h"
#include "Chat.h"
#include <sstream>

std::string animatedclantag;
int iLastTime;
bool bDone = false;

void SetClanTag(const char* tag, const char* name)
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)U::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		and     esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}

void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = U::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0)
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (g_Engine->IsInGame() && g_Engine->IsConnected())
		NET_SetConVar(name, "name");
}

static void Walkbot(Vector& OrigAng, CInput::CUserCmd *cmd, C_BaseEntity *local)
{

	bool walkbotBefore = false;

	Vector viewangles;
	g_Engine->GetViewAngles(viewangles);

	static int OldMouseX = OrigAng.y;
	int mousedx = OldMouseX - OrigAng.y;

	auto fDistanceToWall = [&](Vector diff = Vector(0, 0, 0))->float {
		auto tmpviewangles = viewangles + diff;
		trace_t tr;
		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = local;
		Vector begin = local->GetEyePosition(), t, end;
		AngleVectors2(tmpviewangles, t);
		Normalize(t, end);
		end *= 8192.0f;
		end += begin;
		ray.Init(begin, end);
		g_EngineTrace->TraceRay(ray, 0x4600400B, &filter, &tr);
		return (begin - tr.endpos).Size();
	};

	static float old1, old2, old3;
	if (local->GetVelocity().Length() < 3)
	{
		viewangles.y += 2.0f;
	}
	float Distances = 180;
	if (fDistanceToWall() < Distances) // we are near to some wall
	{
		int turn = 35;
		float negativeDist = fDistanceToWall(Vector(0, -1, 0)), positiveDist = fDistanceToWall(Vector(0, 1, 0));
		if (abs(negativeDist - positiveDist) < 1.0f)
		{
			viewangles.y += turn;
		}
		else
		{
			viewangles.y += positiveDist < negativeDist ? -1 : 1;
		}
	}


	if (abs(viewangles.x) > 0)
		viewangles.x = 1;

	while (viewangles.y > 180.0f)
		viewangles.y -= 360.0f;
	while (viewangles.y < -180.0f)
		viewangles.y += 360.0f;

	g_Engine->SetViewAngles(viewangles);

	if (!walkbotBefore)
	{
		cmd->forwardmove = 450.f;
		walkbotBefore = true;
	}

	else if (walkbotBefore)
	{
		walkbotBefore = false;
		cmd->forwardmove = 450.f;
	}
}

void RMov(CInput::CUserCmd *pCmd, float rotation)
{
	rotation = (float)(DEG2RAD(rotation));

	float cos_rot = cos(rotation);
	float sin_rot = sin(rotation);

	float new_forwardmove = (cos_rot * pCmd->forwardmove) - (sin_rot * pCmd->sidemove);
	float new_sidemove = (sin_rot * pCmd->forwardmove) + (cos_rot * pCmd->sidemove);

	pCmd->forwardmove = new_forwardmove;
	pCmd->sidemove = new_sidemove;
}

void misc::OnCreateMove(CInput::CUserCmd *cmd, C_BaseEntity *local, bool& bSendPacket)
{

	Vector OrigAng = cmd->viewangles;
	OrigAng.Normalized();

	if (g_Options.Misc.Bhop && local->IsAlive())
	{
			if (cmd->buttons & IN_JUMP && !(local->GetMoveType() & MOVETYPE_LADDER))
			{

				int iFlags = local->GetFlags();
				if (!(iFlags & FL_ONGROUND))
					cmd->buttons &= ~IN_JUMP;
			}
		
	}

	if (g_Options.Misc.NoName)
	{
		setclantag(XorStr("\r"));
	}

	if (g_Options.Misc.legitaa)
	{
		QAngle oldAngle = cmd->viewangles;
		float oldForward = cmd->forwardmove;
		float oldSideMove = cmd->sidemove;

		if (local->GetMoveType() == MOVETYPE_LADDER)
		return;
		if (cmd->buttons & IN_ATTACK)
			return;
	
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
		if (pWeapon) {
			CCSGrenade* csGrenade = (CCSGrenade*)pWeapon;

			if (csGrenade->GetThrowTime() > 0.f)
				return;
		}

		static int ChokedPackets = -1;
		ChokedPackets++;
		static bool yFlip;
		if (ChokedPackets < 1)
		{
			bSendPacket = true;
		}
		else
		{
			bSendPacket = false;
			yFlip ? cmd->viewangles.y += 90 : cmd->viewangles.y -= 90;
			ChokedPackets = -1;
		}
		yFlip != yFlip;
	}

	if (g_Options.Misc.memewalk) {
		if (local->GetMoveType() == MOVETYPE_LADDER)
			return;

		if (cmd->forwardmove > 0)
		{
			cmd->buttons |= IN_BACK;
			cmd->buttons &= ~IN_FORWARD;
		}

		if (cmd->forwardmove < 0)
		{
			cmd->buttons |= IN_FORWARD;
			cmd->buttons &= ~IN_BACK;
		}

		if (cmd->sidemove < 0)
		{
			cmd->buttons |= IN_MOVERIGHT;
			cmd->buttons &= ~IN_MOVELEFT;
		}

		if (cmd->sidemove > 0)
		{
			cmd->buttons |= IN_MOVELEFT;
			cmd->buttons &= ~IN_MOVERIGHT;

		}
	}

	if (g_Options.Misc.AirStuckKey)
	{
		if (cmd->buttons & IN_ATTACK)
			return;
		
		static bool air;
		if (GetAsyncKeyState(g_Options.Misc.AirStuckKey) & 0x1)
			air = !air;
		if (air)
			cmd->tick_count = 0xFFFFFFFFFFFFFFFF;
	}

	

	if (g_Options.Misc.CircleSKey)
	{
		float randompastedint;
		Vector StoredViewAngles = cmd->viewangles;
		int GetItDoubled = g_Options.Misc.retrack * randompastedint - g_Globals->interval_per_tick;

		if (GetAsyncKeyState(g_Options.Misc.CircleSKey))
		{
			cmd->viewangles = StoredViewAngles;
			RMov(cmd, GetItDoubled);

		}
		else
		{
			float randompastedint = 0;
		}

	}


	if (g_Options.Misc.ChatSpam)
	{

		static float nextTime = 0.f;
		float flServerTime = local->GetTickBase() * g_Globals->interval_per_tick;

		if (nextTime > flServerTime)
			return;

		nextTime = flServerTime + 0.5f;

		if (!g_Options.Misc.ChatSpam)
			return;

		char str[512];
		if (g_Options.Misc.ChatSpam == 1)
			sprintf(str, "say %s", chat_bot::chat_spam_eng[rand() % 17]);
		else if (g_Options.Misc.ChatSpam == 2)
			sprintf(str, "say %s", chat_bot::chat_spam_pt[rand() % 15]);

		if (g_Engine->IsConnected())
			g_Engine->ClientCmd_Unrestricted(str);
	}

	if (g_Options.Misc.NameChangerFix)
	{
		static bool wasSpamming = true;

		if (g_Options.Misc.NameChangerFix == 1)
		{
			static bool useSpace = true;
			if (useSpace)
			{
				change_name("www.massas.pw-");

			}
			else
			{
				change_name("-www.massas.pw");

			}
			useSpace != useSpace;
		
		}
		else if(g_Options.Misc.NameChangerFix == 2)
		{
			char MEME[4500] = "\xe1\xab\xa8\AIMWARE.net";
			char MEME2[4500] = "\xe1\xab\xa5\AIMWARE.net";
			if (wasSpamming)
			{
				change_name(MEME);
			}
			else
			{
				change_name(MEME2);
			}
			wasSpamming != wasSpamming;
		}
	}

	if (g_Options.Misc.ClantagChanger)
	{
		static int counter = 0;
		static int motion = 0;
		int ServerTime = g_Globals->curtime * (2.5);

		if (g_Options.Misc.ClantagChanger == 1)
		{
			if (counter % 48 == 0)
				motion++;
			int value = ServerTime % 14;
			switch (value) {
			case 0:SetClanTag("massas.pw    ", "http://massas.pw"); break;
			case 1:SetClanTag("assas.pw    m", "http://massas.pw"); break;
			case 2:SetClanTag("ssas.pw    ma", "http://massas.pw"); break;
			case 3:SetClanTag("sas.pw    mas", "http://massas.pw"); break;
			case 4:SetClanTag("as.pw    mass", "http://massas.pw"); break;
			case 5:SetClanTag("s.pw    massa", "http://massas.pw"); break;
			case 6:SetClanTag(".pw    massas", "http://massas.pw"); break;
			case 7:SetClanTag("pw    massas.", "http://massas.pw"); break;
			case 8:SetClanTag("w    massas.p", "http://massas.pw"); break;
			case 9:SetClanTag("    massas.pw", "http://massas.pw"); break;
			case 10:SetClanTag("  massas.pw ", "http://massas.pw"); break;
			case 11:SetClanTag("  massas.pw ", "http://massas.pw"); break;
			case 12:SetClanTag(" massas.pw  ", "http://massas.pw"); break;
			case 13:SetClanTag(" massas.pw  ", "http://massas.pw"); break;
			case 14:SetClanTag("massas.pw  ", "http://massas.pw"); break;

			}
			counter++;
		}
		if (g_Options.Misc.ClantagChanger == 2)
		{
			if (counter % 48 == 0)
				motion++;
			int value = ServerTime % 5;
			switch (value) {
			case 0:SetClanTag("www.massas.pw \r", "http://massas.pw"); break;
			case 1:SetClanTag("", "http://massas.pw"); break;
			case 2:SetClanTag("www.massas.pw \r", "http://massas.pw"); break;
			case 3:SetClanTag("", "http://massas.pw"); break;
			case 4:SetClanTag("www.massas.pw \r", "http://massas.pw"); break;
			case 5:SetClanTag("", "http://massas.pw"); break;

			}
		}
		if (g_Options.Misc.ClantagChanger == 3)
		{
			if (counter % 48 == 0)
				motion++;
			int value = ServerTime % 5;
			switch (value) {
			case 0:SetClanTag("$ massas.pw \r", "http://massas.pw"); break;
			case 1:SetClanTag("$ \r", "http://massas.pw"); break;
			case 2:SetClanTag("$ massas.pw \r", "http://massas.pw"); break;
			case 3:SetClanTag("$ \r", "http://massas.pw"); break;
			case 4:SetClanTag("$ massas.pw \r", "http://massas.pw"); break;
			case 5:SetClanTag("$ \r", "http://massas.pw"); break;

			}
		}
		if (g_Options.Misc.ClantagChanger == 4)
		{
			if (counter % 48 == 0)
				motion++;
			int value = ServerTime % 2;
			switch (value) {
			case 0:SetClanTag(g_Options.Misc.CustomTag, "http://massas.pw"); break;
			case 1:SetClanTag(g_Options.Misc.CustomTag, "http://massas.pw"); break;
			case 2:SetClanTag(g_Options.Misc.CustomTag, "http://massas.pw"); break;
			}
		}
	}

	if (!g_Options.Misc.animatedclantag && animatedclantag.c_str() != G::AnimatedClantag)
	{
		animatedclantag = G::AnimatedClantag;
	}
	if (g_Options.Misc.animatedclantag && animatedclantag.length() > 1)
	{
		if (int(g_Globals->curtime) != iLastTime)
		{
			auto length = animatedclantag.length();
			animatedclantag.insert(0, 1, animatedclantag[length - 2]);
			animatedclantag.erase(length - 1, 1);

			setclantag(animatedclantag.c_str());
			iLastTime = int(g_Globals->curtime);
		}
	}
	if (g_Options.Misc.namespam)
	{
		std::stringstream ss;

		for (auto i = 0; i <= 150; i++)
		{
			ss << static_cast<char>(-1);
		}

		auto cv = g_CVar->FindVar("name");
		*reinterpret_cast<int*>(reinterpret_cast<DWORD>(&cv->fnChangeCallback) + 0xC) = NULL;
		cv->SetValue(ss.str().c_str());
		return;
	}

	if (g_Options.Misc.silentname)
	{
		SetName("\n\xAD\xAD\xAD");
	}

	if (g_Options.Misc.silentstealer)
	{
		bool bDidMeme = false;
		int iNameIndex = -1;
		char chName[130];
		static ConVar* name = g_CVar->FindVar("name");
		{
			for (int iPlayerIndex = 0; iPlayerIndex < g_Engine->GetMaxClients(); iPlayerIndex++)
			{
				C_BaseEntity *pEntity = g_EntityList->GetClientEntity(iPlayerIndex);
				if (!pEntity || pEntity == local || iPlayerIndex == g_Engine->GetLocalPlayer())
					continue;
				if (rand() % 3 == 1)
				{
					iNameIndex = pEntity->GetIndex();
					bDidMeme = true;
				}
			}
			if (bDidMeme)
			{
				player_info_t pInfo;
				g_Engine->GetPlayerInfo(iNameIndex, &pInfo);
				sprintf(chName, "%s ", pInfo.name);
				name->SetValue(chName);
			}
		}

	}
}

static vec_t Normalize_y(vec_t ang)
{
	while (ang < -180.0f)
		ang += 360.0f;
	while (ang > 180.0f)
		ang -= 360.0f;
	return ang;
}

void misc::AutoStrafe(CInput::CUserCmd *cmd, C_BaseEntity *local, QAngle oldangles)
{
	static AutoStrafer Strafer;

	static float move = 450;
	float s_move = move * 0.5065f;
	if (local->GetMoveType() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER))
		return;
	if (cmd->buttons & (IN_FORWARD | IN_MOVERIGHT | IN_MOVELEFT | IN_BACK))
		return;

	if (GetAsyncKeyState(VK_SPACE) && !(local->GetFlags() & FL_ONGROUND) && !CStrafe::IsActive)
	{
		if (g_Options.Misc.AutoStrafe == 1) {

			if (local->GetVelocity().Length2D() == 0 && (cmd->forwardmove == 0 && cmd->sidemove == 0))
			{
				cmd->forwardmove = 450.f;
			}
			else if (cmd->forwardmove == 0 && cmd->sidemove == 0)
			{
				if (cmd->mousedx > 0 || cmd->mousedx > -0)
				{
					cmd->sidemove = cmd->mousedx < 0.f ? -450.f : 450.f;
				}
				else
				{
					auto airaccel = g_CVar->FindVar("sv_airaccelerate");
					auto maxspeed = g_CVar->FindVar("sv_maxspeed");

					static int zhop = 0;
					double yawrad = Normalize_y(oldangles.y) * PI / 180;

					float speed = maxspeed->GetFloat();
					if (cmd->buttons & IN_DUCK)
						speed *= 0.333;

					double tau = g_Globals->interval_per_tick, MA = speed * airaccel->GetFloat();

					int Sdir = 0, Fdir = 0;
					Vector velocity = local->GetVelocity();
					double vel[3] = { velocity[0], velocity[1], velocity[2] };
					double pos[2] = { 0, 0 };
					double dir[2] = { std::cos((oldangles[1] + 10 * zhop) * PI / 180), std::sin((oldangles[1] + 10 * zhop) * PI / 180) };
					oldangles.y = Normalize_y(yawrad * 180 / PI);
					Strafer.strafe_line_opt(yawrad, Sdir, Fdir, vel, pos, 30.0, tau, MA, pos, dir);
					cmd->sidemove = Sdir * 450;
				}
			}
		}
		else if (g_Options.Misc.AutoStrafe == 2)
		{
			if (cmd->mousedx > 1 || cmd->mousedx < -1) {
				cmd->sidemove = cmd->mousedx < 0.f ? -450.f : 450.f;
			}
			else {
				cmd->forwardmove = (1800.f * 4.f) / local->GetVelocity().Length2D();
				cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
				if (cmd->forwardmove > 450.f)
					cmd->forwardmove = 450.f;
			}
		}

	}
	movementfix(oldangles, cmd);
}
