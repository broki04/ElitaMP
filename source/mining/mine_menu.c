#include YSI_Coding\y_hooks

stock GetBagLevel(playerid)
{
    new name[30];
    switch(MineBagLevel[playerid]) {
        case 1:     name = "I{AFAFAF}IIII";
        case 2:     name = "II{AFAFAF}III";
        case 3:     name = "III{AFAFAF}II";
        case 4:     name = "IIII{AFAFAF}I";
        default:    name = "IIIII{AFAFAF}";
    }
    return name;
}

alias:gornictwo("zyly", "ore", "mining", "gornik")
CMD:gornictwo(playerid)
{
    s[0] = EOS;
    catstr(s, C_BROWN, -1, "Aktualnie {c}wydarzenie g�rnicze{/c} jest %s!\n", (oreEvent) ? ("w��czone") : ("wy��czone"));
    catstr(s, C_BROWN, -1, "Dzisiejsza �y�a dnia: {c}%s{/c}\n", mineData[OreDay][mine_name]);
    strcat(s, " \n");
    catstr(s, C_YELLOW, -1, ""ARROW"  "WHITE"Informacje o g�rnictwie\n");
    catstr(s, C_YELLOW, -1, ""ARROW"  "WHITE"Dost�pne z�o�a surowc�w\n");
    catstr(s, C_YELLOW, -1, ""ARROW"  "WHITE"%s\n", (!MinePickaxe[playerid]) ? ("Zakup kilof g�rniczy "VIOLET"(300 pkt. respektu + 10.000$)") : ("Zarz�dzaj kilofem g�rniczym"));
    catstr(s, C_YELLOW, -1, ""ARROW"  "WHITE"%s\n", (!MineBag[playerid]) ? ("Zakup plecak g�rniczy "PINK"(1.000 pkt. respektu + 10.000$)") : ("Zarz�dzaj plecakiem g�rniczym"));
    catstr(s, C_YELLOW, -1, ""ARROW"  "WHITE"Teleportuj si� na: "ORANGE"/Kopalnia");
    return showDialog(playerid, DIALOG_GORNICTWO, DIALOG_STYLE_LIST, ""CORAL"G�rnictwo", s, "Wybierz", "Zamknij"), 1;
}

