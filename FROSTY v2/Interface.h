#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include "Themes.h"
#include <cctype>
#include "Logging.h"
#include "Listener.h"
#include <ShlObj_core.h>

std::vector<std::string> configs;


void GetConfigMassive()
{
	//get all files on folder

	configs.clear();

	static char path[MAX_PATH];
	std::string szPath1;

	if (!SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path)))
		return;

	szPath1 = std::string(path) + "\\MassasHook Configs\\*";


	WIN32_FIND_DATA FindFileData;
	HANDLE hf;

	configs.push_back("Chose Config");

	hf = FindFirstFile(szPath1.c_str(), &FindFileData);
	if (hf != INVALID_HANDLE_VALUE) {
		do {
			std::string filename = FindFileData.cFileName;

			if (filename == ".")
				continue;

			if (filename == "..")
				continue;

			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				if (filename.find(".cfg") != std::string::npos)
				{
					configs.push_back(std::string(filename));
				}
			}
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}

std::vector<std::pair<int, const char*>> guns =
{
	{ WEAPON_AK47,			"AK-47" },
	{ WEAPON_AUG,			"AUG" },
	{ WEAPON_AWP,			"AWP" },
	{ WEAPON_CZ75,			"CZ75-A" },
	{ WEAPON_DEAGLE,		"Desert Eagle" },
	{ WEAPON_DUALS,			"Dual Barettas" },
	{ WEAPON_FAMAS,			"Famas" },
	{ WEAPON_FIVE7, 	"Five-Seven" },
	{ WEAPON_GALIL,		"Galil-AR" },
	{ WEAPON_G3SG1,			"G3SG1" },
	{ WEAPON_GLOCK,			"Glock-18" },
	{ WEAPON_M249,			"M249" },
	{ WEAPON_M4A1S, "M4A1-S" },
	{ WEAPON_M4A1,			"M4A4" },
	{ WEAPON_MAC10,			"MAC-10" },
	{ WEAPON_MAG7,			"Mag-7" },
	{ WEAPON_MP7,			"MP7" },
	{ WEAPON_MP9,			"MP9" },
	{ WEAPON_NEGEV,			"Negev" },
	{ WEAPON_NOVA,			"Nova" },
	{ WEAPON_P2000,		"P2000" },
	{ WEAPON_P250,			"P250" },
	{ WEAPON_P90,			"P90" },
	{ WEAPON_BIZON,			"PP-Bizon" },
	{ WEAPON_REVOLVER,		"R8 Revolver" },
	{ WEAPON_SAWEDOFF,		"Sawed-Off" },
	{ WEAPON_SCAR20,		"SCAR-20" },
	{ WEAPON_SG553,			"SG-556" },
	{ WEAPON_SCOUT,			"Scout" },
	{ WEAPON_TEC9,			"Tec-9" },
	{ WEAPON_UMP45,			"UMP-45" },
	{ WEAPON_USPS,	"USP-s" },
	{ WEAPON_XM1014, "XM1014" },
	{ WEAPON_KNIFE_BAYONET,					"Bayonet Knife" },
	{ WEAPON_KNIFE_FLIP,					"Flip Knife" },
	{ WEAPON_KNIFE_GUT,					"Gut Knife" },
	{ WEAPON_KNIFE_KARAMBIT,					"Karambit Knife" },
	{ WEAPON_KNIFE_M9BAYONET,					"M9 Bayonet Knife" },
	{ WEAPON_KNIFE_HUNTSMAN,					"Hunstman Knife" },
	{ WEAPON_KNIFE_FALCHION,					"Falchion Knife" },
	{ WEAPON_KNIFE_BOWIE,					"Bowie Knife" },
	{ WEAPON_KNIFE_BUTTERFLY,					"Butterfly Knife" },
	{ WEAPON_KNIFE_DAGGER, "Shadow Daggers" },
};


