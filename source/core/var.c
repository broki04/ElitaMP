new MySQL:dbconn;

#define m_query(%0)             mysql_query(dbconn, %0)
#define m_pquery(%0)            mysql_pquery(dbconn, %0)
#define m_pquery_format(%0)     format(dbstr, sizeof(dbstr), %0), mysql_pquery(dbconn, dbstr)
#define m_tquery(%0)            mysql_tquery(dbconn, %0)
#define m_tquery_format(%0)     format(dbstr, sizeof(dbstr), %0), mysql_tquery(dbconn, dbstr)

new dbstr[2048], s[4098], strx[1024];
enum e_playerData {
    bool:player_logged,
	player_accountID,
	player_password[130],
	player_attempts,
	player_visits,
	player_iplast[18],
	player_datelast[65],
	player_admin,
	player_adminLevel,
	player_adminLoad,
	player_adminPass[65],
	player_adminTime,
	player_adminData[255],
	player_adminPoints,
	player_adminPenalty[5],
	player_eventPoints,
	player_eventCount,
	player_money,
	player_kills,
	player_deaths,
	player_suicides,
	player_online,
	player_session,
	player_hourGame,
	player_gangManage[255],
	player_level,
	player_score,
	player_score_needed,
	bool:player_god,
	player_teleportCategory,
	player_saveData[255],
	player_sparingZestaw,
	player_sparingData[5],
	player_sparingGangSpec,
	Float:player_saveHealthArmour[2],
	Float:player_savePosition[4],
	player_saveSkin,
	player_saveColor,
	player_saveWorld,
	player_saveInterior,
	player_saveWeaponData[13],
	player_saveAmmoData[13],
	player_loginTime,
    player_nickname[25],
    player_ipaddress[18],
	player_teleportData[MAX_TELEPORTS],
	player_gangData[MAX_GANGS],
	player_skin,
	player_pickupTimer,
	player_nickTimer,
	player_spawnData,
	Float:player_lastPos[4],
	player_lastInterior,
	player_onUpdate,
	player_premiumTime,
	player_mutetime,
	player_warns,
	player_jailtime,
	bool:player_checkMuted,
	player_serial[65],
	player_banData[255],
	player_antySK,
	Text3D:player_label,
	player_levelPoints,
	player_heist_attempt,
	player_countdown,
	bool:player_vault,
	player_objectData[255],
	player_fps,
	player_drunk,
	player_battleTimer,
	player_antySpam,
	player_lastmessage[144],
	player_floodCounter,
	player_floodTimer[2],
	player_lastPM,
	player_raportData[MAX_PLAYERS + MAX_PLAYERS],
	player_country[MAX_COUNTRY_LENGTH],
	player_city[MAX_CITY_LENGTH],
	player_isp[MAX_AUTONOMOUS_SYSTEM_LENGTH],
	Float:player_zone_damage,
	bool:player_bonus_vcxz,
	player_robbed,
	player_teleportTimer,
	player_teleportRequest,
	bool:player_checkPM,
	bool:player_checkCMD,
	player_fakeNick[25],
	bool:player_isFake,
	player_job_points,
	player_eventData[255],
	player_chatID,
	bool:player_gang_loan,
	player_gang_loan_expire,
	bool:player_holding,
	bool:player_quizDone,
	bool:player_respawn,
	player_gangBases[MAX_GANGBASES],
	player_warTeam,
	player_killstreak,
	player_beststreak,
	player_gangbase,
	player_betdata[5],
	player_fightstyle,
	player_createdVehicle,
	player_businessData[MAX_BUSINESS],
	player_business,
	player_poolID,
	bool:player_sparspec,
};

new 
	PlayerBar:LevelBar[MAX_PLAYERS],
	PlayerBar:FishBar[MAX_PLAYERS],
	PlayerBar:SlotsBar[MAX_PLAYERS],
	PlayerBar:PoolBar[MAX_PLAYERS],

	adminWeekName[25], eventWeekName[25],
	papieskaCount = 0,
	bool:fishEvent,
	OreDay,
	bool:oreEvent,

	pCheatDetected[MAX_PLAYERS] = 0,
	bool:pCheatTemp[MAX_PLAYERS] = false,
	bool:pCheatWH[MAX_PLAYERS] = false,

	pSecretQuestion[MAX_PLAYERS][90],
	pSecretAnswer[MAX_PLAYERS][90],
	pSecretPinCode[MAX_PLAYERS][20],

	Float:pStalyPos[MAX_PLAYERS][4],
	pStalyInterior[MAX_PLAYERS],
	pWeapons[MAX_PLAYERS][7],

	sumoArenaID,
	sumoVehicleCount,

	AccountDataSettings[][] = {"B³êdy w okienku", "Zaproszenia na duele", "Proœby o teleport", "Oferty dueli w wiadomoœci", "Oferty teleportacji w wiadomoœci"},
	bool:pAccountSettings[MAX_PLAYERS][sizeof(AccountDataSettings)],

    playerData[MAX_PLAYERS][e_playerData],
	Iterator:Permission<MAX_PLAYERS>,
	Iterator:Premium<MAX_PLAYERS>
;

enum e_weaponShopData {
    weapon_id,
    weapon_name[50],
    weapon_money,
	bool:weapon_bonus,
    weapon_type
};

