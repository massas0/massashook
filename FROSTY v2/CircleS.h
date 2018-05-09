#include "SDK.h"

float NormalizeYawInPlace(float f)
{
	if (std::isnan(f) || std::isinf(f))
		f = 0;

	if (f > 9999999)
		f = 0;

	if (f < -9999999)
		f = 0;

	while (f < -180.0f) f += 360.0f;
	while (f > 180.0f) f -= 360.0f;

	return f;
}

namespace CStrafe
{
	float RightMovement = -1;
	bool IsActive = false;
	float StrafeAngle = 0;
	QAngle CurrentAngles = Vector(0, 0, 0);
	QAngle LastAngles = Vector(0, 0, 0);

	float GetTraceFractionWorldProps(Vector start, Vector end)
	{
		Ray_t rayF, rayL, rayR;
		CTraceFilterWorldAndPropsOnly filter;
		trace_t traceF, traceL, traceR;

		//filter.pLocal = G::pLocal;

		QAngle angle = CalcAngle(start, end);
		Vector forward, left;
		AngleVectors2(angle, forward);
		AngleVectors2(angle + QAngle(0, 90, 0), forward);
		forward.NormalizeInPlace();
		left.NormalizeInPlace();

		float bodyOffset = 32;

		rayF.Init(start, end);
		rayL.Init(start + left * bodyOffset, end + left * bodyOffset);
		rayR.Init(start - left * bodyOffset, end - left * bodyOffset);

		float fractionF, fractionL, fractionR;

		g_EngineTrace->TraceRay(rayF, PEN_MASK_NONE, (ITraceFilter*)&filter, &traceF);
		g_EngineTrace->TraceRay(rayL, PEN_MASK_NONE, (ITraceFilter*)&filter, &traceL);
		g_EngineTrace->TraceRay(rayR, PEN_MASK_NONE, (ITraceFilter*)&filter, &traceR);

		fractionF = traceF.fraction;
		fractionL = traceL.fraction;
		fractionR = traceR.fraction;

		return min(fractionF, min(fractionL, fractionR));
	}

	void Rotate(CInput::CUserCmd *cmd, float rotation)
	{
		rotation = DEG2RAD(rotation);

		float cos_rot = cos(rotation);
		float sin_rot = sin(rotation);

		float new_forwardmove = (cos_rot * cmd->forwardmove) - (sin_rot * cmd->sidemove);
		float new_sidemove = (sin_rot * cmd->forwardmove) + (cos_rot * cmd->sidemove);

		cmd->forwardmove = new_forwardmove;
		cmd->sidemove = new_sidemove;
	}

	void Start(CInput::CUserCmd *cmd)
	{
		auto pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());

		if (!IsActive)
			StrafeAngle = 0;

		IsActive = true;

		CurrentAngles = cmd->viewangles;
		/*I::pEngineClient->GetViewAngles(CurrentAngles);
		CurrentAngles.x = 0;*/

		if (pLocal->GetVelocity().Length() > 100)
		{
			VectorAngles(pLocal->GetVelocity(), CurrentAngles);
		}

		Vector Forward, Right, Left, VelForward;
		AngleVectors2(CurrentAngles, Forward);
		AngleVectors2(CurrentAngles + QAngle(0, -90, 0), Right);
		AngleVectors2(CurrentAngles + QAngle(0, 90, 0), Left);

		Forward.NormalizeInPlace();
		Right.NormalizeInPlace();
		Left.NormalizeInPlace();

		float frac = GetTraceFractionWorldProps(pLocal->GetAbsOrigin(), pLocal->GetVelocity() * TICKS_TO_TIME(5));
		Vector origin = Vector(0, 0, 5) + pLocal->GetAbsOrigin() + pLocal->GetVelocity().Normalized() * frac;

		float LeftPath = 0; // GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + Left * 250.0f + Vector(0, 0, 5));
		float RightPath = 0; // GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + Right * 250.0f + Vector(0, 0, 5));

