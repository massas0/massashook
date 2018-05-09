#pragma once
#include "HookIncludes.h"
#include <intrin.h>  
using is_hltv_t = bool(__thiscall*)(void*);

bool __fastcall hkIsHltv(void* ecx, void* edx)
{
	try {
		static auto ofunc = hooks::engine.get_original<is_hltv_t>(93);
		uintptr_t pvs_entity;
		__asm
		{
			mov pvs_entity, edi
		}

		Globals::HLTV = ofunc(ecx);
		return ofunc(ecx);
	}
	catch (std::exception(HLTV))
	{
		char buf[199];
		sprintf(buf, "IsHLTV: %s", HLTV.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}