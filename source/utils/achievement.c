#include YSI_Coding\y_hooks

enum e_achievementData {
    achievement_type,
    achievement_name[50],
    achievement_description[255],
    bool:achievement_levelable,
    achievement_levels,
    achievement_required[5],
    achievement_money,
    bool:achievement_display
};

enum {
    ACH_KILLS = 0,
    ACH_DEATHS = 1,
    ACH_LOYAL,
    ACH_DUEL,
    ACH_QUIZ,
    ACH_QUEST,
    ACH_PEDOFIL,
    ACH_HEIST,
    ACH_DISKS,
    ACH_FIGURKA,
    ACH_KONFIDENT,
    ACH_RYBAK,
    ACH_MAGAZYNIER,
    ACH_GRABARZ,
    ACH_ZRZUT,
    ACH_MYSLIWY,
    ACH_GRZYBIARZ,
    ACH_TAXI,
    ACH_DRWAL,
    ACH_TRUCK,
    ACH_PRIVCAR,
    ACH_SCHOOL,
    ACH_GORNIK,
    ACH_BTR,
    ACH_CH,
    ACH_DA,
    ACH_GG,
    ACH_RC,
    ACH_SK,
    ACH_SM,
    ACH_WG,
    ACH_WP,
    ACH_ZP,
    ACH_JAIL,
    ACH_KURCZAK,
};

new const achievementData[][e_achievementData] = {
    {ACH_KILLS,         "Zabijaka",             "Musisz zabi� okre�lon� ilo�� os�b.",               true,   5,      {100, 300, 500, 1000, 10000},   1000,       false},
    {ACH_DEATHS,        "Ofiara losu",          "Musisz zgin�� okre�lon� ilo�� razy.",              true,   5,      {100, 300, 500, 1000, 10000},   300,        false},
    {ACH_LOYAL,         "Wierny gracz",         "Musisz przegra� okre�lon� liczb� godzin.",         true,   5,      {3, 10, 50, 100, 500},          10000,      true},
    {ACH_DUEL,          "Mistrz solowek",       "Musisz wygra� okre�lon� ilo�� dueli.",             true,   5,      {10, 50, 100, 200, 500},        500,        true},
    {ACH_QUIZ,          "Testy reakcji",        "Musisz wykona� okre�lon� ilo�� test�w reakcji.",   true,   5,      {5, 10, 50, 100, 500},          750,        true},
    {ACH_QUEST,         "Zadania dnia",         "Musisz wykona� okre�lon� ilo�� zada� dnia.",       true,   5,      {1, 10, 30, 50, 100},           8000,       false},
    {ACH_PEDOFIL,       "Lowca pedofili",       "Musisz z�apa� okre�lon� ilo�� pedofili.",          true,   5,      {1, 10, 50, 100, 300},          3000,       true},
    {ACH_HEIST,         "Kradziej",             "Musisz okra�� okre�lon� ilo�� razy skarbiec.",     true,   5,      {10, 100, 300, 500, 1000},      500,        true},
    {ACH_DISKS,         "Dyskietki",            "Musisz znale�� okre�lon� ilo�� dyskietek.",        true,   5,      {10, 50, 100, 500, 1000},       700,        true},
    {ACH_FIGURKA,       "Figurki",              "Musisz znale�� okre�lon� ilo�� figurek.",          true,   5,      {1, 10, 50, 100, 500},          4000,       true},
    {ACH_KONFIDENT,     "Konfident",            "Musisz zg�osi� okre�lon� ilo�� os�b.",             true,   5,      {1, 10, 50, 100, 500},          300,        true},
    {ACH_RYBAK,         "Rybak",                "Musisz wy�owi� okre�lon� ilo�� ryb.",              true,   5,      {10, 100, 300, 500, 1000},      500,        true},
    {ACH_MAGAZYNIER,    "Magazynier",           "Musisz od�o�y� okre�lon� ilo�� towar�w.",          true,   5,      {10, 50, 100, 500, 1000},       800,        true},
    {ACH_GRABARZ,       "Grabarz",              "Musisz pogrzeba� okre�lon� ilo�� zw�ok.",          true,   5,      {10, 50, 100, 500, 1000},       800,        true},
    {ACH_ZRZUT,         "Zrzut paczki",         "Musisz zrzuci� okre�lon� ilo�� paczek.",           true,   5,      {10, 50, 100, 500, 1000},       800,        true},
    {ACH_MYSLIWY,       "Mysliwy",              "Musisz zabi� okre�lon� ilo�� jeleni.",             true,   5,      {10, 50, 100, 500, 1000},       800,        true},
    {ACH_GRZYBIARZ,     "Grzybiarz",            "Musisz zebra� okre�lon� ilo�� grzyb�w.",           true,   5,      {10, 50, 100, 500, 1000},       800,        true},
    {ACH_TAXI,          "Taksowkarz",           "Musisz zawie�� okre�lon� ilo�� klient�w.",         true,   5,      {10, 50, 100, 500, 1000},       800,        true},
    {ACH_DRWAL,         "Drwal",                "Musisz �ci�� okre�lon� ilo�� drzew.",              true,   5,      {10, 50, 100, 500, 1000},       800,        true},
    {ACH_TRUCK,         "Spedycja",             "Musisz dostarczy� okre�lon� ilo�� towar�w.",       true,   5,      {10, 100, 500, 1000, 10000},    700,        true},
    {ACH_PRIVCAR,       "Nowa fura",            "Musisz zakupi� drugi prywatny pojazd.",            false,  -1,     {0, 0, 0, 0, 0},                1000,       true},
    {ACH_SCHOOL,        "Prawko",               "Musisz zda� jakikolwiek egzamin na prawo jazdy.",  false,  -1,     {0, 0, 0, 0, 0},                300,        true},
    {ACH_GORNIK,        "Gornik",               "Musisz wykopa� odpowiedni� ilo�� �y� g�rniczych.", true,   5,      {10, 100, 300, 500, 1000},      600,        true},
    {ACH_BTR,           "Battle Royale",        "Musisz wygra� zabaw� /btr okre�lon� ilo�� razy.",  true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_CH,            "Chowany",              "Musisz wygra� zabaw� /ch okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_DA,            "Death Area",           "Musisz wygra� zabaw� /da okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_GG,            "Gun Game",             "Musisz wygra� zabaw� /gg okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_RC,            "Wyscigi",              "Musisz wygra� zabaw� /rc okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_SK,            "Sianko",               "Musisz wygra� zabaw� /sk okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_SM,            "Sumo",                 "Musisz wygra� zabaw� /sm okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_WG,            "War Game",             "Musisz wygra� zabaw� /wg okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_WP,            "Wipeout",              "Musisz wygra� zabaw� /wp okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_ZP,            "Zgadnij pojazd",       "Musisz wygra� zabaw� /zp okre�lon� ilo�� razy.",   true,   5,      {1, 5, 10, 100, 300},           500,        true},
    {ACH_JAIL,          "Wiezien",              "Musisz chocia� raz wyl�dowa� we wi�zieniu.",       false,  -1,     {0, 0, 0, 0, 0},                100,        true},
    {ACH_KURCZAK,       "Kurczak",              "Musisz zabi� okre�lon� ilo�� razy kurczaka.",      true,   5,      {1, 10, 50, 100, 300},          300,        true}
};

