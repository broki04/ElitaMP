#include YSI_Coding\y_hooks

enum e_fishData {
    fish_name[50],
    fish_respect,
    fish_money,
    Float:fish_weight[2],
    fish_class
};

#define FISH_CLASS_REGULAR          0   // pospolita
#define FISH_CLASS_RARE             1   // rzadka
#define FISH_CLASS_LEGENDARY        2   // legendarna

new const fishData[][e_fishData] = {
    // nazwa                    // nagroda      // waga             // klasa
    {"ŒledŸ oceaniczny",        10, 1000,       {1.0, 1.5},         FISH_CLASS_REGULAR},
    {"Belona",                  10, 1000,       {0.4, 1.0},         FISH_CLASS_REGULAR},
    {"P³oæ",                    10, 1000,       {0.5, 1.0},         FISH_CLASS_REGULAR},
    {"Certa",                   10, 1200,       {0.5, 1.0},         FISH_CLASS_REGULAR},
    {"Ognista pstra",           10, 1350,       {1.0, 2.0},         FISH_CLASS_REGULAR},
    {"Beryks",                  10, 1500,       {2.0, 4.0},         FISH_CLASS_REGULAR},
    {"Wzdrêga",                 11, 1100,       {1.0, 2.0},         FISH_CLASS_REGULAR},
    {"Lin",                     11, 2000,       {4.0, 6.0},         FISH_CLASS_REGULAR},
    {"Parposz",                 11, 2300,       {1.0, 2.0},         FISH_CLASS_REGULAR},
    {"Sola zwyczajna",          11, 2500,       {2.0, 3.0},         FISH_CLASS_REGULAR},
    {"Sargus",                  11, 3000,       {3.0, 4.0},         FISH_CLASS_REGULAR},
    {"Koleñ",                   11, 3500,       {4.0, 9.0},         FISH_CLASS_REGULAR},
    {"Pstr¹g têczowy",          12, 1500,       {5.0, 9.0},         FISH_CLASS_REGULAR},
    {"Pa³asz czarny",           12, 2000,       {1.5, 3.0},         FISH_CLASS_REGULAR},
    {"Okoñ pospolity",          12, 3000,       {2.5, 4.0},         FISH_CLASS_REGULAR},
    {"Sieja amerykañska",       13, 3500,       {2.0, 4.0},         FISH_CLASS_REGULAR},
    {"Kleñ",                    14, 2000,       {2.0, 3.0},         FISH_CLASS_REGULAR},
    {"Makrela",                 15, 2000,       {1.0, 2.0},         FISH_CLASS_REGULAR},
    {"Dorsz Atlantycki",        15, 2000,       {8.0, 9.0},         FISH_CLASS_REGULAR},
    {"Leszcz",                  15, 3000,       {3.5, 4.0},         FISH_CLASS_REGULAR},
    {"£upacz",                  15, 3000,       {2.0, 3.0},         FISH_CLASS_REGULAR},
    {"Wêgorz europejski",       15, 3000,       {2.0, 4.0},         FISH_CLASS_REGULAR},
    {"Mintaj",                  16, 4000,       {3.0, 4.0},         FISH_CLASS_REGULAR},
    {"Czeczuga",                17, 2000,       {4.0, 6.0},         FISH_CLASS_REGULAR},
    {"Mandaryn",                17, 2500,       {0.5, 1.3},         FISH_CLASS_REGULAR},
    {"Karaœ pospolity",         17, 3000,       {2.0, 3.0},         FISH_CLASS_REGULAR},
    {"Boleñ",                   20, 3500,       {3.0, 6.0},         FISH_CLASS_REGULAR},
    {"JaŸ",                     20, 3500,       {1.0, 3.0},         FISH_CLASS_REGULAR},
    {"Lipieñ",                  20, 4000,       {1.0, 2.0},         FISH_CLASS_REGULAR},
    {"Dorada królewska",        21, 4000,       {5.0, 6.0},         FISH_CLASS_REGULAR},
    {"Drakon",                  22, 3000,       {15.0, 20.0},       FISH_CLASS_REGULAR},
    {"Pstr¹g Ÿródlany",         22, 4500,       {3.0, 7.5},         FISH_CLASS_REGULAR},
    {"Plamiak",                 22, 5000,       {8.0, 10.0},        FISH_CLASS_REGULAR},
    {"Fl¹dra",                  23, 5000,       {5.0, 7.0},         FISH_CLASS_REGULAR},
    {"Wargacz kniazik",         23, 6000,       {8.0, 12.0},        FISH_CLASS_REGULAR},
    {"Keta",                    24, 2500,       {3.0, 4.0},         FISH_CLASS_REGULAR},
    {"Rdzawiec",                24, 3000,       {10.0, 12.0},       FISH_CLASS_REGULAR},
    {"£osoœ szlachetny",        24, 3000,       {20.0, 25.0},       FISH_CLASS_REGULAR},
    {"¯uchwiak",                24, 3000,       {10.0, 15.0},       FISH_CLASS_REGULAR},
    {"Brzana",                  25, 7000,       {4.0, 8.0},         FISH_CLASS_REGULAR},
    {"Labrask europejeski",     28, 3500,       {10.0, 15.0},       FISH_CLASS_REGULAR},
    {"Koryfena",                28, 5000,       {35.0, 45.0},       FISH_CLASS_REGULAR},
    {"Ryba tygrysia",           28, 8000,       {40.0, 50.0},       FISH_CLASS_REGULAR},
    {"Sum elektryczny",         28, 8500,       {18.0, 25.0},       FISH_CLASS_REGULAR},
    {"Sazan amurski",           29, 7000,       {25.0, 30.0},       FISH_CLASS_REGULAR},
    {"Solandra",                29, 8000,       {70.0, 80.0},       FISH_CLASS_REGULAR},
    {"Oœmiornica olbrzymia",    30, 2000,       {23.0, 41.0},       FISH_CLASS_REGULAR},
    {"Seriola olbrzymia",       30, 3000,       {20.0, 25.0},       FISH_CLASS_REGULAR},
    {"Ka³uga",                  30, 5000,       {50.0, 100.0},      FISH_CLASS_REGULAR},
    {"Bicuda",                  30, 6000,       {5.0, 7.0},         FISH_CLASS_REGULAR},
    {"Jesiotr perski",          30, 7800,       {35.0, 45.0},       FISH_CLASS_REGULAR},

    {"Karp",                    30, 3500,       {10.0, 18.0},       FISH_CLASS_RARE},
    {"Morszczuk atlantycki",    30, 5000,       {13.0, 15.0},       FISH_CLASS_RARE},
    {"Albula",                  31, 6000,       {7.0, 10.0},        FISH_CLASS_RARE},
    {"Sum",                     32, 4500,       {10.0, 17.0},       FISH_CLASS_RARE},
    {"Sierpnik okr¹g³y",        33, 3000,       {30.0, 40.0},       FISH_CLASS_RARE},
    {"Bufalo czarny",           33, 3000,       {25.0, 30.0},       FISH_CLASS_RARE},
    {"Szczupak",                35, 5000,       {16.0, 18.0},       FISH_CLASS_RARE},
    {"Diabe³ morski",           35, 7000,       {35.0, 40.0},       FISH_CLASS_RARE},
    {"£osoœ",                   37, 6500,       {10.0, 15.0},       FISH_CLASS_RARE},
    {"Troæ wêdrowna",           38, 6000,       {10.0, 13.0},       FISH_CLASS_RARE},
    {"P³awikonik",              39, 6000,       {0.2, 0.5},         FISH_CLASS_RARE},
    {"Miêtus",                  39, 7500,       {10.0, 20.0},       FISH_CLASS_RARE},
    {"Amur bia³y",              40, 8000,       {20.0, 30.0},       FISH_CLASS_RARE},
    {"Brosma",                  40, 8300,       {25.0, 30.0},       FISH_CLASS_RARE},
    {"Karmazyn Mentela",        40, 8500,       {4.0, 6.0},         FISH_CLASS_RARE},
    {"Wêgorz elektryczny",      41, 10000,      {15.0, 20.0},       FISH_CLASS_RARE},
    {"Karanks olbrzymi",        42, 8800,       {40.0, 50.0},       FISH_CLASS_RARE},
    {"Bia³orybica",             44, 5000,       {30.0, 40.0},       FISH_CLASS_RARE},
    {"Kr¹p",                    45, 8000,       {0.5, 1.0},         FISH_CLASS_RARE},
    {"Makaira b³êkitna",        45, 8500,       {5.0, 6.0},         FISH_CLASS_RARE},
    {"Zêbacz smugowy",          45, 10000,      {15.0, 23.0},       FISH_CLASS_RARE},
    {"Molwa pospolita",         48, 12000,      {40.0, 60.0},       FISH_CLASS_RARE},
    {"To³pyga",                 49, 13000,      {40.0, 60.0},       FISH_CLASS_RARE},
    {"Barrakuda",               50, 12500,      {30.0, 40.0},       FISH_CLASS_RARE},
    {"Tasergal",                52, 15000,      {10.0, 12.0},       FISH_CLASS_RARE},
    {"Palia jeziorowa",         53, 12000,      {15.0, 40.0},       FISH_CLASS_RARE},
    {"Witlinek",                55, 10000,      {1.0, 2.0},         FISH_CLASS_RARE},
    {"Crappie",                 60, 19000,      {35.0, 45.0},       FISH_CLASS_RARE},
    {"Chalamida",               65, 25000,      {30.0, 40.0},       FISH_CLASS_RARE},
    {"Konger",                  69, 27000,      {80.0, 110.0},      FISH_CLASS_RARE},
    {"Marlin b³êkitny",         75, 30000,      {100.0, 150.0},     FISH_CLASS_RARE},

    {"Z³ota rybka",             100, 20000,     {0.2, 0.5},         FISH_CLASS_LEGENDARY},
    {"Pirania",                 100, 30000,     {2.0, 6.0},         FISH_CLASS_LEGENDARY},
    {"Tawrosz piaskowy",        110, 35000,     {150.0, 180.0},     FISH_CLASS_LEGENDARY},
    {"Niszczuka krokodyla",     120, 50000,     {140.0, 200.0},     FISH_CLASS_LEGENDARY},
    {"Okoñ nilowy",             130, 25000,     {150.0, 200.0},     FISH_CLASS_LEGENDARY},
    {"Tuñczyk d³ugop³etwowy",   150, 40000,     {50.0, 60.0},       FISH_CLASS_LEGENDARY},
    {"Jesiotr syberyjski",      200, 50000,     {70.0, 100.0},      FISH_CLASS_LEGENDARY},
    {"Wstêgor królewski",       210, 45000,     {150.0, 200.0},     FISH_CLASS_LEGENDARY},
    {"Halibut",                 220, 65000,     {160.0, 200.0},     FISH_CLASS_LEGENDARY},
    {"Jesiotr ostronosy",       230, 50000,     {200.0, 300.0},     FISH_CLASS_LEGENDARY},
    {"Itaraja atlantycka",      235, 40000,     {280.0, 320.0},     FISH_CLASS_LEGENDARY},
    {"Pangaz",                  240, 37000,     {270.0, 300.0},     FISH_CLASS_LEGENDARY},
    {"Kosogon",                 245, 50000,     {500.0, 600.0},     FISH_CLASS_LEGENDARY},
    {"Jelec europejski",        250, 70000,     {200.0, 300.0},     FISH_CLASS_LEGENDARY},
    {"¯ar³acz têpog³owy",       260, 50000,     {300.0, 400.0},     FISH_CLASS_LEGENDARY},
    {"¯ó³w zielony",            265, 30000,     {160.0, 200.0},     FISH_CLASS_LEGENDARY},
    {"Anakonda",                270, 80000,     {250.0, 300.0},     FISH_CLASS_LEGENDARY},
    {"Ryba m³ot",               300, 90000,     {300.0, 450.0},     FISH_CLASS_LEGENDARY},
    {"Ryba pi³a",               400, 100000,    {500.0, 650.0},     FISH_CLASS_LEGENDARY}
};