std::vector<std::pair<int, const char*>> weapon_skins =
{
	{ 0,   "Default" },
	{ 692, "Night Riot"},
	{ 693 , "Flame Test" },
	{ 697 , "Black Sand"},
	{ 700 , "Urban Hazard (P2000)"},
	{ 701 , "Grip"},
	{ 702 , "Aloha"},
	{ 645 , "Oxide Blaze"},
	{ 694 , "Moonrise"},
	{ 698 , "Lionfish"},
	{ 699 , "Wild Six"},
	{ 703 , "SWAG - 7"},
	{ 704 , "Arctic Wolf"},
	{ 690 , "Stymphalian"},
	{ 691 , "Mortis"},
	{ 705 , "Cortex"},
	{ 695 , "Neo - Noir"},
	{ 696, "Bloodsport (MP7)"},
	{ 361, "Abyss" },
	{ 253, "Acid Fade" },
	{ 598, "Aerial" },
	{ 154, "Afterimage" },
	{ 609, "Airlock" },
	{ 455, "Akihabara Accept" },
	{ 523, "Amber Fade (Revolver)" },
	{ 246, "Amber Fade" },
	{ 210, "Anodized Gunmetal" },
	{ 197, "Anodized Navy" },
	{ 28,  "Anodized Navy" },
	{ 286, "Antique (Nova)" },
	{ 306, "Antique (PP Bizon)" },
	{ 460, "Aqua Terrace" },
	{ 474, "Aquamarine Revenge" },
	{ 6,   "Arctic Camo" },
	{ 583, "Aristocrat" },
	{ 423, "Armor Core" },
	{ 242, "Army Mesh" },
	{ 245, "Army Recon" },
	{ 298, "Army Sheen" },
	{ 234, "Ash Wood" },
	{ 279, "Asiimov (AWP)" },
	{ 255, "Asiimov (M4A4)" },
	{ 551, "Asiimov (P250)" },
	{ 359, "Asiimov (P90)" },
	{ 442, "Asterion" },
	{ 553, "Atlas" },
	{ 301, "Atomic Alloy" },
	{ 573, "Autotronic (Bayonet)" },
	{ 574, "Autotronic (Flip)" },
	{ 575, "Autotronic (Gut)" },
	{ 576, "Autotronic (Karambit)" },
	{ 577, "Autotronic (M9 Bayonet)" },
	{ 520, "Avalanche" },
	{ 229, "Azure Zebra" },
	{ 174, "BOOM" },
	{ 459, "Bamboo Forest" },
	{ 457, "Bamboo Print" },
	{ 458, "Bamboo Shadow" },
	{ 383, "Basilisk" },
	{ 9,   "Bengal Tiger" },
	{ 503, "Big Iron" },
	{ 549, "Bioleak" },
	{ 172, "Black Laminate (AK-47)" },
	{ 563, "Black Laminate (Bayonet)" },
	{ 564, "Black Laminate (Flip)" },
	{ 565, "Black Laminate (Gut)" },
	{ 566, "Black Laminate (Karambit)" },
	{ 567, "Black Laminate (M9 Bayonet)" },
	{ 190, "Black Limba" },
	{ 629, "Black Sand" },
	{ 557, "Black Tie" },
	{ 166, "Blaze Orange" },
	{ 37,  "Blaze" },
	{ 228, "Blind Spot" },
	{ 75,  "Blizzard Marbleized" },
	{ 217, "Blood Tiger" },
	{ 222, "Blood in the Water" },
	{ 597, "Bloodsport" },
	{ 62,  "Bloomstick" },
	{ 278, "Blue Fissure" },
	{ 226, "Blue Laminate" },
	{ 96,  "Blue Spruce" },
	{ 42,  "Blue Steel" },
	{ 13,  "Blue Streak" },
	{ 216, "Blue Titanium" },
	{ 370, "Bone Machine" },
	{ 27,  "Bone Mask" },
	{ 193, "Bone Pile" },
	{ 77,  "Boreal Forest" },
	{ 159, "Brass" },
	{ 317, "Bratatat" },
	{ 330, "Briar" },
	{ 615, "Briefing" },
	{ 578, "Bright Water (Knife)" },
	{ 579, "Bright Water (Knife)" },
	{ 189, "Bright Water (M4A1-S)" },
	{ 425, "Bronze Deco" },
	{ 39,  "Bulldozer" },
	{ 155, "Bullet Rain" },
	{ 479, "Bunsen Burner" },
	{ 364, "Business Class" },
	{ 632, "Buzz Kill" },
	{ 299, "Caged Steel" },
	{ 339, "Caiman" },
	{ 240, "CaliCamo" },
	{ 3,   "Candy Apple" },
	{ 93,  "Caramel" },
	{ 70,  "Carbon Fiber" },
	{ 391, "Cardiac" },
	{ 589, "Carnivore" },
	{ 388, "Cartel" },
	{ 394, "Cartel" },
	{ 528, "Cartel" },
	{ 44,  "Case Hardened" },
	{ 399, "Catacombs" },
	{ 379, "Cerberus" },
	{ 327, "Chainmail" },
	{ 325, "Chalice" },
	{ 280, "Chameleon" },
	{ 548, "Chantico's Fire" },
	{ 398, "Chatterbox" },
	{ 376, "Chemical Green" },
	{ 593, "Chopper" },
	{ 438, "Chronos" },
	{ 627, "Cirrus" },
	{ 346, "Coach Class" },
	{ 499, "Cobalt Core" },
	{ 231, "Cobalt Disruption" },
	{ 267, "Cobalt Halftone" },
	{ 249, "Cobalt Quartz" },
	{ 67,  "Cold Blooded" },
	{ 47,  "Colony" },
	{ 343, "Commuter" },
	{ 110, "Condemned" },
	{ 351, "Conspiracy" },
	{ 373, "Contamination" },
	{ 46,  "Contractor" },
	{ 22,  "Contrast Spray" },
	{ 274, "Copper Galaxy" },
	{ 41,  "Copper" },
	{ 10,  "Copperhead" },
	{ 509, "Corinthian" },
	{ 281, "Corporal" },
	{ 181, "Corticera (AWP)" },
	{ 184, "Corticera (P2000)" },
	{ 462, "Counter Terrace" },
	{ 466, "Crimson Kimono" },
	{ 12,  "Crimson Web (Deagle)" },
	{ 232, "Crimson Web" },
	{ 310, "Curse" },
	{ 92,  "Cyanospatter" },
	{ 360, "Cyrex (M4A1-S)" },
	{ 312, "Cyrex (SCAR-20)" },
	{ 487, "Cyrex (SG 553)" },
	{ 637, "Cyrex (USP)" },
	{ 444, "Daedalus" },
	{ 411, "Damascus Steel (Knife)" },
	{ 410, "Damascus Steel (Knife)" },
	{ 247, "Damascus Steel (SG 553)" },
	{ 329, "Dark Age" },
	{ 60,  "Dark Water" },
	{ 386, "Dart" },
	{ 471, "Daybreak" },
	{ 610, "Dazzle" },
	{ 403, "Deadly Poison" },
	{ 293, "Death Rattle" },
	{ 156, "Death by Kitty" },
	{ 392, "Delusion" },
	{ 195, "Demeter" },
	{ 153, "Demolition" },
	{ 8,   "Desert Storm" },
	{ 311, "Desert Warfare" },
	{ 336, "Desert-Strike" },
	{ 355, "Desert-Strike" },
	{ 588, "Desolate Space" },
	{ 319, "Detour" },
	{ 603, "Directive" },
	{ 429, "Djinn" },
	{ 178, "Doomkitty" },
	{ 417, "Doppler (Black Pearl)" },
	{ 418, "Doppler (Phase 1)" },
	{ 419, "Doppler (Phase 2)" },
	{ 420, "Doppler (Phase 3)" },
	{ 421, "Doppler (Phase 4)" },
	{ 415, "Doppler (Ruby)" },
	{ 416, "Doppler (Sapphire)" },
	{ 400, "?? Dragon King" },
	{ 344, "Dragon Lore" },
	{ 48,  "Dragon Tattoo" },
	{ 624, "Dragonfire" },
	{ 199, "Dry Season" },
	{ 491, "Dualing Dragons" },
	{ 447, "Duelist" },
	{ 428, "Eco" },
	{ 227, "Electric Hive" },
	{ 422, "Elite Build (AK-47)" },
	{ 525, "Elite Build (AWP)" },
	{ 486, "Elite Build (P90)" },
	{ 182, "Emerald Dragon" },
	{ 300, "Emerald Pinstripe" },
	{ 196, "Emerald" },
	{ 453, "Emerald" },
	{ 480, "Evil Daimyo" },
	{ 590, "Exo" },
	{ 207, "Facets" },
	{ 522, "Fade (Revolver)" },
	{ 38,  "Fade" },
	{ 176, "Faded Zebra" },
	{ 169, "Fallout Warning" },
	{ 378, "Fallout Warning" },
	{ 389, "Fire Elemental" },
	{ 180, "Fire Serpent" },
	{ 546, "Firefight" },
	{ 385, "Firestarter" },
	{ 341, "First Class" },
	{ 345, "First Class" },
	{ 631, "Flashback" },
	{ 541, "Fleet Flock" },
	{ 493, "Flux" },
	{ 5,   "Forest DDPAT" },
	{ 25,  "Forest Leaves" },
	{ 78,  "Forest Night" },
	{ 352, "Fowl Play" },
	{ 295, "Franklin" },
	{ 580, "Freehand" },
	{ 581, "Freehand" },
	{ 582, "Freehand" },
	{ 490, "Frontside Misty" },
	{ 552, "Fubar" },
	{ 524, "Fuel Injector (AK-47)" },
	{ 614, "Fuel Injector (Tec-9)" },
	{ 508, "Fuel Rod" },
	{ 250, "Full Stop" },
	{ 568, "Gamma Doppler (Emerald Marble)" },
	{ 569, "Gamma Doppler (Phase 1)" },
	{ 570, "Gamma Doppler (Phase 2)" },
	{ 571, "Gamma Doppler (Phase 3)" },
	{ 572, "Gamma Doppler (Phase 4)" },
	{ 243, "Gator Mesh" },
	{ 225, "Ghost Camo" },
	{ 554, "Ghost Crusader" },
	{ 634, "Gila" },
	{ 111, "Glacier Mesh" },
	{ 497, "Golden Coil" },
	{ 185, "Golden Koi" },
	{ 436, "Grand Prix" },
	{ 21,  "Granite Marbleized" },
	{ 212, "Graphite" },
	{ 214, "Graphite" },
	{ 104, "Grassland Leaves" },
	{ 95,  "Grassland" },
	{ 188, "Graven" },
	{ 294, "Green Apple" },
	{ 502, "Green Marine" },
	{ 366, "Green Plaid" },
	{ 384, "Griffin" },
	{ 611, "Grim" },
	{ 381, "Grinder" },
	{ 406, "Grotto" },
	{ 2,   "Groundwater" },
	{ 209, "Groundwater" },
	{ 257, "Guardian" },
	{ 290, "Guardian" },
	{ 15,  "Gunsmoke" },
	{ 439, "Hades" },
	{ 328, "Hand Cannon" },
	{ 485, "Handgun" },
	{ 594, "Harvester" },
	{ 198, "Hazard" },
	{ 284, "Heat" },
	{ 431, "Heat" },
	{ 291, "Heaven Guard" },
	{ 314, "Heaven Guard" },
	{ 273, "Heirloom" },
	{ 220, "Hemoglobin" },
	{ 218, "Hexane" },
	{ 390, "Highwayman" },
	{ 219, "Hive" },
	{ 33,  "Hot Rod" },
	{ 445, "Hot Rod" },
	{ 377, "Hot Shot" },
	{ 309, "Howl" },
	{ 241, "Hunting Blind" },
	{ 456, "Hydroponic" },
	{ 475, "Hyper Beast (AWP)" },
	{ 430, "Hyper Beast (M4A1-S)" },
	{ 537, "Hyper Beast (Nova)" },
	{ 61,  "Hypnotic" },
	{ 440, "Icarus Fell" },
	{ 599, "Ice Cap" },
	{ 472, "Impact Drill" },
	{ 591, "Imperial Dragon" },
	{ 515, "Imperial" },
	{ 536, "Impire" },
	{ 602, "Imprint" },
	{ 333, "Indigo" },
	{ 592, "Iron Clad" },
	{ 623, "Ironwork" },
	{ 171, "Irradiated Alert" },
	{ 303, "Isaac" },
	{ 357, "Ivory" },
	{ 316, "Jaguar" },
	{ 539, "Jambiya" },
	{ 340, "Jet Set" },
	{ 542, "Judgement of Anubis" },
	{ 202, "Jungle DDPAT" },
	{ 147, "Jungle Dashed" },
	{ 122, "Jungle Spray" },
	{ 16,  "Jungle Tiger" },
	{ 151, "Jungle" },
	{ 205, "Jungle" },
	{ 265, "Kami (Five-SeveN)" },
	{ 308, "Kami (Galil-AR)" },
	{ 504, "Kill Confirmed" },
	{ 326, "Knight" },
	{ 356, "Koi" },
	{ 527, "Kumicho Dragon" },
	{ 362, "Labyrinth" },
	{ 534, "Lapis Gator" },
	{ 540, "Lead Conduit" },
	{ 342, "Leather" },
	{ 26,  "Lichen Dashed" },
	{ 51,  "Lightning Strike" },
	{ 596, "Limelight" },
	{ 558, "Lore (Bayonet)" },
	{ 559, "Lore (Flip)" },
	{ 560, "Lore (Gut)" },
	{ 561, "Lore (Karambit)" },
	{ 562, "Lore (M9 Bayonet)" },
	{ 483, "Loudmouth" },
	{ 266, "Magma" },
	{ 402, "Malachite" },
	{ 395, "Man-o'-war (AWP)" },
	{ 432, "Man-o'-war (Negev)" },
	{ 413, "Marble Fade" },
	{ 261, "Marina" },
	{ 321, "Master Piece" },
	{ 200, "Mayan Dreams" },
	{ 587, "Mecha Industries (M4A1-S)" },
	{ 626, "Mecha Industries (Famas)" },
	{ 446, "Medusa" },
	{ 258, "Mehndi" },
	{ 177, "Memento" },
	{ 34,  "Metallic DDPAT" },
	{ 296, "Meteorite" },
	{ 468, "Midnight Storm" },
	{ 441, "Minotaur's Labyrinth" },
	{ 467, "Mint Kimono" },
	{ 164, "Modern Hunter" },
	{ 335, "Module" },
	{ 427, "Monkey Business" },
	{ 450, "Moon in Libra" },
	{ 204, "Mosaico" },
	{ 90,  "Mudder" },
	{ 404, "Muertos" },
	{ 382, "Murky" },
	{ 397, "Naga" },
	{ 496, "Nebula Crusader" },
	{ 538, "Necropos" },
	{ 481, "Nemesis" },
	{ 464, "Neon Kimono" },
	{ 600, "Neon Revolution" },
	{ 433, "Neon Rider" },
	{ 477, "Neural Net" },
	{ 236, "Night Ops" },
	{ 40,  "Night" },
	{ 223, "Nightshade" },
	{ 254, "Nitro" },
	{ 322, "Nitro" },
	{ 372, "Nuclear Garden" },
	{ 168, "Nuclear Threat" },
	{ 179, "Nuclear Threat" },
	{ 369, "Nuclear Waste" },
	{ 211, "Ocean Foam" },
	{ 213, "Ocean Foam" },
	{ 550, "Oceanic" },
	{ 365, "Olive Plaid" },
	{ 545, "Orange Crash" },
	{ 83,  "Orange DDPAT" },
	{ 465, "Orange Kimono" },
	{ 141, "Orange Peel" },
	{ 434, "Origami" },
	{ 313, "Orion" },
	{ 349, "Osiris" },
	{ 36,  "Ossified" },
	{ 518, "Outbreak" },
	{ 183, "Overgrowth" },
	{ 157, "Palm" },
	{ 201, "Palm" },
	{ 448, "Pandora's Box" },
	{ 276, "Panther" },
	{ 454, "Para Green" },
	{ 443, "Pathfinder" },
	{ 608, "Petroglyph" },
	{ 584, "Phobos" },
	{ 526, "Photic Zone" },
	{ 347, "Pilot" },
	{ 84,  "Pink DDPAT" },
	{ 251, "Pit Viper" },
	{ 506, "Point Disarray" },
	{ 315, "Poison Dart" },
	{ 74,  "Polar Camo" },
	{ 107, "Polar Mesh" },
	{ 435, "Pole Position" },
	{ 622, "Polymer" },
	{ 449, "Poseidon" },
	{ 514, "Power Loader" },
	{ 612, "Powercore" },
	{ 535, "Praetorian" },
	{ 170, "Predator" },
	{ 556, "Primal Saber" },
	{ 260, "Pulse" },
	{ 287, "Pulse" },
	{ 338, "Pulse" },
	{ 407, "Quicksilver" },
	{ 167, "Radiation Hazard" },
	{ 375, "Radiation Hazard" },
	{ 498, "Rangeen" },
	{ 484, "Ranger" },
	{ 555, "Re-Entry" },
	{ 367, "Reactor" },
	{ 595, "Reboot" },
	{ 543, "Red Astor" },
	{ 275, "Red FragCam" },
	{ 14,  "Red Laminate" },
	{ 348, "Red Leather" },
	{ 320, "Red Python" },
	{ 248, "Red Quartz" },
	{ 259, "Redline" },
	{ 282, "Redline" },
	{ 307, "Retribution" },
	{ 510, "Retrobution" },
	{ 507, "Ricochet" },
	{ 488, "Riot" },
	{ 263, "Rising Skull" },
	{ 318, "Road Rash" },
	{ 478, "Rocket Pop" },
	{ 604, "Roll Cage" },
	{ 262, "Rose Iron" },
	{ 332, "Royal Blue" },
	{ 625, "Royal Consorts" },
	{ 532, "Royal Legion" },
	{ 512, "Royal Paladin" },
	{ 482, "Ruby Poison Dart" },
	{ 203, "Rust Coat" },
	{ 323, "Rust Coat" },
	{ 414, "Rust Coat" },
	{ 72,  "Safari Mesh" },
	{ 119, "Sage Spray" },
	{ 148, "Sand Dashed" },
	{ 208, "Sand Dune" },
	{ 99,  "Sand Dune" },
	{ 116, "Sand Mesh" },
	{ 630, "Sand Scale" },
	{ 124, "Sand Spray" },
	{ 264, "Sandstorm" },
	{ 289, "Sandstorm" },
	{ 175, "Scorched" },
	{ 71,  "Scorpion" },
	{ 605, "Scumbria (Five-SeveN)" },
	{ 505, "Scumbria (XM1014)" },
	{ 473, "Seabird" },
	{ 405, "Serenity" },
	{ 288, "Sergeant" },
	{ 221, "Serum" },
	{ 368, "Setting Sun" },
	{ 636, "Shallow Grave" },
	{ 516, "Shapewood" },
	{ 192, "Shattered" },
	{ 452, "Shipping Forecast" },
	{ 252, "Silver Quartz" },
	{ 32,  "Silver" },
	{ 11,  "Skulls" },
	{ 304, "Slashed" },
	{ 59,  "Slaughter" },
	{ 616, "Slipstream" },
	{ 30,  "Snake Camo" },
	{ 633, "Sonar" },
	{ 500, "Special Delivery" },
	{ 547, "Spectre" },
	{ 194, "Spitfire" },
	{ 165, "Splash Jam" },
	{ 162, "Splash" },
	{ 43, "Stained" },
	{ 277, "Stainless" },
	{ 230, "Steel Disruption" },
	{ 628, "Stinger" },
	{ 494, "Stone Cold" },
	{ 100, "Storm" },
	{ 371, "Styx" },
	{ 451, "Sun in Leo" },
	{ 469, "Sunset Storm ?" },
	{ 470, "Sunset Storm ?" },
	{ 358, "Supernova" },
	{ 492, "Survivor Z" },
	{ 601, "Syd Mead" },
	{ 401, "System Lock" },
	{ 337, "Tatter" },
	{ 244, "Teardown" },
	{ 521, "Teclu Burner" },
	{ 191, "Tempest" },
	{ 463, "Terrace" },
	{ 285, "Terrain" },
	{ 533, "The Battlestar" },
	{ 511, "The Executioner" },
	{ 269, "The Fuschia Is Now" },
	{ 256, "The Kraken" },
	{ 519, "Tiger Moth" },
	{ 409, "Tiger Tooth" },
	{ 350, "Tigris" },
	{ 272, "Titanium Bit" },
	{ 101, "Tornado" },
	{ 206, "Tornado" },
	{ 305, "Torque" },
	{ 489, "Torque" },
	{ 374, "Toxic" },
	{ 393, "Tranquility" },
	{ 363, "Traveler" },
	{ 268, "Tread Plate" },
	{ 613, "Triarch" },
	{ 283, "Trigon" },
	{ 530, "Triumvirate" },
	{ 233, "Tropical Storm" },
	{ 635, "Turf" },
	{ 297, "Tuxedo" },
	{ 437, "Twilight Galaxy" },
	{ 334, "Twist" },
	{ 98,  "Ultraviolet" },
	{ 271, "Undertow" },
	{ 17,  "Urban DDPAT" },
	{ 149, "Urban Dashed" },
	{ 354, "Urban Hazard" },
	{ 387, "Urban Hazard" },
	{ 143, "Urban Masked" },
	{ 135, "Urban Perforated" },
	{ 237, "Urban Rubble" },
	{ 396, "Urban Shock" },
	{ 426, "Valence" },
	{ 529, "Valence" },
	{ 238, "VariCamo Blue" },
	{ 235, "VariCamo" },
	{ 606, "Ventilator" },
	{ 544, "Ventilators" },
	{ 270, "Victoria" },
	{ 585, "Violent Daimyo" },
	{ 20,  "Virus" },
	{ 302, "Vulcan" },
	{ 158, "Walnut" },
	{ 638, "Wasteland Princess" },
	{ 380, "Wasteland Rebel (AK-47)" },
	{ 586, "Wasteland Rebel (Glock)" },
	{ 353, "Water Elemental" },
	{ 224, "Water Sigil" },
	{ 186, "Wave Spray" },
	{ 136, "Waves Perforated" },
	{ 607, "Weasel" },
	{ 102, "Whiteout" },
	{ 73,  "Wings" },
	{ 501, "Wingshot" },
	{ 76,  "Winter Forest" },
	{ 424, "Worm God" },
	{ 495, "Wraiths" },
	{ 215, "X-Ray" },
	{ 476, "Yellow Jacket" },
	{ 517, "Yorick" },
	{ 187, "Zirka" },
};

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define BUILDSTAMP ( __DATE__ )
typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;