new const weaponShopData[][e_weaponShopData] = {
    {WEAPON_KNIFE,      "Nó¿",                      50, false,		0},
    {WEAPON_BAT,        "Kij basebalowy",           30, false,		0},
    {WEAPON_KATANA,     "Katana",                   50,	false,		0},
    {WEAPON_CHAINSAW,   "Pi³a ³añcuchowa",          5000, false,	0},

    {WEAPON_DEAGLE,     "Deagle",                   1000, false,	1},
    {WEAPON_SILENCED,   "Pistolet z t³umikiem",     750, false,		1},
    {WEAPON_COLT45,     "Pistolet 9mm",             500, false,		1},

    {WEAPON_SHOTGUN,    "Shotgun",                  3000, false,	2},
    {WEAPON_SHOTGSPA,   "Combat Shotgun",           5000, false,	2},
    {WEAPON_SAWEDOFF,   "Sawn-off Shotgun",         8000, false,	2},

    {WEAPON_TEC9,       "Tec-9",                    3500, false, 	3},
    {WEAPON_UZI,        "UZI",                      3500, false, 	3},
    {WEAPON_MP5,        "MP5",                      3000, false, 	3},

    {WEAPON_AK47,       "AK-47",                    5000, false, 	4},
    {WEAPON_M4,         "M4",                       7500, false,    4},
 
    {WEAPON_RIFLE,      "Gwintówka",                3500, false,	5},
    {WEAPON_SNIPER,     "Snajperka",                8000, false, 	5},

	{WEAPON_GRENADE, 	"Granat", 					10000, true, 	6},
	{WEAPON_MOLTOV, 	"Mo³otow", 					10000, true, 	6}
};

