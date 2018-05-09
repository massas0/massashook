
#include "LegitBot.h"
#include "Render.h"
#include "SDK.h"
#include "EnginePrediction.h"
#include "Global.h"
#define NOMINMAX
#include <Windows.h>
#include <stdio.h>
#include <random>
#include <string>
#include <vector>


#define		HITGROUP_HEAD			1
#define		HITGROUP_CHEST			2
#define		HITGROUP_STOMACH		3
#define		HITGROUP_LEFTARM		4    
#define		HITGROUP_RIGHTARM		5
#define		HITGROUP_LEFTLEG		6
#define		HITGROUP_RIGHTLEG		7

legitbot::legitbot()
{
    best_target = -1;

    view_angle =	QAngle(0.0f, 0.0f, 0.0f);
    aim_angle =		QAngle(0.0f, 0.0f, 0.0f);
    delta_angle =	QAngle(0.0f, 0.0f, 0.0f);
    final_angle =	QAngle(0.0f, 0.0f, 0.0f);

    hitbox_position = Vector(0.0f, 0.0f, 0.0f);

    aim_key = 0;
    aim_smooth = 1;
    aim_fov = 0;
    randomized_smooth = 0;
    recoil_min = 0;
    recoil_max = 0;
    randomized_angle = 0;
	is_locked = false;
	bone = 0;
	psilent = false;
}
float get_fov(const QAngle &viewAngles, const QAngle &aimAngles)
{
    Vector ang, aim;
    AngleVectors(viewAngles, &aim);
    AngleVectors(aimAngles, &ang);

    return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
float random_number_range(float min, float max)
{
    std::random_device device;
    std::mt19937 engine(device());
    std::uniform_real_distribution<> distribution(min, max);
    return static_cast< float >(distribution(engine));
}


bool shoot;
static int custom_delay = 0;

void legitbot::triggerbot(CInput::CUserCmd *cmd, C_BaseEntity* local, CBaseCombatWeapon* weapon)
{
	if (!local->IsAlive())
		return;

	if (weapon) {
		if (weapon->ammo() == 0)
			return;
		if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon)) return;
		if (*weapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64) return;
	}



	QAngle ViewAngles = cmd->viewangles + local->localPlayerExclusive()->GetAimPunchAngle();


	Vector CrosshairForward;
	AngleVectors(ViewAngles, &CrosshairForward);
	CrosshairForward *= weapon->GetCSWpnData()->m_fRange;


	Vector TraceSource = local->GetEyePosition();
	Vector TraceDestination = TraceSource + CrosshairForward;

	Ray_t Ray;
	trace_t Trace;
	CTraceFilter Filter;

	Filter.pSkip = local;

	Ray.Init(TraceSource, TraceDestination);
	g_EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->IsAlive())
		return;
	if (Trace.m_pEnt->HasGunGameImmunity())
		return;


	if (local->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
		return;

	if (!hit_chance(local, cmd, weapon, Trace.m_pEnt))
		return;

	bool didHit = false;
	if ((g_Options.Legitbot.Triggerbot.Filter.Head && Trace.hitgroup == 1)
		|| (g_Options.Legitbot.Triggerbot.Filter.Chest && Trace.hitgroup == 2)
		|| (g_Options.Legitbot.Triggerbot.Filter.Stomach && Trace.hitgroup == 3)
		|| (g_Options.Legitbot.Triggerbot.Filter.Arms && (Trace.hitgroup == 4 || Trace.hitgroup == 5))
		|| (g_Options.Legitbot.Triggerbot.Filter.Legs && (Trace.hitgroup == 6 || Trace.hitgroup == 7)))
	{
		didHit = true;
	}

	if (g_Options.Legitbot.Triggerbot.Delay >= 1)
	{
		if (custom_delay >= g_Options.Legitbot.Triggerbot.Delay / 30)
		{
			if (didHit)
			{
				custom_delay = 0;
				shoot = true;
				cmd->buttons |= IN_ATTACK;
			}
		}
		else
		{
			custom_delay++;
		}
	}

}

void legitbot::OnCreateMove(CInput::CUserCmd *pCmd, C_BaseEntity *local)
{

	if (!g_Options.Legitbot.MainSwitch)
		return;

    CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
	
	if (local && local->IsAlive() && pWeapon)
	{
        do_aimbot(local, pWeapon, pCmd);
	}
}

