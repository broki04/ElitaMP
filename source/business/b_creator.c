#include YSI_Coding\y_hooks

new 
    pBusinessCreatorName[MAX_PLAYERS][50],
    pBusinessCreatorCost[MAX_PLAYERS],
    pBusinessCreatorType[MAX_PLAYERS],
    Float:pBusinessCreatorPosition[MAX_PLAYERS][3]
;

hook function ResetPlayerData(playerid) {
    pBusinessCreatorType[playerid] = -1;
    pBusinessCreatorCost[playerid] = 0;
    pBusinessCreatorName[playerid] = "";
    pBusinessCreatorPosition[playerid] = Float:{0.0, 0.0, 0.0};
    return continue(playerid);
}

CMD:bcreator(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    if(Iter_Free(Business) == -1) return dialogBox(playerid, C_RED, "Limit biznesów wyczerpany.");

    s[0] = EOS;
    s = ""LORANGE"Kategoria\t"WHITE"Rezultat\n";

    catstr(s, C_LORANGE, -1, "Nazwa biznesu:\t"WHITE"%s\n", (strlen(pBusinessCreatorName[playerid]) < 2) ? ("---") : (form("%s", pBusinessCreatorName[playerid])));
    catstr(s, C_LORANGE, -1, "Typ biznesu:\t"WHITE"%s\n", GetBusinessType(pBusinessCreatorType[playerid]));
    catstr(s, C_LORANGE, -1, "Koszt zakupu:\t"WHITE"%s$\n", strdot(pBusinessCreatorCost[playerid]));
    catstr(s, C_LORANGE, -1, "Pozycja biznesu:\t"WHITE"%s\n", (pBusinessCreatorPosition[playerid][0] == 0.0) ? (""RED"-") : (""GREEN"+"));
    strcat(s, " \n");
    catstr(s, C_LRED, -1, "Stwórz biznes");
    return showDialog(playerid, DIALOG_BUSINESS_CREATOR, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Kreator biznesu", s, "Wybierz", "Zamknij"), 1;
}
flags:bcreator(HEAD)

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_BUSINESS_CREATOR_NAME: {
            if(!response) return callcmd::bcreator(playerid);

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(strlen(dest) < 5 || strlen(dest) > 20) return showDialog(playerid, DIALOG_BUSINESS_CREATOR_NAME, DIALOG_STYLE_INPUT, ""BLUE2"Kreator biznesu - nazwa", ""WHITE"Wpisz poni¿ej nazwê dla biznesu:\n\n"RED"Dozwolony limit znaków to: "WHITE"5-20 "RED"znaków.", "Ustaw", "Cofnij");
            
            format(pBusinessCreatorName[playerid], 50, "%s", dest);
            return callcmd::bcreator(playerid), 1;
        }

        case DIALOG_BUSINESS_CREATOR_MONEY: {
            if(!response) return callcmd::bcreator(playerid);

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 1000000 || amount > 10000000) return showDialog(playerid, DIALOG_BUSINESS_CREATOR_MONEY, DIALOG_STYLE_INPUT, ""PINK"Kreator biznesu - koszt gotówka", ""WHITE"Wpisz poni¿ej, ile biznes ma kosztowaæ "RED"w gotówce"WHITE":\n\n"RED"Nieprawid³owa iloœæ gotówki! "LY"(1.000.000$ - 10.000.000$)", "Ustaw", "Cofnij");
            
            pBusinessCreatorCost[playerid] = amount;
            return callcmd::bcreator(playerid), 1;
        }

        case DIALOG_BUSINESS_CREATOR: {
            if(!response) return 1;

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_BUSINESS_CREATOR_NAME, DIALOG_STYLE_INPUT, ""BLUE2"Kreator biznesu - nazwa", ""WHITE"Wpisz poni¿ej nazwê dla biznesu:", "Ustaw", "Cofnij");
                
                case 1: {
                    s[0] = EOS;
                    for(new i = 0; i < MAX_BUSINESS_TYPES; i++) format(s, sizeof(s), "%s"LORANGE"  "ARROW" "WHITE"%s\n", s, GetBusinessType(i));

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ii
                        if(!rr) return callcmd::bcreator(playerid);
                        pBusinessCreatorType[playerid] = ll;
                        return callcmd::bcreator(playerid), 1;
                    }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_LIST, ""VIOLET"Kreator biznesu - typ", s, "Ustaw", "Cofnij");
                }

                case 2: showDialog(playerid, DIALOG_BUSINESS_CREATOR_MONEY, DIALOG_STYLE_INPUT, ""PINK"Kreator biznesu - koszt gotówka", ""WHITE"Wpisz poni¿ej, ile biznes ma kosztowaæ "RED"w gotówce"WHITE":", "Ustaw", "Cofnij");
                
                case 3: {
                    new Float:x, Float:y, Float:z;
                    GetPlayerPos(playerid, x, y, z);

                    pBusinessCreatorPosition[playerid][0] = x;
                    pBusinessCreatorPosition[playerid][1] = y;
                    pBusinessCreatorPosition[playerid][2] = z;

                    callcmd::bcreator(playerid);
                }

                case 5: {
                    if(strlen(pBusinessCreatorName[playerid]) < 2) return dialogBox(playerid, C_RED, "Nie ustalono {c}nazwy{/c} dla tworzonego biznesu.");
                    if(pBusinessCreatorCost[playerid] < 1) return dialogBox(playerid, C_RED, "Nie ustalono {c}kosztu{/c} zakupu tworzonego biznesu.");
                    if(pBusinessCreatorType[playerid] == -1) return dialogBox(playerid, C_RED, "Nie ustalono {c}typu{/c} tworzonego biznesu.");
                
                    inline onCreateBusiness() {
                        if(Iter_Free(Business) == -1) return 1;

                        new businessid = cache_insert_id();
                        CreateBusiness(businessid, 0, true);
                        
                        msg(playerid, C_LGREEN, "Stworzono biznes! "LY"(%d, %s - %s)", businessid, GetBusinessType(pBusinessCreatorType[playerid]), pBusinessCreatorName[playerid]);

                        pBusinessCreatorType[playerid] = -1;
                        pBusinessCreatorCost[playerid] = 0;
                        pBusinessCreatorName[playerid] = "";
                        pBusinessCreatorPosition[playerid] = Float:{0.0, 0.0, 0.0};
                        return 1;
                    }

                    dbstr[0] = EOS;
                    format(dbstr, sizeof(dbstr), "INSERT INTO es_business (name, ownerid, cost, position, type) VALUES ('%s', -1, %d, '%f, %f, %f', %d);", pBusinessCreatorName[playerid], pBusinessCreatorCost[playerid], unpackXYZ(pBusinessCreatorPosition[playerid]), pBusinessCreatorType[playerid]);
                    m_tquery_inline(dbconn, using inline onCreateBusiness, dbstr);
                }

                default: callcmd::bcreator(playerid);
            }
            return 1;
        }
    }
    return 0;
}