		for (float i = 5.0f; i < 90.0f; i += 5.0f)
		{
			Vector val, var;
			AngleVectors2(CurrentAngles + QAngle(0, i, 0), val);
			AngleVectors2(CurrentAngles + QAngle(0, -i, 0), var);

			LeftPath += min(
				GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + val * 250.0f + Vector(0, 0, 5)),
				min(
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5 + 32), origin + val * 250.0f + Vector(0, 0, 5 + 32)),
					min(
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Left * 50, origin + val * 250.0f + Vector(0, 0, 5) + Left * 50),
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Right * 50, origin + val * 250.0f + Vector(0, 0, 5) + Right * 50)
					)
				)
			);

			RightPath += min(
				GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + var * 250.0f + Vector(0, 0, 5)),
				min(
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5 + 32), origin + var * 250.0f + Vector(0, 0, 5 + 32)),
					min(
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Left * 50, origin + var * 250.0f + Vector(0, 0, 5) + Left * 50),
						GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + Right * 50, origin + var * 250.0f + Vector(0, 0, 5) + Right * 50)
					)
				)
			);
		}

		if (LeftPath > RightPath)
			RightMovement = -1;
		else
			RightMovement = 1;
	}

	void Strafe(CInput::CUserCmd *cmd)
	{
		auto pLocal = (C_BaseEntity*)g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
		Vector Velocity = pLocal->GetVelocity();

		Vector velFw = Velocity.Normalized();

		Vector VelLeft, VelRight, VelAng;

		VectorAngles(Velocity, VelAng);
		AngleVectors2(Velocity + QAngle(0, -90, 0), VelRight);
		AngleVectors2(Velocity + QAngle(0, 90, 0), VelLeft);
		VelLeft.Normalized();
		VelRight.Normalized();

		float frac = GetTraceFractionWorldProps(pLocal->GetAbsOrigin(), pLocal->GetVelocity() * TICKS_TO_TIME(5));
		Vector origin = Vector(0, 0, 5) + pLocal->GetAbsOrigin() + pLocal->GetVelocity().Normalized() * frac;

		Velocity.z = 0;
		float Speed = Velocity.Length();
		if (Speed < 45) Speed = 45;
		//if (Speed > 800) Speed = 800;

		float retrack = g_Options.Misc.retrack + (Speed / 1500.0f) * 5.0f;

		float FinalPath = min(
			GetTraceFractionWorldProps(origin + Vector(0, 0, 5), origin + Vector(0, 0, 5) + Velocity / g_Options.Misc.divisor),
			min(
				GetTraceFractionWorldProps(origin + Vector(0, 0, 5 + 32), origin + Vector(0, 0, 5 + 32) + Velocity / g_Options.Misc.divisor),
				min(
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + VelLeft * 50, origin + Vector(0, 0, 5) + Velocity / g_Options.Misc.divisor + VelLeft * 50),
					GetTraceFractionWorldProps(origin + Vector(0, 0, 5) + VelRight * 50, origin + Vector(0, 0, 5) + Velocity / g_Options.Misc.divisor + VelRight * 50)
				)
			)
		);

		float Delta = (275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / g_Globals->interval_per_tick)) * retrack;

		static int lastChangeDir = 0;

		/*if (Delta > 30)
		{
		if (abs(lastChangeDir - I::pGlobals->tickcount) > 16)
		{
		RightMovement *= -1;
		lastChangeDir = I::pGlobals->tickcount;
		}
		}*/

		float DeltaAngle = RightMovement * min(Delta, 15.0f);
		StrafeAngle += DeltaAngle;

		if (fabs(StrafeAngle) >= 360.0f)
		{
			StrafeAngle = 0.0f;
			IsActive = false;
			//RightMovement = 0;
			LastAngles = Vector(0, 0, 0);
		}
		else
		{
			if (LastAngles != Vector(0, 0, 0))
				StrafeAngle -= NormalizeYawInPlace(LastAngles.y - cmd->viewangles.y);

			cmd->forwardmove = (cos((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * 450.0f);
			cmd->sidemove = (sin((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * 450.0f);
		}

		LastAngles = cmd->viewangles;
	}

};