new 
    FishDay,

    bool:pFishing[MAX_PLAYERS],
    pFishRode[MAX_PLAYERS],
    Float:pFishZylka[MAX_PLAYERS],
    pFishBait[MAX_PLAYERS][5],
    pFishBaitUsed[MAX_PLAYERS],
    Float:pFishStatus[MAX_PLAYERS]
;

#define INVALID_RODE_ID         -1
#define FISH_PICKUP_RODE        0
#define FISH_PICKUP_ULEP        1
#define FISH_PICKUP_RYBAK       2

hook function ResetPlayerData(playerid)
{
    pFishing[playerid] = false;
    pFishRode[playerid] = pFishBaitUsed[playerid] = INVALID_RODE_ID;
    pFishZylka[playerid] = pFishStatus[playerid] = 0.0;
    pFishBait[playerid] = {0, 0, 0, 0, 0};
    return continue(playerid);
}

stock Init_Fishing()
{
    new const Float:fishLabelPositions[][] = {
        {860.3605, 470.0412, 2.3849},
        {853.4077, 497.5674, 2.3911},
        {832.4686, 491.9124, 2.3887},
        {839.8156, 464.6304, 2.3921}
    };
    
    for(new i = 0; i < sizeof(fishLabelPositions); i++) {
        new fstr[128]; format(fstr, sizeof(fstr), ""BLUE"POMOST PO£OWOWY "LY"#%d\n"CORAL"Aby rozpocz¹æ "WHITE"³owienie"CORAL", kliknij: "WHITE"Y", i + 1);
        CreateDynamic3DTextLabel(fstr, -1, fishLabelPositions[i][0], fishLabelPositions[i][1], fishLabelPositions[i][2], 30.0, .worldid=0);
    }

    FishDay = random(sizeof(fishData));
    return printf(" >>> [FISH]: Wczytywanych rybek: %d (ryba dnia: %s)", sizeof(fishData), fishData[FishDay][fish_name]), 1;
}

