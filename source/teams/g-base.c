#include YSI_Coding\y_hooks

#define BASE_OBJECT_COST    3000

enum e_gangBasesData {
    Float:base_position[4],
    Float:base_spawn[4],
    Float:base_pickup[3]
};
//insert into es_gangs_bases (name, ownerid, cost, settings, sell, sell_time) values ('Baza gangowa', -1, 0, '0, 0', 0, 0);
new 
    gangBasesData[][e_gangBasesData] = {
        // strefa bazy gangowej                             // pozycja spawnowania siê                  // pozycja pickupa
        {{2554.4302, -2148.7678, 2613.6589, -2246.5984},    {2589.8103, -2220.7412, 1.6604, 6.6635},    {2574.7883, -2145.8264, -0.1957}},
        {{-246.5379, 1897.4773, -375.4801, 1727.5659},      {-291.6657, 1846.4583, 42.2891, 110.7158},  {-375.2053, 1744.2432, 43.0036}},
        {{-2060.7129, 1500.8307, -2097.3232, 1352.3632},    {0.0, 0.0, 0.0, 0.0},                       {-2093.66, 1348.55, 7.1456}},
        {{360.2604, -2763.5337, 472.4255, -2653.7957},      {0.0, 0.0, 0.0, 0.0},                       {366.0947, -2768.0393, 5.3608}}
    },

    Iterator:GangBases<MAX_GANGBASES>,
    GangBaseOwner[MAX_GANGBASES],
    GangBaseZone[MAX_GANGBASES],
    GangBaseName[MAX_GANGBASES][50],
    GangBasePickup[MAX_GANGBASES],  
    GangBaseCost[MAX_GANGBASES],
    GangBaseDoor[MAX_GANGBASES],
    bool:GangBaseDoorStatus[MAX_GANGBASES],
    Float:GangBaseDoorZ[MAX_GANGBASES],
    bool:GangBaseSettings[MAX_GANGBASES][2],
    Text3D:GangBaseLabel[MAX_GANGBASES],
    bool:GangBaseSell[MAX_GANGBASES],
    GangBaseSellTime[MAX_GANGBASES],
    GangBaseRectangle[MAX_GANGBASES]
;

stock Init_GangBases()
{
    inline onFetchBases() {
        for(new i = 0; i < cache_num_rows(); i++) {
            if(Iter_Free(GangBases) == -1) break;

            cache_get_value_name_int(i, "ownerid", GangBaseOwner[i]);
            cache_get_value_name(i, "name", GangBaseName[i]);
            cache_get_value_name_int(i, "cost", GangBaseCost[i]);
            cache_get_value_name_bool(i, "sell", GangBaseSell[i]);
            cache_get_value_name_int(i, "sell_time", GangBaseSellTime[i]);

            new buffer[90];
            cache_get_value_name(i, "settings", buffer);
            sscanf(buffer, "p<,>a<b>[2]", GangBaseSettings[i]);

            GangBaseZone[i] = CreateZone(unpackXYZR(gangBasesData[i][base_position]));
            CreateZoneBorders(GangBaseZone[i]);

            if(IsValidDynamicPickup(GangBasePickup[i])) DestroyDynamicPickup(GangBasePickup[i]);
            if(IsValidDynamic3DTextLabel(GangBaseLabel[i])) DestroyDynamic3DTextLabel(GangBaseLabel[i]);

            GangBasePickup[i] = CreateDynamicPickup(1313, 1, unpackXYZ(gangBasesData[i][base_pickup]), .interiorid=0, .worldid=0);
            GangBaseRectangle[i] = CreateDynamicRectangle(unpackXYZR(gangBasesData[i][base_position]), .worldid=0, .interiorid=0);
            GangBaseLabel[i] = CreateDynamic3DTextLabel(" ", -1, unpackXYZ(gangBasesData[i][base_pickup]) + 0.25, 30.0, .worldid=0, .interiorid=0);
            
            UpdateDynamic3DTextLabelText(GangBaseLabel[i], -1, UpdateBaseLabel(i));
            Streamer_SetIntData(STREAMER_TYPE_AREA, GangBaseRectangle[i], E_STREAMER_EXTRA_ID, i + BASE_MAGIC);

            switch(i) {
                case 0: {
                    GangBaseDoor[i] = CreateDynamicObject(19912, 2583.242187, -2146.553466, 0.648383, 0.000000, 0.000000, 0.000000, -1, -1, -1, 300.00, 300.00); 
                    SetDynamicObjectMaterial(GangBaseDoor[i], 0, 19480, "signsurf", "sign", 0xFFFFFFFF);
                    SetDynamicObjectMaterial(GangBaseDoor[i], 1, 18996, "mattextures", "policeshieldgls", 0xFF000000);
                    GangBaseDoorZ[i] = 0.648383;
                }

                case 1: {
                    GangBaseDoor[i] = CreateDynamicObject(19912, -373.52542, 1731.88147, 44.48202,   0.00000, 0.00000, -91.01997);
                    GangBaseDoorZ[i] = 44.48202;
                }

                case 2: {
                    GangBaseDoor[i] = CreateDynamicObject(19912, -2064.14746, 1354.84326, 8.90890, 0.00000, 0.00000, 0.00000);
                    GangBaseDoorZ[i] = 8.90890;
                }

                case 3: {
                    GangBaseDoor[i] = CreateDynamicObject(19912, 377.88928, -2763.25928, 6.97407,   0.00000, 0.00000, -0.36000);
                    GangBaseDoorZ[i] = 6.97407;
                }
            }
            GangBaseDoorStatus[i] = true;

            dbstr[0] = EOS;
            format(dbstr, sizeof(dbstr), "SELECT * FROM es_gangs_basesobj WHERE baseid=%d LIMIT %d;", i, MAX_BASE_OBJECTS);
            m_pquery(dbstr, "FetchBaseObjects", "dd", GangBaseOwner[i], i);

            Iter_Add(GangBases, i);
        }
        return printf(" >>> [TEAM-BASE]: Wczytanych baz zespo³owych: %d", Iter_Count(GangBases)), 1;
    }
    return m_tquery_inline(dbconn, using inline onFetchBases, "SELECT * FROM es_gangs_bases ORDER BY id ASC"), 1;
}

