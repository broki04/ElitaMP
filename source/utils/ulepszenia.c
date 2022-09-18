#define ULEPSZENIE_INVISIBLE        0
#define ULEPSZENIE_HEAL             1
#define ULEPSZENIE_PRIVCAR          2
#define ULEPSZENIE_KAMIZELKA        3
#define ULEPSZENIE_STYLE            4

#define ULEPSZENIE_KILLER           0
#define ULEPSZENIE_ACHIEVEMENT      1
#define ULEPSZENIE_QUEST            2

enum e_ulepszeniaData {
    ulep_id,
    ulep_name[50],
    ulep_description[255],
    ulep_money
};

new 
    ulepszeniaData[][e_ulepszeniaData] = {
        {ULEPSZENIE_INVISIBLE,  "Niewidzialnoœæ",           "Mo¿liwoœæ w³¹czenia niewidzialnoœci komend¹ /invisible.",      300000},
        {ULEPSZENIE_HEAL,       "£atwiejsze leczenie",      "Mniejszy czas blokady leczenia - z 30 sekund do 10 sekund.",   500000},
        {ULEPSZENIE_PRIVCAR,    "Wiêkszy gara¿",            "Zwiêkszenie limitu posiadanych pojazdów z 3 do 5.",            5000000},
        {ULEPSZENIE_KAMIZELKA,  "Kamizelka na spawnie",     "Kamizelka na ka¿dym spawnie po miniêciu ochrony przez SK.",    50000},
        {ULEPSZENIE_STYLE,      "Zmiana stylu walki",       "Mo¿liwoœæ zmiany stylu walki, u¿ywaj¹c komendy /style.",       70000}
    },
    ulepszeniaLevelData[][] = {"Premia za zabójstwo", "Cenniejsze osi¹gniêcia", "Wiêksza stawka za zadanie dnia"},

    bool:pUlepszenia[MAX_PLAYERS][sizeof(ulepszeniaData)],
    pUlepszeniaLevel[MAX_PLAYERS][sizeof(ulepszeniaLevelData)]
;

stock ResetUlepszeniaData(playerid)
{
    for(new i = 0; i < sizeof(ulepszeniaData); i++) pUlepszenia[playerid][i] = false;
    for(new i = 0; i < sizeof(ulepszeniaLevelData); i++) pUlepszeniaLevel[playerid][i] = 0;
}

stock getUlepszeniaData(playerid)
{
    new str[128];
    for(new i = 0; i < sizeof(ulepszeniaData); i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pUlepszenia[playerid][i]);
    return str;
}

stock getUlepszeniaLevelData(playerid)
{
    new str[128];
    for(new i = 0; i < sizeof(ulepszeniaLevelData); i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pUlepszeniaLevel[playerid][i]);
    return str;
}

stock ulepszenieError(playerid, i)
{
    s[0] = EOS;
    catstr(s, C_RED, C_YELLOW, "Nie posiadasz odblokowanego ulepszenia: {c}%s{/c}.\n", ulepszeniaData[i][ulep_name]);
    catstr(s, C_RED, C_YELLOW, "Ulepszenie odblokujesz pod komend¹: {c}/ulepszenia{/c}.");
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Wyst¹pi³ b³¹d", s, "OK", #), 1;
}

