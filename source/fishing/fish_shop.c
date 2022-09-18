#include YSI_Coding\y_hooks

enum e_fishRodeData {
    fish_rode_type,
    fish_rode_name[25],
    fish_rode_money,
    Float:fish_rode_amount
};

#define FISH_RODE_NORMAL        0
#define FISH_RODE_SUPER         1
#define FISH_RODE_EXTRA         2
#define FISH_RODE_ELITA         3

new const fishRodeData[][e_fishRodeData] = {
    {FISH_RODE_NORMAL,      "Wêdka kl. normalnej",      5000,       1.35},
    {FISH_RODE_SUPER,       "Wêdka kl. super",          10000,      1.75},
    {FISH_RODE_EXTRA,       "Wêdka kl. extra",          30000,      2.50},
    {FISH_RODE_ELITA,       "Wêdka kl. elitarnej",      50000,      3.25}
};

enum e_fishBaitData {
    fish_bait_type,
    fish_bait_name[50],
    fish_bait_money,
    Float:fish_bait_chance
};

#define FISH_BAIT_CHLEB     0
#define FISH_BAIT_ROBAK     1
#define FISH_BAIT_CIASTO    2
#define FISH_BAIT_GLON      3
#define FISH_BAIT_MIESO     4

new const fishBaitData[][e_fishBaitData] = {
    {FISH_BAIT_CHLEB,       "Chleb",    5,      0.5},
    {FISH_BAIT_ROBAK,       "Robak",    10,     1.0},
    {FISH_BAIT_CIASTO,      "Ciasto",   20,     1.5},
    {FISH_BAIT_GLON,        "Glon",     25,     2.0},
    {FISH_BAIT_MIESO,       "Miêso",    30,     3.0}
};

stock ShowFishBaits(playerid)
{
    s[0] = EOS;
    s = ""WHITE"Typ przynêty\t"LY"Iloœæ\t"GREEN"Cena\n";
    for(new i = 0; i < sizeof(fishBaitData); i++) format(s, sizeof(s), "%s"WHITE"%s\t"LY"x%d\t"GREEN"%d$\n", s, fishBaitData[i][fish_bait_name], pFishBait[playerid][i], fishBaitData[i][fish_bait_money]);
    return showDialog(playerid, DIALOG_FISH_BAIT, DIALOG_STYLE_TABLIST_HEADERS, ""VIOLET"Przynêty rybackie", s, "Zakup", "Cofnij"), 1;
}

