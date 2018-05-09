#pragma once
#include "singleton.hpp"
#include "MiscClasses.h"
#include "Interfaces.h"
#include "Interface.h"
#include "Sounds.h"
#include "Logging.h"
#include "Walkbot.h"
#include <sstream>


#pragma comment(lib, "winmm.lib")
		class item_purchase

			: public singleton<item_purchase>
		{
			class item_purchase_listener
				: public IGameEventListener2
			{
			public:
				void start()
				{
					g_EventManager->AddListener(this, "item_purchase", false);
					g_EventManager->AddListener(this, "player_hurt", false);
					g_EventManager->AddListener(this, "weapon_fire", false);
					g_EventManager->AddListener(this, "player_death", false);
					g_EventManager->AddListener(this, "bomb_planted", false);
					g_EventManager->AddListener(this, "bomb_exploded", false);
					g_EventManager->AddListener(this, "bomb_dropped", false);
					g_EventManager->AddListener(this, "bomb_defused", false);
					g_EventManager->AddListener(this, "bomb_begindefuse", true);
					g_EventManager->AddListener(this, "bomb_abortdefuse", true);
					g_EventManager->AddListener(this, "round_freeze_end", false);
					g_EventManager->AddListener(this, "round_start", false);

				}
				void stop()
				{
					g_EventManager->RemoveListener(this);
				}
				void FireGameEvent(IGameEvent *event) override
				{
					singleton()->on_fire_event(event, false);
				}
				int GetEventDebugID(void) override
				{
					return 42 /*0x2A*/;
				}
			};

		public:

			static item_purchase* singleton()
			{
				static item_purchase* instance = new item_purchase;
				return instance;
			}

			void initialize()
			{
				listener.start();
			}

			void remove()
			{
				listener.stop();
			}

			void on_fire_event(IGameEvent* event, bool drawing)
			{
				static AppLog log;
				if (drawing)
				{

				}
				else
				{

				}

				if (!drawing)
				{
					C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
					if (!strcmp(event->GetName(), "item_purchase"))
					{
						auto buyer = event->GetInt("userid");
						std::string gun = event->GetString("weapon");

						if (strstr(gun.c_str(), "molotov")
							|| strstr(gun.c_str(), "nade")
							|| strstr(gun.c_str(), "kevlar")
							|| strstr(gun.c_str(), "decoy")
							|| strstr(gun.c_str(), "suit")
							|| strstr(gun.c_str(), "flash")
							|| strstr(gun.c_str(), "vest")
							|| strstr(gun.c_str(), "cutter")
							|| strstr(gun.c_str(), "defuse")
							)  return;
						auto player_index = g_Engine->GetPlayerForUserID(buyer);
						C_BaseEntity* player = (C_BaseEntity*)g_EntityList->GetClientEntity(player_index);
						player_info_t pinfo;

						if (player && local && g_Engine->GetPlayerInfo(player_index, &pinfo))
						{
							gun.erase(gun.find("weapon_"), 7);
							if (g_Options.Visuals.EventLog) {
								if (player->GetTeamNum() == local->GetTeamNum())
								{

								}
								else
								{
									g_CVar->ConsoleColorPrintf(Color(g_Options.Colors.MenuTheme[0] * 255, g_Options.Colors.MenuTheme[1] * 255, g_Options.Colors.MenuTheme[2] * 255), "[MassasHook] ");
									g_CVar->ConsoleColorPrintf(Color(219, 216, 72), """%s bought %s\n", pinfo.name, gun.c_str());
									std::stringstream string;
									string << pinfo.name << " bought " << gun.c_str();
									visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(219, 216, 72), g_Globals->curtime));
								}
							}

						}
					}

					if (!strcmp(event->GetName(), "round_start"))
					{
						if (g_Options.Misc.WalkRobot)
						{
							Walkbot->marker = 0;
							g_Engine->ClientCmd("buy ak47; buy m4a1");
						}
					}
					if (!strcmp(event->GetName(), "weapon_fire"))
					{
						auto player = event->GetInt("userid");
						auto player_index = g_Engine->GetPlayerForUserID(player);
						C_BaseEntity* player_entity = (C_BaseEntity*)g_EntityList->GetClientEntity(player_index);
						if (player_entity == local)
						{
							G::missedshots++;
						}
					}

					if (!strcmp(event->GetName(), "player_death"))
					{
						auto died = event->GetInt("userid");
						auto killer = event->GetInt("attacker");
						std::string weapon = event->GetString("weapon_fauxitemid");

						auto died_index = g_Engine->GetPlayerForUserID(died);
						auto killer_index = g_Engine->GetPlayerForUserID(killer);
						C_BaseEntity* died_ = (C_BaseEntity*)g_EntityList->GetClientEntity(died_index);
						C_BaseEntity* killer_ = (C_BaseEntity*)g_EntityList->GetClientEntity(killer_index);

						if ((died_ == local) || (killer_ == local))
						{
							G::missedshots = 0;
						}

						player_info_t info;
						g_Engine->GetPlayerInfo(died_index, &info);

						if (died_ == local)
						{
							if (g_Options.Misc.WalkRobot)
								Walkbot->marker = 0;

							switch (g_Options.Misc.event_spam)
							{
							case 1:
								g_Engine->ClientCmd_Unrestricted(u8"say How dare you kille me? :((");
								break;
							case 2:
								g_Engine->ClientCmd_Unrestricted(u8"say Como é que te atreves a matar um usuario do MassasHook?");
								break;
							}
						}
						if (killer_ == local)
						{
							std::stringstream ss;
							std::stringstream ss1;
							switch (g_Options.Misc.event_spam)
							{
							case 1:
								ss << "say " << info.name << ", you got rekt by MassasHook, using a weapon named " << event->GetString("weapon") << ".";
								g_Engine->ClientCmd_Unrestricted(ss.str().c_str());
								break;
							case 2:
								ss1 << "say mpt " << info.name << ", acabaste de ser destruido por MassasHook, usando " << event->GetString("weapon") << ".";
								g_Engine->ClientCmd_Unrestricted(ss1.str().c_str());
								break;
							}
						}

					}

					if (!strcmp(event->GetName(), "bomb_planted"))
					{
						// The game launched the bomb_beginplant event
						int nPlanter = event->GetInt("userid");
						if (!nPlanter)
							return;

						player_info_t planter_info;
						g_Engine->GetPlayerInfo(nPlanter, &planter_info);

						std::stringstream ss;
						switch (g_Options.Misc.event_spam)
						{
						case 1:
							ss << "say Wow, " << planter_info.name << ", you planted the bomb like a real muslim!";
							break;
						case 2:
							ss << "say Wow, " << planter_info.name << " plantou a bomba como um verdadeiro terrorista";
							break;
						}

						g_Engine->ClientCmd_Unrestricted(ss.str().c_str());

					}

					if (!strcmp(event->GetName(), "bomb_exploded"))
					{
						if (g_Options.Misc.event_spam)
							g_Engine->ClientCmd_Unrestricted("say BOOOOOM!!!");

					}

					if (!strcmp(event->GetName(), "bomb_dropped"))
					{
						int nDropper = event->GetInt("userid");
						if (!nDropper)
							return;

						player_info_t info;
						g_Engine->GetPlayerInfo(nDropper, &info);

						std::stringstream ss;
						switch (g_Options.Misc.event_spam)
						{
						case 1:
							ss << "say Oh no! " << info.name << " dropped the fuckin bomb!";
							break;
						case 2:
							ss << "say " << info.name << " dropou a bomba!";
							break;
						}

						g_Engine->ClientCmd_Unrestricted(ss.str().c_str());
					}

					if (!strcmp(event->GetName(), "bomb_defused"))
					{
						int nDefuser = event->GetInt("userid");
						if (!nDefuser)
							return;

						player_info_t info; g_Engine->GetPlayerInfo(nDefuser, &info);

						std::stringstream ss;

						switch (g_Options.Misc.event_spam)
						{
						case 1:
							ss << "say " << info.name << " defused the bomb with SWAT style! Bin Laden really sad atm..."; break;
						case 2:
							ss << "say " << info.name << " defusou a bomba :("; break;
						}

						g_Engine->ClientCmd_Unrestricted(ss.str().c_str());
					}

					if (!strcmp(event->GetName(), "bomb_begindefuse"))
					{
						//[WARNING: This event isn't called anymore, idk why]
						int nDefuser = event->GetInt("userid");
						if (!nDefuser)
							return;

						player_info_t info; g_Engine->GetPlayerInfo(nDefuser, &info);

						std::stringstream ss;
						switch (g_Options.Misc.event_spam)
						{
						case 1:
							ss << "say " << info.name << " started defusing the mothafucking bomb!";
							if (event->GetInt("haskit"))
								ss << " And he has got a l33t defuser!!!";
							break;
						case 2:
							ss << "say " << info.name << " ta a defusar a bomba, matem no!";
							if (event->GetInt("haskit"))
								ss << " Ainda por cima tem kit!!";
							break;
						}

						g_Engine->ClientCmd_Unrestricted(ss.str().c_str());
					}

					if (!strcmp(event->GetName(), "bomb_abortdefuse"))
					{
						//[WARNING: This event isn't called anymore, idk why]
						int nAborter = event->GetInt("userid");
						if (!nAborter)
							return;

						player_info_t info; g_Engine->GetPlayerInfo(nAborter, &info);

						// Continue only if I'm not the aborter
						if (g_Engine->GetPlayerForUserID(nAborter) == g_Engine->GetLocalPlayer())
							return;

						std::stringstream ss;

						switch (g_Options.Misc.event_spam)
						{
						case 1:
							ss << "say " << info.name << " aborted the bomb defusal like a fucking pussy!!! MAYBE HE NEEDS SOME MASSASHOOK ;)"; break;
						case 2:
							ss << u8"say esqueçam pessoal, o " << info.name << " parou de dar defuse, que grande cona"; break;
						}
						g_Engine->ClientCmd_Unrestricted(ss.str().c_str());
					}

					if (!strcmp(event->GetName(), "round_freeze_end"))
					{
						switch (g_Options.Misc.event_spam)
						{
						case 1:
							g_Engine->ClientCmd_Unrestricted("say What a wonderful day to use MassasHook"); break;
						case 2:
							g_Engine->ClientCmd_Unrestricted("say Que belo dia para usar MassasHook"); break;
						}
					}

					if (!strcmp(event->GetName(), "player_hurt"))
					{
						auto bitch = event->GetInt("userid");
						auto coolguy49 = event->GetInt("attacker");
						int dmg = event->GetInt("dmg_health");


						auto bitch_index = g_Engine->GetPlayerForUserID(bitch);
						auto coolguy49_index = g_Engine->GetPlayerForUserID(coolguy49);
						C_BaseEntity* bitch_ = (C_BaseEntity*)g_EntityList->GetClientEntity(bitch_index);
						C_BaseEntity* coolguy49_ = (C_BaseEntity*)g_EntityList->GetClientEntity(coolguy49_index);
						player_info_t bitch_info;
						player_info_t coolguy49_info;

						if (coolguy49_ == local)
						{
							G::missedshots--;
							G::hitmarkeralpha = 1.f;
							switch (g_Options.Visuals.Hitsound)
							{
							case 0: break;
							case 1: PlaySoundA(rawData, NULL, SND_ASYNC | SND_MEMORY); break; // Default
							case 2:	PlaySoundA(roblox, NULL, SND_ASYNC | SND_MEMORY); break; // Anime
							case 3:	PlaySoundA(china, NULL, SND_ASYNC | SND_MEMORY); break; // CHINA
							case 4: PlaySoundA(skeethitmarker_wav, NULL, SND_ASYNC | SND_MEMORY); break; // Skeet
							case 5: PlaySoundA(quake, NULL, SND_ASYNC | SND_MEMORY); break;
							}
							
						}

						if (bitch && local && g_Engine->GetPlayerInfo(bitch_index, &bitch_info), g_Engine->GetPlayerInfo(coolguy49_index, &coolguy49_info) && bitch_ && coolguy49_)
						{
							if (g_Options.Visuals.EventLog) {
								if (coolguy49_->GetTeamNum() == local->GetTeamNum())
								{
									g_CVar->ConsoleColorPrintf(Color(g_Options.Colors.MenuTheme[0] * 255, g_Options.Colors.MenuTheme[1] * 255, g_Options.Colors.MenuTheme[2] * 255), "[MassasHook] ");
									g_CVar->ConsoleColorPrintf(Color(94, 219, 72), """%s did %i to %s.\n", coolguy49_info.name, dmg, bitch_info.name);
									std::stringstream string;
									string << coolguy49_info.name << " did " << dmg << " to " << bitch_info.name << ".";
									visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(94, 219, 72), g_Globals->curtime));
								}
								else {
									g_CVar->ConsoleColorPrintf(Color(g_Options.Colors.MenuTheme[0] * 255, g_Options.Colors.MenuTheme[1] * 255, g_Options.Colors.MenuTheme[2] * 255), "[MassasHook] ");
									g_CVar->ConsoleColorPrintf(Color(191, 92, 30), """%s did %i to %s.\n", coolguy49_info.name, dmg, bitch_info.name);
									std::stringstream string;
									string << coolguy49_info.name << " did " << dmg << " to " << bitch_info.name << ".";
									visuals::instance().events.push_front(visuals::loginfo(string.str(), Color(191, 92, 30), g_Globals->curtime));
								}
							}
						}
					}
				}
			}
		private:
			item_purchase_listener  listener;
		};

		item_purchase purchase;