new 
    pAchievementData[MAX_PLAYERS][sizeof(achievementData)],
    pAchievement[MAX_PLAYERS][sizeof(achievementData)],
    pAchievementStatus[MAX_PLAYERS][sizeof(achievementData)],
    pAchievementLevel[MAX_PLAYERS][sizeof(achievementData)],
    bool:pAchievementDone[MAX_PLAYERS][sizeof(achievementData)]
;

hook function ResetPlayerData(playerid) {
    for(new i = 0; i < sizeof(achievementData); i++) {
        pAchievementData[playerid][i] = pAchievement[playerid][i] = pAchievementStatus[playerid][i] = pAchievementLevel[playerid][i] = 0;
        pAchievementDone[playerid][i] = false; }
    return continue(playerid);
}

stock GetAchievement(playerid, achievementid) return pAchievement[playerid][achievementid];
stock achievement(playerid, achievementid)
{
    if(achievementid == -1) return 1;
    pAchievement[playerid][achievementid] ++;
    pAchievementStatus[playerid][achievementid] ++;

    if(pAchievementDone[playerid][achievementid]) return 1;
    if(achievementData[achievementid][achievement_levelable]) {
        new level = pAchievementLevel[playerid][achievementid];
        if((pAchievementStatus[playerid][achievementid] >= achievementData[achievementid][achievement_required][level]) && !pAchievementDone[playerid][achievementid]) {
            pAchievementLevel[playerid][achievementid] ++;
            pAchievementStatus[playerid][achievementid] = 0;

            if(pAchievementLevel[playerid][achievementid] >= achievementData[achievementid][achievement_levels]) {
                pAchievementDone[playerid][achievementid] = true;

                new money = achievementData[achievementid][achievement_money] * achievementData[achievementid][achievement_levels];
                if(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT] > 0) {
                    new Float:chuj = 0.0;
                    switch(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT]) {
                        case 1: chuj = 0.10;
                        case 2: chuj = 0.25;
                        case 3: chuj = 0.35;
                        case 4: chuj = 0.50;
                        case 5: chuj = 0.75;
                    }
                    money += floatround(money * chuj);
                }

                giveMoney(playerid, money);
                addLevelScore(playerid, math_random(300, 500));

                playerTextDrawSetString(playerid, tdAchievement[0], "%s", achievementData[achievementid][achievement_name]);
                playerTextDrawSetString(playerid, tdAchievement[1], "~w~~h~Wykonales(as) ~y~~h~cale ~w~~h~osiagniecie!~n~~w~~h~Nagroda: ~r~~h~%s$", strdot(money));
            } else {
                new money = achievementData[achievementid][achievement_money];
                if(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT] > 0) {
                    new Float:chuj = 0.0;
                    switch(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT]) {
                        case 1: chuj = 0.10;
                        case 2: chuj = 0.25;
                        case 3: chuj = 0.35;
                        case 4: chuj = 0.50;
                        case 5: chuj = 0.75;
                    }
                    money = money + floatround(money * chuj);
                }

                giveMoney(playerid, money);
                addLevelScore(playerid, math_random(30, 50));
           
                playerTextDrawSetString(playerid, tdAchievement[0], "%s", achievementData[achievementid][achievement_name]);
                playerTextDrawSetString(playerid, tdAchievement[1], "~w~~h~Wykonales(as) ~y~~h~poz. %d ~w~~h~osiagniecia!~n~~w~~h~Nagroda: ~r~~h~%s$", pAchievementLevel[playerid][achievementid] + 1, strdot(money));
            }

            for(new i = 0; i < 2; i++) PlayerTextDrawShow(playerid, tdAchievement[i]);
            SetTimerEx("HideAchievement", 1000 * 8, false, "d", playerid);
        } else {
            if(achievementData[achievementid][achievement_display]) {
                playerTextDrawSetString(playerid, tdAchievement[0], "%s", achievementData[achievementid][achievement_name]);
                playerTextDrawSetString(playerid, tdAchievement[1], "~w~~h~Postep osiagniecia ~p~~h~(poz. %d)~w~~h~: ~g~~h~~h~%d~w~~h~/~g~~h~%d", pAchievementLevel[playerid][achievementid], pAchievementStatus[playerid][achievementid], achievementData[achievementid][achievement_required][level]);

                SetTimerEx("HideAchievement", 1000 * 5, false, "d", playerid);
            }
        }
    } else {
        pAchievementDone[playerid][achievementid] = true;

        new money = achievementData[achievementid][achievement_money];
        if(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT] > 0) {
            new Float:chuj = 0.0;
            switch(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT]) {
                case 1: chuj = 0.10;
                case 2: chuj = 0.25;
                case 3: chuj = 0.35;
                case 4: chuj = 0.50;
                case 5: chuj = 0.75;
            }
            money = money + floatround(money * chuj);
        }

        giveMoney(playerid, money);
        addLevelScore(playerid, math_random(100, 300));

        playerTextDrawSetString(playerid, tdAchievement[0], "%s", achievementData[achievementid][achievement_name]);
        playerTextDrawSetString(playerid, tdAchievement[1], "~w~~h~Wykonales(as) ~y~~h~jednorazowe ~w~~h~osiagniecie!~n~~w~~h~Nagroda: ~r~~h~%s$", strdot(money));

        for(new i = 0; i < 2; i++) PlayerTextDrawShow(playerid, tdAchievement[i]);
        SetTimerEx("HideAchievement", 1000 * 8, false, "d", playerid);
    }
    return 1;
}

