#include "Interfaces.h"
#include "Hooks.h"
#include "Render.h"
#include "SDK.h"
#include "Variables.h"
#include "Configuration.hpp"

#include "recvproxy.h"
#include "Misc.h"

#include <wininet.h>
#include <cstring>
#include <windows.h>
#include <iostream>
#include <urlmon.h>
#include <ctype.h>
#include <time.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <strsafe.h>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include "XorStr.hpp"
#include <vcruntime_exception.h>

typedef std::chrono::system_clock Clock;
bool unload;
bool denied;

void init()
{
	try {
		U::CloseConsole();
		U::Log("Initialising Interfaces...", LOG_WARNING);
		InitialiseInterfaces();
		U::Log("Getting Netvars...", LOG_WARNING);
		g_Netvars->GetNetvars();
		U::Log("Initialising Fonts...", LOG_WARNING);
		g_Render->SetupFonts();
		U::Log("Hooking everything...", LOG_WARNING);
		hooks::initialize();
		NetvarHook();


		U::Log("Ready.", LOG_ADVICE);
		Beep(400, 50);
	}
	catch (std::exception(ex))
	{
		char buf[100];
		sprintf(buf, u8"Erro na Inicialização: %s", ex.what());
		MessageBox(NULL, buf, "Whoops, MassasHook Crashed", MB_OK);
	}
}

bool on_dll_attach(void* base)
{

	if (!U::SyncUser())
	{
		denied = true;
	}
	else
	{
		init();
	}

	while (unload == false || denied == false)
	{
		Sleep(1000);
	}

	if (unload) {
		UnloadProxy();
		hooks::cleanup();
	}

	Sleep(200);
	FreeLibraryAndExitThread((HMODULE)base, 0);
}

bool on_dll_detach()
{
	return 1;
}

BOOL WINAPI DllMain(
	_In_      HINSTANCE hinstDll,
	_In_      DWORD     fdwReason,
	_In_opt_	LPVOID    lpvReserved
)
{

	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)on_dll_attach, hinstDll, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			return on_dll_detach();
	}
	return TRUE;
}