new const KoloryGraczy[] = {
    0x00FF00FF, 0xFF0000FF, 0xFF8C13FF, 0xC715FFFF, 0x620005ff, 0x20B2AAFF, 0xFF00FFFF, 0xDC143CFF, 0x6495EDFF, 0xF0E68CFF,
	0xFF1493FF, 0xF4A460FF, 0xEE82EEFF, 0xAFAFAFFF, 0xFFFF00FF, 0xFF6666FF, 0xFFFFAAFF, 0xFDCA31FF, 0x004cffff, 0x00ff8cff,
	0x8B4513FF, 0x4949A0FF, 0x148B8BFF, 0x14FF7FFF, 0x556B2FFF, 0x0FD9FAFF, 0x10DC29FF, 0x534081FF, 0x0495CDFF, 0xEF6CE8FF, 
	0x247C1BFF, 0x0C8E5DFF, 0x778899FF, 0xCB7ED3FF, 0x65ADEBFF, 0x5C1ACCFF, 0xF2F853FF, 0x11F891FF, 0x7B39AAFF, 0x53EB10FF, 
	0x275222FF, 0xF09F5BFF, 0x3D0A4FFF, 0x22F767FF, 0xD63034FF, 0x9A6980FF, 0xDFB935FF, 0x3793FAFF, 0x90239DFF, 0xE9AB2FFF,
	0x057F94FF, 0xB98519FF, 0x388EEAFF, 0x028151FF, 0xA55043FF, 0x0DE018FF, 0x93AB1CFF, 0x95BAF0FF, 0x369976FF, 0x18F71FFF, 
	0xC0C0C0FF, 0xFF8080FF, 0x804040FF, 0x808000FF, 0xFF8040FF, 0x00FF00FF, 0x00B000FF, 0x808080FF, 0x400080FF, 0x8000FFFF,
	0xFF0080FF, 0xFF00FFFF, 0xFF80FFFF, 0xFF80C0FF, 0x8080C0FF, 0x0000A0FF, 0x0080FFFF, 0x0080C0FF, 0x8080FFFF, 0x00FFFFFF,
	0x9AC831FF, 0x40D0BBFF, 0x97BFE8FF, 0x809B84FF, 0x0080C0FF, 0xE6E6E6FF, 0x67BD3EFF, 0xC0C0C0FF, 0x8EBC49FF, 0x33CCFFAA,
	0x491B9EFF, 0x829DC7FF, 0xBCE635FF, 0xCEA6DFFF, 0x20D4ADFF, 0x2D74FDFF, 0x3C1C0DFF, 0x12D6D4FF, 0x48C000FF, 0x2A51E2FF, 
	0xFC42A8FF, 0x2FC827FF, 0x1A30BFFF, 0xB740C2FF, 0x42ACF5FF, 0x2FD9DEFF, 0xFAFB71FF, 0x05D1CDFF, 0xC471BDFF, 0x94436EFF, 
	0xCE79EEFF, 0xBD1EF2FF, 0x93B7E4FF, 0x3214AAFF, 0x184D3BFF, 0xAE4B99FF, 0x7E49D7FF, 0x4C436EFF, 0xFA24CCFF, 0xCE76BEFF, 
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF, 0x8A2CD7FF, 0x6152C2FF, 0xCF72A9FF, 0xE59338FF, 0xEEDC2DFF,
	0xd60000FF, 0xc4001dFF, 0xFF1744FF, 0xc50e29FF, 0xFF616fFF, 0xB71c1cFF, 0xaf4448FF, 0xe57373FF, 0x4db6acFF, 0x009624FF, 
	0xef9a9aFF, 0xfd558fFF, 0x8e0038FF, 0xC51116FF, 0xff4081FF, 0xFF79b0FF, 0xff80abFF, 0xffb2ddFF, 0x880e4fFF, 0x64dd17FF, 
	0xbc477bFF, 0xc2185bFF, 0xec407aFF, 0xf48fb1FF, 0x7200caFF, 0xaa00ffFF, 0xd500f9FF, 0xe040fbFF, 0xb64fc8FF, 0x81c784FF, 
	0x3a80fcFF, 0x6a1b9aFF, 0x9c27b0FF, 0xba68c8FF, 0x883997FF, 0x7c4dffFF, 0xb388ffFF, 0x5e35b1FF, 0x7e57c2FF, 0x00e676FF, 
	0xb39ddbFF, 0x0026caFF, 0x304ffeFF, 0x7a7cffFF, 0x0043caFF, 0x536dfeFF, 0x5870cbFF, 0x8c9effFF, 0x303f9fFF, 0xb0bec5FF, 
	0x7986cbFF, 0x2962ffFF, 0x448affFF, 0x82b1ffFF, 0x005cb2FF, 0x1e88e5FF, 0x42a5f5FF, 0x0094ccFF, 0x40c4ffFF, 0x424242FF, 
	0x4ba3c7FF, 0x81d4faFF, 0x00e5ffFF, 0x00cbccFF, 0x00838fFF, 0x00acc1FF, 0x00bfa5FF, 0x1de9b6FF, 0x009688FF, 0x795548FF, 
	0x32cb00FF, 0x76ff03FF, 0xb2ff59FF, 0xaed581FF, 0x90cc00FF, 0xc6ff00FF, 0xb8cc00FF, 0xeeff41FF, 0xf4ff81FF, 0xc63f17FF, 
	0xcabf45FF, 0xFFF59dFF, 0xcbc26dFF, 0xc67c00FF, 0xffd740FF, 0xff6f00FF, 0xc79100FF, 0xFFCA28FF, 0xffab40FF, 0xca9b52FF, 
	0x1874CDFF, 0xFFB90FFF, 0xDC143CFF, 0x98F5FFFF, 0x7FFFD4FF, 0x912CEEFF, 0xC00040FF, 0x66CC99FF, 0x00CCCCFF, 0x33CCFFFF,
	0x33FFFFFF, 0x009933FF, 0xFF3333FF, 0x3333CCFF, 0x003366FF, 0x008080FF, 0xB22222FF, 0xFF1493FF, 0x8A2BE2FF, 0x32CD32FF,
	0xE0FFFFFF, 0x00BFFFFF, 0xFF6030FF, 0xFFB100FF, 0x406090FF, 0x0000FFFF, 0x8000FFFF, 0x40E010FF, 0xFF8040FF, 0xFF00F0FF,
	0xC0FF50FF, 0x80FFC0FF, 0x8020FFFF, 0x40A0E0FF, 0xFFFF40FF, 0x008040FF, 0xC02050FF, 0xFFA070FF, 0xFF8010FF, 0xFF0000FF,
	0x80A0C0FF, 0x80C040FF, 0x24C005FF, 0x00FFFFFF, 0x00A0D0FF, 0x004070FF, 0x80E0FFFF, 0x8020B0FF, 0xA79C20FF, 0x0080B0FF,
	0x80C0FFFF, 0x004080FF, 0xE0D560FF, 0xFF00D5FF, 0xF4A460FF, 0x4169E1FF, 0x006400FF, 0x00FFFFFF, 0xDAA520FF, 0x9370DBFF,
	0x000080FF, 0xDC143CFF, 0xFF4500FF, 0x708090FF, 0xFF6347FF, 0x800080FF, 0x00FF7FFF, 0x20B2AAFF, 0xFF1493FF, 0x6633FFFF,
	0x00FF99FF, 0xCCCC00FF, 0xFFCC00FF, 0xFF9900FF, 0xCC66FFFF, 0x5F9F9FFF, 0x7093DBFF, 0x238E23FF, 0x4D4DFFFF, 0x3333FFFF,
	0x9F945CFF, 0xDCDE3DFF, 0x10C9C5FF, 0x70524DFF, 0x0BE472FF, 0x8A2CD7FF, 0x6152C2FF, 0xCF72A9FF, 0xE59338FF, 0xEEDC2DFF,
	0x3FE65CFF, 0xFFD720FF, 0xBD34DAFF, 0x54137DFF, 0xAF2FF3FF, 0x4B8987FF, 0xE3AC12FF, 0xC1F7ECFF, 0xA04E0AFF, 0xD8C762FF,
	0x00c853FF, 0x1faa00FF, 0x43a047FF, 0x2bbd7eFF, 0xefdcd5FF, 0x8eacbbFF, 0xbcaaa4FF, 0xc97b63FF, 0xD8C762CC, 0x00a6ffff
};