stock UpdateBaseLabel(baseid)
{
    new bstr[256];
    if(GangBaseOwner[baseid] == -1 || GangBaseSell[baseid]) {
        format(bstr, sizeof(bstr), ""GOLD"Baza "WHITE"#%d\n"GOLD"Lokalizacja: "WHITE"%s, %s\n\n", baseid + 1, getZoneName(unpackXYZ(gangBasesData[baseid][base_pickup])), getCityName(unpackXYZ(gangBasesData[baseid][base_pickup])));
        format(bstr, sizeof(bstr), "%s"BLUE2"Baza jest dostêpna do kupna za: "LY""EURO"%s\n", bstr, strdot(GangBaseCost[baseid]));
        format(bstr, sizeof(bstr), "%s"CORAL"Aby zakupiæ bazê, kliknij przycisk: "WHITE"SPACJA + N", bstr);
    } else {
        format(bstr, sizeof(bstr), ""GOLD"Baza: "WHITE"\"%s\"\n"GOLD"Lokalizacja: "WHITE"%s, %s\n\n", GangBaseName[baseid], getZoneName(unpackXYZ(gangBasesData[baseid][base_pickup])), getCityName(unpackXYZ(gangBasesData[baseid][base_pickup])));
        format(bstr, sizeof(bstr), "%s"BLUE2"Nale¿y do zespo³u: {%06x}%s [%s]\n", bstr, getGangColor(GangBaseOwner[baseid]) >>> 8, getGangName(GangBaseOwner[baseid]), getGangTag(GangBaseOwner[baseid]));
        format(bstr, sizeof(bstr), "%s"CORAL"Aby zarz¹dzaæ baz¹, kliknij przycisk: "WHITE"SPACJA + N", bstr);
    }
    return bstr;
}

stock GetNearestBase(playerid, Float:dist = 3.0)
{
    new id = -1;
    foreach(new i : GangBases) {
        if(IsPlayerInDynamicArea(playerid, GangBaseRectangle[i]) || IsPlayerInRangeOfPoint(playerid, dist, unpackXYZ(gangBasesData[i][base_pickup]))) {
            id = i;
            break;
        }
    }
    return id;
}

stock CheckGangBase(gangid)
{
    new id = -1;
    foreach(new i : GangBases) {
        if(GangBaseOwner[i] == gangid && !GangBaseSell[i]) {
            id = i;
            break;
        }
    }
    return id;
}

