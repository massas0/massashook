#pragma once
#include "HookIncludes.h"
#include "Entities.h"


class ResoSetup
{
public:
	bool didhit;
	void Resolve(C_BaseEntity * pEntity, int CurrentTarget);
	
	void StoreThings(C_BaseEntity* pEntity);
	void anglestore(C_BaseEntity * pEntity);
	void StoreExtra(C_BaseEntity* pEntity);
	std::map<int, QAngle>StoredAngles; //y and x lol (stored)
	std::map<int, QAngle>NewANgles; //y and x lol (new)
	std::map<int, float>storedlby;
	std::map<int, float>panicangle;
	std::map<int, float>storedhp;
	std::map<int, float>badangle;
	std::map<int, float>storedpanic;
	std::map<int, float>newlby;
	std::map<int, float>storeddelta;
	std::map<int, float>newdelta;
	std::map<int, float>finaldelta;
	std::map<int, float>storedlbydelta;
	std::map<int, float>storedhealth;
	std::map<int, float>newlbydelta;
	std::map<int, float>finallbydelta;
	float newsimtime;
	float storedsimtime;
	bool lbyupdated;
	float storedlbyFGE;

	float storedanglesFGE;
	float storedsimtimeFGE;
	bool didhitHS;
	void StoreFGE(C_BaseEntity* pEntity);
	static ResoSetup GetInst()
	{
		static ResoSetup instance;
		return instance;
	}
};

extern ResoSetup* resolver;