alias:rybki("ryby", "lowienie", "fishing", "fishes")
CMD:rybki(playerid)
{
    s[0] = EOS;
    catstr(s, C_CORAL, -1, "Aktualnie {c}wydarzenie rybackie{/c} jest %s!\n", (fishEvent) ? ("w³¹czone") : ("wy³¹czone"));
    catstr(s, C_CORAL, -1, "Dzisiejsza rybka dnia: {c}%s{/c}\n", fishData[FishDay][fish_name]);
    strcat(s, " \n");
    catstr(s, C_LY, -1, ""ARROW"  "WHITE"Informacje o systemie rybek\n");
    catstr(s, C_LY, -1, ""ARROW"  "WHITE"Okazy rybek do z³owienia "RED"(%d okazów)\n", sizeof(fishData));
    catstr(s, C_LY, -1, ""ARROW"  "WHITE"Podium najlepszych rybaków\n");
    catstr(s, C_LY, -1, ""ARROW"  "WHITE"Mobilny sklep rybacki\n");
    catstr(s, C_LY, -1, ""ARROW"  "WHITE"Teleportuj siê na: "PINK"/Lowisko");
    return showDialog(playerid, DIALOG_FISH_PANEL, DIALOG_STYLE_LIST, ""YELLOW"System rybek", s, "Wybierz", "Zamknij"), 1;
}

