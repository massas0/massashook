#pragma once
#include <set>
#include <map>
#include <unordered_map>

extern void RenderInterface();

struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::string tagName;
};


struct Variables
{
	Variables()
	{

	}
	bool HWIDPassed;
	bool Unload;
	bool Exploit;
	bool clantag;
	bool obs;
	bool Dark = true;

	char ConfigName[52];
	char Tag[32] = "";
	char Name[32] = "";

	struct Ragebot_s
	{
		bool	MainSwitch;
		bool 	Enabled;
		bool 	AutoFire;
		float 	FOV;
		bool 	Silent;
		bool	AutoPistol;
		int		KeyPress;
		bool	AimStep;
		bool AutoFreeze;


		//AA Builder
		bool	BuilderAAs;
		bool	Jitter;
		bool	FJitter;
		bool	LBYBreaker;
		float	BuilderReal;
		float	BuilderFake;
		float	BuilderPitch;
		float	JitterRange;
		float	FJitterRange;


		bool	EnabledAntiAim;
		int		SubAATabs;
		bool	PreAAs;
		int		Pitch;
		int		YawTrue;
		int		YawFake;
		int		YawTrueMove;
		float	PitchAdder;
		float	YawTrueAdder;
		float	YawTrueSide1;
		float	YawTrueSide2;
		int		TrueKeyS;
		float	YawFakeAdder;
		float	YawFakeMove;
		bool	AtTarget;
		bool	Edge;
		bool	KnifeAA;
		bool	FakeLag;
		int		FakeLagAmt;
		bool	FakeChams;


		bool	FriendlyFire;
		int		Hitbox;
		int		Hitscan;
		int fakewalk;
		float lbydel;
		float	Pointscale;
		bool	Multipoint;
		float	Multipoints;

		bool	AntiRecoil;
		bool	AutoWall;
		bool	AutoStop;
		bool	AutoCrouch;
		bool	AutoScope;
		float	MinimumDamageSniper;
		float	MinimumDamageRifle;
		float	MinimumDamagePistol;
		float	MinimumDamageHeavy;
		float	MinimumDamageSmg;
		float	MinimumDamageRevolver;
		bool	Hitchance;
		float	HitchanceSniper;
		float	HitchancePistol;
		float	HitchanceRifle;
		float	HitchanceHeavy;
		float	HitchanceSmgs;
		float	HitchanceRevolver;
		int	Resolver;
		int overrideresolver;
		bool	FakeLagFix;
		bool	PosAdjust;
		int waitfortick;
		bool only_if_possible;
		bool	playerlist;
		int		BAIMkey;
		float	bruteAfterX;
		bool	MovingAA;
	} Ragebot;

	struct
	{
		bool MainSwitch;
		bool backtrack;
		float backtrackTicks;
		bool backtrackD;
		struct
		{
			bool 	Enabled;
			bool AutoPistol;
			bool	Resolver;

		} Aimbot;

		int hitboxrifle;
		int MainKey = 1;
		float MainSmooth;
		float Mainfov;
		float main_random_smooth;
		float main_recoil_min;
		float main_recoil_max;
		float main_randomized_angle;
		bool main_autoshoot;
		int main_bone;
		bool main_silent;


		int hitboxpistol;
		int PistolKey = 6;
		float Pistolfov;
		float PistolSmooth;
		float pistol_random_smooth;
		float pistol_recoil_min;
		float pistol_recoil_max;
		float pistol_randomized_angle;
		bool pistol_autoshoot;
		int pistol_bone;
		bool pistol_silent;


		int hitboxsniper;
		int SniperKey = 6;
		float Sniperfov;
		float SniperSmooth;
		float sniper_random_smooth;
		float sniper_recoil_min;
		float sniper_recoil_max;
		float sniper_randomized_angle;
		bool sniper_autoshoot;
		int sniper_bone;
		bool sniper_silent;



		struct
		{
			bool	Enabled;
			float Delay;
			int Key = 6;
			float hitchance;
			struct
			{
				bool Head;
				bool Arms;
				bool Chest;
				bool Stomach;
				bool Legs;
			} Filter;

		} Triggerbot;

	} Legitbot;

	struct
	{
		bool legitmode;

		int ToggleKey;
		bool TeamESP;
		bool Box;
		int BoxType;
		int health;
		bool Armor;
		bool Money;
		bool VisCheck;
		bool Name;
		bool Info;
		bool Distance;
		bool speclist;
		bool Skeleton;
		bool Callout;
		int Weapon;
		int wpnmode;
		bool Flashed;
		bool AimLine;
		bool angleLines;
		bool barrel;
		int barrelL;
		bool DrawAwall;
		bool LBYIndicator;

		bool Bomb;
		bool resolveMode;

		bool Droppedguns;
		bool Grenades;

		int chamsubtab = 0;

		int chammode;

		int glow_style;

		bool Chams;
		int matierial;
		bool Teamchams;
		bool Chams2;
		int matierial2;
		bool Teamchams2;
		int champlayeralpha;

		bool Chamweapon;
		bool XQZ;
		bool XQZ2;