stock Task:RunBaseOptions(playerid, baseid)
{
    new Task:t = task_new();
    if(pGang[playerid] == -1) return dialogBox(playerid, C_RED, "Nie jesteœ w ¿adnym zespole!"), t;

    if(GangBaseOwner[baseid] == -1) {
        if(CheckGangBase(pGang[playerid]) != -1) return dialogBox(playerid, C_RED, "Twój zespó³ posiada ju¿ zakupion¹ bazê!"), t;
        if(GangBaseOwner[baseid] == -1 && gangData[pGang[playerid]][gang_ownerid] != playerData[playerid][player_accountID]) return dialogBox(playerid, C_RED, "Tylko w³aœciciel zespo³u mo¿e zakupiæ bazê!"), t;

        s[0] = EOS;
        catstr(s, C_BLUE2, -1, "Baza {c}#%d{/c} znajduje siê w: {c}%s (%s){/c}.\n", baseid + 1, getZoneName(unpackXYZ(gangBasesData[baseid][base_pickup])), getCityName(unpackXYZ(gangBasesData[baseid][base_pickup])));
        catstr(s, C_BLUE2, -1, "Obecna cena za zakup tej bazy wynosi: {c}€%s{/c}.\n", strdot(GangBaseCost[baseid]));
        strcat(s, " \n");
        catstr(s, C_LY, -1, "Kliknij tutaj, by zakupiæ bazê za: {c}€%s{/c}.", strdot(GangBaseCost[baseid]));
        showDialog(playerid, DIALOG_BASE_BUY, DIALOG_STYLE_LIST, ""YELLOW"Zakup bazy gangowej", s, "Zakup", "Anuluj");
    } else {
        if(!GangBaseSell[baseid]) {
            if(GangBaseOwner[baseid] == -1 && gangData[pGang[playerid]][gang_ownerid] != playerData[playerid][player_accountID]) return dialogBox(playerid, C_RED, "Tylko w³aœciciel zespo³u mo¿e zarz¹dzaæ baz¹!"), t;

            s[0] = EOS;
            s = ""WHITE"Polecenie\t"LY"Rezultat\n";
            catstr(s, C_BLUE2, -1, "Lokalizacja bazy\t"LORANGE"%s (%s)\n", getZoneName(unpackXYZ(gangBasesData[baseid][base_pickup])), getCityName(unpackXYZ(gangBasesData[baseid][base_pickup])));
            catstr(s, C_BLUE2, -1, "Wartoœæ bazy\t"LORANGE"€%s\n", strdot(GangBaseCost[baseid]));
            strcat(s, " \n");
            catstr(s, C_CORAL, -1, "Nazwa bazy zespo³u\t"LY"%s\n", GangBaseName[baseid]);
            catstr(s, C_CORAL, -1, "Zarz¹dzaj wartoœci¹ bazy\t"GREEN""EURO"%s\n", strdot(GangBaseCost[baseid]));
            catstr(s, C_CORAL, -1, "Wystaw bazê na sprzeda¿\t"LIME""EURO"%s\n", strdot(GangBaseCost[baseid]));
            catstr(s, C_CORAL, -1, "Obiekty w bazie\t"LIME"%d/%d\n", GetBaseObjectCount(baseid), MAX_BASE_OBJECTS);
            strcat(s, " \n");
            catstr(s, C_VIOLET, -1, "Obra¿enia dla osób spoza zespo³u\t%s\n", (GangBaseSettings[baseid][0]) ? (""GREEN"TAK") : (""RED"NIE"));
            catstr(s, C_VIOLET, -1, "Automatyczny spawn cz³onków\t%s\n", (GangBaseSettings[baseid][1]) ? (""GREEN"TAK") : (""RED"NIE"));
            showDialog(playerid, DIALOG_BASE_PANEL, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Zarz¹dzanie baz¹ gangow¹", s, "Wybierz", "Zamknij");
        } else {
            if(gangData[pGang[playerid]][gang_ownerid] == playerData[playerid][player_accountID]) {
                s[0] = EOS;
                catstr(s, C_LIME, -1, "1. \t"YELLOW"Zmiana nazwy bazy "LRED"(%s)\n", GangBaseName[baseid]);
                catstr(s, C_LIME, -1, "2. \t"YELLOW"Zmiana wartoœci bazy "LIME"("EURO"%s)\n", strdot(GangBaseCost[baseid]));
                catstr(s, C_LIME, -1, "3. \t"YELLOW"Anuluj sprzeda¿ bazy\n");
                showDialog(playerid, DIALOG_BASE_SELL_PANEL, DIALOG_STYLE_LIST, ""YELLOW"Panel sprzeda¿y bazy", s, "Wybierz", "Zamknij");
            } else {
                s[0] = EOS;
                catstr(s, C_BLUE2, -1, "Baza {c}#%d{/c} znajduje siê w: {c}%s (%s){/c}.\n", baseid + 1, getZoneName(unpackXYZ(gangBasesData[baseid][base_pickup])), getCityName(unpackXYZ(gangBasesData[baseid][base_pickup])));
                catstr(s, C_BLUE2, -1, "W³aœcicielem bazy jest zespó³ {%06x}%s [%s]{/c}, a wartoœæ wynosi: {c}"EURO"%s{/c}.\n", getGangColor(GangBaseOwner[baseid]) >>> 8, getGangName(GangBaseOwner[baseid]), getGangTag(GangBaseOwner[baseid]), strdot(GangBaseCost[baseid]));
                strcat(s, " \n");
                catstr(s, C_LY, -1, "Kliknij tutaj, by zakupiæ bazê za: {c}€%s{/c}.", strdot(GangBaseCost[baseid]));
                showDialog(playerid, DIALOG_BASE_BUY, DIALOG_STYLE_LIST, ""YELLOW"Zakup bazy gangowej", s, "Zakup", "Anuluj");
            }
        }
    }

    playerData[playerid][player_saveData][0] = baseid;
    return t;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    new baseid = GetNearestBase(playerid);
    if(baseid != -1 && PRESSED(KEY_SPRINT | KEY_NO)) return task_await(RunBaseOptions(playerid, baseid));
    return 1;
}

hook OnPlayerEnterDynArea(playerid, areaid)
{
    if(playerData[playerid][player_gangbase] != -1) playerData[playerid][player_gangbase] = -1;

    new i = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID), baseid = i - BASE_MAGIC;
    if(0 <= baseid < Iter_Count(GangBases)) playerData[playerid][player_gangbase] = baseid;
    return 1;
}