void FullUpdate()
{
	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(U::FindPattern("engine.dll", reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();
}

//--------------------------------------------
//Weapon CFG stuff

const char* weaponnames(int id)
{
	switch (id)
	{
	case 1:
		return "Desert Eagle";
	case 2:
		return "Dual Barettas";
	case 3:
		return "Five-Seven";
	case 4:
		return "Glock-17";
	case 7:
		return "AK-47";
	case 8:
		return "AUG";
	case 9:
		return "AWP";
	case 10:
		return "Famas";
	case 11:
		return "G3SG1";
	case 13:
		return "Galil-AR";
	case 14:
		return "M249";
	case 60:
		return "M4A1-S";
	case 16:
		return "M4A4";
	case 17:
		return "Mac-10";
	case 19:
		return "P90";
	case 24:
		return "UMP-45";
	case 25:
		return "XM1014";
	case 26:
		return "PP-Bizon";
	case 27:
		return "Mag-7";
	case 28:
		return "Negev";
	case 29:
		return "Sawed-Off";
	case 30:
		return "Tec-9";
	case 32:
		return "P2000";
	case 33:
		return "MP7";
	case 34:
		return "MP9";
	case 35:
		return "Nova";
	case 36:
		return "P250";
	case 38:
		return "SCAR-20";
	case 39:
		return "SG556";
	case 40:
		return "SSG08";
	case 61:
		return "USP-S";
	case 63:
		return "CZ75-A";
	case 64:
		return "R8 Revolver";
	case 508:
		return "M9 Bayonet";
	case 500:
		return "Bayonet";
	case 505:
		return "Flip Knife";
	case 506:
		return "Gut Knife";
	case 507:
		return "Karambit";
	case 509:
		return "Hunstman Knife";
	case 512:
		return "Falchion";
	case 514:
		return "Bowie Knife";
	case 515:
		return "Butterfly";
	case 516:
		return "Shadow Daggers";

	default:
		return "";
	}
}

bool IsUtility(ItemDefinitionIndexx index)
{
	switch (index)
	{
	case ItemDefinitionIndexx::ITEM_NONE:
	case ItemDefinitionIndexx::WEAPON_C4:
	case ItemDefinitionIndexx::WEAPON_FLASH:
	case ItemDefinitionIndexx::WEAPON_HE:
	case ItemDefinitionIndexx::WEAPON_INC:
	case ItemDefinitionIndexx::WEAPON_MOLOTOV:
	case ItemDefinitionIndexx::WEAPON_SMOKE:
	case ItemDefinitionIndexx::WEAPON_DECOY:
	case ItemDefinitionIndexx::WEAPON_KNIFE_T:
	case ItemDefinitionIndexx::WEAPON_KNIFE_CT:
	case ItemDefinitionIndexx::GLOVE_T_SIDE:
	case ItemDefinitionIndexx::GLOVE_CT_SIDE:
	case ItemDefinitionIndexx::GLOVE_SPORTY:
	case ItemDefinitionIndexx::GLOVE_SLICK:
	case ItemDefinitionIndexx::GLOVE_LEATHER_WRAP:
	case ItemDefinitionIndexx::GLOVE_STUDDED_BLOODHOUND:
	case ItemDefinitionIndexx::GLOVE_MOTORCYCLE:
	case ItemDefinitionIndexx::GLOVE_SPECIALIST:
	case ItemDefinitionIndexx::GLOVE_HYDRA:
		return true;
	default:
		return false;
	}
}

bool Contains(const std::string &word, const std::string &sentence) {
	if (word == "" || sentence == "")
		return true;

	return sentence.find(word) != std::string::npos;
}

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), (int(*)(int))std::tolower);

	return str;
}
//--------------------------------------------
bool defaultk;
bool bayonet;
bool flip;
bool gut;
bool karambit;
bool m9bayonet;
bool huntsman;
bool falchion;
bool bowie;
bool butterfly;
bool daggers;

