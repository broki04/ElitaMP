#define ARROW                       "»"
#define EURO                        "€"

#define NAME                        ""WHITE"Polski "YELLOW"Elita "GREEN"Serwer"
#define SERVER_TAG                  ""YELLOW"Elita"GREEN"Serwer"

#define KEY_AIM					    128

#define playSound(%0,%1)            PlayerPlaySound(%0, %1, 0.0, 0.0, 0.0)
#define form(%0)                    (format(strx, sizeof(strx), %0), strx)
#define function%1(%2)              forward %1(%2); public %1(%2)
#define Loop(%0,%1)                 for(new %0, __li%0 = %1; %0 != __li%0; %0++)
#define IsValidSkin(%0)             ((0 <= %0 <= 311) && %0 != 74)
#define IsValidWeapon(%0)           ((%0 >= 0 || %0 <= 18) || (%0 >= 22 && %0 <= 46)) && !(%0 > 18 || %0 < 22)
#define IsPremium(%0)               (Iter_Contains(Premium, %0))
#define GetVehicleName(%0)          (VehicleNames[%0 - 400])
#define setOdliczanie(%0,%1)		playerData[%0][player_countdown] = %1
#define CheckPlayerGame(%0,%1)      (Iter_Contains(Game[%1], %0) && pGame[%0] == %1 && GameStarted[%1])
#define ResetVehiclePaintjob(%0)    ChangeVehiclePaintjob(%0, 3)    

#define RGBAToARGB(%0)              (%0 >>> 8 | %0 << 24)
#define RGBAToRGB(%0)               (((%0) >>> 8) & 0xFFFFFF)

#define unpackXYZ(%0)               %0[0],%0[1],%0[2]
#define unpackXYZR(%0)              %0[0],%0[1],%0[2],%0[3]
#define setAlpha(%0,%1)             (((%0 >> 24) & 0xFF) << 24 | ((%0 >> 16) & 0xFF) << 16 | ((%0 >> 8) & 0xFF) << 8 | floatround((float(%0 & 0xFF) / 255) * %1))
#define dli(%1,%2,%3,%4)            ((%1==1)?(%2):(((%1% 10>1)&&(%1% 10<5)&&!((%1% 100>=10)&&(%1% 100<=21)))?(%3):(%4)))

#define PRESSED(%0)                 (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#define RELEASED(%0)                (((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))
#define HOLDING(%0)                 ((newkeys & (%0)) == (%0))

#define setInterior                 SetPlayerInterior    
#define setWorld                    SetPlayerVirtualWorld

#define m_pquery_inline             MySQL_PQueryInline
#define m_tquery_inline             MySQL_TQueryInline

#define BET_DUEL        0
#define BET_GAME        1
#define BET_EVENT       2
#define BET_SPAR        3
#define BET_ZONE        4

#define GRACZ           0
#define MODERATOR       1
#define ADMIN           2
#define VICE            3
#define HEAD            4

#define blockCMD(%0,%1,%2); \
    if(IsBlockCommand(%0,%1,%2) == 1) \
        return 1;

#if !defined isnull
    #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

#define TIME_TYPE_MS            0
#define TIME_TYPE_UNIX          1

#define ERROR_NOT_FOUND	 		0xFFFE
#define ERROR_FOUND_MULTIPLE 	0xFFFF

#define ADMIN_TOP_POINTS        0
#define ADMIN_TOP_EVENTS        1

#define ADMIN_PENALTY_BAN 		0
#define ADMIN_PENALTY_KICK 		1
#define ADMIN_PENALTY_MUTE 		2
#define ADMIN_PENALTY_WARN 		3
#define ADMIN_PENALTY_JAIL 		4

// virtual worlds
enum {
    VW_MAGAZYNIER = 100,
    VW_KURIER = 101,
    VW_GRABARZ = 102,
    VW_ZRZUT,
    VW_TAXI,
    VW_MYSLIWY,
    VW_DRWAL,
    VW_GRZYBIARZ,
    VW_AMMUNATION,
    VW_WYBIERALKA,
    VW_BANK,
    VW_HEIST,
    VW_FUEL,
    VW_JAIL,
    VW_CH,
    VW_ZP,
    VW_DA,
    VW_RC,
    VW_BTR,
    VW_SM,
    VW_GG,
    VW_WP,
    VW_SK,
    VW_WG,
    VW_WORD,
    VW_EVENT,
    VW_BUSINESS,
    VW_ARENA
};

// streamer magic
#define ZONE_MAGIC                  1000
#define NODM_MAGIC                  2000
#define BASE_MAGIC                  3000

// prefix 
#define PYTAJNIK                    ""VIOLET"?  "
#define PLUS                        ""GREEN"+  "

// chat settings
#define CHAT_TYPE_GLOBAL            0
#define CHAT_TYPE_LOCAL             1
#define CHAT_TYPE_GANGS             2
