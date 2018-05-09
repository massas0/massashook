#include "Resolver.h"

namespace Globals
{
	int Shots;
	int missedshots;
	QAngle RealAngle;
	QAngle FakeAngle;
	Vector AimPoint;
	bool shouldflip;
	bool walkbot;
	bool legitbotting;
	bool ySwitch;
	float NextTime;
	int resolvemode = 1;
	float fakeAngle;
	float OldSimulationTime[65];
	bool error;
	Vector pviewangles;
	bool HLTV;
	CInput::CUserCmd CMD;
}

float OldLowerBodyYaw[64];
float YawDelta[64];
float reset[64];
float Delta[64];
float Resolved_angles[64];
int iSmart;
static int jitter = -1;
float LatestLowerBodyYaw[64];
bool LbyUpdated[64];
float YawDifference[64];
float OldYawDifference[64];
float LatestLowerBodyYawUpdateTime[64];
float OldDeltaY;
float tolerance = 20.f;

Vector vecZero = Vector(0.0f, 0.0f, 0.0f);
QAngle angZero = QAngle(0.0f, 0.0f, 0.0f);

#define M_RADPI		57.295779513082f

QAngle pCalcAngle(Vector src, Vector dst)
{
	Vector delta = src - dst;
	if (delta == vecZero)
	{
		return angZero;
	}

	float len = delta.Length();

	if (delta.z == 0.0f && len == 0.0f)
		return angZero;

	if (delta.y == 0.0f && delta.x == 0.0f)
		return angZero;

	QAngle angles;
	angles.x = (asinf(delta.z / delta.Length()) * M_RADPI);
	angles.y = (atanf(delta.y / delta.x) * M_RADPI);
	angles.z = 0.0f;
	if (delta.x >= 0.0f) { angles.y += 180.0f; }



	return angles;
}

float pVec2Ang(Vector Velocity)
{
	if (Velocity.x == 0 || Velocity.y == 0)
		return 0;
	float rise = Velocity.x;
	float run = Velocity.y;
	float value = rise / run;
	float theta = atan(value);
	theta = RAD2DEG(theta) + 90;
	if (Velocity.y < 0 && Velocity.x > 0 || Velocity.y < 0 && Velocity.x < 0)
		theta *= -1;
	else
		theta = 180 - theta;
	return theta;
}
void pclamp(float &value)
{
	while (value > 180)
		value -= 360;
	while (value < -180)
		value += 360;
}
float pclamp2(float value)
{
	while (value > 180)
		value -= 360;
	while (value < -180)
		value += 360;
	return value;
}

struct BruteforceInfo
{
	enum BruteforceStep : unsigned int {
		BF_STEP_YAW_STANDING,
		BF_STEP_YAW_FAKEWALK,
		BF_STEP_YAW_AIR,
		BF_STEP_YAW_DUCKED,
		BF_STEP_YAW_PITCH,
		BF_STEP_COUNT,
	};

	unsigned char step[BF_STEP_COUNT];
	bool changeStep[BF_STEP_COUNT];
	bool missedBySpread;
	int missedCount;
	int spentBullets;
};

int GetEstimatedServerTickCount(float latency)
{
	return (int)floorf((float)((float)(latency) / (float)((uintptr_t)&g_Globals->interval_per_tick)) + 0.5) + 1 + (int)((uintptr_t)&g_Globals->tickcount);
}

inline float pRandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}

bool HasFakeHead(C_BaseEntity* pEntity)
{
	//lby should update if distance from lby to eye angles exceeds 35 degrees
	return abs(pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw()) > 35;
}

bool Lbywithin35(C_BaseEntity* pEntity) {
	//lby should update if distance from lby to eye angles less than 35 degrees
	return abs(pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw()) < 35;
}

bool IsMovingOnGround(C_BaseEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->GetVelocity().Length2D() > 40.f && FL_ONGROUND;
}

bool IsMovingOnInAir(C_BaseEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return !(pEntity->GetFlags() & FL_ONGROUND);
}

bool OnGround(C_BaseEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->GetFlags() & FL_ONGROUND;
}

bool IsFakeWalking(C_BaseEntity* pEntity) {
	//Check if a player is moving, but at below a velocity of 36
	return IsMovingOnGround(pEntity) && pEntity->GetVelocity().Length2D() < 36.0f;
}