new const VehicleColoursTableRGBA[] = {
    0x000000FF, 0xF5F5F5FF, 0x2A77A1FF, 0x840410FF, 0x263739FF, 0x86446EFF, 0xD78E10FF, 0x4C75B7FF, 0xBDBEC6FF, 0x5E7072FF,
    0x46597AFF, 0x656A79FF, 0x5D7E8DFF, 0x58595AFF, 0xD6DAD6FF, 0x9CA1A3FF, 0x335F3FFF, 0x730E1AFF, 0x7B0A2AFF, 0x9F9D94FF,
    0x3B4E78FF, 0x732E3EFF, 0x691E3BFF, 0x96918CFF, 0x515459FF, 0x3F3E45FF, 0xA5A9A7FF, 0x635C5AFF, 0x3D4A68FF, 0x979592FF,
    0x421F21FF, 0x5F272BFF, 0x8494ABFF, 0x767B7CFF, 0x646464FF, 0x5A5752FF, 0x252527FF, 0x2D3A35FF, 0x93A396FF, 0x6D7A88FF,
    0x221918FF, 0x6F675FFF, 0x7C1C2AFF, 0x5F0A15FF, 0x193826FF, 0x5D1B20FF, 0x9D9872FF, 0x7A7560FF, 0x989586FF, 0xADB0B0FF,
    0x848988FF, 0x304F45FF, 0x4D6268FF, 0x162248FF, 0x272F4BFF, 0x7D6256FF, 0x9EA4ABFF, 0x9C8D71FF, 0x6D1822FF, 0x4E6881FF,
    0x9C9C98FF, 0x917347FF, 0x661C26FF, 0x949D9FFF, 0xA4A7A5FF, 0x8E8C46FF, 0x341A1EFF, 0x6A7A8CFF, 0xAAAD8EFF, 0xAB988FFF,
    0x851F2EFF, 0x6F8297FF, 0x585853FF, 0x9AA790FF, 0x601A23FF, 0x20202CFF, 0xA4A096FF, 0xAA9D84FF, 0x78222BFF, 0x0E316DFF,
    0x722A3FFF, 0x7B715EFF, 0x741D28FF, 0x1E2E32FF, 0x4D322FFF, 0x7C1B44FF, 0x2E5B20FF, 0x395A83FF, 0x6D2837FF, 0xA7A28FFF,
    0xAFB1B1FF, 0x364155FF, 0x6D6C6EFF, 0x0F6A89FF, 0x204B6BFF, 0x2B3E57FF, 0x9B9F9DFF, 0x6C8495FF, 0x4D8495FF, 0xAE9B7FFF,
    0x406C8FFF, 0x1F253BFF, 0xAB9276FF, 0x134573FF, 0x96816CFF, 0x64686AFF, 0x105082FF, 0xA19983FF, 0x385694FF, 0x525661FF,
    0x7F6956FF, 0x8C929AFF, 0x596E87FF, 0x473532FF, 0x44624FFF, 0x730A27FF, 0x223457FF, 0x640D1BFF, 0xA3ADC6FF, 0x695853FF,
    0x9B8B80FF, 0x620B1CFF, 0x5B5D5EFF, 0x624428FF, 0x731827FF, 0x1B376DFF, 0xEC6AAEFF, 0x000000FF,
    0x177517FF, 0x210606FF, 0x125478FF, 0x452A0DFF, 0x571E1EFF, 0x010701FF, 0x25225AFF, 0x2C89AAFF, 0x8A4DBDFF, 0x35963AFF,
    0xB7B7B7FF, 0x464C8DFF, 0x84888CFF, 0x817867FF, 0x817A26FF, 0x6A506FFF, 0x583E6FFF, 0x8CB972FF, 0x824F78FF, 0x6D276AFF,
    0x1E1D13FF, 0x1E1306FF, 0x1F2518FF, 0x2C4531FF, 0x1E4C99FF, 0x2E5F43FF, 0x1E9948FF, 0x1E9999FF, 0x999976FF, 0x7C8499FF,
    0x992E1EFF, 0x2C1E08FF, 0x142407FF, 0x993E4DFF, 0x1E4C99FF, 0x198181FF, 0x1A292AFF, 0x16616FFF, 0x1B6687FF, 0x6C3F99FF,
    0x481A0EFF, 0x7A7399FF, 0x746D99FF, 0x53387EFF, 0x222407FF, 0x3E190CFF, 0x46210EFF, 0x991E1EFF, 0x8D4C8DFF, 0x805B80FF,
    0x7B3E7EFF, 0x3C1737FF, 0x733517FF, 0x781818FF, 0x83341AFF, 0x8E2F1CFF, 0x7E3E53FF, 0x7C6D7CFF, 0x020C02FF, 0x072407FF,
    0x163012FF, 0x16301BFF, 0x642B4FFF, 0x368452FF, 0x999590FF, 0x818D96FF, 0x99991EFF, 0x7F994CFF, 0x839292FF, 0x788222FF,
    0x2B3C99FF, 0x3A3A0BFF, 0x8A794EFF, 0x0E1F49FF, 0x15371CFF, 0x15273AFF, 0x375775FF, 0x060820FF, 0x071326FF, 0x20394BFF,
    0x2C5089FF, 0x15426CFF, 0x103250FF, 0x241663FF, 0x692015FF, 0x8C8D94FF, 0x516013FF, 0x090F02FF, 0x8C573AFF, 0x52888EFF,
    0x995C52FF, 0x99581EFF, 0x993A63FF, 0x998F4EFF, 0x99311EFF, 0x0D1842FF, 0x521E1EFF, 0x42420DFF, 0x4C991EFF, 0x082A1DFF,
    0x96821DFF, 0x197F19FF, 0x3B141FFF, 0x745217FF, 0x893F8DFF, 0x7E1A6CFF, 0x0B370BFF, 0x27450DFF, 0x071F24FF, 0x784573FF,
    0x8A653AFF, 0x732617FF, 0x319490FF, 0x56941DFF, 0x59163DFF, 0x1B8A2FFF, 0x38160BFF, 0x041804FF, 0x355D8EFF, 0x2E3F5BFF,
    0x561A28FF, 0x4E0E27FF, 0x706C67FF, 0x3B3E42FF, 0x2E2D33FF, 0x7B7E7DFF, 0x4A4442FF, 0x28344EFF
};