stock ShowFishRodes(playerid)
{
    s[0] = EOS;
    s = ""WHITE"Typ wêdki\t"LY"Cena\n";

    catstr(s, C_GREEN, -1, "Stan ¿y³ki: \t"LGREEN"%.1f\n", pFishZylka[playerid]);
    strcat(s, " \n");
    for(new i = 0; i < sizeof(fishRodeData); i++) catstr(s, C_LY, -1, ""LY"%s\t"ORANGE"%s$\n", fishRodeData[i][fish_rode_name], strdot(fishRodeData[i][fish_rode_money]));
    return showDialog(playerid, DIALOG_FISH_RODE, DIALOG_STYLE_TABLIST_HEADERS, ""BROWN"Wêdki rybackie", s, "Zakup", "Cofnij"), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_FISH_SHOP: {
            if(!response) return callcmd::rybki(playerid);

            switch(listitem) {
                case 0: ShowFishRodes(playerid);
                case 1: ShowFishBaits(playerid);
                case 2: ShowFishUlepszenia(playerid);
            }
            return 1;
        }

        case DIALOG_FISH_BAIT: {
            if(!response) return OnDialogResponse(playerid, DIALOG_FISH_PANEL, 1, 6, "");
           
            new amount = ((pFishUlepszenia[playerid][FISH_BAITS]) ? (50) : (10));
            if(pFishBait[playerid][listitem] >= amount) return sendError(playerid, ""RED"Posiadasz ju¿ maksymaln¹ iloœæ przynêty: {c}%s{/c}.", fishBaitData[listitem][fish_bait_name]);

            playerData[playerid][player_saveData][0] = listitem;
            playerData[playerid][player_saveData][1] = amount - pFishBait[playerid][listitem];

            s[0] = EOS;
            catstr(s, C_LGREEN, -1, "Wybrany typ przynêty: {c}%s{/c}\n"LGREEN"Koszt za sztukê: {c}%d${/c}\n\n"LY"Mo¿esz zakupiæ jeszcze {c}%d szt.{/c} tej przynêty!\n"LY"Wpisz poni¿ej iloœæ, któr¹ chcesz zakupiæ:", fishBaitData[listitem][fish_bait_name], fishBaitData[listitem][fish_bait_money], playerData[playerid][player_saveData][1]);
            return showDialog(playerid, DIALOG_FISH_BAIT_AMOUNT, DIALOG_STYLE_INPUT, ""BLUE2"Zakup przynêty", s, "Dalej", "Cofnij"), 1;
        }

        case DIALOG_FISH_BAIT_AMOUNT: {
            if(!response) return ShowFishBaits(playerid);

            new amount, baitid = playerData[playerid][player_saveData][0], result = playerData[playerid][player_saveData][1];
            if(sscanf(inputtext, "d", amount) || (amount < 1 || amount > result)) {
                s[0] = EOS;
                catstr(s, C_LGREEN, -1, "Wybrany typ przynêty: {c}%s{/c}\n"LGREEN"Koszt za sztukê: {c}%d${/c}\n\n"LY"Mo¿esz zakupiæ jeszcze {c}%d szt.{/c} tej przynêty!\n"LY"Wpisz poni¿ej iloœæ, któr¹ chcesz zakupiæ:", fishBaitData[baitid][fish_bait_name], fishBaitData[baitid][fish_bait_money], result);
                return showDialog(playerid, DIALOG_FISH_BAIT_AMOUNT, DIALOG_STYLE_INPUT, ""BLUE2"Zakup przynêty", s, "Dalej", "Cofnij");
            }

            new money = fishBaitData[baitid][fish_bait_money] * amount;
            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ll, ii
                if(!rr) return 1;
                
                if(getMoney(playerid) < money) return valueError(playerid, money);
                giveMoney(playerid, -money);
                addLevelScore(playerid, math_random(10, 30));

                pFishBait[playerid][baitid] += amount;
                m_pquery_format("UPDATE es_accounts SET fish_bait='%s' WHERE id=%d LIMIT 1;", GetFishBaits(playerid), playerData[playerid][player_accountID]);
                return msg(playerid, C_MINT, "Zakupi³eœ(aœ) {c}%d szt.{/c} przynêty {c}%s{/c} za: {c}%s${/c}.", amount, fishBaitData[baitid][fish_bait_name], strdot(money));
            }

            s[0] = EOS;
            format(s, sizeof(s), ""WHITE"Podsumowanie zakupu przynêty.\n\n"WHITE"Typ przynêty: "PINK"%s\n"WHITE"Iloœæ przynêty: "RED"x%d\n"WHITE"Cena transakcji: "LY"%s$\n\n"BLUE"Czy napewno chcesz zakupiæ przynêty?\n"BLUE"Ta operacja nie bêdzie odwracalna.", fishBaitData[baitid][fish_bait_name], amount, strdot(money));
            return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""PINK"Podsumowanie zakupu przynêty", s, "Zakup", "Anuluj"), 1;
        }

        case DIALOG_FISH_RODE: {
            if(!response) return OnDialogResponse(playerid, DIALOG_FISH_PANEL, 1, 6, "");
            switch(listitem) {
                case 0: {
                    if(pFishZylka[playerid] >= 70.0) return sendError(playerid, ""RED"Twoja ¿y³ka posiada obecnie wystarczaj¹c¹ wytrzyma³oœæ!");

                    new  money = 0;
                    if(pFishZylka[playerid] < 70.0)         money = 5000;
                    else if(pFishZylka[playerid] < 50.0)    money = 10000;
                    else if(pFishZylka[playerid] < 30.0)    money = 15000;
                    else if(pFishZylka[playerid] < 15.0)    money = 30000;
                    else                                    money = 50000;

                    if(getMoney(playerid) < money) return valueError(playerid, money);

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ll, ii
                        
                        if(!rr) return ShowFishRodes(playerid);
                        if(getMoney(playerid) < money) return valueError(playerid, money);

                        pFishZylka[playerid] = (pFishUlepszenia[playerid][FISH_ZYLKA]) ? (300.0) : (100.0);
                        giveMoney(playerid, -money);

                        m_pquery_format("UPDATE es_accounts SET fish_zylka=%.1f WHERE id=%d LIMIT 1;", pFishZylka[playerid], playerData[playerid][player_accountID]);
                        msg(playerid, C_LIME, "Uzupe³ni³eœ(aœ) {c}wytrzyma³oœæ{/c} ¿y³ki! Cena: {c}%s${/c}.", strdot(money));
                        return ShowFishRodes(playerid), 1;
                    }

                    s[0] = EOS;
                    catstr(s, C_LIME, -1, "Czy chcesz uzupe³niæ {c}wytrzyma³oœæ{/c} swojej ¿y³ki?\n");
                    catstr(s, C_LIME, -1, "Koszt uzupe³nienia bêdzie wynosiæ {c}%s${/c}.\n\n", strdot(money));
                    catstr(s, C_LY, -1, "Kliknij poni¿ej odpowiedni przycisk, by uzupe³niæ wytrzyma³oœæ ¿y³ki!");
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BLUE"Wytrzyma³oœæ ¿y³ki", s, "Uzupe³nij", "Cofnij");
                }
                case 1: ShowFishRodes(playerid);

                default: {
                    new rodeid = listitem - 2;
                    if(getMoney(playerid) < fishRodeData[rodeid][fish_rode_money]) return valueError(playerid, fishRodeData[rodeid][fish_rode_money]);
                    if(pFishRode[playerid] >= rodeid) return sendError(playerid, ""RED"Posiadasz ju¿ zakupion¹ {c}tak¹ sam¹{/c} b¹dŸ {c}lepsz¹{/c} wêdkê.");
                    giveMoney(playerid, -fishRodeData[rodeid][fish_rode_money]);

                    pFishRode[playerid] = rodeid;
                    m_pquery_format("UPDATE es_accounts SET fish_rode=%d WHERE id=%d LIMIT 1;", rodeid, playerData[playerid][player_accountID]);

                    s[0] = EOS;
                    catstr(s, C_BLUE2, -1, "Pomyœlnie zakupi³eœ(aœ) wêdkê: {c}%s{/c}.\n", fishRodeData[rodeid][fish_rode_name]);
                    catstr(s, C_BLUE2, -1, "Koszt zakupu wynosi: {c}%s$.", strdot(fishRodeData[rodeid][fish_rode_money]));
                    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Zakup wêdki", s, "OK", #);
                }
            }
            return 1;
        }
    }
    return 0;
}