alias:oreevent("kopevent", "gornikevent", "lodeevent", "mineevent")
CMD:oreevent(playerid)
{
    if(!oreEvent) return sendError(playerid, ""RED"Obecnie {c}event g�rniczy{/c} jest wy��czony!");

    s[0] = EOS;
    catstr(s, C_GREEN, C_LY, "Obecnie trwa {c}wydarzenie g�rnicze{/c} na naszym serwerze {c}aktywowane{/c} przez administracj�!\n");
    catstr(s, C_GREEN, C_LY, "Podczas aktywnego {c}wydarzenia{/c} otrzymujesz {c}wi�ksze szanse{/c} na zdobycie punkt�w g�rniczych!\n");
    catstr(s, C_GREEN, C_LY, "Jak mo�na si� domy�li�, otrzymujesz r�wnie� wi�ksz� nagrod� o {c}50 procent{/c} za sprzeda� surowc�w!");
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Event g�rniczy", s, "OK", #), 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {        
        case DIALOG_GORNICTWO_PLECAK: {
            if(!response) return callcmd::gornictwo(playerid);

            switch(listitem) {
                case 0, 1: OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 6, "");

                case 2: {
                    if(!MinePoints[playerid]) return sendError(playerid, ""RED"Nie posiadasz �adnych {c}pkt. g�rnika{/c}, by m�c ulepszy� sw�j plecak!");
                    if(MineBagLevel[playerid] >= 5) return sendError(playerid, ""RED"Tw�j {c}plecak g�rniczy{/c} jest ju� maksymalnie ulepszony!");

                    MinePoints[playerid] --;
                    MineBagLevel[playerid] ++;

                    m_pquery_format("UPDATE es_accounts SET mine_bag=%d, mine_points=%d WHERE id=%d LIMIT 1;", MineBagLevel[playerid], MinePoints[playerid], playerData[playerid][player_accountID]);
                    OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 6, "");
                }

                case 3: {
                    s[0] = EOS;
                    s = ""WHITE"Surowiec\t"BLUE"Posiadana waga\n";
                    for(new i = 0; i < sizeof(mineData); i++) format(s, sizeof(s), "%s"WHITE"%s\t"BLUE"%d kg\n", s, mineData[i][mine_name], MineWeight[playerid][i]);

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 6, ""); }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Zawarto�� plecaka g�rniczego", s, "OK", #);
                }
            }
            return 1;
        }

        case DIALOG_PICKAXE_NAME: {
            if(!response) return OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, "");

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!IsValidGangBuffer(dest, 3, 20)) return showDialog(playerid, DIALOG_PICKAXE_NAME, DIALOG_STYLE_INPUT, ""BROWN"Nazwa kilofa", ""WHITE"Wpisz poni�ej nazw� dla swojego kilofu:\n\n"RED"Dozwolony limit znak�w w nazwie kilofa to "WHITE"3-20 "RED"znaki.", "Ustaw", "Cofnij");
            
            format(MinePickaxeName[playerid], 50, "%s", dest);
            m_pquery_format("UPDATE es_accounts SET pickaxe_name='%s' WHERE id=%d LIMIT 1;", dest, playerData[playerid][player_accountID]);
            return OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, ""), 1;
        }

        case DIALOG_PICKAXE_MANAGE: {
            if(!response) return callcmd::gornictwo(playerid);

            switch(listitem) {
                case 1: OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, ""); 
                case 0: {
                    if(MinePickaxeHealth[playerid][PICKAXE_HEALTH_STATUS] >= 70.0) return dialogBox(playerid, C_RED, ""RED"Nie musisz naprawia� swojego kilofa!");

                    new money = 0, Float:health = MinePickaxeHealth[playerid][PICKAXE_HEALTH_STATUS];
                    if(health < 70.0)       money = 5000;
                    else if(health < 50.0)  money = 10000;
                    else if(health < 30.0)  money = 15000;
                    else if(health < 15.0)  money = 30000;
                    else                    money = 50000;

                    if(getMoney(playerid) < money) return valueError(playerid, money);
                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ll, ii

                        if(!rr) return OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, "");
                        if(getMoney(playerid) < money) return valueError(playerid, money);

                        MinePickaxeHealth[playerid][PICKAXE_HEALTH_STATUS] = MinePickaxeHealth[playerid][PICKAXE_HEALTH_LIMIT];
                        giveMoney(playerid, -money);

                        m_pquery_format("UPDATE es_accounts SET pickaxe_health='%.2f, %.2f' WHERE id=%d LIMIT 1;", MinePickaxeHealth[playerid][0], MinePickaxeHealth[playerid][1], playerData[playerid][player_accountID]);
                        msg(playerid, C_LIME, "Naprawi�e�(a�) sw�j kilof! Cena: {c}%s${/c}.", strdot(money));
                        return OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, ""), 1;
                    }

                    s[0] = EOS;
                    catstr(s, C_LIME, -1, "Czy napewno chcesz {c}naprawi�{/c} sw�j kilof?\n");
                    catstr(s, C_LIME, -1, "Koszt naprawy b�dzie wynosi� {c}%s${/c}.\n\n", strdot(money));
                    catstr(s, C_LY, -1, "Kliknij poni�ej odpowiedni przycisk, by naprawi� sw�j kilof!");
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BLUE"Naprawa kilofu", s, "Napraw", "Cofnij");
                }
                case 2: showDialog(playerid, DIALOG_PICKAXE_NAME, DIALOG_STYLE_INPUT, ""BROWN"Nazwa kilofa", ""WHITE"Wpisz poni�ej nazw� dla swojego kilofu:", "Ustaw", "Cofnij");

                case 3: {
                    if(!MinePoints[playerid]) return dialogBox(playerid, C_RED, ""RED"Nie posiadasz �adnych {c}punkt�w g�rniczych{/c}, by m�c ulepszy� wytrzyma�o�� kilofa.");
                    if(MinePickaxeHealth[playerid][PICKAXE_HEALTH_LIMIT] >= 500.0) return dialogBox(playerid, C_RED, ""RED"Wytrzyma�o�� {c}kilofa g�rniczego{/c} jest ju� maksymalnie ulepszona!");

                    MinePoints[playerid] --;
                    MinePickaxeHealth[playerid][PICKAXE_HEALTH_LIMIT] += 100.0;

                    m_pquery_format("UPDATE es_accounts SET pickaxe_health='%.2f, %.2f' WHERE id=%d LIMIT 1;", MinePickaxeHealth[playerid][0], MinePickaxeHealth[playerid][1], playerData[playerid][player_accountID]);
                    return OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, ""), 1;
                }

                case 4: {
                    if(!MinePoints[playerid]) return dialogBox(playerid, C_RED, ""RED"Nie posiadasz �adnych {c}punkt�w g�rniczych{/c}, by m�c ulepszy� wytrzyma�o�� kilofa.");
                    if(MinePickaxeLevel[playerid] >= 5) return dialogBox(playerid, C_RED, ""RED"Poziom {c}kilofa g�rniczego{/c} jest ju� maksymalny!");

                    MinePoints[playerid] --;
                    MinePickaxeLevel[playerid] ++;

                    m_pquery_format("UPDATE es_accounts SET pickaxe=%d WHERE id=%d LIMIT 1;", MinePickaxeLevel[playerid], playerData[playerid][player_accountID]);
                    return OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, ""), 1;
                }
            }
            return 1;
        }

        case DIALOG_GORNICTWO: {
            if(!response) return 1;

            switch(listitem) {
                case 0..2: callcmd::gornictwo(playerid);

                case 3: {
                    s[0] = EOS;
                    catstr(s, C_WHITE, -1, "System g�rnictwa zosta� opracowany w taki spos�b, by by� jednym z najlepszych metod zarobk�w.\n");
                    catstr(s, C_WHITE, -1, "Posiadamy wiele rodzaj�w surowc�w oraz zaimplementowany system sprzeda�y, co� w rodzaju spedycji.\n\n");

                    catstr(s, C_FAJNY, -1, "Wydobycie surowc�w:\n");
                    catstr(s, C_WHITE, -1, "Same zmiany zasz�y r�wnie� w wydobyciu surowc�w. Trzeba zacz�� od tego, �e dane z�o�e mo�e wydobywa� "RED"okre�lona "WHITE"liczba os�b.\n");
                    catstr(s, C_WHITE, -1, "R�wnie� wprowadzone zosta�y "GREEN"poziom wytrzyma�o�ci "WHITE"surowc�w - �eby takowy poziom spad�, trzeba spe�ni� kilka czynnik�w.\n");
                    catstr(s, C_WHITE, -1, "Po wydobyciu, otrzymujesz "LY"kilka kilogram�w "WHITE"danego surowca, kt�re potem mo�esz "BLUE2"sprzeda� "WHITE"na /kopalnia.\n\n");

                    catstr(s, C_FAJNY, -1, "Sprzeda� surowc�w:\n");
                    catstr(s, C_WHITE, -1, "Nowo�ci� na naszym serwerze jest "PINK"opracowana sprzeda� surowc�w"WHITE", niczym na serwerach RPG w MTA!\n");
                    catstr(s, C_WHITE, -1, "Jedynym warunkiem, by m�c rozpocz�� sprzeda�, musisz "RED"posiada� "WHITE"przynajmniej "MINT"50kg"WHITE" w plecaku g�rniczym.\n");
                    catstr(s, C_WHITE, -1, "Twoim zadaniem b�dzie dostarczenie za�adunku do punktu odbioru oznaczonego na mapie.\n\n");

                    catstr(s, C_FAJNY, -1, "Panel g�rniczy:\n");
                    catstr(s, C_WHITE, -1, "Na naszym serwerze r�wnie� w "YELLOW"panelu g�rniczym "WHITE"(/gornictwo) b�dziesz m�g� zarz�dza�, np. swoim plecakiem, czy te� kilofem.\n");
                    catstr(s, C_WHITE, -1, "Przyk�adowo, mo�esz zwi�kszy� wytrzyma�o�� kilofu, czy te� pojemno�� swojego plecaka g�rniczego, przez co b�dziesz m�g� wi�cej sprzeda�.\n");
                    catstr(s, C_WHITE, -1, "R�wnie� ulepszaj�c poziom kilofu, b�dziesz m�g� wydobywa� wi�cej kilogram�w danego surowca, co zwi�kszy doch�d podczas sprzeda�y!");

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return callcmd::gornictwo(playerid); }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BROWN"Informacje o g�rnictwie", s, "OK", #);
                }

                case 4: {
                    s[0] = EOS;
                    s = ""WHITE"Nazwa surowca\t"LY"Nagroda za kilogram\n";
                    for(new i = 0; i < sizeof(mineData); i++) format(s, sizeof(s), "%s"WHITE"%s\t"LY"%s$\n", s, mineData[i][mine_name], strdot(mineData[i][mine_money]));
                    
                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ll, rr, ii
                        return callcmd::gornictwo(playerid), 1; }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""BROWN"Dost�pne z�o�a surowc�w", s, "OK", #);
                }

                case 5: {
                    if(!MinePickaxe[playerid]) {
                        if(getMoney(playerid) < 10000) return valueError(playerid, 10000);
                        giveMoney(playerid, -10000);

                        MinePickaxe[playerid] = true;
                        MinePickaxeHealth[playerid] = Float:{100.0, 100.0};
                        MinePickaxeLevel[playerid] = 1;
                        format(MinePickaxeName[playerid], 50, "Kilof g�rniczy");

                        m_pquery_format("UPDATE es_accounts SET pickaxe_health='%.2f, %.2f', pickaxe=%d WHERE id=%d LIMIT 1;", MinePickaxeHealth[playerid][0], MinePickaxeHealth[playerid][1], MinePickaxeLevel[playerid], playerData[playerid][player_accountID]);
                        msg(playerid, C_LIME, "Zakupi�e�(a�) {c}kilof g�rniczy{/c} za: {c}10.000${/c}.");
                        OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 5, "");
                    } else {
                        s[0] = EOS;
                        s = ""BROWN"Polecenie\t"WHITE"Rezultat\n";

                        catstr(s, C_WHITE, -1, "Wytrzyma�o�� kilofa:\t"GREEN"%.2f/%.2f HP\n", MinePickaxeHealth[playerid][0], MinePickaxeHealth[playerid][1]);
                        strcat(s, " \n");
                        catstr(s, C_BROWN, -1, "Nazwa kilofu:\t"DRED"%s\n", MinePickaxeName[playerid]);
                        catstr(s, C_BROWN, -1, "Ulepsz wytrzyma�o��:\t"YELLOW"%.2f HP\n", MinePickaxeHealth[playerid][PICKAXE_HEALTH_LIMIT]);
                        catstr(s, C_BROWN, -1, "Zwi�ksz poziom:\t"LGREEN"[%s"LGREEN"]\n", GetPickaxeLevel(playerid));
                        showDialog(playerid, DIALOG_PICKAXE_MANAGE, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Zarz�dzanie kilofem g�rniczym", s, "Wybierz", "Cofnij");
                    }
                }

                case 6: {
                    if(!MineBag[playerid]) {
                        if(getMoney(playerid) < 10000) return valueError(playerid, 10000);
                        giveMoney(playerid, -10000);

                        MineBag[playerid] = true;
                        MineBagLevel[playerid] = 1;

                        m_pquery_format("UPDATE es_accounts SET mine_bag=%d WHERE id=%d LIMIT 1;", MineBagLevel[playerid], playerData[playerid][player_accountID]);
                        OnDialogResponse(playerid, DIALOG_GORNICTWO, 1, 6, "");
                    } else {
                        s[0] = EOS;
                        catstr(s, C_YELLOW, -1, "Posiadasz aktualnie {c}%d pkt.{/c} g�rnika!\n", MinePoints[playerid]);
                        strcat(s, " \n");
                        catstr(s, C_BLUE, -1, "Ulepsz plecak: "LY"%s\n", GetBagLevel(playerid));
                        catstr(s, C_BLUE, -1, "Zawarto�� plecaka "ORANGE"(%d/%d kg)", GetBagWeight(playerid), GetBagAmount(playerid));
                        showDialog(playerid, DIALOG_GORNICTWO_PLECAK, DIALOG_STYLE_LIST, ""YELLOW"Plecak g�rniczy", s, "Wybierz", "Cofnij");
                    }
                }
                case 7: EnterTeleport(playerid, "kopalnia");
            }
            return 1;
        }
    }
    return 0;
}