#define getNick(%0)     playerData[%0][player_nickname]
#define getIP(%0)       playerData[%0][player_ipaddress]
#define getSerial(%0)	playerData[%0][player_serial]

new const VehicleNames[][] = {
	{"Landstalker"},{"Bravura"},{"Buffalo"},{"Linerunner"},{"Perrenial"},{"Sentinel"},
	{"Dumper"},{"Firetruck"},{"Trashmaster"},{"Stretch"},{"Manana"},{"Infernus"},{"Voodoo"},
	{"Pony"},{"Mule"},{"Cheetah"},{"Ambulance"},{"Leviathan"},{"Moonbeam"},{"Esperanto"},{"Taxi"},
	{"Washington"},{"Bobcat"},{"Mr. Whoopee"},{"BF. Injection"},{"Hunter"},{"Premier"},{"Enforcer"},
	{"Securicar"},{"Banshee"},{"Predator"},{"Bus"},{"Rhino"},{"Barracks"},{"Hotknife"},{"Article Trailer"},
	{"Previon"},{"Coach"},{"Cabbie"},{"Stallion"},{"Rumpo"},{"RC Bandit"},{"Romero"},{"Packer"},{"Monster"},
	{"Admiral"},{"Squalo"},{"Seasparrow"},{"Pizzaboy"},{"Tram"},{"Article Trailer 2"},{"Turismo"},{"Speeder"},
	{"Reefer"},{"Tropic"},{"Flatbed"},{"Yankee"},{"Caddy"},{"Solair"},{"RC Van"},{"Skimmer"},
	{"PCJ-600"},{"Faggio"},{"Freeway"},{"RC Baron"},{"RC Raider"},{"Glendale"},{"Oceanic"},{"Sanchez"},
	{"Sparrow"},{"Patriot"},{"Quad"},{"Coastguard"},{"Dinghy"},{"Hermes"},{"Sabre"},{"Rustler"},{"ZR-350"},
	{"Walton"},{"Regina"},{"Comet"},{"BMX"},{"Burrito"},{"Camper"},{"Marquis"},{"Baggage"},{"Dozer"},
	{"Maverick"},{"News Chopper"},{"Rancher"},{"FBI Rancher"},{"Virgo"},{"Greenwood"},{"Jetmax"},{"Hotring"},
	{"Sandking"},{"Blista Compact"},{"Police Maverick"},{"Boxville"},{"Benson"},{"Mesa"},{"RC Goblin"},
	{"Hotring Racer A"},{"Hotring Racer B"},{"Bloodring Banger"},{"Rancher"},{"Super GT"},{"Elegant"},
	{"Journey"},{"Bike"},{"Mountain Bike"},{"Beagle"},{"Cropdust"},{"Stunt"},{"Tanker"},{"Roadtrain"},
	{"Nebula"},{"Majestic"},{"Buccaneer"},{"Shamal"},{"Hydra"},{"FCR-900"},{"NRG-500"},{"HPV1000"},
	{"Cement Truck"},{"Tow Truck"},{"Fortune"},{"Cadrona"},{"FBI Truck"},{"Willard"},{"Forklift"},
	{"Tractor"},{"Combine"},{"Feltzer"},{"Remington"},{"Slamvan"},{"Blade"},{"Freight"},{"Streak"},
	{"Vortex"},{"Vincent"},{"Bullet"},{"Clover"},{"Sadler"},{"Firetruck LA"},{"Hustler"},{"Intruder"},
	{"Primo"},{"Cargobob"},{"Tampa"},{"Sunrise"},{"Merit"},{"Utility"},{"Nevada"},{"Yosemite"},{"Windsor"},
	{"Monster A"},{"Monster B"},{"Uranus"},{"Jester"},{"Sultan"},{"Stratum"},{"Elegy"},{"Raindance"},
	{"RC Tiger"},{"Flash"},{"Tahoma"},{"Savanna"},{"Bandito"},{"Freight Flat"},{"Streak Carriage"},
	{"Kart"},{"Mower"},{"Dunerider"},{"Sweeper"},{"Broadway"},{"Tornado"},{"AT-400"},{"DFT-30"},{"Huntley"},
	{"Stafford"},{"BF-400"},{"Newsvan"},{"Tug"},{"Article Trailer 3"},{"Emperor"},{"Wayfarer"},{"Euros"},{"Mobile Hotdog"},
	{"Club"},{"Freight Carriage"},{"Trailer 3"},{"Andromada"},{"Dodo"},{"RC Cam"},{"Launch"},{"Police Car (LSPD)"},
	{"Police Car (SFPD)"},{"Police Car (LVPD)"},{"Police Ranger"},{"Picador"},{"SWAT Van"},{"Alpha"},
	{"Phoenix"},{"Glendale"},{"Sadler"},{"Luggage Trailer A"},{"Luggage Trailer B"},{"Stair Trailer"},
	{"Boxville"},{"Farm Plow"},{"Utility Trailer"}
};