float oldlowerbodyyaw;
float antilbytime;
float lbyupdatetime;
bool switchlol;

float GetLat()
{
	INetChannelInfo *nci = g_Engine->GetNetChannelInfo();
	if (nci)
	{

		float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
		return Latency;
	}
	else
	{

		return 0.0f;
	}
}

bool testlby()
{
	C_BaseEntity* LocalPlayer = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

	float flServerTime = (float)(LocalPlayer->GetTickBase()  * g_Globals->interval_per_tick);


	if (oldlowerbodyyaw != LocalPlayer->GetLowerBodyYaw())
	{

		lbyupdatetime++;
		oldlowerbodyyaw = LocalPlayer->GetLowerBodyYaw();
		switchlol = !switchlol;
		lbyupdatetime = flServerTime;
	}

	if ((lbyupdatetime + 0.3 - (GetLat() * 2) < flServerTime) && (LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (lbyupdatetime + (0.3) - (GetLat() * 2) < flServerTime)
		{
			lbyupdatetime += (0.3);
		}
		return true;
	}
	return false;
}

static bool memerino = false;
void ResoSetup::Resolve(C_BaseEntity* pEntity, int CurrentTarget) // int + currenttarget = bad
{
	C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());


	int angletolerance;
	angletolerance = pEntity->GetEyeAngles()->y + 180.00000;

	float consider;
	float v24;
	double v20;



	if (g_Engine->IsConnected() & pLocal->IsPlayer())
	{

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
		std::string aa_info[64];


		//------bool------//

		std::vector<int> HitBoxesToScan;




		bool MeetsLBYReq;
		bool maybefakehead = 0;

		//------bool------//

		//------float------//
		float org_yaw;
		float server_time = pLocal->GetTickBase() * g_Globals->interval_per_tick;

		//------float------//


		//------Statics------//
		static Vector Skip[65];
		static float StaticHeadAngle = 0;

		static bool GetEyeAngles[65]; //Resolve: Frame EyeAngle
		static bool GetLowerBodyYawTarget[65]; //Resolve: LowerBody
		static bool isLBYPredictited[65];
		static bool switch2;

		static float OldLowerBodyYaws[65];
		static float OldYawDeltas[65];
		static float oldTimer[65];



		ResoSetup::NewANgles[pEntity->GetIndex()] = *pEntity->GetEyeAngles();
		ResoSetup::newlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
		ResoSetup::newsimtime = pEntity->GetSimulationTime();
		ResoSetup::newdelta[pEntity->GetIndex()] = pEntity->GetEyeAngles()->y;
		ResoSetup::newlbydelta[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
		ResoSetup::finaldelta[pEntity->GetIndex()] = ResoSetup::newdelta[pEntity->GetIndex()] - ResoSetup::storeddelta[pEntity->GetIndex()];
		ResoSetup::finallbydelta[pEntity->GetIndex()] = ResoSetup::newlbydelta[pEntity->GetIndex()] - ResoSetup::storedlbydelta[pEntity->GetIndex()];
		if (newlby == storedlby)
			ResoSetup::lbyupdated = false;
		else
			ResoSetup::lbyupdated = true;
		StoreThings(pEntity);

		C_BaseEntity* player = (C_BaseEntity*)pLocal;
		C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		INetChannelInfo *nci = g_Engine->GetNetChannelInfo();
		Vector* eyeAngles = player->GetEyeAngles();

		if (pEntity->GetFlags() & FL_ONGROUND)
			MeetsLBYReq = true;
		else
			MeetsLBYReq = false;

		StoreFGE(pEntity);

		float OldLowerBodyYaw;




		static float LatestLowerBodyYawUpdateTime[55];
		static float LatestLowerBodyYawUpdateTime1[55];
		static float brokenlby[55];

		static float time_at_update[65];
		float kevin[64];
		static bool bLowerBodyIsUpdated = false;

		if (pEntity->GetLowerBodyYaw() != kevin[pEntity->GetIndex()])
			bLowerBodyIsUpdated = true;
		else
			bLowerBodyIsUpdated = false;





		bool IsMoving;



		float breakcomp = pEntity->GetEyeAngles()->y - (pEntity->GetLowerBodyYaw() - 35);



		int missedLogHits[65];


		bool sidejitter2;
		bool sidejitter1;

		bool bLowerBodyUpdated = false;
		static bool isMoving;
		float PlayerIsMoving = abs(pEntity->GetVelocity().Length());
		if (PlayerIsMoving > 0.1) isMoving = true;
		else if (PlayerIsMoving <= 0.1) isMoving = false;
		bool FakeWalking;
		bool shouldbrute;
		float oldlowerbodyyaw;
		bool IsUsingFakeAngles = false;
		int avg = 1;
		int count = 1;
		bool IsFast;

		if (pEntity->GetFlags() & FL_ONGROUND & pEntity->GetVelocity().Length2D() >= 170.5)
			IsFast = true;
		else
			IsFast = false;

		float server = (float)(pLocal->GetTickBase()  * g_Globals->interval_per_tick);

		int flip = (int)floorf(g_Globals->curtime / 0.34) % 3;
		static bool bFlipYaw;
		float flInterval = g_Globals->interval_per_tick;

		if (ResoSetup::storedlby[pEntity->GetIndex()] > pEntity->GetLowerBodyYaw() + 119)
			lbyupdated = true;






		if (g_Options.Ragebot.Resolver == 0)
		{

		}
		else if (g_Options.Ragebot.Resolver == 1)
		{

			if (Globals::missedshots >= g_Options.Ragebot.bruteAfterX && g_Options.Ragebot.bruteAfterX)
			{
				switch (Globals::missedshots % 5)
				{
				case 1:	pEntity->GetLowerBodyYaw() + 80; break;
				case 2: pEntity->GetLowerBodyYaw() + 150; break;
				case 3: pEntity->GetLowerBodyYaw() - 150; break;
				case 4: pEntity->GetLowerBodyYaw() - 80; break;
				case 5: pEntity->GetLowerBodyYaw() - (90 - rand() % 180); break;
				}
			}

			if (GetAsyncKeyState(g_Options.Ragebot.overrideresolver))
			{

				switch (testlby())
				{
				case 0: angletolerance;
				case 1: angletolerance + 45; break;
				case 2: angletolerance - 45; break;
				case 3: angletolerance - 140; break;
				case 4: angletolerance + 140; break;
				case 5: (angletolerance - angletolerance); break;
				}

			}

			// when someone is moving, their lby is always updating. So getting lby, or applying OldLowerBody is enough.
			if (IsMovingOnGround(pEntity))
			{
				switch (Globals::missedshots % 4)
				{
				case 1:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw(); break;
				case 2:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 20; break;
				case 3:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 20; break;
				case 4:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw(); break;
				}

			}

			else  // very basic stuff. Nowadays everyone uses opposite AAs or close to opposite AAs, soooooooo
			{
				if (pEntity->GetEyeAngles()->x <= 20 && pEntity->GetEyeAngles()->y < (pEntity->GetLowerBodyYaw() + 20) && pEntity->GetEyeAngles()->y >(pEntity->GetLowerBodyYaw() - 20)) // this MIGHT fuck me over, but in the case you meet a legit AA, this should do K
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
				else
				{
					switch (Globals::Shots % 10)
					{
					case 1: pEntity->GetEyeAngles()->y -= 180; break;
					case 2: pEntity->GetEyeAngles()->y -= 160; break;
					case 3: pEntity->GetEyeAngles()->y += 160; break;
					case 4: pEntity->GetEyeAngles()->y -= 180; break;
					case 5: pEntity->GetEyeAngles()->y -= 130; break;
					case 6: pEntity->GetEyeAngles()->y += 130; break;
					case 7: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 45); break;
					case 8: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 45); break;
					case 9: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 120); break;
					case 10: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 120); break;
					}
				}
				/* cases 6-9 (MASSIVE meme) should effectively make even the use of this resolver in HvH
				actually not too bad. The above is just usual ayyware memes, but i guess if you are shooting more than
				1 person at once, or are dumping fakes, this will inevetably compensate for LBY breaks. Maybe case 6
				is a bit too late for such a thing, but hey, not a bad thing to build off of. */
			}

		}
		else if (g_Options.Ragebot.Resolver == 2)
		{

			if (Globals::missedshots >= g_Options.Ragebot.bruteAfterX && g_Options.Ragebot.bruteAfterX)
			{
				switch (Globals::missedshots % 5)
				{
				case 1:	pEntity->GetLowerBodyYaw() + 80; break;
				case 2: pEntity->GetLowerBodyYaw() + 150; break;
				case 3: pEntity->GetLowerBodyYaw() - 150; break;
				case 4: pEntity->GetLowerBodyYaw() - 80; break;
				case 5: pEntity->GetLowerBodyYaw() - (90 - rand() % 180); break;
				}
			}
			if (GetAsyncKeyState(g_Options.Ragebot.overrideresolver))
			{

				switch (testlby())
				{
				case 0: angletolerance;
				case 1: angletolerance + 45; break;
				case 2: angletolerance - 45; break;
				case 3: angletolerance - 140; break;
				case 4: angletolerance + 140; break;
				case 5: (angletolerance - angletolerance); break;
				}

			}
			float v18 = pEntity->GetEyeAngles()->y + 180.0;
			float v19 = pEntity->GetEyeAngles()->y + 180.0;

			if (v19 <= 180.0)
			{
				if (v18 < -180.0)
					v19 = v18 + 360.0;
			}
			else
			{
				v19 = v18 - 360.0;
			}
			v20 = pRandomFloat(-90.0, 90.0) + v19;

			/*Second one...*/
			float v22;//lets assume v20 = viewangles or some shit
			v22 = pEntity->GetEyeAngles()->y + 180.0;
			float v23; float v24;
			double v20;
			if (IsMovingOnGround(pEntity))
				pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
			else {
				if ((pEntity->GetEyeAngles()->y + 180.0) <= 180.0)
				{
					if (v22 < -180.0)
						v22 = v22 + 360.0;
				}
				else
				{
					v22 = v22 - 360.0;
				}
				v23 = v22 - pEntity->GetLowerBodyYaw();
				if (v23 <= 180.0)
				{
					if (v23 < -180.0)
						v23 = v23 + 360.0;
				}
				else
				{
					v23 = v23 - 360.0;
				}
				if (v23 >= 0.0)
					v24 = pRandomFloat(0.0, v23 / 2);
				else
					v24 = pRandomFloat(v23 / 2, 0.0);
				v20 = v24 + pEntity->GetEyeAngles()->y;
				pEntity->GetEyeAngles()->y = v20;
			}
			if (Globals::Shots > 3)
			{
				static bool k = false;
				avg /= count;

				float add = 0;

				if (avg > 145)
					add = 180;
				else if (avg > 110)
					add = 135;
				else if (avg > 75)
					add = 90;
				else if (avg > 40)
					add = 45;
				k != k;
				pEntity->GetEyeAngles()->y += (k) ? add : -add;
			}

			else if (IsMovingOnGround(pEntity))
			{
				if (IsFakeWalking(pEntity))
				{

				}
				pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
			}
			else if (IsMovingOnInAir(pEntity))
			{
				switch (Globals::Shots % 4)//logging hits for everyhitgroup//not anymore
				{
				case 1:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 45;
					break;
				case 2:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 45;
					break;
				case 3:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 90;
					break;
				case 4:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 90;
					break;
					//continue;
				}
			}
			else//NOT MOVING
			{

				if (missedLogHits[pEntity->GetIndex()] >= 1)
				{
					switch (missedLogHits[pEntity->GetIndex()] - 1)
					{
					case 0:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
						break;
					case 1:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 180;
						break;
					case 2:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 90;
						break;
					case 3:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 90;
						break;
					}
				}
				if (Lbywithin35(pEntity))//if lby and eye angles subtracted < 35 then we assume that theyre using an lby breaker
				{
					switch (Globals::missedshots % 7)
					{
					case 1: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw() - 160; break;
					case 2: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw() + 160; break;
					case 3: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw() + 130; break;
					case 4: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - (OldLowerBodyYaw + 90); break;
					case 5: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw()) - 120; break;
					case 6: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw()) + 120; break;
					case 7: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw()) - (150 - rand() % 30); break;
					}
				}
				else if (HasFakeHead(pEntity))//lby and eye angles arent synced eZ resolve
				{
					pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw();
				}
				else {
					pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw() + pRandomFloat(30, -30);

				}
			}
			//}
			float yClamped = pEntity->GetEyeAngles()->y;
			while (yClamped < -180.0f)
				yClamped += 360.0f;

			while (yClamped > 180.0f)
				yClamped -= 360.0f;
			pEntity->GetEyeAngles()->y = yClamped;



		}

		else if (g_Options.Ragebot.Resolver == 3)
		{



			for (int i = 0; i < g_Engine->GetMaxClients(); ++i)
			{
				C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
				C_BaseEntity *player = (C_BaseEntity*)g_EntityList->GetClientEntity(i);

				if (!player || player->IsDormant() || player->IsAlive() || (DWORD)player == (DWORD)pLocal)
					continue;

				if (!player)
					continue;

				if (pLocal)
					continue;

				if (pLocal && player)
				{

					Vector* eyeAngles = player->GetEyeAngles();

					if (eyeAngles->x > 90.0 || eyeAngles->x < 60.0) eyeAngles->x = -88.9f;
					else if (eyeAngles->x > 60.0 && eyeAngles->x < 20.0) eyeAngles->x = 50.f;
					else if (eyeAngles->x > 20.0 && eyeAngles->x < -30.0) eyeAngles->x = 0.f;
					else if (eyeAngles->x < -30.0 && eyeAngles->x < 60.0) eyeAngles->x = 55.f;
					else eyeAngles->x = 88.9f;




				}

			}
			if (GetAsyncKeyState(g_Options.Ragebot.overrideresolver))
			{

				switch (testlby())
				{
				case 0: angletolerance;
				case 1: angletolerance + 45; break;
				case 2: angletolerance - 45; break;
				case 3: angletolerance - 140; break;
				case 4: angletolerance + 140; break;
				case 5: (angletolerance - angletolerance); break;
				}

			}
			if (Globals::missedshots >= g_Options.Ragebot.bruteAfterX && g_Options.Ragebot.bruteAfterX)
			{
				switch (Globals::missedshots % 5)
				{
				case 1:	pEntity->GetLowerBodyYaw() + 80; break;
				case 2: pEntity->GetLowerBodyYaw() + 150; break;
				case 3: pEntity->GetLowerBodyYaw() - 150; break;
				case 4: pEntity->GetLowerBodyYaw() - 80; break;
				case 5: pEntity->GetLowerBodyYaw() - (90 - rand() % 180); break;
				}
			}

			if (LatestLowerBodyYawUpdateTime[pEntity->GetIndex()] < pEntity->GetSimulationTime() || bLowerBodyIsUpdated)
			{
				pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
				LatestLowerBodyYawUpdateTime[pEntity->GetIndex()] = pEntity->GetSimulationTime() + 1.1;
			}
			else
			{
				if (IsMovingOnGround(pEntity))
				{
					if (IsFakeWalking(pEntity))
					{
					}
					else
					{
						switch (Globals::missedshots % 4)
						{
						case 1:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw(); break;
						case 2:
						{
							if (memerino)
								pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 15);
							else pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 15);

							memerino = !memerino;
						}
						break;

						case 3:
						{
							if (memerino)
								pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 30);
							else pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 30);

							memerino = !memerino;
						}
						break;
						case 4:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw(); break;
						}

					}
				}
				else if (IsMovingOnInAir(pEntity))
				{

					switch (Globals::missedshots % 7)
					{
					case 1: pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw(); break;
					case 2: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 25; break;
					case 3: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 25; break;
					case 4: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 135; break;
					case 5: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 135; break;
					case 6: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 165; break;
					case 7: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 165; break;
					}
				}
				else if (!IsMoving && MeetsLBYReq)
				{
					if (Lbywithin35(pEntity))
					{
						if (HasFakeHead(pEntity))
						{
							(pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw()) - 170;

							switch (Globals::missedshots % 7)
							{
							case 1: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw() - 160); break;
							case 2: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw() + 160; break;
							case 3: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw() + 130; break;
							case 4: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - (OldLowerBodyYaw + 90); break;
							case 5: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw()) - 120; break;
							case 6: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw()) + 120; break;
							case 7: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw()) - (150 - rand() % 30); break;
							}
						}
						else
						{
							switch (Globals::missedshots % 7)
							{
							case 1: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - (pEntity->GetLowerBodyYaw() - 180); break;
							case 2: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - (pEntity->GetLowerBodyYaw() - 160); break;
							case 3: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - (pEntity->GetLowerBodyYaw() - 130); break;
							case 4: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y + (pEntity->GetLowerBodyYaw() + 160); break;
							case 5: pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y + (pEntity->GetLowerBodyYaw() + 130); break;
							case 6: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y + pEntity->GetLowerBodyYaw()) + 120; break;
							case 7: pEntity->GetEyeAngles()->y = (pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw()) - (150 - rand() % 30); break;
							}
						}
					}

				}
				else if (Globals::missedshots >= (g_Options.Ragebot.bruteAfterX && g_Options.Ragebot.bruteAfterX))
				{
					switch (Globals::Shots % 10)
					{
					case 1: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 180); break;
					case 2: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 150); break;
					case 3: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 150); break;
					case 4: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 110); break;
					case 5: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 75); break;
					case 6: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 110); break;
					case 7: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 75); break;
					case 8: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() - 40); break;
					case 9: pEntity->GetEyeAngles()->y = (pEntity->GetLowerBodyYaw() + 40); break;
					case 10: pEntity->GetEyeAngles()->y - 180; break;

					}
				}
				else
				{
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();

					switch (Globals::Shots % 9)
					{
					case 1: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 170; break;
					case 2: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 180; break;
					case 3: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 160; break;
					case 4: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 160; break;
					case 5: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 110; break;
					case 6: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 110; break;
					case 7: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 40; break;
					case 8: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 40; break;
					case 9: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 180; break;
					}
				}
			}

		}



		else if (g_Options.Ragebot.Resolver == 4)
		{



			for (int i = 0; i < g_Engine->GetMaxClients(); ++i)
			{
				C_BaseEntity* pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
				C_BaseEntity *player = (C_BaseEntity*)g_EntityList->GetClientEntity(i);

				if (!player || player->IsDormant() || player->IsAlive() || (DWORD)player == (DWORD)pLocal)
					continue;



				Vector* eyeAngles = player->GetEyeAngles();

				if (eyeAngles->x > 90.0 || eyeAngles->x < 60.0) eyeAngles->x = -88.9f;
				else if (eyeAngles->x > 60.0 && eyeAngles->x < 20.0) eyeAngles->x = 55.f;
				else if (eyeAngles->x > 20.0 && eyeAngles->x < -30.0) eyeAngles->x = 0.f;
				else if (eyeAngles->x < -30.0 && eyeAngles->x < 60.0) eyeAngles->x = 55.f;
				eyeAngles->x = 88.9f;







			}

			if (GetAsyncKeyState(g_Options.Ragebot.overrideresolver))
			{

				switch (testlby())
				{
				case 0: angletolerance;
				case 1: angletolerance + 45; break;
				case 2: angletolerance - 45; break;
				case 3: angletolerance - 140; break;
				case 4: angletolerance + 140; break;
				case 5: (angletolerance - angletolerance); break;
				}

			}

			if (LatestLowerBodyYawUpdateTime[pEntity->GetIndex()] < pEntity->GetSimulationTime() || bLowerBodyIsUpdated)
			{
				pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
				LatestLowerBodyYawUpdateTime[pEntity->GetIndex()] = pEntity->GetSimulationTime() + 1.1;
			}
			else
			{

				if (IsMovingOnGround(pEntity))
				{
					if (IsFakeWalking(pEntity))
					{
					}
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();

					if (Globals::Shots >= 2)
					{
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
					}
				}
				else if (Globals::missedshots >= g_Options.Ragebot.bruteAfterX && g_Options.Ragebot.bruteAfterX)
				{
					switch (Globals::missedshots % 5)
					{
					case 0: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 80; break;
					case 1: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 150; break;
					case 2: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 150; break;
					case 3: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 80; break;
					case 4: pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - (90 - rand() % 180); break;
					}
				}
				else if (ResoSetup::didhitHS)
				{

					if (HasFakeHead(pEntity) && Globals::Shots >= 2)
					{
						switch (Globals::Shots % 5)
						{
						case 0:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - (OldLowerBodyYaw - 35);
							break;
						case 1:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + OldLowerBodyYaw;
							break;
						case 2:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 90;
							break;
						case 3:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 90;
							break;
						case 4:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 170;
							break;
						}
					}
					else
					{
						if (Globals::Shots >= 2 && Globals::missedshots >= 2)
						{
							switch (Globals::Shots % 5)
							{
							case 0:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 30;
								break;
							case 1:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 30;
								break;
							case 2:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 110;
								break;
							case 3:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 160;
								break;
							case 4:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 180;
								break;

							}
						}
						else
						{
							switch (Globals::Shots % 3)
							{
							case 0:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - OldLowerBodyYaw;
								break;
							case 1:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + OldLowerBodyYaw;
								break;
							case 2:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 180;
								break;
							}
						}
					}
				}


				else if (IsMovingOnInAir(pEntity))
				{
					switch (Globals::Shots % 6)
					{
					case 1:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 40;
						break;
					case 2:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 40;
						break;
					case 3:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 80;
						break;
					case 4:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 80;
						break;
					case 5:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 170;
						break;
					case 6:
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 170;
						break;

					}
				}
				else
				{
					if (HasFakeHead(pEntity))
					{
						pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - pEntity->GetLowerBodyYaw();
					}

					else if (IsMovingOnGround(pEntity))
					{

						if (LbyUpdated)
						{
							switch (Globals::missedshots % 6)
							{
							case 1:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
								break;
							case 2:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 20;
								break;
							case 3:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 20;
								break;
							case 4:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
								break;
							case 5:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 35;
								break;
							case 6:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 35;
								break;
							}
						}

					}

					else if (MeetsLBYReq && !IsMoving && pEntity->GetHealth() >= ResoSetup::storedhealth[pEntity->GetIndex()])
					{
						if ((pEntity->GetEyeAngles()->y + 180.0) <= 180.0)
						{
							if (angletolerance < -180.0)
								angletolerance = angletolerance + 360.0;
						}
						else
						{
							angletolerance = angletolerance - 360.0;
						}

						consider = angletolerance - pEntity->GetLowerBodyYaw();

						if (consider <= 180.0)
						{
							if (consider < -180.0)
								consider = consider + 360.0;
						}
						else
						{
							consider = consider - 360.0;
						}
						if (consider >= 0.0)
							v24 = pRandomFloat(0.0, consider / 2);
						else
							v24 = pRandomFloat(consider / 2, 0.0);
						v20 = v24 + pEntity->GetEyeAngles()->y;
						pEntity->GetEyeAngles()->y = v20;
					}

					else
					{
						if (Lbywithin35(pEntity))
						{
							switch (Globals::missedshots % 8)
							{
							case 1:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 170;
								break;
							case 2:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 160;
								break;
							case 3:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 40;
								break;
							case 4:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 40;
								break;
							case 5:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 100;
								break;
							case 6:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 100;
								break;
							case 7:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 165;
								break;
							case 8:
								pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 165;
								break;
							}
						}
					}
				}
			}


		}

	}

}

void ResoSetup::StoreFGE(C_BaseEntity* pEntity)
{
	ResoSetup::storedanglesFGE = pEntity->GetEyeAngles()->y;
	ResoSetup::storedlbyFGE = pEntity->GetLowerBodyYaw();
	ResoSetup::storedsimtimeFGE = pEntity->GetSimulationTime();
}

void ResoSetup::StoreThings(C_BaseEntity* pEntity)
{
	ResoSetup::StoredAngles[pEntity->GetIndex()] = *pEntity->GetEyeAngles();
	ResoSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResoSetup::storedsimtime = pEntity->GetSimulationTime();
	ResoSetup::storeddelta[pEntity->GetIndex()] = pEntity->GetEyeAngles()->y;
	ResoSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
}

void ResoSetup::anglestore(C_BaseEntity * pEntity)
{
	ResoSetup::badangle[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();

}

void ResoSetup::StoreExtra(C_BaseEntity * pEntity)
{
	ResoSetup::storedpanic[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResoSetup::panicangle[pEntity->GetIndex()] = (pEntity->GetEyeAngles()->y - 30);

}

ResoSetup* resolver = new ResoSetup();