void doknives(bool* disknife)
{
	defaultk = false;
	bayonet = false;
	flip = false;
	gut = false;
	karambit = false;
	m9bayonet = false;
	huntsman = false;
	falchion = false;
	bowie = false;
	butterfly = false;
	daggers = false;

	*disknife = true;
}

const char* HitmarkSound[] =
{
	"Disabled",
	"Default",
	"Roblox",
	"Trump",
	"Metallic",
	"Quake"
};

const char* AngleStyles[] =
{
	"Disabled",
	"Ground",
	"Crosshair"
};

const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}


void RenderInterface() {

	auto& style = ImGui::GetStyle();
	bool is_renderer_active = renderer->IsActive();
	static int tab;

	if (is_renderer_active)
	{
		int x, y;
		g_Engine->GetScreenSize(x, y);

		style.Colors[ImGuiCol_TitleBg] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		if (!g_Options.Dark) {

			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.4f);
			style.Colors[ImGuiCol_ComboBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
			style.Colors[Massashook_LisboxBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
			style.Colors[ImGuiCol_Text] = ImVec4(0.05f, 0.05f, 0.05f, 1.f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
		}
		else
		{
			style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.4f);
			style.Colors[ImGuiCol_ComboBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.f);
			style.Colors[Massashook_LisboxBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.f);
			style.Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.9f, 0.9f, 0.9f, 1.f);
		}
		style.Colors[ImGuiCol_Button] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], .75f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.85f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.4f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.8f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.9f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.8f);
		style.Colors[ImGuiCol_Header] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);

		static bool legit, rage, visuals, mis, skins;
		static bool options = true;

		{
			using namespace ImGui;

			Begin("MassasHook", &is_renderer_active, ImVec2(700, 500), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

			style.ItemSpacing = ImVec2(0, 0);

			{
				if (ToggleButton("RAGE", &rage, ImVec2(114, 40))) 
				{
					legit = false;
					visuals = false;
					mis = false;
					skins = false;
					options = false;
				} SameLine();
				if (ToggleButton("LEGIT", &legit, ImVec2(114, 40))) 
				{
					rage = false;
					visuals = false;
					mis = false;
					skins = false;
					options = false;
				} SameLine();
				if (ToggleButton("VISUALS", &visuals, ImVec2(114, 40)))  
				{
					rage = false;
					legit = false;
					mis = false;
					skins = false;
					options = false;
				} SameLine();
				if (ToggleButton("MISC", &mis, ImVec2(114, 40)))  
				{
					rage = false; legit = false;
					visuals = false; skins = false;
					options = false;
				} SameLine();
				if (ToggleButton("SKINS", &skins, ImVec2(114, 40)))  
				{
					legit = false; rage = false; visuals = false;
					mis = false; options = false;
				} SameLine();
				if (ToggleButton("OPTIONS", &options, ImVec2(114, 40)))
				{
					rage = false; legit = false; visuals = false; mis = false, skins = false;
				} 
			}
		
			style.ItemSpacing = ImVec2(8, 8);

			Spacing();
			static int selectedcfg = 0;
			static std::string cfgname = "default";

			if (rage)
			{

				Checkbox("Active", &g_Options.Ragebot.MainSwitch);
				Separator();

				Columns(2, 0, false);
				BeginChild(40, ImVec2(-1, 400), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Aimbot");
				Separator();
				Spacing();

				Checkbox(("Enabled"), &g_Options.Ragebot.Enabled);
				Checkbox(("Silent Aim"), &g_Options.Ragebot.Silent);
				Hotkey(("Key"), &g_Options.Ragebot.KeyPress);
				SliderFloat(("FOV"), &g_Options.Ragebot.FOV, 0, 180, "%.0f");
				Checkbox(("Auto Fire"), &g_Options.Ragebot.AutoFire);
				Checkbox(("Auto Scope"), &g_Options.Ragebot.AutoScope);
				Checkbox(("Auto Stop"), &g_Options.Ragebot.AutoFreeze);
				Checkbox(("No Recoil"), &g_Options.Ragebot.AntiRecoil);
				Spacing();
				Checkbox(("Friendly Fire"), &g_Options.Ragebot.FriendlyFire);
				Combo(("Resolver"), &g_Options.Ragebot.Resolver, "Off\0\rSimple\0\rBruteforece\0\rPrediction\0\rSmart\0\0", -1);
				Combo(("Hitbox"), &g_Options.Ragebot.Hitbox, RaimBones, ARRAYSIZE(RaimBones));
				Combo(("Hitscan"), &g_Options.Ragebot.Hitscan, hitscan, ARRAYSIZE(hitscan));

				EndChild();
				NextColumn();

				static bool weapons = true;
				static bool aa = false;

				if (ToggleButton("Weapons", &weapons, ImVec2(338, 40))) aa = false;
				if (ToggleButton("Anti Aim", &aa, ImVec2(338, 40))) weapons = false;

				if (aa)
				{
					BeginChild(48, ImVec2(-1, 303), true, 0);
					TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Anti-Aim");
					Separator();
					Spacing();

					ImGui::Checkbox(("Enabled"), &g_Options.Ragebot.EnabledAntiAim);
					Spacing();
					Combo("Pitch", &g_Options.Ragebot.Pitch, "Off\0\rDown\0\rUp\0\0", -1);
					Combo("Yaw Real", &g_Options.Ragebot.YawTrue, "Off\0\rBackwards\0\rSideways\0\rBackJitter\0\0", -1);
					if (g_Options.Ragebot.YawTrue == 2)
						Hotkey("Side Switch", &g_Options.Ragebot.TrueKeyS);
					Combo("Yaw Fake", &g_Options.Ragebot.YawFake, "Off\0\rBackwards\0\rSideways\0\rBackJitter\0\0", -1);

					EndChild();
				}
				if (weapons)
				{
					BeginChild(49, ImVec2(-1, 303), true, 0);
					TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Weapon Settings");
					Separator();
					Spacing();

					Text("Min Damage");
					SliderFloat(("Snipers##1"), &g_Options.Ragebot.MinimumDamageSniper, 1.f, 100.f, "%.0f");
					SliderFloat(("Rifles##1"), &g_Options.Ragebot.MinimumDamageRifle, 1.f, 100.f, "%.0f");
					SliderFloat(("Pistols##1"), &g_Options.Ragebot.MinimumDamagePistol, 1.f, 100.f, "%.0f");
					SliderFloat(("Heavies##1"), &g_Options.Ragebot.MinimumDamageHeavy, 1.f, 100.f, "%.0f");
					SliderFloat(("SMGs##1"), &g_Options.Ragebot.MinimumDamageSmg, 1.f, 100.f, "%.0f");
					SliderFloat(("Revolver / Deagle##1"), &g_Options.Ragebot.MinimumDamageRevolver, 1.f, 100.f, "%.0f");
					Spacing();
					Text("Hitchance");
					SliderFloat(("Snipers"), &g_Options.Ragebot.HitchanceSniper, 0.f, 100.f, "%.0f");
					SliderFloat(("Rifles"), &g_Options.Ragebot.HitchanceRifle, 0.f, 100.f, "%.0f");
					SliderFloat(("Pistols"), &g_Options.Ragebot.HitchancePistol, 0.f, 100.f, "%.0f");
					SliderFloat(("SMGs"), &g_Options.Ragebot.HitchanceSmgs, 0.f, 100.f, "%.0f");
					SliderFloat(("Heavies"), &g_Options.Ragebot.HitchanceHeavy, 0.f, 100.f, "%.0f");
					SliderFloat(("Revolver / Deagle"), &g_Options.Ragebot.HitchanceRevolver, 0.f, 100.f, "%.0f");

					EndChild();
				}
			}

			if (legit)
			{

				Checkbox(("Active"), &g_Options.Legitbot.MainSwitch);
				Separator();
				Spacing();

				Columns(3, 0, false);
				
				BeginChild(20, ImVec2(-1, 60), true, 0);
				Checkbox("Backtrack", &g_Options.Legitbot.backtrack);
				SliderFloat("Ticks", &g_Options.Legitbot.backtrackTicks, 0, 12, "%1.f");
				EndChild();
				BeginChild(21, ImVec2(-1, 300), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Rifles");
				Separator();
				Spacing();

				Combo(("Hitbox##0"), &g_Options.Legitbot.main_bone, aimBones, ARRAYSIZE(aimBones));
				Hotkey(("Key##0"), &g_Options.Legitbot.MainKey);
				Checkbox(("Silent Aim##0"), &g_Options.Legitbot.main_silent);
				PushItemWidth(-1);
				SliderFloat(("Smooth##0"), &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
				if (!g_Options.Misc.WalkRobot)
					SliderFloat(("FOV##0"), &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
				else
					SliderFloat(("FOV##0"), &g_Options.Legitbot.Mainfov, 0.00f, 180.00f, "%.2f");
				SliderFloat(("Min RCS##0"), &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
				SliderFloat(("Max RCS##0"), &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
				PopItemWidth();
				Checkbox(("Auto Fire##0"), &g_Options.Legitbot.main_autoshoot);

				EndChild();
				NextColumn();

				BeginChild(22, ImVec2(-1, 370), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Pistols");
				Separator();
				Spacing();

				Combo(("Hitbox##1"), &g_Options.Legitbot.pistol_bone, aimBones, ARRAYSIZE(aimBones));
				Hotkey(("Key##1"), &g_Options.Legitbot.PistolKey);
				Checkbox(("Silent Aim##1"), &g_Options.Legitbot.pistol_silent);
				PushItemWidth(-1);
				SliderFloat(("Smooth##1"), &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
				if (!g_Options.Misc.WalkRobot)
					SliderFloat(("FOV##0"), &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
				else
					SliderFloat(("FOV##0"), &g_Options.Legitbot.Pistolfov, 0.00f, 180.00f, "%.2f");
				SliderFloat(("Min RCS##1"), &g_Options.Legitbot.pistol_recoil_min, 1.00f, 100.00f, "%.2f");
				SliderFloat(("Max RCS##1"), &g_Options.Legitbot.pistol_recoil_max, 1.00f, 100.00f, "%.2f");
				PopItemWidth();
				Checkbox(("Auto Fire##1"), &g_Options.Legitbot.pistol_autoshoot);

				EndChild();

				NextColumn();

				BeginChild(23, ImVec2(-1, 370), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Snipers");
				Separator();
				Spacing();
				Combo(("Hitbox##2"), &g_Options.Legitbot.sniper_bone, aimBones, ARRAYSIZE(aimBones));
				Hotkey(("Key##2"), &g_Options.Legitbot.SniperKey);
				Checkbox(("Silent Aim##2"), &g_Options.Legitbot.sniper_silent);
				PushItemWidth(-1);
				SliderFloat(("Smooth##2"), &g_Options.Legitbot.SniperSmooth, 1.00f, 100.00f, "%.2f");
				if (!g_Options.Misc.WalkRobot)
					SliderFloat(("FOV##0"), &g_Options.Legitbot.Sniperfov, 0.00f, 30.00f, "%.2f");
				else
					SliderFloat(("FOV##0"), &g_Options.Legitbot.Sniperfov, 0.00f, 180.00f, "%.2f");
				SliderFloat(("Min RCS##2"), &g_Options.Legitbot.sniper_recoil_min, 1.00f, 100.00f, "%.2f");
				SliderFloat(("Max RCS##2"), &g_Options.Legitbot.sniper_recoil_max, 1.00f, 100.00f, "%.2f");
				PopItemWidth();
				Checkbox(("Auto Fire##2"), &g_Options.Legitbot.sniper_autoshoot);
				EndChild();
			}

			if (visuals)
			{
				Columns(2, 0, false);
				BeginChild(13, ImVec2(-1, 500), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Players");
				Separator();
				Spacing();
				Hotkey("Toggle Key", &g_Options.Visuals.ToggleKey);

				Checkbox("Teammates", &g_Options.Visuals.TeamESP);	
				Checkbox("Enemies", &g_Options.Visuals.Box);
				Combo("Box ESP", &g_Options.Visuals.BoxType, "None\0\rFull\0\rCorners\0\r3D\0\0", -1);
				Checkbox("Skeleton ESP", &g_Options.Visuals.Skeleton);
				Checkbox("Name", &g_Options.Visuals.Name);
				Checkbox("Info", &g_Options.Visuals.Info);
				Checkbox("Distance", &g_Options.Visuals.Distance);
				Combo("Weapon", &g_Options.Visuals.Weapon, "Off\0\rText\0\rIcon\0\0");
				Combo("Health", &g_Options.Visuals.health, "Off\0\rBar\0\rText\0\rBoth\0\0");
				Checkbox("Money", &g_Options.Visuals.Money);
				Checkbox("Backtrack History", &g_Options.Legitbot.backtrackD);
				Spacing();
				Combo("Chams", &g_Options.Visuals.matierial, "Off\0\rNormal\0\rFlat\0\0", -1);
				Checkbox("XQZ", &g_Options.Visuals.XQZ2);
				Spacing();
				Checkbox("Glow", &g_Options.Visuals.glowplayer);
				SliderInt("Glow Alpha", &g_Options.Visuals.playerglowalpha, 0, 255);
				EndChild();

				NextColumn();
				BeginChild(14, ImVec2(-1, 267), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "World");
				Separator();
				Spacing();

				Checkbox("Glow Weapons", &g_Options.Visuals.weaponglow);
				Checkbox("Glow C4", &g_Options.Visuals.bombglow);
				Checkbox("Grenade Prediction", &g_Options.Visuals.GrenadePrediction);
				Checkbox("Angle Lines", &g_Options.Visuals.angleLines);
				Checkbox("Bomb ESP", &g_Options.Visuals.Bomb);
				Checkbox("Dropped Weapons", &g_Options.Visuals.Droppedguns);
				if (Checkbox("Nightmode", &g_Options.Misc.nightMode))
					g_Options.Misc.colored_walls = false;
				if (Checkbox("Colored Walls", &g_Options.Misc.colored_walls))
					g_Options.Misc.nightMode = false;

				EndChild();

				BeginChild(15, ImVec2(-1, 226), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Other");
				Separator();
				Spacing();

				Hotkey("Thirdperson", &g_Options.Visuals.TPKey);
				Combo("TP Angles", &g_Options.Visuals.TPAng, "Real\0\rFake");
	
				Spacing();
				Checkbox("Hitmarker", &g_Options.Visuals.Hitmarker);
				Combo(("Hitsound"), &g_Options.Visuals.Hitsound, HitmarkSound, ARRAYSIZE(HitmarkSound));
				Spacing();
				Checkbox("Spectators List", &g_Options.Visuals.speclist);
				Checkbox("Sniper Crosshair", &g_Options.Visuals.SniperCrosshair);
				Checkbox("Spread Crosshair", &g_Options.Visuals.spread);
				Checkbox("Extended In-Game radar", &g_Options.Visuals.Radar.Enabled);
				Spacing();
				SliderFloat("Field of View", &g_Options.Visuals.FOVChanger, 0, 160, "%.0f");
				SliderFloat("Viewmodel FOV", &g_Options.Visuals.viewmodelChanger, 0, 130, "%.0f");
				Spacing();
				Checkbox("Remove Scope", &g_Options.Visuals.noscopeborder);
				Checkbox("Remove Flash", &g_Options.Visuals.NoFlash);
				Checkbox("Remove Smoke", &g_Options.Visuals.NoSmoke);
				EndChild();

			}

			if (mis)
			{
				Columns(2, 0, false);
				BeginChild(9, ImVec2(-1, 215), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Movement");
				Separator();
				Spacing();

				Checkbox("Auto Jump", &g_Options.Misc.Bhop);
				Combo("Auto Strafe", &g_Options.Misc.AutoStrafe, "Off\0\rNormal\0\Rage\0\0", -1);
				Spacing();
				Checkbox(("Slide Walk"), &g_Options.Misc.memewalk);
				Combo("Walkbot", &g_Options.Misc.WalkRobot, "Off\0\rDust 2\0\0");
				SliderInt("Fake Lag", &g_Options.Ragebot.FakeLagAmt, 0, 15);
				Checkbox("Legit AA", &g_Options.Misc.legitaa);
				EndChild();

				BeginChild(10, ImVec2(-1, 108), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Matchmaking");
				Separator();
				Spacing();
				ImGui::Checkbox(("Auto Accept"), &g_Options.Misc.auto_accept);
				ImGui::Checkbox(("Rank Reveal"), &g_Options.Misc.rank_reveal);
				EndChild();
				NextColumn();

				BeginChild(11, ImVec2(-1, 330), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Misc");
				Separator();
				Spacing();

				Combo("Clan Tag", &g_Options.Misc.ClantagChanger, ("None\0\rSlide\0\rFlash\0\rFlash Hide\0\rCustom\0\0"), -1);
				if (g_Options.Misc.ClantagChanger == 4)
				{
					InputText("Custom Tag", g_Options.Misc.CustomTag, 128);
					Spacing();
				}
				ImGui::InputText(("Custom Name"), g_Options.Misc.CustomName, 128);
				if (ImGui::Button("Apply", ImVec2(-1, 25)))
					SetName(g_Options.Misc.CustomName);

				Spacing();

				if (Button("Silent Name", ImVec2(-1, 25))) g_Options.Misc.silentname = true; else g_Options.Misc.silentname = false;
				if (Button("Name Exploit", ImVec2(-1, 25))) g_Options.Misc.namespam = true; else g_Options.Misc.namespam = false;
				if (Button("Hide Name", ImVec2(-1, 25))) g_Options.Misc.NoName = true; else g_Options.Misc.NoName = false;

				Spacing();

				Checkbox("Name Stealer", &g_Options.Misc.silentstealer);
				Combo("Chat Spam", &g_Options.Misc.ChatSpam, "Off\0\rEnglish\0\rPortiguese\0\0", -1);
				Combo("Chat Bot", &g_Options.Misc.event_spam, ("Off\0\rEnglish\0\rPortuguese\0\0"), -1);


				EndChild();
			}

			if (skins)
			{
				Checkbox("Active", &g_Options.Skinchanger.Enabled);
				Separator();
				
				static int currWeap = g_Options.Menu.currentWeapon;
				int pk = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin;
				static int currWeapSkin = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin;
				static char weapName[18];
				static char filterSkins[32];
				static char filterGuns[32];

				Columns(2, 0, false);
				InputText(("Search"), filterGuns, IM_ARRAYSIZE(filterGuns));
				BeginChild(5, ImVec2(-1, 260), true, 0);
				for (auto it : guns)
				{
					if (strcmp(it.second, "<-Default->") == 0)
						continue;

					if (!(std::string(it.second).find(std::string(filterGuns)) != std::string::npos))
						continue;

					const bool item_selected = ((int)it.first == (int)g_Options.Menu.currentWeapon);

					//ImGui::PushID(it.first);
					if (Selectable(it.second, item_selected))
					{
						g_Options.Menu.currentWeapon = (int)it.first;
					}
					//ImGui::PopID();
				}
				EndChild();
				NextColumn();
				std::string skinName = weaponnames(g_Options.Menu.currentWeapon);
				InputText(("Search##1"), filterSkins, IM_ARRAYSIZE(filterSkins));
				InputInt(("Custom ID"), &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin);
				BeginChild(("##SKINS"), ImVec2(-1, 233), true,  0);
				std::string skinStr = "";
				int curItem = -1;
				int s = 0;
				for (auto it : weapon_skins)
				{
					if (!(std::string(it.second).find(std::string(filterSkins)) != std::string::npos))
						continue;

					bool selected = ((int)it.first == (int)g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin);

					if (Selectable(it.second, selected))
					{
						g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin = (int)it.first;
						//FullUpdate();
						break;
					}
				}
				EndChild();

				Columns(1);

				BeginChild(7, ImVec2(-1, 100), true, 0);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Models");
				Separator();
				Spacing();
				Columns(2, 0, false);
				Combo("Knife Model", &g_Options.Skinchanger.knifemodel, "Default\0\rBayonet\0\rFlip\0\rGut\0\rKarambit\0\rM9 Baynet\0\rHuntsman\0\rFalshion\0\rBowie\0\rButerfly\0\rShadow Daggers\0\0", -1);
				NextColumn();
				Combo("Glove Model", &g_Options.Skinchanger.glove, "Default\0\rBloodhound\0\rSport\0\rDriver\0\rHand Wraps\0\rMotorcycle\0\rSpecialist\0\rHydra\0\0");
				const char* gstr;
				if (g_Options.Skinchanger.glove == 1)
					gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
				else if (g_Options.Skinchanger.glove == 2)
					gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\rVice\0\rOmega\0\rAmphibious\0\rBronze Morph\0\0";
				else if (g_Options.Skinchanger.glove == 3)
					gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\rOvertake\0\rRacing Green\0\rKing Snake\0\rImperial Plaid\0\0";
				else if (g_Options.Skinchanger.glove == 4)
					gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\rCobalt Skulls\0\rOverprint\0\rDuct Tape\0\rArboreal\0\0";
				else if (g_Options.Skinchanger.glove == 5)
					gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\rTurtle\0\rTransport\0\rPolygon\0\rPOW!\0\0";
				else if (g_Options.Skinchanger.glove == 6)
					gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\rCrimson Web\0\rBuckshot\0\rFade\0\rMogul\0\0";
				else if (g_Options.Skinchanger.glove == 7)
					gstr = "Emerald\0\rMangrove\0\rRattler\0\rCase Hardened\0\0";
				else
					gstr = "";

				Combo("Glove Skin", &g_Options.Skinchanger.gloeskin, gstr, -1);

		
				EndChild();

				if (Button("Apply (Force Update)", ImVec2(-1, 30)))
					FullUpdate();

			}

			if (options)
			{
				Columns(2, 0, false);

				GetConfigMassive();
				BeginChild(1, ImVec2(-1, 200), true, ImGuiWindowFlags_AlwaysAutoResize);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Configs");
				Separator();
				Spacing();
				if (Combo("File", &selectedcfg, [](void* data, int idx, const char** out_text)
				{
					*out_text = configs[idx].c_str();
					return true;
				}, nullptr, configs.size(), 10))
				{
					cfgname = configs[selectedcfg];
					cfgname.erase(cfgname.length() - 4, 4);
					strcpy(g_Options.ConfigName, cfgname.c_str());
				}

				static bool create;
				if (Button("Save", ImVec2(144, 50))) Config->Save(); SameLine();
				if (Button("Load", ImVec2(144, 50)))
				{
					Config->Load();
					FullUpdate();
				}

				if (Button("New Config", ImVec2(-1, 23))) create = !create;
				if (Button("Reset Settings", ImVec2(-1, 23))) Config->Setup();

				if (create)
				{
					Begin("##Create", &create, ImVec2(200, 300), 1.f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
					Text("Config Name:");
					InputText("Name", g_Options.ConfigName, 52);
					if (Button("Create and save"))
					{
						Config->Save();
						create = !create;
					}
					if (Button("Cancel"))
						create = !create;
					End();
				}

				EndChild();

				BeginChild(2, ImVec2(-1, 110), true, ImGuiWindowFlags_AlwaysAutoResize);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Misc");
				Separator();
				Spacing();
				Columns(2, 0, false);
				if (Button("UNLOAD", ImVec2(100, 50))) {
					g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
					unload = true;
					denied = true;
				}
				NextColumn();
				Checkbox("Log Events", &g_Options.Visuals.EventLog);
				EndChild();

				NextColumn();

				BeginChild(3, ImVec2(-1, 400), true, ImGuiWindowFlags_AlwaysAutoResize);
				TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Colors");
				Separator();
				Spacing();
				Checkbox("Dark Mode", &g_Options.Dark);
				ImGui::MyColorEdit3("Menu Color", g_Options.Colors.MenuTheme, 1 << 7);
				Spacing();

				MyColorEdit3("Team ESP", g_Options.Colors.TeamESP, 1 << 7);
				MyColorEdit3("Enemy ESP", g_Options.Colors.EnemyESP, 1 << 7);
				MyColorEdit3("Team Glow", g_Options.Colors.TeamGlow, 1 << 7);
				MyColorEdit3("Enemy Glow", g_Options.Colors.EnemyGlow, 1 << 7);
				MyColorEdit3("Weapon Glow", g_Options.Colors.WeaponGlow, 1 << 7);
				MyColorEdit3("C4 Glow", g_Options.Colors.C4glow, 1 << 7);
				MyColorEdit3("Team Chams Visible", g_Options.Colors.TeamChamsVis, 1 << 7);
				MyColorEdit3("Team Chams Invisible", g_Options.Colors.TeamChamsNVis, 1 << 7);
				MyColorEdit3("Enemy Chams Visible", g_Options.Colors.EnemyChamsVis, 1 << 7);
				MyColorEdit3("Enemy Chams Invisible", g_Options.Colors.EnemyChamsNVis, 1 << 7);
				MyColorEdit3("Colored Walls", g_Options.Colors.ColWalls, 1 << 7);

				EndChild();
			}


			End();
		}		

	}
}

