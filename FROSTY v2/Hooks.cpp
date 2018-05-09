#include "HookIncludes.h"
#include "HooksList.h"
#include "Listener.h"
#include "SceneEnd.h"

float lineLBY;
float lineRealAngle;
float lineFakeAngle;
namespace hooks
{
    vfunc_hook panel;
    vfunc_hook client;
    vfunc_hook clientmode;
    vfunc_hook modelrender;
    vfunc_hook prediction;
    vfunc_hook surface;
    vfunc_hook eventmanager;
    vfunc_hook d3d;
    vfunc_hook viewrender;
	vfunc_hook renderview;
    vfunc_hook engine;
    vfunc_hook sv_cheats;
	vfunc_hook steamgamecoordinator;

    void initialize()
    {
        client.setup(g_CHLClient);
		U::Log("g_CHClient", LOG_WARNING);
        client.hook_index(36, hkFrameStageNotify);
		U::Log("g_CHClient->FrameStageNotify", LOG_ADVICE);
        client.hook_index(21, hkCreateMove);
		U::Log("g_CHClient->CreateMove", LOG_ADVICE);

        //prediction.setup(g_Prediction);
		//U::Log("g_Prediction", LOG_WARNING);
       // prediction.hook_index(14, hkInPrediction);
		//U::Log("g_Prediction->InPrediction", LOG_ADVICE);

        clientmode.setup(g_ClientMode);
		U::Log("g_ClientMode", LOG_WARNING);
        clientmode.hook_index(18, hkOverrideView);
		U::Log("g_ClientMode->OverrideView", LOG_ADVICE);
		clientmode.hook_index(35, hkGetViewModelFOV);
		U::Log("g_ClientMode->hkGetViewModelFOV", LOG_ADVICE);
        clientmode.hook_index(44, hkDoPostScreenSpaceEffects);
		U::Log("g_ClientMode->DoPostScreenSpaceEffects", LOG_ADVICE);

        panel.setup(g_Panel);
		U::Log("g_Panel", LOG_WARNING);
        panel.hook_index(41, hkPaintTraverse);
		U::Log("g_Panel->PaintTraverse", LOG_ADVICE);

		modelrender.setup(g_ModelRender);
		U::Log("g_ModelRender", LOG_WARNING);
		modelrender.hook_index(21, hkDrawModelExecute);
		U::Log("g_ModelRender->DrawModelExecute", LOG_ADVICE);

        surface.setup(g_Surface);
		U::Log("g_Surface", LOG_WARNING);
        surface.hook_index(82, hkPlaySound);
		U::Log("g_Surface->PlaySound", LOG_ADVICE);

        engine.setup(g_Engine);
		U::Log("g_Engine", LOG_WARNING);
        engine.hook_index(27, hkIsConnected);
		U::Log("g_Engine->IsConnected", LOG_ADVICE);
        engine.hook_index(93, hkIsHltv);

        m_present = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;//big ( large ) obs bypass
		U::Log("m_Present", LOG_ADVICE);
        m_reset = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;  //big ( large ) obs bypass
		U::Log("m_Reset", LOG_ADVICE);


        oPresent = **reinterpret_cast<Present_T**>(m_present);
        oReset = **reinterpret_cast<Reset_t**>(m_reset);

        **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(&hkPresent);
        **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(&hkReset);

        item_purchase::singleton()->initialize();
    }
    void cleanup()
    {
        item_purchase::singleton()->remove();
        client.unhook_all();
        prediction.unhook_all();
        clientmode.unhook_all();
        panel.unhook_all();
        modelrender.unhook_all();
        surface.unhook_all();
        //EventManager.Unhook();
        viewrender.unhook_all();
		renderview.unhook_all();
        //d3d.Unhook();
        engine.unhook_all();
        **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(oPresent);
        **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(oReset);
        renderer->~Renderer();
    }

}