bool ShouldFire()
{
	C_BaseEntity* pLocalEntity = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * g_Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

static int wait = 0;
bool shouldreturn;
void legitbot::do_aimbot(C_BaseEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd)
{

	Globals::legitbotting = false;

    if (!g_Options.Legitbot.MainSwitch)
        return;

    if (!weapon)
        return;

    if (!local)
        return;
    if (!cmd)
        return;

    if (!local->IsAlive())
        return;

    if (!weapon->ammo() > 0)
        return;


    if (weapon->ammo() == 0)
        return;

    if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon)) 
        return;


    weapon_settings(weapon);

	if (!g_Options.Misc.WalkRobot) {
		if (!G::PressedKeys[aim_key])
			shouldreturn = true;
		else
			shouldreturn = false;
	}
	else
		shouldreturn = false;

	if (shouldreturn)
		return;


    best_target = get_target(local, weapon, cmd, hitbox_position);


    if (best_target == -1)
        return;
    C_BaseEntity* entity = (C_BaseEntity*)g_EntityList->GetClientEntity(best_target);
    if (!entity)
        return;



    if (get_distance(local->GetEyePosition(), hitbox_position) > 8192.0f)
        return;


    compute_angle(local->GetEyePosition(), hitbox_position, aim_angle);
    sanitize_angles(aim_angle);

    if (hitbox_position == Vector(0, 0, 0))
        return;

    aim_angle -= get_randomized_recoil(local);
    aim_angle += get_randomized_angles(local);

    sanitize_angles(view_angle);

    delta_angle = view_angle - aim_angle;
    sanitize_angles(delta_angle);

    float randomSmoothing = 1.0f;

    if (randomized_smooth > 1.0f)
        randomSmoothing = random_number_range(randomized_smooth / 10.0f, 1.0f);

    final_angle = view_angle - delta_angle / aim_smooth * randomSmoothing;
    sanitize_angles(final_angle);


    if (!sanitize_angles(final_angle))
        return;

	if (fire)
	{
		QAngle ViewAngles = cmd->viewangles + local->localPlayerExclusive()->GetAimPunchAngle();


		Vector CrosshairForward;
		AngleVectors(ViewAngles, &CrosshairForward);
		CrosshairForward *= weapon->GetCSWpnData()->m_fRange;


		Vector TraceSource = local->GetEyePosition();
		Vector TraceDestination = TraceSource + CrosshairForward;

		Ray_t Ray;
		trace_t Trace;
		CTraceFilter Filter;

		Filter.pSkip = local;

		Ray.Init(TraceSource, TraceDestination);
		g_EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);


		if (Trace.hitgroup == HITGROUP_HEAD || Trace.hitgroup == HITGROUP_CHEST || Trace.hitgroup == HITGROUP_LEFTARM
			|| Trace.hitgroup == HITGROUP_LEFTLEG || Trace.hitgroup == HITGROUP_RIGHTARM || Trace.hitgroup == HITGROUP_RIGHTLEG
			|| Trace.hitgroup == HITGROUP_STOMACH)
		{

			if (wait >= 2)
				cmd->buttons |= IN_ATTACK;
			else
				wait++;
		}
		else
		{
			wait = 0;
		}
	}

	if (psilent)
	{
		Vector Oldview = cmd->viewangles;
		Vector qAimAngles = cmd->viewangles;
		float Oldsidemove = cmd->sidemove;
		float Oldforwardmove = cmd->forwardmove;

		static int ChokedPackets = -1;


		if (!cmd->buttons & IN_ATTACK)
			return;

		if (ShouldFire() && ChokedPackets < 6)
		{
			G::SendPacket = false;
			cmd->viewangles = final_angle;
			ChokedPackets++;
		}
		else
		{
			G::SendPacket = true;
			cmd->viewangles = Oldview;
			cmd->sidemove = Oldsidemove;
			cmd->forwardmove = Oldforwardmove;
			ChokedPackets = -1;
		}

		cmd->viewangles.z = 0;
		Globals::legitbotting = true;

	}
	else
	{
		cmd->viewangles = final_angle;
		g_Engine->SetViewAngles(cmd->viewangles);
		Globals::legitbotting = true;
	}
	
}