		bool handcham;
		bool weaponviewcham;
		int chamhandalpha;
		bool fakeangle;
		int weaponmat;
		int handmat;
		int weaponhandalpha;

		bool Crosshair;
		bool xSpread;
		bool xRecoil;

		bool Hitmarker;
		int Hitsound;

		bool EventLog;
		bool SniperCrosshair;
		bool spread;
		bool NoVisualRecoil;
		float FOVChanger;
		float viewmodelChanger = 0;
		bool NoFlash;
		bool NoSmoke;
		int TPKey;
		int TPAng;
		float tpdist = 200;
		bool ThirdPerson = true;
		bool Time = true;

		bool specmode;

		bool noscopeborder;
		bool GrenadePrediction;
		bool bChamsFakeAA;

		int glowsubtab = 0;
		bool glowplayer;
		int playerglowalpha;
		bool teamglow;

		bool weaponglow;
		int weaponglowalpha;

		bool bombglow;
		int bombglowalpha;

		bool debugWindow;

		struct
		{
			float backgroundalpha;
			bool Enabled;
			bool EnemyOnly;
			int range;
			bool Nicks;
		} Radar;

	} Visuals;

	struct
	{
		bool whitelist;
		bool trashtalk;
	} Playerlist;

	struct
	{
		bool antiuntrusted = true;
		bool silentstealer;
		int ragequit;
		bool 	Bhop;
		int 	AutoStrafe;
		int		CircleSKey;
		float divisor;
		float retrack;
		float MinVel;
		int		AirStuckKey;
		int		AutoBlockKey;
		int lagexploit;
		bool memewalk;
		bool legitaa;
		int lagexploitmultiplier = 3;
		float lagexploitvalue;
		float FakeLag;
		bool AdaptiveFakeLag;
		bool nightMode;
		bool colored_walls;
		int event_spam;
		int NameSpammer;
		int NameChangerFix;
		int ChatSpam;
		bool NoName;
		char ChatSpamMode[128] = "massas.pw - Your best choice.";
		char CustomName[128] = "www.massas.pw";
		char CustomTag[128];
		int ClantagChanger;
		float ClanTagSpeed = 1.f;
		bool syncclantag;
		bool SpecList;
		bool FPSBooster;
		int SkyBoxChanger;
		bool namespam;
		bool silentname;
		int spammer;
		int AutoDefuse;
		bool Spam;
		bool isRecording;
		bool isReplaying;
		bool RecordPath;
		bool AutoAccept;
		bool SpoofConfirmation = false;
		bool animatedclantag = false;
		int customtab;
		int Slowmo;

		bool niggatest;
		bool rank_reveal;
		bool auto_accept;
		int WalkRobot;

	} Misc;
	struct
	{
		bool Enabled;

		int knifemodel;

		int glove;
		int gloeskin;


		struct
		{
			bool ChangerEnabled = false;
			int ChangerSkin = 0;
			char ChangerName[32] = "";
			bool stat;
			int statamount;
			bool customstat;
			int customstatamount;
			char Name;
		} W[519];

		std::unordered_map<std::string, std::set<std::string>> weaponSkins;
		std::unordered_map<std::string, skinInfo> skinMap;
		std::unordered_map<std::string, std::string> skinNames;
		std::vector<std::string> weaponNames;
	} Skinchanger;

	struct
	{
		bool	Opened = false;
		int 	Key;
		int		rageSubtab;
		int		ConfigFile = 0;
		int		Theme = 0;
		int		iTab;
		bool	bShowTabs;
		float   flMainAlpha;
		float   flTabsAlpha;
		int currentWeapon;
		int currentColor;
	} Menu;

	struct
	{
		float TeamESP[3] = { 0.17f, 0.73f, 0.20f };
		float EnemyESP[3] = { 0.71f, 0.89f, 0.16f };
		float TeamDLights[3] = { 0, 1.f, 0 };
		float EnemyDLights[3] = { 1.f, 0, 0 };
		float EnemyGlow[3] = { 0.94f, 0.80f, 0.13f };
		float TeamGlow[3] = { 0.17f, 0.73f, 0.20f };
		float WeaponGlow[3] = { 1.f, 1.f, 1.f };
		float C4glow[3] = { 1.f, 0, 1.f };
		float EnemyChamsVis[3] = { 0.17f, 0.37f, 0.87f };
		float EnemyChamsNVis[3] = { 0.91f, 0.71f, 0.11f };
		float TeamChamsVis[3] = { 0.6f, 0.6f, 0.6f };
		float TeamChamsNVis[3] = { 0, 1.f, 0 };
		float WeaponCham[3] = { 1.f, 0, 0 };
		float HandCham[3] = { 1.f, 0, 0 };
		float WeaponViewCham[3] = { 1.f, 0, 0 };
		float MenuTheme[3] = { 0.65f, 0.12f, 0.12f }; 
		float MenuTheme2[3] = { 0.8f, 0.8f, 0.8f };
		float MenuText[3] = { 0.95f, 0.95f, 0.95f };
		float ColWalls[3] = { 0.55f, 0.19f, 0.21f };
	}Colors;
};

extern Variables g_Options;