hook OnPlayerLeaveDynArea(playerid, areaid)
{
    if(playerData[playerid][player_gangbase] != -1) playerData[playerid][player_gangbase] = -1;
    return 1;
}

stock CheckBaseTimer(playerid)
{
    if(playerData[playerid][player_gangbase] != -1) {
        new baseid = playerData[playerid][player_gangbase];
        if(pGang[playerid] != GangBaseOwner[baseid] && GangBaseSettings[baseid][0]) {
            new Float:health; GetPlayerHealth(playerid, health);
            SetPlayerHealth(playerid, health - 5.0);
            GameTextForPlayer(playerid, "~r~~h~BAZA ~y~NIE NALEZY ~r~~h~DO TWOJEGO ZESPOLU!", 2000, 3);
        }
    }    
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_BASE_SELL_NAME: {
            if(!response) return task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!IsValidGangBuffer(dest, 3, 20)) return showDialog(playerid, DIALOG_BASE_SELL_NAME, DIALOG_STYLE_INPUT, ""BLUE2"Zmiana nazwy bazy", ""WHITE"Wpisz poni¿ej nazwê dla swojej bazy:\n\n"RED"Nieprawid³owa d³ugoœæ nazwy - dozwolona to: "WHITE"3-20 "RED"znaki.", "Ustaw", "Cofnij");
            
            new baseid = playerData[playerid][player_saveData][0];
            format(GangBaseName[baseid], 50, "%s", dest);
            m_pquery_format("UPDATE es_gangs_bases SET name='%s' WHERE id=%d LIMIT 1;", dest, baseid + 1);
            UpdateDynamic3DTextLabelText(GangBaseLabel[baseid], -1, UpdateBaseLabel(baseid));
            return task_await(RunBaseOptions(playerid, baseid)), 1;
        }

        case DIALOG_BASE_SELL_COST: {
            if(!response) return task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));

            new cost;
            if(sscanf(inputtext, "d", cost)) return showDialog(playerid, DIALOG_BASE_SELL_COST, DIALOG_STYLE_INPUT, ""GREEN"Zmiana wartoœci bazy", ""WHITE"Wpisz poni¿ej wartoœæ bazy:\n\n"RED"Wartoœæ podaje siê w liczbach.", "Zmieñ", "Cofnij");
            if(cost < 300000 || cost > 10000000) return showDialog(playerid, DIALOG_BASE_SELL_COST, DIALOG_STYLE_INPUT, ""GREEN"Zmiana wartoœci bazy", ""WHITE"Wpisz poni¿ej wartoœæ bazy:\n\n"RED"Dozwolona wartoœæ bazy to: "WHITE""EURO"300.000-"EURO"10.000.000"RED".", "Zmieñ", "Cofnij");

            GangBaseCost[playerData[playerid][player_saveData][0]] = cost;
            m_pquery_format("UPDATE es_gangs_bases SET cost=%d WHERE id=%d LIMIT 1;", cost, playerData[playerid][player_saveData][0] + 1);
            return task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0])), 1;
        }

        case DIALOG_BASE_SELL_PANEL: {
            if(!response) return 1;

            new baseid = playerData[playerid][player_saveData][0];
            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_BASE_SELL_NAME, DIALOG_STYLE_INPUT, ""BLUE2"Zmiana nazwy bazy", ""WHITE"Wpisz poni¿ej nazwê dla swojej bazy:", "Ustaw", "Cofnij");
                case 1: showDialog(playerid, DIALOG_BASE_SELL_COST, DIALOG_STYLE_INPUT, ""GREEN"Zmiana wartoœci bazy", ""WHITE"Wpisz poni¿ej wartoœæ bazy:", "Zmieñ", "Cofnij");
            
                case 2: {
                    s[0] = EOS;
                    catstr(s, C_LRED, -1, "Czy napewno chcesz {c}anulowaæ{/c} sprzeda¿ bazy zespo³u?\n");
                    catstr(s, C_LRED, -1, "Spowoduje To {c}uniemo¿liwienie{/c} sprzeda¿y przez nastêpne {c}3{/c} dni!\n");
                    catstr(s, C_LY, -1, "Kliknij poni¿ej odpowiedni przycisk, by anulowaæ sprzeda¿ bazy!");

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, ll, ii
                        if(!rr) return task_await(RunBaseOptions(playerid, baseid)), 1;

                        GangBaseSell[baseid] = false;
                        GangBaseSellTime[baseid] = gettime() + UnixTime('d', 3);

                        m_pquery_format("UPDATE es_gangs_bases SET sell=0, sell_time=%d WHERE id=%d LIMIT 1;", GangBaseSellTime[baseid], baseid + 1);
                        return task_await(RunBaseOptions(playerid, baseid)), 1;
                    }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""RED"Anulowanie sprzeda¿y bazy", s, "Anuluj", "Cofnij");
                }
            }
            return 1;
        }

        case DIALOG_BASE_NAME: {
            if(!response) return task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));
            
            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(!IsValidGangBuffer(dest, 3, 20)) return showDialog(playerid, DIALOG_BASE_NAME, DIALOG_STYLE_INPUT, ""LIME"Nazwa bazy", ""WHITE"Wpisz poni¿ej nazwê dla swojej bazy:\n\n"RED"Nieprawid³owa d³ugoœæ nazwy - dozwolona to: "WHITE"3-20 "RED"znaki.", "Ustaw", "Cofnij");
            
            new baseid = playerData[playerid][player_saveData][0];
            format(GangBaseName[baseid], 50, "%s", dest);
            m_pquery_format("UPDATE es_gangs_bases SET name='%s' WHERE id=%d LIMIT 1;", dest, baseid + 1);

            new gangid = pGang[playerid];
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ zmieni³ nazwê bazy na: {c}%s{/c}.", getGangTag(gangid), dest);
            UpdateDynamic3DTextLabelText(GangBaseLabel[baseid], -1, UpdateBaseLabel(baseid));
            return task_await(RunBaseOptions(playerid, baseid)), 1;
        }

        case DIALOG_BASE_COST: {
            if(!response) return task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));

            new cost;
            if(sscanf(inputtext, "d", cost)) return showDialog(playerid, DIALOG_BASE_COST, DIALOG_STYLE_INPUT, ""LIME"Wartoœæ bazy", ""WHITE"Wpisz poni¿ej, jak¹ wartoœæ ma posiadaæ Twoja baza:\n\n"RED"Wartoœæ podaje siê w liczbach.", "Ustaw", "Cofnij");
            if(cost < 300000 || cost > 10000000) return showDialog(playerid, DIALOG_BASE_COST, DIALOG_STYLE_INPUT, ""LIME"Wartoœæ bazy", ""WHITE"Wpisz poni¿ej, jak¹ wartoœæ ma posiadaæ Twoja baza:\n\n"RED"Dozwolona wartoœæ bazy to: "WHITE""EURO"300.000-"EURO"10.000.000"RED".", "Ustaw", "Cofnij");

            new baseid = playerData[playerid][player_saveData][0];
            GangBaseCost[baseid] = cost;
            m_pquery_format("UPDATE es_gangs_bases SET cost=%d WHERE id=%d LIMIT 1;", cost, baseid + 1);

            new gangid = pGang[playerid];
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ ustawi³ now¹ wartoœæ bazy! "LY"("EURO"%s)", getGangTag(gangid), strdot(cost));
            return task_await(RunBaseOptions(playerid, baseid)), 1;
        }

        case DIALOG_BASE_SELL: {
            if(!response) return task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));

            new baseid = playerData[playerid][player_saveData][0];
            GangBaseSell[baseid] = true;
            
            new gangid = pGang[playerid];
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ wystawi³ bazê na sprzeda¿! "LY"("EURO"%s)", getGangTag(gangid), strdot(GangBaseCost[baseid]));
            return m_pquery_format("UPDATE es_gangs_bases SET sell=1 WHERE id=%d LIMIT 1;", baseid), 1;
        }

        case DIALOG_BASE_PANEL: {
            if(!response || pGang[playerid] == -1) return 1;

            new baseid = playerData[playerid][player_saveData][0];
            switch(listitem) {
                case 0..2, 7: task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));

                case 3: showDialog(playerid, DIALOG_BASE_NAME, DIALOG_STYLE_INPUT, ""LIME"Nazwa bazy", ""WHITE"Wpisz poni¿ej nazwê dla swojej bazy:", "Ustaw", "Cofnij");
                case 4: showDialog(playerid, DIALOG_BASE_COST, DIALOG_STYLE_INPUT, ""LIME"Wartoœæ bazy", ""WHITE"Wpisz poni¿ej, jak¹ wartoœæ ma posiadaæ Twoja baza:", "Ustaw", "Cofnij");

                case 5: {
                    if(GangBaseSellTime[baseid] > gettime()) return dialogBox(playerid, C_RED, "Aktualnie nie mo¿esz wystawiæ bazy zespo³u na sprzeda¿!\n"RED"Bêdziesz móg³ to zrobiæ za: {c}%s{/c}.", ConvertTime(GangBaseSellTime[baseid], TIME_TYPE_UNIX));
                    if(GangBaseCost[baseid] < 300000) return dialogBox(playerid, C_RED, "Wartoœæ bazy zespo³u nie mo¿e byæ mniejsza ni¿ {c}"EURO"300.000{/c}.");

                    s[0] = EOS;
                    catstr(s, C_LBLUE, -1, "Czy napewno chcesz wystawiæ na sprzeda¿ bazê: {c}%s{/c}?\n", GangBaseName[baseid]);
                    catstr(s, C_LBLUE, -1, "Zespó³ kupuj¹cy bêdzie musia³ zap³aciæ: {c}"EURO"%s{/c}!\n", strdot(GangBaseCost[baseid]));
                    catstr(s, C_LIME, -1, "Kliknij poni¿ej odpowiedni przycisk, by wystawiæ bazê na sprzeda¿!");
                    showDialog(playerid, DIALOG_BASE_SELL, DIALOG_STYLE_MSGBOX, ""YELLOW"Sprzeda¿ bazy zespo³u", s, "Wystaw", "Cofnij");
                }
                case 6: ShowBaseObjectShop(playerid, baseid);

                default: {
                    GangBaseSettings[baseid][listitem - 8] = !GangBaseSettings[baseid][listitem - 8];
                    m_pquery_format("UPDATE es_gangs_bases SET settings='%d, %d' WHERE id=%d LIMIT 1;", GangBaseSettings[baseid][0], GangBaseSettings[baseid][1], baseid + 1);
                    task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));
                }
            }
            return 1;
        }

        case DIALOG_BASE_MENU: {
            if(!response) return 1;

            new baseid = -1;
            if(pGang[playerid] != -1) baseid = CheckGangBase(pGang[playerid]);

            switch(listitem) {
                case 0: {

                }

                case 1: {
                    s[0] = EOS;
                    s = ""WHITE"Nazwa bazy\t"LY"Lokalizacja\t"GREEN"Cena\t"GREY"W³aœciciel\n";
                    foreach(new i : GangBases) catstr(s, C_WHITE, -1, "%s\t"LY"%s, %s\t"GREEN""EURO"%s\t%s\n", GangBaseName[i], getZoneName(unpackXYZ(gangBasesData[i][base_pickup])), getCityName(unpackXYZ(gangBasesData[i][base_pickup])), strdot(GangBaseCost[i]), (GangBaseOwner[i] == -1) ? (""GREY"---") : (form("{%06x}%s [%s]", getGangColor(GangBaseOwner[i]) >>> 8, getGangName(GangBaseOwner[i]), getGangTag(GangBaseOwner[i]))));
                    
                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return callcmd::bazy(playerid), 1; } 
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE2"Dostêpne bazy zespo³owe", s, "OK", #);
                }

                case 2: {

                }

                case 3: callcmd::bazy(playerid);
                case 4: { if(baseid != -1) callcmd::tspawn(playerid); }
                case 5: { if(baseid != -1) task_await(RunBaseOptions(playerid, baseid)); }
                case 6: {
                    new c;
                    if(!c) return dialogBox(playerid, C_RED, ""RED"System wymaga przetestowania na oddzielnym, testowym serwerze. Cierpliwoœci!");

                    if(baseid != -1) {
                        s[0] = EOS;
                        s = ""WHITE"Gang\t"YELLOW"Liczba osób\n";

                        new x = 0;
                        foreach(new i : Gangs) {
                            if(pGang[playerid] == i) continue;

                            new _baseid = CheckGangBase(i);
                            if(_baseid != -1) {
                                format(s, sizeof(s), "%s{%06x}%s [%s]\n"YELLOW"%d %s\n", s, getGangColor(i) >>> 8, getGangName(i), getGangTag(i), countGangMembers(i), ConvertTimeFormat(countGangMembers(i), "osób", "osoba", "osoby", "osób"));
                                playerData[playerid][player_gangBases][x++] = i;
                            }
                        }

                        if(!x) return dialogBox(playerid, C_RED, "Aktualnie brak zespo³ów do przeprowadzenia napaœci!");
                        showDialog(playerid, DIALOG_GANG_HEIST, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Napady zespo³owe", s, "Wybierz", "Cofnij");
                    }
                }
            }
            return 1;
        }

        case DIALOG_BASE_BUY: {
            if(!response || pGang[playerid] == -1) return 1;
            
            switch(listitem) {
                case 3: {
                    new gangid = pGang[playerid], baseid = playerData[playerid][player_saveData][3];
                    if(gangData[gangid][gang_ownerid] != playerData[playerid][player_accountID] || CheckGangBase(gangid) != -1) return 1;
                    if(gangData[gangid][gang_budget] < GangBaseCost[baseid]) return dialogBox(playerid, C_RED, "Twojemu zespo³owi brakuje {c}"EURO"%s{/c} na zakup tej bazy!", strdot(GangBaseCost[baseid] - gangData[gangid][gang_budget]));
                
                    if(GangBaseSell[baseid]) {
                        new ownerid = GangBaseOwner[baseid];
                        giveGangBudget(ownerid, GangBaseCost[baseid]);
                        foreach(new i : GangMember[ownerid]) msg(i, getGangColor(ownerid), "{c}(%s)*{/c}  Twój zespó³ sprzeda³ bazê za: {c}"EURO"%s{/c}.", getGangTag(ownerid), strdot(GangBaseCost[baseid]));
                    }

                    GangBaseOwner[baseid] = gangid;
                    GangBaseSell[baseid] = false;
                    giveGangBudget(gangid, -GangBaseCost[baseid]);

                    HideZoneForAll(GangBaseZone[baseid]);
                    ShowZoneForAll(GangBaseZone[baseid], setAlpha(getGangColor(gangid), 160));
                    UpdateDynamic3DTextLabelText(GangBaseLabel[baseid], -1, UpdateBaseLabel(baseid));

                    foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ zakupi³ bazê za {c}"EURO"%s{/c}! {c}(/tspawn){/c}", getGangTag(gangid), strdot(GangBaseCost[baseid]));
                    m_pquery_format("UPDATE es_gangs_bases SET ownerid=%d, sell=0 WHERE id=%d LIMIT 1;", gangid, baseid + 1);
                }
                default: task_await(RunBaseOptions(playerid, playerData[playerid][player_saveData][0]));
            }
            return 1;
        }
    }
    return 0;
}