function HideAchievement(playerid) {
    for(new i = 0; i < 2; i++) PlayerTextDrawHide(playerid, tdAchievement[i]);
    return 1;
}

stock GetAchievementCount(playerid)
{
    new astr[512];
    for(new i = 0; i < sizeof(achievementData); i++) format(astr, sizeof(astr), "%s%s%d", astr, i == 0 ? ("") : (","), pAchievement[playerid][i]);
    return astr;
}

stock GetAchievementStatus(playerid)
{
    new astr[512];
    for(new i = 0; i < sizeof(achievementData); i++) format(astr, sizeof(astr), "%s%s%d", astr, i == 0 ? ("") : (","), pAchievementStatus[playerid][i]);
    return astr;
}

stock GetAchievementLevel(playerid)
{
    new astr[512];
    for(new i = 0; i < sizeof(achievementData); i++) format(astr, sizeof(astr), "%s%s%d", astr, i == 0 ? ("") : (","), pAchievementLevel[playerid][i]);
    return astr;
}

stock GetAchievementDone(playerid)
{
    new astr[512];
    for(new i = 0; i < sizeof(achievementData); i++) format(astr, sizeof(astr), "%s%s%d", astr, i == 0 ? ("") : (","), pAchievementDone[playerid][i]);
    return astr;
}

