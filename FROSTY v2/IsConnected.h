#pragma once
#include "HookIncludes.h"
#include <intrin.h>  
using is_connected_t = bool(__stdcall*)(void*);

bool __stdcall hkIsConnected()
{
	try {
		static auto ofunc = hooks::engine.get_original<is_connected_t>(27);

		void* returna = _ReturnAddress();
		static auto v1 = (uintptr_t)U::pattern_scan(GetModuleHandleW(L"client.dll"), "75 04 B0 01 5F") - 2;

		if ((uintptr_t)returna == v1) return false;
		return ofunc;
	}
	catch (std::exception(IC))
	{
		char buf[100];
		sprintf(buf, "IsConnected: %s", IC.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}