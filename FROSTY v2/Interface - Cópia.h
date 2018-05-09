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

	configs.push_back("<Chose config>");

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
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.f); 
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.4f); 
		style.Colors[ImGuiCol_Button] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], .75f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.85f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.4f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.8f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.9f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.7f, 0.7f, 0.7f, 1.f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[Massashook_LisboxBg] = ImVec4(0.8f, 0.8f, 0.8f, 1.f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 0.8f);
		style.Colors[ImGuiCol_Header] = ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f);
		style.Colors[ImGuiCol_Text] = ImVec4(0.05f, 0.05f, 0.05f, 1.f);


		ImGui::Begin("##MASSASHOOKMENU", &is_renderer_active, ImVec2(600, 400), 1.f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
		{
			
			ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f) ,"MassasHook"); 

			ImGui::BeginChild(3, ImVec2(130, 300), true);
			if (ImGui::Button("Ragebot", ImVec2(114, 26))) tab = 0;
			if (ImGui::Button("Anti-Aim", ImVec2(114, 26))) tab = 5;
			if (ImGui::Button("Legitbot", ImVec2(114, 26))) tab = 1;
			if (ImGui::Button("Players", ImVec2(114, 26))) tab = 2;
			if (ImGui::Button("Visuals", ImVec2(114, 26))) tab = 6;
			if (ImGui::Button("Misc", ImVec2(114, 26))) tab = 3;
			if (ImGui::Button("Skins", ImVec2(114, 26))) tab = 4;
			ImGui::EndChild();
			ImGui::SameLine();
			if (tab == 0)
			{
				ImGui::BeginChild(7, ImVec2(450, 300), true);
				{
					ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Ragebot"); ImGui::SameLine();
					ImGui::Checkbox(("##RAGEACTIVE"), &g_Options.Ragebot.MainSwitch);
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Columns(2);
					ImGui::Checkbox(("Enable Ragebot"), &g_Options.Ragebot.Enabled);
					ImGui::Checkbox(("Silent Aim"), &g_Options.Ragebot.Silent);
					ImGui::Hotkey(("Key"), &g_Options.Ragebot.KeyPress);
					ImGui::PushItemWidth(-1);
					ImGui::SliderFloat(("FOV"), &g_Options.Ragebot.FOV, 0, 180);
					ImGui::PopItemWidth();
					ImGui::Spacing();
					ImGui::Checkbox(("Auto Fire"), &g_Options.Ragebot.AutoFire);
					ImGui::Checkbox(("Auto Pistol"), &g_Options.Ragebot.AutoPistol);
					ImGui::Checkbox(("Auto Scope"), &g_Options.Ragebot.AutoScope);
					ImGui::Checkbox(("Auto Stop and Duck"), &g_Options.Ragebot.AutoFreeze);
					ImGui::Checkbox(("No Recoil"), &g_Options.Ragebot.AntiRecoil);
					ImGui::Spacing();
					ImGui::Checkbox(("Target Teammates"), &g_Options.Ragebot.FriendlyFire);
					ImGui::Combo(("Resolver"), &g_Options.Ragebot.Resolver, "Off\0\rSimple\0\rBruteforece\0\rPrediction\0\rSmart\0\0", -1);
					ImGui::PushItemWidth(-1);
					ImGui::SliderFloat(("BF after X shots"), &g_Options.Ragebot.bruteAfterX, 0, 10);
					ImGui::PopItemWidth();
					ImGui::Combo(("Hitbox"), &g_Options.Ragebot.Hitbox, RaimBones, ARRAYSIZE(RaimBones));
					ImGui::Combo(("Hitscan"), &g_Options.Ragebot.Hitscan, hitscan, ARRAYSIZE(hitscan));
					ImGui::Checkbox(("Lag Compensation"), &g_Options.Ragebot.PosAdjust);
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ImGui::Text("Min Damage");
					ImGui::SliderFloat(("Snipers##1"), &g_Options.Ragebot.MinimumDamageSniper, 1.f, 100.f, "%.2f");
					ImGui::SliderFloat(("Rifles##1"), &g_Options.Ragebot.MinimumDamageRifle, 1.f, 100.f, "%.2f");
					ImGui::SliderFloat(("Pistols##1"), &g_Options.Ragebot.MinimumDamagePistol, 1.f, 100.f, "%.2f");
					ImGui::SliderFloat(("Heavies##1"), &g_Options.Ragebot.MinimumDamageHeavy, 1.f, 100.f, "%.2f");
					ImGui::SliderFloat(("SMGs##1"), &g_Options.Ragebot.MinimumDamageSmg, 1.f, 100.f, "%.2f");
					ImGui::SliderFloat(("Revolver / Deagle##1"), &g_Options.Ragebot.MinimumDamageRevolver, 1.f, 100.f, "%.2f");
					ImGui::Spacing();
					ImGui::Checkbox(("Hitchance"), &g_Options.Ragebot.Hitchance);
					ImGui::SliderFloat(("Snipers"), &g_Options.Ragebot.HitchanceSniper, 0.f, 100.f, "%.1f");
					ImGui::SliderFloat(("Rifles"), &g_Options.Ragebot.HitchanceRifle, 0.f, 100.f, "%.1f");
					ImGui::SliderFloat(("Pistols"), &g_Options.Ragebot.HitchancePistol, 0.f, 100.f, "%.1f");
					ImGui::SliderFloat(("SMGs"), &g_Options.Ragebot.HitchanceSmgs, 0.f, 100.f, "%.1f");
					ImGui::SliderFloat(("Heavies"), &g_Options.Ragebot.HitchanceHeavy, 0.f, 100.f, "%.1f");
					ImGui::SliderFloat(("Revolver / Deagle"), &g_Options.Ragebot.HitchanceRevolver, 0.f, 100.f, "%.1f");
					ImGui::PopItemWidth();
				}
				ImGui::EndChild();
			}
			else if (tab == 5)
			{
				ImGui::BeginChild(7, ImVec2(450, 300), true);
				{
					ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Anti-Aim"); ImGui::SameLine();
					ImGui::Checkbox(("##RAGEACTIVE"), &g_Options.Ragebot.MainSwitch);
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Checkbox(("Enable Anti-Aim"), &g_Options.Ragebot.EnabledAntiAim);
					ImGui::Combo(XorStr("##VIEW"), &g_Options.Ragebot.SubAATabs, "Simple\0\rAdvanced\0\0", -1);
					ImGui::Spacing();
					if (g_Options.Ragebot.SubAATabs == 0)
					{
						g_Options.Ragebot.PreAAs = true;
						g_Options.Ragebot.BuilderAAs = false;
						ImGui::Combo(("Pitch"), &g_Options.Ragebot.Pitch, antiaimpitch, ARRAYSIZE(antiaimpitch));
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(("Pitch Adder: "), &g_Options.Ragebot.PitchAdder, -89, 90, "%1.f");
						ImGui::PopItemWidth();
						ImGui::Spacing();
						ImGui::Combo(("Yaw"), &g_Options.Ragebot.YawTrue, antiaimyawtrue, ARRAYSIZE(antiaimyawtrue));
						ImGui::Hotkey(("Switch Side"), &g_Options.Ragebot.TrueKeyS);
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(("Side 1"), &g_Options.Ragebot.YawTrueSide1, -179, 179);
						ImGui::SliderFloat(("Side 2"), &g_Options.Ragebot.YawTrueSide2, -179, 179);
						ImGui::PopItemWidth();
						ImGui::Spacing();
						ImGui::Combo(("Fake Yaw"), &g_Options.Ragebot.YawFake, antiaimyawfake, ARRAYSIZE(antiaimyawfake));
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(("Fake Adder: "), &g_Options.Ragebot.YawFakeAdder, -180, 180, "%1.f");
						ImGui::PopItemWidth();
					}
					else if (g_Options.Ragebot.SubAATabs == 1)
					{
						g_Options.Ragebot.PreAAs = false;
						g_Options.Ragebot.BuilderAAs = true;
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat(("Pitch Angle: "), &g_Options.Ragebot.BuilderPitch, -89, 89, "%1.f");
						ImGui::SliderFloat(("Real Angle: "), &g_Options.Ragebot.BuilderReal, -180, 180, "%1.f");
						ImGui::SliderFloat(("Fake Angle: "), &g_Options.Ragebot.BuilderFake, -180, 180, "%1.f");
						ImGui::Spacing();
						ImGui::Checkbox(("Enable Real Jitter"), &g_Options.Ragebot.Jitter);
						ImGui::SliderFloat(("Jitter Range: "), &g_Options.Ragebot.JitterRange, -90, 90, "%1.f");
						ImGui::Spacing();
						ImGui::Checkbox(("Enable Fake Jitter"), &g_Options.Ragebot.FJitter);
						ImGui::SliderFloat(("FJitter Range: "), &g_Options.Ragebot.FJitterRange, -90, 90, "%1.f");
						ImGui::Spacing();
						ImGui::Checkbox(("LBY Breaker"), &g_Options.Ragebot.LBYBreaker);
						ImGui::PopItemWidth();
					}

					ImGui::Checkbox(("Wall Detection"), &g_Options.Ragebot.Edge);
					ImGui::Checkbox(("Anti-Aim on knife"), &g_Options.Ragebot.KnifeAA);

				}
				ImGui::EndChild();
			}
			else if (tab == 1)
			{
				static int stab;
				ImGui::BeginChild(56, ImVec2(450, 300), true);
				ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Legitbot"); ImGui::SameLine();
				ImGui::Checkbox(("##LEGITMS"), &g_Options.Legitbot.MainSwitch);
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Checkbox(("Backtrack"), &g_Options.Legitbot.backtrack); ImGui::SameLine();
				ImGui::SliderFloat(("Ticks: "), &g_Options.Legitbot.backtrackTicks, 0, 12, "%1.f");
				ImGui::Spacing();
				if (ImGui::Button("Rifles", ImVec2(90, 26))) stab = 0; ImGui::SameLine();
				if (ImGui::Button("Pistols", ImVec2(90, 26))) stab = 1; ImGui::SameLine();
				if (ImGui::Button("Snipers", ImVec2(90, 26))) stab = 2;
				if (stab == 0)
				{
					ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Rifles");
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Combo(("Hitbox##0"), &g_Options.Legitbot.main_bone, aimBones, ARRAYSIZE(aimBones));
					ImGui::Hotkey(("Key##0"), &g_Options.Legitbot.MainKey);
					ImGui::Checkbox(("Silent Aim##0"), &g_Options.Legitbot.main_silent);
					ImGui::PushItemWidth(-1);
					ImGui::SliderFloat(("Smooth##0"), &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
					ImGui::SliderFloat(("FOV##0"), &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
					ImGui::SliderFloat(("Min RCS##0"), &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
					ImGui::SliderFloat(("Max RCS##0"), &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::Checkbox(("Auto Fire##0"), &g_Options.Legitbot.main_autoshoot);
				}
				else if (stab == 1)
				{
					ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Pistols");
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Combo(XorStr("Hitbox##1"), &g_Options.Legitbot.pistol_bone, aimBones, ARRAYSIZE(aimBones));
					ImGui::Hotkey(XorStr("Key##1"), &g_Options.Legitbot.PistolKey);
					ImGui::Checkbox(XorStr("Silent Aim##1"), &g_Options.Legitbot.pistol_silent);
					ImGui::PushItemWidth(-1);
					ImGui::SliderFloat(XorStr("Smooth##1"), &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
					ImGui::SliderFloat(XorStr("FOV##1"), &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
					ImGui::SliderFloat(XorStr("Min RCS##1"), &g_Options.Legitbot.pistol_recoil_min, 1.00f, 100.00f, "%.2f");
					ImGui::SliderFloat(XorStr("Max RCS##1"), &g_Options.Legitbot.pistol_recoil_max, 1.00f, 100.00f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::Checkbox(XorStr("Auto Fire##1"), &g_Options.Legitbot.pistol_autoshoot);
				}
				else if (stab == 2)
				{
					ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Snipers");
					ImGui::Separator();
					ImGui::Spacing();
					ImGui::Combo(XorStr("Hitbox##2"), &g_Options.Legitbot.sniper_bone, aimBones, ARRAYSIZE(aimBones));
					ImGui::Hotkey(XorStr("Key##2"), &g_Options.Legitbot.SniperKey);
					ImGui::Checkbox(XorStr("Silent Aim##2"), &g_Options.Legitbot.sniper_silent);
					ImGui::PushItemWidth(-1);
					ImGui::SliderFloat(XorStr("Smooth##2"), &g_Options.Legitbot.SniperSmooth, 1.00f, 100.00f, "%.2f");
					ImGui::SliderFloat(XorStr("FOV##2"), &g_Options.Legitbot.Sniperfov, 0.00f, 30.00f, "%.2f");
					ImGui::SliderFloat(XorStr("Min RCS##2"), &g_Options.Legitbot.sniper_recoil_min, 1.00f, 100.00f, "%.2f");
					ImGui::SliderFloat(XorStr("Max RCS##2"), &g_Options.Legitbot.sniper_recoil_max, 1.00f, 100.00f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::Checkbox(XorStr("Auto Fire##2"), &g_Options.Legitbot.sniper_autoshoot);
				}
				ImGui::EndChild();
			}
			else if (tab == 2)
			{
				ImGui::BeginChild(25, ImVec2(450, 300), true);
				ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Players"); 
				//ImGui::SameLine();
				//ImGui::Checkbox("Anti-OBS", &g_Options.obs);
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Columns(2);
				ImGui::Hotkey("Toggle Key", &g_Options.Visuals.ToggleKey);
				ImGui::Checkbox(("Team ESP"), &g_Options.Visuals.TeamESP);
				ImGui::SameLine();
				ImGui::MyColorEdit3("##TeamColor", g_Options.Colors.TeamESP, 1 << 7);
				ImGui::Checkbox(("Enemy ESP"), &g_Options.Visuals.Box);
				ImGui::SameLine();
				ImGui::MyColorEdit3("##EnrmyColor", g_Options.Colors.EnemyESP, 1 << 7);
				ImGui::Combo("##BoxPlayer", &g_Options.Visuals.BoxType, "None\0\rFull\0\rCorners\0\r3D\0\0", -1);
				ImGui::Checkbox(("Skeleton"), &g_Options.Visuals.Skeleton);
				ImGui::Checkbox(("Backtrack Ticks"), &g_Options.Legitbot.backtrackD);
				ImGui::Checkbox(("Name"), &g_Options.Visuals.Name);
				ImGui::Checkbox(("Info"), &g_Options.Visuals.Info);
				ImGui::Checkbox(("Distance"), &g_Options.Visuals.Distance);
				ImGui::Combo(("Weapon"), &g_Options.Visuals.Weapon, "Off\0\rText\0\rIcon\0\0");
				ImGui::Combo("Health", &g_Options.Visuals.health, "Off\0\rBar\0\rText\0\rBoth\0\0");
				ImGui::Checkbox(("Money"), &g_Options.Visuals.Money);
				ImGui::NextColumn();
				ImGui::Checkbox(("Enemy Chams"), &g_Options.Visuals.Chams2);
				ImGui::SameLine();
				ImGui::MyColorEdit3("##EnemyCham", g_Options.Colors.EnemyChamsVis, 1 << 7);
				ImGui::Checkbox(("Team Chams"), &g_Options.Visuals.Teamchams2);
				ImGui::SameLine();
				ImGui::MyColorEdit3("##TeamCham", g_Options.Colors.TeamChamsVis, 1 << 7);
				ImGui::Checkbox(("XQZ"), &g_Options.Visuals.XQZ2);
				ImGui::SameLine();
				ImGui::MyColorEdit3("##Xqzcolor", g_Options.Colors.EnemyChamsNVis, 1 << 7);
				ImGui::Combo(("Material"), &g_Options.Visuals.matierial, "Normal\0\rFlat\0\rWireframe\0\rGlass\0\rGold\0\rPlatinum\0\rPlastic\0\rCrystal\0\0", -1);
				ImGui::Spacing();
				if (ImGui::Button("Player"))
				{
					g_Options.Visuals.glowsubtab = 0;
				}
				ImGui::SameLine();
				if (ImGui::Button("Drops"))
				{
					g_Options.Visuals.glowsubtab = 1;
				}
				ImGui::SameLine();
				if (ImGui::Button("Bomb"))
				{
					g_Options.Visuals.glowsubtab = 2;
				}
				if (g_Options.Visuals.glowsubtab == 0)
				{
					ImGui::PushItemWidth(-1);
					ImGui::Checkbox(("Enemy Glow"), &g_Options.Visuals.glowplayer);
					ImGui::SameLine();
					ImGui::MyColorEdit3("##glow", g_Options.Colors.EnemyGlow, 1 << 7);
					ImGui::Checkbox(("Team Glow"), &g_Options.Visuals.teamglow);
					ImGui::SameLine();
					ImGui::MyColorEdit3("##teamglow", g_Options.Colors.TeamGlow, 1 << 7);
					ImGui::SliderInt(("Opacity"), &g_Options.Visuals.playerglowalpha, 0, 255);
					ImGui::PopItemWidth();
				}
				if (g_Options.Visuals.glowsubtab == 1)
				{
					ImGui::PushItemWidth(-1);
					ImGui::Checkbox(("Weapon Glow"), &g_Options.Visuals.weaponglow);
					ImGui::SameLine();
					ImGui::MyColorEdit3("##ESP", g_Options.Colors.WeaponGlow, 1 << 7);
					ImGui::SliderInt(("Opacity"), &g_Options.Visuals.weaponglowalpha, 0, 255);
					ImGui::PopItemWidth();
				}
				if (g_Options.Visuals.glowsubtab == 2)
				{
					ImGui::PushItemWidth(-1);
					ImGui::Checkbox(("Bomb Glow"), &g_Options.Visuals.bombglow);
					ImGui::SameLine();
					ImGui::MyColorEdit3("##ESP", g_Options.Colors.C4glow, 1 << 7);
					ImGui::SliderInt(("Opacity"), &g_Options.Visuals.bombglowalpha, 0, 255);
					ImGui::PopItemWidth();
				}
				ImGui::EndChild();


			}
			else if (tab == 6)
			{
				ImGui::BeginChild(72, ImVec2(450, 300), true);
				ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Visuals");
				//ImGui::SameLine();
				//ImGui::Checkbox("Anti-OBS", &g_Options.obs);
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Checkbox(("Grenade Prediction"), &g_Options.Visuals.GrenadePrediction);
				ImGui::Checkbox(("Aim lines"), &g_Options.Visuals.AimLine);
				ImGui::Checkbox(("Angle Lines"), &g_Options.Visuals.angleLines);
				ImGui::Checkbox(("Bomb ESP"), &g_Options.Visuals.Bomb);
				ImGui::Checkbox(("Grenades"), &g_Options.Visuals.Grenades);
				ImGui::Checkbox(("Dropped Weapons"), &g_Options.Visuals.Droppedguns);
				ImGui::Checkbox(("No Smoke"), &g_Options.Visuals.NoSmoke);
				ImGui::Checkbox(("No Flash"), &g_Options.Visuals.NoFlash);
				ImGui::Checkbox(("No Scope"), &g_Options.Visuals.noscopeborder);
				ImGui::Checkbox(("Nightmode"), &g_Options.Misc.nightMode);
				ImGui::Checkbox(("Colored Walls"), &g_Options.Misc.colored_walls); ImGui::SameLine();
				ImGui::MyColorEdit3("##ColoredWalls", g_Options.Colors.ColWalls, 1 << 7);
				ImGui::Checkbox(("Thirdperson"), &g_Options.Visuals.ThirdPerson); ImGui::SameLine();
				ImGui::Hotkey(("Key##273"), &g_Options.Visuals.TPKey);
				ImGui::Checkbox(("Hitmarker"), &g_Options.Visuals.Hitmarker);
				ImGui::Combo(("Hitsound"), &g_Options.Visuals.Hitsound, HitmarkSound, ARRAYSIZE(HitmarkSound));
				ImGui::Checkbox(("Spectators"), &g_Options.Visuals.speclist);
				ImGui::Checkbox(("Event Log"), &g_Options.Visuals.EventLog);
				ImGui::Checkbox("Sniper Crosshair", &g_Options.Visuals.SniperCrosshair);
				ImGui::Spacing();
				ImGui::Checkbox(("Extended In-Game radar"), &g_Options.Visuals.Radar.Enabled);
				ImGui::Spacing();
				ImGui::PushItemWidth(-1);
				ImGui::SliderFloat(("Field of View"), &g_Options.Visuals.FOVChanger, 0, 160, "%.0f");
				ImGui::SliderFloat(("Viewmodel FOV"), &g_Options.Visuals.viewmodelChanger, 0, 130, "%.0f");
				ImGui::PopItemWidth();
				ImGui::EndChild();
			}
			else if (tab == 3)
			{
				ImGui::BeginChild(59, ImVec2(450, 300), true);
				ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Misc");
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::Columns(2);
				ImGui::Checkbox(("Bunny Hop"), &g_Options.Misc.Bhop);
				ImGui::Combo(("Auto Strafe"), &g_Options.Misc.AutoStrafe, ("Off\0\rNormal\0\Rage\0\0"), -1);
				ImGui::Spacing();
				ImGui::Checkbox(("Meme Walk"), &g_Options.Misc.memewalk);
				ImGui::SliderInt(("Fakelag"), &g_Options.Ragebot.FakeLagAmt, 0, 15);
				ImGui::Checkbox(("Legit AA"), &g_Options.Misc.legitaa);
				ImGui::Hotkey(("Auto-Block"), &g_Options.Misc.AutoBlockKey);
				ImGui::Checkbox(("Walkbot"), &g_Options.Misc.WalkRobot);
				ImGui::Combo(("Chat Bot"), &g_Options.Misc.event_spam, ("Off\0\rEnglish\0\rPortuguese\0\0"), -1);
				ImGui::Spacing();
				ImGui::Checkbox(("Auto Accept"), &g_Options.Misc.auto_accept);
				ImGui::Checkbox(("Rank Reveal"), &g_Options.Misc.rank_reveal);
				ImGui::NextColumn();
				ImGui::Combo("Clan Tag", &g_Options.Misc.ClantagChanger, ("None\0\rSlide\0\rFlash\0\rFlash Hide\0\rCustom\0\0"), -1);
				if (g_Options.Misc.ClantagChanger == 4)
					ImGui::InputText("Custom Tag", g_Options.Misc.CustomTag, 128);
				ImGui::Spacing();
				if (ImGui::Button("Silent Name")) g_Options.Misc.silentname = true; else g_Options.Misc.silentname = false;
				ImGui::Checkbox("Name Stealer", &g_Options.Misc.silentstealer);
				if (ImGui::Button("Name Exploit")) g_Options.Misc.namespam = true; else g_Options.Misc.namespam = false;
				if (ImGui::Button("Hide Name")) g_Options.Misc.NoName = true; else g_Options.Misc.NoName = false;
				ImGui::Spacing();
				ImGui::Text("Name Spam");
				ImGui::Combo("##NAMESPAM", &g_Options.Misc.NameChangerFix, ("None\0\rMassasHook\0\0"), -1);
				ImGui::Spacing();
				ImGui::Checkbox(("Chat Spam"), &g_Options.Misc.ChatSpam);
				ImGui::PushItemWidth(-1);
				ImGui::InputText(("##SpamMessage"), g_Options.Misc.ChatSpamMode, 128);
				ImGui::PopItemWidth();
				ImGui::Text("");

				ImGui::InputText(("##Name"), g_Options.Misc.CustomName, 128); 
				if (ImGui::Button(("Set Name")))
				{
					SetName(g_Options.Misc.CustomName);
				}
				ImGui::EndChild();

			}
			else if (tab == 4)
			{
				static int currWeap = g_Options.Menu.currentWeapon;
				int pk = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin;
				static int currWeapSkin = g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin;
				static float weapW = 0.005f;
				static int weapS = -1;
				static int weapST = -1;
				static char weapName[18];
				static char filterSkins[32];
				static char filterGuns[32];
				static int isCT = 1;

				static int stab;

				ImGui::BeginChild(67, ImVec2(450, 325), true);
				ImGui::TextColored(ImVec4(g_Options.Colors.MenuTheme[0], g_Options.Colors.MenuTheme[1], g_Options.Colors.MenuTheme[2], 1.f), "Skinchanger"); ImGui::SameLine();
				ImGui::Checkbox(("##SKIN"), &g_Options.Skinchanger.Enabled); ImGui::SameLine();
				if (ImGui::Button(("Force Update"), ImVec2(-1, 20))) FullUpdate();
				ImGui::Separator();
				ImGui::Spacing();
				if (ImGui::Button("Skins", ImVec2(210, 30))) stab = 0;
				ImGui::SameLine();
				if (ImGui::Button("Models", ImVec2(210, 30))) stab = 1;
				ImGui::Columns(2);
				if (stab == 0)
				{
					ImGui::PushItemWidth(-1);
					ImGui::InputText(("##FILTERWEAPONS"), filterGuns, IM_ARRAYSIZE(filterGuns));
					ImGui::PopItemWidth();
					ImGui::ListBoxHeader(("##GUNS"), ImVec2(-1, 228));
					for (auto it : guns)
					{
						if (strcmp(it.second, "<-Default->") == 0)
							continue;

						if (!(std::string(it.second).find(std::string(filterGuns)) != std::string::npos))
							continue;

						const bool item_selected = ((int)it.first == (int)g_Options.Menu.currentWeapon);

						//ImGui::PushID(it.first);
						if (ImGui::Selectable(it.second, item_selected))
						{
							g_Options.Menu.currentWeapon = (int)it.first;
						}
						//ImGui::PopID();
					}
					ImGui::ListBoxFooter();
					ImGui::NextColumn();
					std::string skinName = weaponnames(g_Options.Menu.currentWeapon);
					{
						//	ImGui::InputText("Name", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].Name, 32);
						//	ImGui::InputInt("Stattrack", &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].customstatamount);
						ImGui::PushItemWidth(-1);
						ImGui::InputText(("##FILTERSKINS"), filterSkins, IM_ARRAYSIZE(filterSkins));
						ImGui::InputInt(("##custom ID"), &g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin);
						ImGui::PopItemWidth();
						ImGui::ListBoxHeader(("##SKINS"), ImVec2(-1, 200));
						std::string skinStr = "";
						int curItem = -1;
						int s = 0;
						for (auto it : weapon_skins)
						{
							if (!(std::string(it.second).find(std::string(filterSkins)) != std::string::npos))
								continue;

							bool selected = ((int)it.first == (int)g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin);

							//ImGui::PushID(it.first);
							if (ImGui::Selectable(it.second, selected))
							{
								g_Options.Skinchanger.W[g_Options.Menu.currentWeapon].ChangerSkin = (int)it.first;
								FullUpdate();
								break;
							}

							//ImGui::PopID();
						}
					}

						ImGui::ListBoxFooter();
				}
				else if (stab == 1)
				{
					ImGui::Columns(2);
					ImGui::ListBoxHeader("##KNIVES", ImVec2(-1, 230));
					{
						if (ImGui::RadioButton(("Default"), &g_Options.Skinchanger.knifemodel, 0))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Bayonet"), &g_Options.Skinchanger.knifemodel, 1))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Flip Knife"), &g_Options.Skinchanger.knifemodel, 2))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Gut Knife"), &g_Options.Skinchanger.knifemodel, 3))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Karambit"), &g_Options.Skinchanger.knifemodel, 4))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("M9 Bayonet"), &g_Options.Skinchanger.knifemodel, 5))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Huntsman"), &g_Options.Skinchanger.knifemodel, 6))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Falchion"), &g_Options.Skinchanger.knifemodel, 7))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Bowie"), &g_Options.Skinchanger.knifemodel, 8))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Butterfly Knife"), &g_Options.Skinchanger.knifemodel, 9))
						{
							FullUpdate();
						}
						if (ImGui::RadioButton(("Shadow Daggers"), &g_Options.Skinchanger.knifemodel, 10))
						{
							FullUpdate();
						}
					}
					ImGui::ListBoxFooter();
					ImGui::NextColumn();
					ImGui::ListBoxHeader("##Gloves", ImVec2(-1, 210));
					if (ImGui::RadioButton(("Default##2"), &g_Options.Skinchanger.glove, 0))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(("Bloodhound"), &g_Options.Skinchanger.glove, 1))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(("Sport"), &g_Options.Skinchanger.glove, 2))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(("Driver"), &g_Options.Skinchanger.glove, 3))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(("Hand Wraps"), &g_Options.Skinchanger.glove, 4))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(("Motorcycle"), &g_Options.Skinchanger.glove, 5))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(("Specialst"), &g_Options.Skinchanger.glove, 6))
					{
						FullUpdate();
					}
					if (ImGui::RadioButton(("Hydra"), &g_Options.Skinchanger.glove, 7))
					{
						FullUpdate();
					}

					const char* gstr;
					if (g_Options.Skinchanger.glove == 1)
					{
						gstr = "Charred\0\rSnakebite\0\rBronzed\0\rGuerilla\0\0";
					}
					else if (g_Options.Skinchanger.glove == 2)
					{
						gstr = "Hedge Maze\0\rPandoras Box\0\rSuperconductor\0\rArid\0\rVice\0\rOmega\0\rAmphibious\0\rBronze Morph\0\0";
					}
					else if (g_Options.Skinchanger.glove == 3)
					{
						gstr = "Lunar Weave\0\rConvoy\0\rCrimson Weave\0\rDiamondback\0\rOvertake\0\rRacing Green\0\rKing Snake\0\rImperial Plaid\0\0";
					}
					else if (g_Options.Skinchanger.glove == 4)
					{
						gstr = "Leather\0\rSpruce DDPAT\0\rSlaughter\0\rBadlands\0\rCobalt Skulls\0\rOverprint\0\rDuct Tape\0\rArboreal\0\0";
					}
					else if (g_Options.Skinchanger.glove == 5)
					{
						gstr = "Eclipse\0\rSpearmint\0\rBoom!\0\rCool Mint\0\rTurtle\0\rTransport\0\rPolygon\0\rPOW!\0\0";
					}
					else if (g_Options.Skinchanger.glove == 6)
					{
						gstr = "Forest DDPAT\0\rCrimson Kimono\0\rEmerald Web\0\rFoundation\0\rCrimson Web\0\rBuckshot\0\rFade\0\rMogul\0\0";
					}
					else if (g_Options.Skinchanger.glove == 7)
					{
						gstr = "Emerald\0\rMangrove\0\rRattler\0\rCase Hardened\0\0";
					}
					else
					{
						gstr = "";
					}
					ImGui::PushItemWidth(-1);
					if (ImGui::Combo(XorStr("##2"), &g_Options.Skinchanger.gloeskin, gstr, -1))
						FullUpdate();

					ImGui::PopItemWidth();
					ImGui::ListBoxFooter();
				}


				ImGui::EndChild();
			}
				
		
		}
		ImGui::End();

		ImGui::Begin("##MASSASHOOKMENU2", &is_renderer_active, ImVec2(600, 400), 1.f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_ShowBorders);
		{
			GetConfigMassive();
			ImGui::Text("Config");
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::BeginChild(1, ImVec2(150, 180), true);
			ImGui::Spacing();
			static int selectedcfg = 0;
			static std::string cfgname = "default";
			if (ImGui::Combo("File", &selectedcfg, [](void* data, int idx, const char** out_text)
			{
				*out_text = configs[idx].c_str();
				return true;
			}, nullptr, configs.size(), 10))
			{
				cfgname = configs[selectedcfg];
				cfgname.erase(cfgname.length() - 4, 4);
				strcpy(g_Options.ConfigName, cfgname.c_str());
			}
			if (ImGui::Button("Save")) Config->Save();
			if (ImGui::Button("Load")) Config->Load();
			static bool toggle;
			if (ImGui::Button("Create New Config")) toggle = !toggle;
			if (toggle)
			{
				ImGui::InputText("Name", g_Options.ConfigName, 52);
				if (ImGui::Button("Create and Save")) {
					Config->Save();
					toggle = !toggle;
				}
				ImGui::Spacing();
			}

			if (ImGui::Button(("Unhook")))
			{
				g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
				unload = true;
				denied = true;
			}
			ImGui::MyColorEdit3(("Menu Color"), g_Options.Colors.MenuTheme, 1 << 7);
			if (ImGui::Button("Reset Settings")) Config->Setup();
			ImGui::EndChild();

		}
		ImGui::End();

	}
}