CMD:brama(playerid)
{
    if(pGang[playerid] == -1) return dialogBox(playerid, C_RED, "Nie jesteœ w ¿adnym zespole.");

    new baseid = GetNearestBase(playerid, 30.0);
    if(baseid == -1) return dialogBox(playerid, C_RED, ""RED"Nie jesteœ w pobli¿u ¿adnej bazy zespo³owej!");

    new gangid = pGang[playerid];
    if(CheckGangBase(gangid) != baseid) return dialogBox(playerid, C_RED, "Twój zespó³ nie jest w³aœcicielem tej bazy!");
    
    new Float:x, Float:y, Float:z;
    GetDynamicObjectPos(GangBaseDoor[baseid], x, y, z);

    GangBaseDoorStatus[baseid] = !GangBaseDoorStatus[baseid];
    showGameDraw(playerid, (!GangBaseDoorStatus[baseid]) ?  ("~g~~h~~h~OTWIERANIE BRAMY~w~~h~...") : ("~r~~h~ZAMYKANIE BRAMY~w~~h~..."), 2);
    return MoveDynamicObject(GangBaseDoor[baseid], x, y, (GangBaseDoorStatus[baseid]) ? (GangBaseDoorZ[baseid]) : (GangBaseDoorZ[baseid] - 15.0), 7.0), 1;
}