new const WeaponNames[][] = {
	"Piesci", "Kastet", "Kij golfowy",
	"Palka policyjna", "Noz", "Kij baseballowy",
	"Lopata", "Kij bilardowy", "Katana",
	"Pila mechaniczna", "Fioletowe dildo", "Biale dildo",
	"Dlugie biale dildo", "Biale dildo", "Kwiaty",
	"Laska", "Granaty", "Gas lzawiacy",
	"Koktail molotova", "Unknown", "Unknown", "Unknown",
	"Pistolet 9mm", "Pistolet z tlumikiem", "Desert Eagle",
	"Shotgun", "Sawn-off Shotgun", "Combat Shotgun", "Micro Uzi",
	"MP5", "AK-47", "M4", "Tec9", "Strzelba", "Sniper Rifle",
	"RPG", "Wyrzutnia rakiet", "Miotacz ognia", "Minigun",
	"£adunki wybuchowe", "Detonator", "Spray", "Gaœnica",
	"Aparat", "Gogle noktowizyjne", "Gogle termiczne", "Spadochron",
	"Unknown", "Unknown", "Pojazd", "Smigla", "Eksplozja", "Parkowanie",
	"Utoniecie", "Kolizja", "Kolizja", "Unknown"
};

new const animationList[][] = {
	"AIRPORT",      "ATTRACTORS",   "BAR",          "BASEBALL",     "BD_FIRE",
	"BEACH",        "BENCHPRESS",   "BF_INJECTION", "BIKE_DBZ",     "BIKED",
	"BIKEH",        "BIKELEAP",     "BIKES",        "BIKEV",        "BLOWJOBZ",
	"BMX",          "BOMBER",       "BOX",          "BSKTBALL",     "BUDDY",
	"BUS",          "CAMERA",       "CAR",          "CAR_CHAT",     "CARRY",
	"CASINO",       "CHAINSAW",     "CHOPPA",       "CLOTHES",      "COACH",
	"COLT45",       "COP_AMBIENT",  "COP_DVBYZ",    "CRACK",        "CRIB",
	"DAM_JUMP",     "DANCING",      "DEALER",       "DILDO",        "DODGE",
	"DOZER",        "DRIVEBYS",     "FAT",          "FIGHT_B",      "FIGHT_C",
	"FIGHT_D",      "FIGHT_E",      "FINALE",       "FINALE2",      "FLAME",
	"FLOWERS",      "FOOD",         "FREEWEIGHTS",  "GANGS",        "GFUNK",
	"GHANDS",       "GHETTO_DB",    "GOGGLES",      "GRAFFITI",     "GRAVEYARD",
	"GRENADE",      "GYMNASIUM",    "HAIRCUTS",     "HEIST9",       "INT_HOUSE",
	"INT_OFFICE",   "INT_SHOP",     "JST_BUISNESS", "KART",         "KISSING",
	"KNIFE",        "LAPDAN1",      "LAPDAN2",      "LAPDAN3",      "LOWRIDER",
	"MD_CHASE",     "MD_END",       "MEDIC",        "MISC",         "MTB",
	"MUSCULAR",     "NEVADA",       "ON_LOOKERS",   "OTB",          "PARACHUTE",
	"PARK",         "PAULNMAC",     "PED",          "PLAYER_DVBYS", "PLAYIDLES",
	"POLICE",       "POOL",         "POOR",         "PYTHON",       "QUAD",
	"QUAD_DBZ",     "RAPPING",      "RIFLE",        "RIOT",         "ROB_BANK",
	"ROCKET",       "RUNNINGMAN",   "RUSTLER",      "RYDER",        "SCRATCHING",
	"SEX",          "SHAMAL",       "SHOP",         "SHOTGUN",      "SILENCED",
	"SKATE",        "SMOKING",      "SNIPER",       "SNM",          "SPRAYCAN",
	"STRIP",        "SUNBATHE",     "SWAT",         "SWEET",        "SWIM",
	"SWORD",        "TANK",         "TATTOOS",      "TEC",          "TRAIN",
	"TRUCK",        "UZI",          "VAN",          "VENDING",      "VORTEX",
	"WAYFARER",     "WEAPONS",      "WOP",          "WUZI"
};

new const guiVehicleModels[][] = {
	{10,	60, 76, 111, 112, 113, 119, 153, 177, 192, 193}, //airplanes
	{7,		17, 69, 87, 88, 97, 148, 163}, //helis
	{14,	48, 61, 62, 63, 68, 71, 81, 109, 110, 121, 122, 123, 181, 186}, //motors and bikes
	{4,		39, 80, 133, 155}, //cabriolets
	{26,	22, 82, 98, 99, 209, 124, 178, 55, 3, 14, 182, 43, 114, 200, 13, 115, 40, 143, 205, 59, 131, 8, 152, 78, 56, 154}, //industrials
	{8,		12, 134, 135, 136, 166, 167, 175, 176}, //lowriders
	{13,	0, 24, 44, 70, 89, 95, 100, 105, 156, 157, 168, 173, 179}, //offroad
	{16,	7, 16, 20, 27, 31, 33, 37, 38, 90, 128, 144, 196, 197, 198, 199, 201}, //public
	{34,	45, 104, 1, 118, 127, 142, 107, 162, 185, 19, 126, 204, 66, 92, 74, 146, 117, 10, 151, 116, 67, 26, 36, 147, 5, 180, 160, 150, 189, 140, 91, 129, 21, 561}, //saloon
	{20,	2, 11, 15, 29, 51, 75, 77, 94, 96, 102, 103, 106, 141, 158, 159, 165, 187, 189, 202, 203}, //sport
	{10,	30, 46, 52, 53, 54, 72, 73, 84, 93, 195}, //boats
	{21,	85, 57, 83, 108, 132, 86, 6, 130, 34, 145, 188, 171, 172, 23, 42, 28, 9, 174, 125, 183, 139} //unicals
};