stock OnDialogUlepszenia(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_ULEPSZENIA: {
            if(!response) return 1;

            switch(listitem) {
                case 0..1: callcmd::ulepszenia(playerid);

                case 2: {
                    s[0] = EOS;
                    s = ""WHITE"Ulepszenie\t"BLUE"Cena\t"RED"Sta"GREEN"tus\n";
                    
                    for(new i = 0; i < sizeof(ulepszeniaData); i++) catstr(s, C_WHITE, -1, ""WHITE"%s\t"BLUE"%s$\t%s\n", ulepszeniaData[i][ulep_name], strdot(ulepszeniaData[i][ulep_money]), (pUlepszenia[playerid][i]) ? (""GREEN"(TAK)") : (""RED"(NIE)"));
                    showDialog(playerid, DIALOG_ULEPSZENIA_SELECT, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Ulepszenia jednorazowe", s, "Zakup", "Cofnij");
                }

                case 3: {
                    s[0] = EOS;
                    s = ""WHITE"Ulepszenie\t"BLUE"Poziom ulepszenia\n";

                    for(new i = 0; i < sizeof(ulepszeniaLevelData); i++) {
                        new tempBuffer[65];
                        switch(pUlepszeniaLevel[playerid][i]) {
                            case 1: tempBuffer = ""GREEN"I"RED"IIII";
                            case 2: tempBuffer = ""GREEN"II"RED"III";
                            case 3: tempBuffer = ""GREEN"III"RED"II";
                            case 4: tempBuffer = ""GREEN"IIII"RED"I";
                            case 5: tempBuffer = ""GREEN"IIIII";
                            default: tempBuffer = ""RED"IIIII";
                        }
                        catstr(s, C_WHITE, -1, ""WHITE"%s\t%s\n", ulepszeniaLevelData[i], tempBuffer);
                    }
                    showDialog(playerid, DIALOG_ULEPSZENIA_LEVEL, DIALOG_STYLE_TABLIST_HEADERS, ""GREEN"Ulepszenia poziomowane", s, "Ulepsz", "Cofnij");
                }
            }
            return 1;
        }

        case DIALOG_ULEPSZENIA_LEVEL: {
            if(!response) return callcmd::ulepszenia(playerid);

            if(pUlepszeniaLevel[playerid][listitem] >= 5) return dialogBox(playerid, C_RED, ""RED"Wybrane ulepszenie posiada ju¿ maksymalny poziom ulepszenia!");
            if(playerData[playerid][player_levelPoints] < 1) return dialogBox(playerid, C_RED, ""RED"By móc zwiêkszyæ poziom ulepszenia, potrzebujesz "YELLOW"1 pkt. "RED"umiejêtnoœci.\n"RED"Punkty umiejêtnoœci postaci dostajesz za "YELLOW"zwiêkszenie poziomu "RED"gry.");
        
            playerData[playerid][player_levelPoints] -= 1;
            pUlepszeniaLevel[playerid][listitem] ++;

            m_pquery_format("UPDATE es_accounts SET ulepszenia_level='%s' WHERE id=%d LIMIT 1;", getUlepszeniaLevelData(playerid), playerData[playerid][player_accountID]);
            msg(playerid, C_ORANGE, "Zwiêkszy³eœ(aœ) poziom ulepszenia {c}\"%s\"{/c} do {c}%d poz{/c}.", ulepszeniaLevelData[listitem], pUlepszeniaLevel[playerid][listitem]);
            return callcmd::ulepszenia(playerid);
        }

        case DIALOG_ULEPSZENIA_SELECT: {
            if(!response) return callcmd::ulepszenia(playerid);

            if(pUlepszenia[playerid][listitem]) return dialogBox(playerid, C_RED, ""RED"Posiadasz ju¿ zakupione to ulepszenie.");

            SetPVarInt(playerid, "ulepszenie_listitem", listitem);

            s[0] = EOS;
            catstr(s, C_WHITE, -1, "Czy napewno chcesz zakupiæ ulepszenie "RED"%s{/c}?\n", ulepszeniaData[listitem][ulep_name]);
            catstr(s, C_WHITE, -1, "Bêdzie Ciê to kosztowa³o: "YELLOW"%s${/c}.\n", strdot(ulepszeniaData[listitem][ulep_money]));
            catstr(s, C_WHITE, -1, "Zwrot zakupu "LRED"nie bêdzie{/c} mo¿liwy!");

            new title[128]; catstr(title, C_ORANGE, -1, "%s", ulepszeniaData[listitem][ulep_name]);
            return showDialog(playerid, DIALOG_ULEPSZENIE_BUY, DIALOG_STYLE_MSGBOX, title, s, "Zakup", "Cofnij"), 1;
        }

        case DIALOG_ULEPSZENIE_BUY: {
            if(!response) return OnDialogResponse(playerid, DIALOG_ULEPSZENIA, 1, 2, "");

            new i = GetPVarInt(playerid, "ulepszenie_listitem");
            if(pUlepszenia[playerid][i]) return dialogBox(playerid, C_RED, ""RED"Posiadasz ju¿ zakupione to ulepszenie.");
            if(getMoney(playerid) < ulepszeniaData[i][ulep_money]) return valueError(playerid, ulepszeniaData[i][ulep_money]);
            giveMoney(playerid, -ulepszeniaData[i][ulep_money]);

            pUlepszenia[playerid][i] = true;
            m_pquery_format("UPDATE es_accounts SET ulepszenia='%s' WHERE id=%d LIMIT 1;", getUlepszeniaData(playerid), playerData[playerid][player_accountID]);
            return msg(playerid, C_LBLUE, "Zakupiono ulepszenie {c}\"%s\"{/c} - cena: {c}%s${/c}.", ulepszeniaData[i][ulep_name], strdot(ulepszeniaData[i][ulep_money])), 1;
        }
    }
    return 0;
}

alias:ulepszenia("abilities", "umiejetnosci")
CMD:ulepszenia(playerid)
{
    s[0] = EOS;
    catstr(s, C_BLUE, -1, "Punkty umiejêtnoœci: {c}%s pkt{/c}.\n", strdot(playerData[playerid][player_levelPoints]));
    strcat(s, " \n");
    catstr(s, C_BLUE2, -1, "1. \t"WHITE"Ulepszenia jednorazowe "LY"(iloœæ: %d)\n", sizeof(ulepszeniaData));
    catstr(s, C_BLUE2, -1, "2. \t"WHITE"Ulepszenia poziomowane "LY"(iloœæ: %d)\n", sizeof(ulepszeniaLevelData));
    return showDialog(playerid, DIALOG_ULEPSZENIA, DIALOG_STYLE_LIST, ""GOLD"Ulepszenia postaci", s, "Wybierz", "Zamknij"), 1;
}