CMD:fishevent(playerid)
{
    if(!fishEvent) return sendError(playerid, ""RED"Obecnie {c}event rybacki{/c} jest wy³¹czony!");

    s[0] = EOS;
    catstr(s, C_GREEN, C_LY, "Obecnie trwa {c}wydarzenie rybackie{/c} na naszym serwerze {c}aktywowane{/c} przez administracjê!\n");
    catstr(s, C_GREEN, C_LY, "Podczas aktywnego {c}wydarzenia{/c} otrzymujesz {c}wiêksze szanse{/c} na z³owienie rybki legendarnej!\n");
    catstr(s, C_GREEN, C_LY, "Jak mo¿na siê domyœliæ, otrzymujesz równie¿ wiêksz¹ nagrodê o {c}50 procent{/c} za z³owienie rybki!");
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Event rybacki", s, "OK", #), 1;
}
CMD:lowisko(playerid) return EnterTeleport(playerid, "wedkarstwo");

stock OnFishDialog(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_FISH_LIST: {
            if(!response) return callcmd::rybki(playerid);

            AddDialogListitem(playerid, ""WHITE"Nazwa ryby\t"LY"Dopuszczalna waga\t"GREEN"Nagroda\n");
            for(new i = 0; i < sizeof(fishData); i++) { if(fishData[i][fish_class] == listitem) { AddDialogListitem(playerid, ""WHITE"%s\t"LY"%.2f - %.2f kg\t"GREEN"%s$\n", fishData[i][fish_name], fishData[i][fish_weight][0], fishData[i][fish_weight][1], strdot(fishData[i][fish_money])); } }
            return showDialog(playerid, DIALOG_FISH_CATEGORY, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Okazy", #, "OK", #, 10), 1;
        }
        case DIALOG_FISH_CATEGORY: return (listitem != INVALID_LISTITEM) ? (OnDialogResponse(playerid, DIALOG_FISH_PANEL, 1, 4, "")) : (1);
        
        case DIALOG_FISH_PANEL: {
            if(!response) return 1;

            switch(listitem) {
                case 0..2: callcmd::rybki(playerid);

                case 3: {
                    // ...
                }
                case 4: showDialog(playerid, DIALOG_FISH_LIST, DIALOG_STYLE_LIST, ""BLUE2"Typy ryb", ""WHITE"Pospolite\n"WHITE"Rzadkie\n"WHITE"Legendarne", "Wybierz", "Cofnij");

                case 5: {
                    inline onRybakTop()
                    {
                        s[0] = EOS;
                        s = ""BLACK"ID\t"WHITE"Nick\t"CORAL"Wynik\n";

                        new id, nick[25], result;
                        for(new i = 0; i < cache_num_rows(); i++) {
                            cache_get_value_int(i, 0, id);
                            cache_get_value(i, 1, nick, 25);
                            cache_get_value_int(i, 2, result);

                            format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s ryb\n", s, i + 1, nick, strdot(result));
                        }

                        inline onTopDialog(pp, dd, rr, ll, string:ii[]) {
                            #pragma unused pp, dd, rr, ll, ii
                            return callcmd::rybki(playerid); }
                        return Dialog_ShowCallback(playerid, using inline onTopDialog, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Podium najlepszych rybaków", s, "OK", #), 1;
                    }

                    dbstr[0] = EOS;
                    format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_RYBAK + 1);
                    m_tquery_inline(dbconn, using inline onRybakTop, dbstr);
                }

                case 6: {
                    s[0] = EOS;
                    catstr(s, -1, C_YELLOW, ""WHITE""ARROW"  {c}Wêdki{/c}\n");
                    catstr(s, -1, C_YELLOW, ""WHITE""ARROW"  {c}Przynêty{/c}\n");
                    catstr(s, -1, C_YELLOW, ""WHITE""ARROW"  {c}Ulepszenia{/c}\n");
                    showDialog(playerid, DIALOG_FISH_SHOP, DIALOG_STYLE_LIST, ""CORAL"Sklep rybacki", s, "Wybierz", "Cofnij");
                }
                case 7: callcmd::lowisko(playerid);
            }
            return 1;
        }
    }
    return 0;
}