bool:isNumeric(const string[]) return !sscanf(string, "{f}");
bool:checkAdminLevel(playerid, level) 
{
    if(playerData[playerid][player_adminLevel] >= level) return true;
    return false;
}

stock containsIP(const string[])
{
	static Regex:regex;
	if(!regex) regex = Regex_New(".*[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,7}[0-9]{3,5}.*");
	return Regex_Check(string, regex);
}

stock isValidPassword(const string[])
{
	static Regex:regex;
	if(!regex) regex = Regex_New("^[a-zA-Z0-9!@#\\$%\\^&*()_+-\\[\\];\\\\<>,\\.\\?`~\"]{5,25}$");
	return Regex_Check(string, regex);
}

stock correctNick(const string[])
{
	static Regex:regex;
	if(!regex) regex = Regex_New("^[a-zA-Z0-9_$.@()=\\[\\]]{3,20}+$");
	return Regex_Check(string, regex);
}

stock checkIP(const string[])
{
	static Regex:regex;
	if(!regex) regex = Regex_New(".*[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,7}[0-9]{3,5}.*");
	return Regex_Check(string, regex);
}

Float:gSpeed(Float:vel_x, Float:vel_y, Float:vel_z) return floatsqroot(floatpower(vel_x, 2.0) + floatpower(vel_y, 2.0) + floatpower(vel_z, 2.0)) * 215.0;
Float:GetVehSpeed(vehicleid)
{
	new Float:v[3];
    GetVehicleVelocity(vehicleid, v[0], v[1], v[2]);
    return gSpeed(v[0], v[1], v[2]);
}

Float:getRatio(kills, deaths)
{
	new Float:ratio = float(kills) / float(deaths);
	if(float(deaths) == 0) ratio = float(kills);
	return ratio;
}
stock Float:CompressRotation(const Float:rotation) return (rotation-floatround(rotation/360.0,floatround_floor)*360.0);
stock Float:GetDistance3D(const Float:x1, const Float:y1, const Float:z1, const Float:x2, const Float:y2, const Float:z2) return floatsqroot(floatpower(x1-x2,2.0)+floatpower(y1-y2,2.0)+floatpower(z1-z2,2.0));
stock Float:GetDistanceBetweenPoints3D(Float:x1,Float:y1,Float:z1,Float:x2,Float:y2,Float:z2) return VectorSize(x1-x2,y1-y2,z1-z2);
stock Float:GetPointsDistanceToPoint(Float:x, Float:y, Float:_x, Float:_y) return floatsqroot(((x - _x) * (x - _x)) + ((y - _y) * (y - _y)));

stock bool:IsPlayerSwimming(playerid) {
	switch(GetPlayerAnimationIndex(playerid)) { case 1538, 1539, 1541, 1544: return true; }
	return false;
}

stock bool:BlockAdminTroll(playerid, targetid) {
	if((playerData[targetid][player_admin] >= playerData[playerid][player_admin]) && playerData[targetid][player_accountID] != 1) return true;
	return false;
}

bool:SkinKobiecy(skinid)
{
	switch(skinid) {
        case 9, 10..13, 31, 38..41, 54..56, 63, 64, 69, 75..77, 85, 87, 88..93, 129, 130, 131, 138..141, 145, 148, 150..152, 157, 169, 172, 178, 190..199, 201, 205, 207, 211, 214, 215, 216, 218, 219, 224..226, 231, 232, 233, 237, 238, 243, 244..246, 251, 256, 257, 263, 298, 306..309: {
			return true;
		}
	}
	return false;
}

enum e_wipeoutObjectPosition {
	Float:wipeout_start_pos[3],
	Float:wipeout_last_pos[3]
};

