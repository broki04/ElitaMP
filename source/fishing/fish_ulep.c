#include YSI_Coding\y_hooks

enum e_fishUlepszeniaData {
    fish_ulep_id,
    fish_ulep_name[90],
    fish_ulep_money
};

#define FISH_BAITS              0
#define FISH_REWARD             1
#define FISH_ZYLKA              2
#define FISH_LEGENDARY          3

new const fishUlepszeniaData[][e_fishUlepszeniaData] = {
    {FISH_BAITS,        "Wiêcej miejsca na przynêty",                       75000},
    {FISH_REWARD,       "Wiêksze wynagrodzenie za wy³owienie",              100000},
    {FISH_ZYLKA,        "Bardziej wytrzyma³a ¿y³ka",                        30000},
    {FISH_LEGENDARY,    "Wiêksza szansa na z³owienie legendarnej ryby",     500000}
};
new bool:pFishUlepszenia[MAX_PLAYERS][sizeof(fishUlepszeniaData)];

hook function ResetPlayerData(playerid)
{
    for(new i = 0; i < sizeof(fishUlepszeniaData); i++) pFishUlepszenia[playerid][i] = false;
    return continue(playerid);
}

stock GetFishUlepszenia(playerid)
{
    new fstr[128];
    for(new i = 0; i < sizeof(fishUlepszeniaData); i++) format(fstr, sizeof(fstr), "%s%s%i", fstr, i == 0 ? ("") : (","), pFishUlepszenia[playerid][i]);
    return fstr;
}

stock ShowFishUlepszenia(playerid)
{
    s[0] = EOS;
    s = ""WHITE"Nazwa\t"YELLOW"Koszt\t"GREEN"Sta"RED"tus\n";
    for(new i = 0; i < sizeof(fishUlepszeniaData); i++) format(s, sizeof(s), "%s"WHITE"%s\t"YELLOW"%s$\t%s\n", s, fishUlepszeniaData[i][fish_ulep_name], strdot(fishUlepszeniaData[i][fish_ulep_money]), (pFishUlepszenia[playerid][i]) ? (""GREEN"ODBLOKOWANE") : (""RED"ZABLOKOWANE"));
    return showDialog(playerid, DIALOG_FISH_ULEPSZENIA, DIALOG_STYLE_TABLIST_HEADERS, ""PINK"Ulepszenia rybackie", s, "Zakup", "Zamknij"), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_FISH_ULEPSZENIA: {
            if(!response) return OnDialogResponse(playerid, DIALOG_FISH_PANEL, 1, 6, ""), 1;
            if(getMoney(playerid) < fishUlepszeniaData[listitem][fish_ulep_money]) return valueError(playerid, fishUlepszeniaData[listitem][fish_ulep_money]);
            if(pFishUlepszenia[playerid][listitem]) return sendError(playerid, ""RED"Posiadasz ju¿ zakupione ulepszenie: {c}%s{/c}.", fishUlepszeniaData[listitem][fish_ulep_name]);

            s[0] = EOS;
            catstr(s, C_VIOLET, -1, "Czy napewno chcesz zakupiæ ulepszenie rybackie: {c}%s{/c}?\n", fishUlepszeniaData[listitem][fish_ulep_name]);
            catstr(s, C_VIOLET, -1, "Koszt zakupu wyniesie: {c}%s${/c}.\n\n", strdot(fishUlepszeniaData[listitem][fish_ulep_money]));
            catstr(s, C_LY, -1, "Pamiêtaj, ¿e zwrot gotówki po zakupie {c}nie bêdzie{/c} mo¿liwy!\n");
            catstr(s, C_LY, -1, "Dzia³anie dodatku dzia³a {c}odrazu{/c} po zakupie ulepszenia!");
            
            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ll, ii

                if(!rr) return 1;
                if(getMoney(playerid) < fishUlepszeniaData[listitem][fish_ulep_money]) return valueError(playerid, fishUlepszeniaData[listitem][fish_ulep_money]);
                if(pFishUlepszenia[playerid][listitem]) return sendError(playerid, ""RED"Posiadasz ju¿ zakupione ulepszenie: {c}%s{/c}.", fishUlepszeniaData[listitem][fish_ulep_name]);
                giveMoney(playerid, -fishUlepszeniaData[listitem][fish_ulep_money]);
                
                pFishUlepszenia[playerid][listitem] = true;
                m_pquery_format("UPDATE es_accounts SET fish_ulep='%s' WHERE id=%d LIMIT 1;", GetFishUlepszenia(playerid), playerData[playerid][player_accountID]);
                msg(playerid, C_LIME, "Zakupi³eœ(aœ) ulepszenie rybackie {c}%s{/c} za: {c}%s${/c}.", fishUlepszeniaData[listitem][fish_ulep_name], strdot(fishUlepszeniaData[listitem][fish_ulep_money]));
                return ShowFishUlepszenia(playerid), 1; 
            }
            new title[128]; format(title, sizeof(title), ""LIME"%s", fishUlepszeniaData[listitem][fish_ulep_name]);
            return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, title, s, "Zakup", "Cofnij"), 1;
        }
    }
    return 0;
}