CMD:tspawn(playerid)
{
    if(pGang[playerid] == -1) return dialogBox(playerid, C_RED, "Nie jesteœ w ¿adnym zespole.");

    new baseid = CheckGangBase(pGang[playerid]);
    if(baseid == -1) return sendError(playerid, ""RED"Twój zespó³ nie posiada zakupionej bazy.");
    return Teleport(playerid, false, unpackXYZR(gangBasesData[baseid][base_spawn]), 0, 0, true), 1;
}

alias:baza("gbaza", "bazamanage")
CMD:baza(playerid)
{
    if(pGang[playerid] == -1) return dialogBox(playerid, C_RED, "Nie jesteœ w ¿adnym zespole.");
    if(playerData[playerid][player_accountID] != gangData[pGang[playerid]][gang_ownerid]) return dialogBox(playerid, C_RED, "Musisz byæ {c}w³aœciciel{/c} zespo³u!");

    new gangid = pGang[playerid], baseid = CheckGangBase(gangid);
    if(gangid != -1 && baseid != -1) task_await(RunBaseOptions(playerid, baseid));
    return 1;
}

CMD:bazy(playerid)
{
    s[0] = EOS;
    catstr(s, C_BLUE, -1, "1. "LBLUE"Informacje o bazach zespo³owych\n");
    catstr(s, C_BLUE, -1, "2. "LBLUE"Dostêpne bazy zespo³owe "LY"(%d)\n", Iter_Count(GangBases));
    catstr(s, C_BLUE, -1, "3. "LBLUE"Jak szybko zarobiæ na bazê zespo³u?\n");
    strcat(s, " \n");

    if(pGang[playerid] != -1 && CheckGangBase(pGang[playerid]) != -1) {
        catstr(s, C_LIME, -1, "Teleport do bazy zespo³owej\n");
        if(playerData[playerid][player_accountID] == gangData[pGang[playerid]][gang_ownerid]) {
            catstr(s, C_LIME, -1, "Zarz¹dzanie baz¹ zespo³u\n");
            //catstr(s, C_DRED, -1, "Napaœæ na bazy gangowe\n");
        }
    }
    return showDialog(playerid, DIALOG_BASE_MENU, DIALOG_STYLE_LIST, ""ORANGE"Bazy zespo³owe", s, "Wybierz", "Zamknij"), 1;
}