new const wipeoutObjectPosition[][e_wipeoutObjectPosition] = {
	// WIPEOUT ARENA #1
	{{-3802.07617188,-310.20996094,6.84739399},				{-3802.07617188,-310.20996094,0.84739399}},
	{{-3801.23388672,-301.93493652,1.37603593},				{-3801.23339844,-301.93457031,7.12603569}},
	{{-3800.76660156,-293.40722656,7.24035597},				{-3800.76660156,-293.40722656,0.99035597}},
	{{-3794.57128906,51.73828125,23.98047447},				{-3794.57128906,51.73828125,20.48047447}},
	{{-3797.92089844,52.06152344,20.27721786},				{-3797.92089844,52.06152344,23.52721786}},
	{{-3801.25000000,52.32812500,23.67039871},				{-3801.25000000,52.32812500,19.67039871}},
	{{-3804.33496094,53.72851562,18.88958359},				{-3804.33496094,53.72851562,22.88958359}},
	{{-3807.14062500,51.83300781,20.09797478},				{-3807.14062500,51.83300781,22.59797478}},
	{{-3785.67578125,-492.27832031,9.49069786},				{-3785.67578125,-492.27832031,4.24069786}},
	{{-3792.99218750,-492.40625000,7.31917238},				{-3792.99218750,-492.40625000,-1.68082762}},
	{{-3796.90722656,-387.39453125,7.37413788},				{-3803.54907227,-387.40545654,7.37413788}},
	{{-3803.39404297,-382.87469482,7.37413788},				{-3797.13085938,-383.21438599,7.37413788}},
	{{-3797.16894531,-377.61328125,7.37413788},				{-3803.63720703,-376.95233154,7.37413788}},
	{{-3798.89941406,-242.02577209,18.20804787},			{-3802.64941406,-242.14212036,18.20804787}},
	{{-3802.84912109,-237.95976257,18.20804787},			{-3798.59863281,-237.82669067,18.20804787}},
	{{-3798.62817383,-232.00032043,18.20804787},			{-3802.62792969,-232.12451172,18.20804787}},
	{{-3802.72119141,-224.93785095,18.20804787},			{-3798.47070312,-224.80520630,18.20804787}},
	{{-3803.00122070,-208.67289734,18.20804787},			{-3798.50097656,-208.53277588,18.20804787}},
	{{-3798.36645508,-204.05342102,18.20804787},			{-3802.86621094,-204.19281006,18.20804787}},
	{{-3803.36132812,-194.90661621,18.20804787},			{-3798.36132812,-194.75061035,18.20804787}},
	{{-3797.95556641,-189.55856323,18.20804787},			{-3802.20507812,-189.68991089,18.20804787}},

	// WIPEOUT ARENA #2
	{{1607.59997559,-3002.50000000,10.69999981},			{1607.50000000,-3005.39990234,10.69999981}},	
	{{1600.40002441,-3005.10009766,13.89999962},			{1600.40002441,-3002.50000000,13.89999962}},
	{{1592.59997559,-3002.39990234,17.29999924},			{1592.40002441,-3004.89990234,17.29999924}},
	{{1576.40002441,-2999.00000000,18.79999924},			{1575.30004883,-3004.39990234,18.79999924}},
	{{1568.50000000,-3003.69995117,18.79999924},			{1569.40002441,-2998.30004883,18.79999924}},
	{{1563.00000000,-2996.80004883,18.79999924},			{1561.69995117,-3002.89990234,20.00000000}},
	{{1375.69995117,-2993.60009766,16.89999962},			{1376.19995117,-2998.80004883,16.89999962}},
	{{1351.59997559,-2995.30004883,13.69999981},			{1351.59997559,-2995.30004883,10.00000000}},

	// WIPEOUT ARENA #3
	{{3067.39892578,-2770.09619141,8.20234680},				{3054.10009766,-2769.80004883,8.19999981}},
	{{3054.38330078,-2769.82885742,8.20234680},				{3066.89990234,-2769.80004883,8.19999981}},
	{{3059.50854492,-2779.89184570,8.20234680},				{3050.89990234,-2786.10009766,8.19999981}},
	{{3064.65014648,-2774.59643555,8.20234680},				{3071.50000000,-2780.60009766,8.19999981}},
	{{3060.98388672,-2782.85205078,8.20234680},				{3061.00000000,-2782.89990234,4.50000000}},
	{{3060.97387695,-2791.88671875,8.20234680},				{3061.00000000,-2791.89990234,4.50000000}},
	{{3060.82055664,-2799.17700195,8.20234680},				{3060.80004883,-2799.19995117,4.19999981}},
	{{3060.74926758,-2807.46411133,8.20234680},				{3060.69995117,-2807.50000000,4.19999981}},
	{{3060.50854492,-2818.84741211,8.20234680},				{3060.50000000,-2818.80004883,12.50000000}},
	{{3060.22656250,-2823.33789062,8.20234680},				{3060.19995117,-2823.30004883,12.19999981}},
	{{3059.92968750,-2828.07861328,8.20234680},				{3059.89990234,-2828.10009766,12.00000000}},
	{{3061.76074219,-2940.69628906,4.00000000},				{3061.80004883,-2940.69995117,1.00000000}},
	{{3062.24023438,-3017.24096680,0.00000000},				{3062.19995117,-3017.19995117,2.00000000}},
	{{3064.00439453,-3132.40917969,1.00000000},				{3064.00000000,-3132.39990234,7.80000019}},

	// WIPEOUT ARENA #4
	{{3284.42944336,-470.49111938,49.09507751},				{3284.39990234,-470.50000000,44.79999924}},
	{{3278.67626953,-470.08715820,49.09507751},				{3278.69995117,-470.10000610,44.59999847}},
	{{3291.24365234,-917.56616211,21.80256653},				{3291.19995117,-917.59997559,14.10000038}},
	{{3291.29492188,-812.12591553,21.80256653},				{3291.30004883,-812.09997559,16.29999924}},
	{{3302.40625000,-606.51562500,34.00000000},				{3302.39990234,-606.50000000,26.79999924}},
	{{3312.26074219,-540.63085938,25.63918495},				{3312.30004883,-540.59997559,22.89999962}},
	{{3294.00000000,-675.59997559,-3.00000000},				{3295.19995117,-659.40002441,-3.00000000}}
};