bool legitbot::hit_chance(C_BaseEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, C_BaseEntity* target)
{
    Vector forward, right, up;

    constexpr auto max_traces = 150;

    AngleVectors(cmd->viewangles, &forward, &right, &up);

    int total_hits = 0;
    int needed_hits = static_cast<int>(max_traces * (g_Options.Legitbot.Triggerbot.hitchance / 100.f));

    weapon->UpdateAccuracyPenalty(weapon);

    auto eyes = local->GetEyePosition();
    auto flRange = weapon->GetCSWpnData()->m_fRange;

    for (int i = 0; i < max_traces; i++) {
        RandomSeed(i + 1);

        float fRand1 = RandomFloat(0.f, 1.f);
        float fRandPi1 = RandomFloat(0.f, XM_2PI);
        float fRand2 = RandomFloat(0.f, 1.f);
        float fRandPi2 = RandomFloat(0.f, XM_2PI);

        float fRandInaccuracy = fRand1 * weapon->GetInaccuracy();
        float fRandSpread = fRand2 * weapon->GetSpread();

        float fSpreadX = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
        float fSpreadY = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

        auto viewSpreadForward = (forward + fSpreadX * right + fSpreadY * up).Normalized();

        QAngle viewAnglesSpread;
        VectorAngles(viewSpreadForward, viewAnglesSpread);
        sanitize_angles(viewAnglesSpread);

        Vector viewForward;
        AngleVectors(viewAnglesSpread, &viewForward);
        viewForward.NormalizeInPlace();

        viewForward = eyes + (viewForward * flRange);

        trace_t tr;
        Ray_t ray;
        ray.Init(eyes, viewForward);

        g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);


        if (tr.m_pEnt == target)
            total_hits++;

        if (total_hits >= needed_hits)
            return true;

        if ((max_traces - i + total_hits) < needed_hits)
            return false;
    }

    return false;
}

int GetMenuHitbox(int index)
{
	if (index == 0)
		return 10;
	if (index == 1)
		return CSGOHitboxID::Head;
	if (index == 2)
		return CSGOHitboxID::Neck;
	if (index == 3)
		return CSGOHitboxID::Chest;
	if (index == 4)
		return CSGOHitboxID::Pelvis;
}


void legitbot::weapon_settings(CBaseCombatWeapon* weapon)
{
    if (!weapon)
        return;


    if (MiscFunctions::IsSniper(weapon))
    {
        aim_key = g_Options.Legitbot.SniperKey;
        aim_fov = g_Options.Legitbot.Sniperfov;
        randomized_smooth = 1;
        recoil_min = g_Options.Legitbot.sniper_recoil_min;
        recoil_max = g_Options.Legitbot.sniper_recoil_max;
        randomized_angle = 1;
		bone = GetMenuHitbox(g_Options.Legitbot.sniper_bone);
		fire = g_Options.Legitbot.sniper_autoshoot;
		psilent = g_Options.Legitbot.sniper_silent;
		if (psilent)
			aim_smooth = 1;
		else
			aim_smooth = g_Options.Legitbot.SniperSmooth;;
    }
    else if (MiscFunctions::IsPistol(weapon))
    {
        aim_key = g_Options.Legitbot.PistolKey;
        aim_fov = g_Options.Legitbot.Pistolfov;
        randomized_smooth = 1;
        recoil_min = g_Options.Legitbot.pistol_recoil_min;
        recoil_max = g_Options.Legitbot.pistol_recoil_max;
        randomized_angle = 1;
		bone = GetMenuHitbox(g_Options.Legitbot.pistol_bone);
		fire = g_Options.Legitbot.pistol_autoshoot;
		psilent = g_Options.Legitbot.pistol_silent;
		if (psilent)
			aim_smooth = 1;
		else
			aim_smooth = g_Options.Legitbot.PistolSmooth;
    }
    else
    {
        aim_key = g_Options.Legitbot.MainKey;
        aim_fov = g_Options.Legitbot.Mainfov;
        randomized_smooth = 1;
        recoil_min = g_Options.Legitbot.main_recoil_min;
        recoil_max = g_Options.Legitbot.main_recoil_max;
        randomized_angle = 1;
		bone = GetMenuHitbox(g_Options.Legitbot.main_bone);
		fire = g_Options.Legitbot.main_autoshoot;
		psilent = g_Options.Legitbot.main_silent;
		if (psilent)
			aim_smooth = 1;
		else
			aim_smooth = g_Options.Legitbot.MainSmooth;
    }
}