alias:osiagniecia("achievements")
CMD:osiagniecia(playerid)
{
    ClearDialogListitems(playerid);

    s[0] = EOS;
    strcat(s, ""WHITE"1. \t"BLUE"Wszystkie osi�gni�cia\n");
    strcat(s, ""WHITE"2. \t"BLUE"Osi�gni�cia wykonane\n");
    strcat(s, ""WHITE"3. \t"BLUE"Osi�gni�cia niewykonane\n");
    strcat(s, ""WHITE"4. \t"BLUE"Osi�gni�cia poziomowane\n");
    strcat(s, ""WHITE"5. \t"BLUE"Osi�gni�cia niepoziomowane\n");
    return showDialog(playerid, DIALOG_ACHIEVEMENTS, DIALOG_STYLE_LIST, ""GOLD"Kategorie osi�gni��", s, "Wybierz", "Zamknij"), 1;
}

stock ShowAchievementList(playerid, typeid)
{
    AddDialogListitem(playerid, ""CORAL"Osi�gniecie\t"WHITE"Status\n");

    new x = 0;
    for(new i = 0; i < sizeof(achievementData); i++) {
        new bool:achievement_done = (pAchievementDone[playerid][i]) ? (true) : (false), bool:achievement_canLevel = (achievementData[i][achievement_levelable]) ? (true) : (false);

        if(achievement_canLevel && typeid != 4) {
            if(achievement_done && typeid != 2) {
                AddDialogListitem(playerid, ""CORAL"%s\t"GREEN"Wykonane "LY"(%d lvl)\n", achievementData[i][achievement_name], achievementData[i][achievement_levels]);
                pAchievementData[playerid][x++] = i;
            } else if(typeid != 1 && !achievement_done) {
                AddDialogListitem(playerid, ""CORAL"%s\t"WHITE"%d/%d "LY"(%d lvl)\n", achievementData[i][achievement_name], pAchievement[playerid][i], achievementData[i][achievement_required][pAchievementLevel[playerid][i]], pAchievementLevel[playerid][i]);
                pAchievementData[playerid][x++] = i;
            }
        } else if(typeid != 3 && !achievement_canLevel) {
            if(achievement_done && typeid != 2) {
                AddDialogListitem(playerid, ""CORAL"%s\t"GREEN"Wykonane\n", achievementData[i][achievement_name]);
                pAchievementData[playerid][x++] = i;
            } else if(typeid != 1 && !achievement_done) {
                AddDialogListitem(playerid, ""CORAL"%s\t"GREY"???\n", achievementData[i][achievement_name]);
                pAchievementData[playerid][x++] = i;
            }
        }
    }

    playerData[playerid][player_saveData][0] = typeid;
    return showDialog(playerid, DIALOG_ACHIEVEMENT_LIST, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Osi�gni�cia", #, "Wybierz", "Cofnij", 10), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_ACHIEVEMENTS: {
            if(!response) return 1;

            ClearDialogListitems(playerid);
            return ShowAchievementList(playerid, listitem), 1;
        }

        case DIALOG_ACHIEVEMENT_LIST: {
            if(!response) return callcmd::osiagniecia(playerid);

            if(listitem != INVALID_LISTITEM) {
                new i = pAchievementData[playerid][listitem], money = achievementData[i][achievement_money];
                if(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT] > 0) {
                    new Float:chuj = 0.0;
                    switch(pUlepszeniaLevel[playerid][ULEPSZENIE_ACHIEVEMENT]) {
                        case 1: chuj = 0.10;
                        case 2: chuj = 0.25;
                        case 3: chuj = 0.35;
                        case 4: chuj = 0.50;
                        case 5: chuj = 0.75;
                    }
                    money = money + floatround(money * chuj);
                }

                s[0] = EOS;
                s = ""BLUE2"Kryterium\t"WHITE"Wynik\n";
                catstr(s, C_RED, -1, "Nazwa:\t"WHITE"%s%s\n", achievementData[i][achievement_name], (achievementData[i][achievement_levelable]) ? (form(" "LY"(%d lvl)", pAchievementLevel[playerid][i])) : (""));
                catstr(s, C_BLUE2, -1, "Status:\t"WHITE"%s\n", (!pAchievementDone[playerid][i]) ? ((achievementData[i][achievement_levelable]) ? (form("%d/%d", pAchievement[playerid][i], achievementData[i][achievement_required][pAchievementLevel[playerid][i]])) : (""RED"Nie wykonano")) : (""GREEN"Wykonane"));
                catstr(s, C_YELLOW, -1, "Nagroda:\t"WHITE"%s$\n", strdot(money));
                catstr(s, C_ORANGE, -1, "Opis:\t"WHITE"%s", achievementData[i][achievement_description]);
            
                inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                    #pragma unused pp, dd, rr, ll, ii
                    return ShowAchievementList(playerid, playerData[playerid][player_saveData][0]); }
                new title[128]; format(title, sizeof(title), ""LGREEN"%s", achievementData[i][achievement_name]);
                Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, title, s, "OK", #);
            }
            return 1;
        }
    }
    return 0;
}