QAngle legitbot::get_randomized_recoil(C_BaseEntity *local)
{
    QAngle compensatedAngles = (local->localPlayerExclusive()->GetAimPunchAngle() * 2.0f) * (random_number_range(recoil_min, recoil_max) / 100.0f);
    sanitize_angles(compensatedAngles);

    return (local->m_iShotsFired() > 1 ? compensatedAngles : QAngle(0.0f, 0.0f, 0.0f));
}

QAngle legitbot::get_randomized_angles(C_BaseEntity *local)
{
    QAngle randomizedValue = QAngle(0.0f, 0.0f, 0.0f);

    float randomRate = random_number_range(-randomized_angle, randomized_angle);
    float randomDeviation = random_number_range(-randomized_angle, randomized_angle);

    switch (rand() % 2)
    {
    case 0:
        randomizedValue.x = (randomRate * cos(randomDeviation));
        randomizedValue.y = (randomRate * cos(randomDeviation));
        randomizedValue.z = (randomRate * cos(randomDeviation));
        break;
    case 1:
        randomizedValue.x = (randomRate * sin(randomDeviation));
        randomizedValue.y = (randomRate * sin(randomDeviation));
        randomizedValue.z = (randomRate * sin(randomDeviation));
        break;
    }

    sanitize_angles(randomizedValue);

    return (local->m_iShotsFired() > 1 ? randomizedValue : QAngle(0.0f, 0.0f, 0.0f));
}
bool get_hitbox_pos(C_BaseEntity* entity, int hitbox, Vector &output)
{
    if (hitbox >= 20)
        return false;

    const model_t *model = entity->GetModel();
    if (!model)
        return false;

    studiohdr_t *studioHdr = g_ModelInfo->GetStudiomodel(model);
    if (!studioHdr)
        return false;

    matrix3x4 matrix[128];
    if (!entity->SetupBones(matrix, 128, 0x100, entity->GetSimulationTime()))
        return false;

    mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
    if (!studioBox)
        return false;

    Vector min, max;

    VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
    VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

    output = (min + max) * 0.5f;

    return true;
}
bool legitbot::get_hitbox(C_BaseEntity *local, C_BaseEntity *entity, Vector &destination)
{
    int bestHitbox = -1;
    float best_fov = aim_fov;

    static const std::vector<int> hitboxes = { (int)CSGOHitboxID::Head, (int)CSGOHitboxID::Neck, (int)CSGOHitboxID::Chest, (int)CSGOHitboxID::Stomach, (int)CSGOHitboxID::Pelvis };

    for (auto hitbox : hitboxes)
    {
        Vector temp;
        if (!get_hitbox_pos(entity, hitbox, temp))
            continue;

        float fov = get_fov(view_angle, compute_angle(local->GetEyePosition(), temp));
        if (fov < best_fov)
        {
            best_fov = fov;
			if (bone == 10)
				bestHitbox = hitbox;
			else
				bestHitbox = bone;
        }
    }

    if (bestHitbox != -1)
    {
        if (!get_hitbox_pos(entity, bestHitbox, destination))
            return true;
    }

    return false;
}


int legitbot::get_target(C_BaseEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd, Vector &destination)
{
    int best_target = -1;
    float best_fov = aim_fov;

    g_Engine->GetViewAngles(view_angle);

    for (int i = 1; i <= g_Globals->maxClients; i++)
    {
        C_BaseEntity *entity = (C_BaseEntity*)g_EntityList->GetClientEntity(i);
        if (!entity
            || entity == local
            || entity->IsDormant()
            || entity->GetLifeState() != LIFE_ALIVE
            || entity->HasGunGameImmunity()
            || entity->GetClientClass()->m_ClassID != (int)ClassID::CCSPlayer
            || entity->GetTeamNum() == local->GetTeamNum()
            || !(entity->GetFlags() & FL_ONGROUND))
            continue;

        Vector hitbox;
        if (get_hitbox(local, entity, hitbox))
            continue;

        float fov = get_fov(view_angle + (local->localPlayerExclusive()->GetAimPunchAngle() * 2.0f), compute_angle(local->GetEyePosition(), hitbox));
        if (fov < best_fov && fov < aim_fov)
        {	
            if (MiscFunctions::IsVisible(local, entity, 0))
            {
                best_fov = fov;
                destination = hitbox;
                best_target = i;
            }
        }
    }

    return best_target;
}