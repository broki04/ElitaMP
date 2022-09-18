#include YSI_Coding\y_hooks

new 
    bool:pWarGame[MAX_PLAYERS],
    Float:pWarGamePosition[MAX_PLAYERS][3][4],
    pWarGameWeapons[MAX_PLAYERS][3],
    pWarGameName[MAX_PLAYERS][50],
    pWarGameZone[MAX_PLAYERS]
;

#define WAR_GAME_ZONE       0
#define WAR_GAME_TEAM1      1
#define WAR_GAME_TEAM2      2

hook function ResetPlayerData(playerid)
{
    pWarGameZone[playerid] = -1;
    pWarGame[playerid] = false;
    for(new i = 0; i < 3; i++) pWarGamePosition[playerid][i] = Float:{0.0, 0.0, 0.0, 0.0};
    pWarGameWeapons[playerid] = {-1, -1, -1};
    return continue(playerid);
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(pWarGame[playerid] && pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
    return 1;
}

alias:wgc("wgcreator")
CMD:wgc(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    s[0] = EOS;
    s = ""WHITE"Polecenie\t"CORAL"Rezultat\n";
    catstr(s, C_WHITE, -1, "Nazwa areny:\t"CORAL"%s\n", (strlen(pWarGameName[playerid]) > 2) ? (form("%s", pWarGameName[playerid])) : ("---"));
    catstr(s, C_WHITE, -1, "Strefa areny:\t"RED"%s\n", (pWarGamePosition[playerid][WAR_GAME_ZONE][0] == 0.0) ? ("BRAK") : ("USTALONO"));
    catstr(s, C_WHITE, -1, "Pozycja atakuj¹cych:\t"RED"%s\n", (pWarGamePosition[playerid][WAR_GAME_TEAM1][0] == 0.0) ? ("BRAK") : ("USTALONO"));
    catstr(s, C_WHITE, -1, "Pozycja broni¹cych:\t"GREEN"%s\n", (pWarGamePosition[playerid][WAR_GAME_TEAM2][0] == 0.0) ? ("BRAK") : ("USTALONO"));
    catstr(s, C_WHITE, -1, "Wybrane bronie:\t"LY"%s\n", (pWarGameWeapons[playerid][0] == -1) ? ("BRAK") : (form("%s, %s, %s", WeaponNames[pWarGameWeapons[playerid][0]], WeaponNames[pWarGameWeapons[playerid][1]], WeaponNames[pWarGameWeapons[playerid][2]])));
    strcat(s, ""LIME"Stwórz arenê\n");
    return showDialog(playerid, DIALOG_WARGAME_CREATOR, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Kreator /WG", s, "Wybierz", "Zamknij"), 1;
}
flags:wgc(HEAD)

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_WARGAME_NAME: {
            if(!response) return callcmd::wgc(playerid);

            new dest[90]; mysql_escape_string(inputtext, dest, sizeof(dest));
            if(strlen(dest) < 3 || strlen(dest) > 20) return showDialog(playerid, DIALOG_WARGAME_NAME, DIALOG_STYLE_INPUT, ""GOLD"Nazwa areny", ""WHITE"Wpisz poni¿ej nazwê areny:\n\n"RED"Dozwolona liczba znaków to: "WHITE"3-20 "RED"znaki.", "Ustaw", "Cofnij");

            format(pWarGameName[playerid], 50, "%s", dest);
            return callcmd::wgc(playerid), 1;
        }

        case DIALOG_WARGAME_WEAPONS: {
            if(!response) return callcmd::wgc(playerid);

            new weap[3];
            if(sscanf(inputtext, "ddd", weap[0], weap[1], weap[2]) || !IsValidWeapon(weap[0]) || !IsValidWeapon(weap[1]) || !IsValidWeapon(weap[2])) return showDialog(playerid, DIALOG_WARGAME_WEAPONS, DIALOG_STYLE_INPUT, ""GREEN"Bronie na arenê", ""WHITE"Wpisz poni¿ej trzy ID broni, które bêd¹ na arenie!\n"WHITE"Wpisuj wed³ug wzoru: "LY"24 25 34\n\n"RED"Wprowadzone ID broni jest z³e.", "Ustaw", "Cofnij");
            
            for(new i = 0; i < 3; i++) pWarGameWeapons[playerid][i] = weap[i];
            return callcmd::wgc(playerid), 1;
        }

        case DIALOG_WARGAME_CREATOR: {
            if(!response) return 1;

            switch(listitem) {
                case 0: showDialog(playerid, DIALOG_WARGAME_NAME, DIALOG_STYLE_INPUT, ""GOLD"Nazwa areny", ""WHITE"Wpisz poni¿ej nazwê areny:", "Ustaw", "Cofnij");
                
                case 1: {
                    if(pWarGame[playerid]) return dialogBox(playerid, setAlpha(C_RED, 160), "Tworzysz ju¿ strefê areny do /WG!");
                    
                    pWarGame[playerid] = true;
                    showGameDraw(playerid, "~y~TWORZENIE ARENY!~n~~y~UZYCIE: ~w~~h~STRZALKI ~p~~h~(RADAR)", 10);

                    new Float:unusedFloat;
                    GetPlayerPos(playerid, pWarGamePosition[playerid][WAR_GAME_ZONE][0], pWarGamePosition[playerid][WAR_GAME_ZONE][1], unusedFloat);
                    GetPlayerPos(playerid, pWarGamePosition[playerid][WAR_GAME_ZONE][2], pWarGamePosition[playerid][WAR_GAME_ZONE][3], unusedFloat);
                }

                case 2, 3: {
                    new Float:pos[4]; getPosition(playerid, false, unpackXYZR(pos));
                    for(new i = 0; i < 4; i++) pWarGamePosition[playerid][(listitem == 2) ? (WAR_GAME_TEAM1) : (WAR_GAME_TEAM2)][i] = pos[i];
                    callcmd::wgc(playerid);
                }
                case 4: showDialog(playerid, DIALOG_WARGAME_WEAPONS, DIALOG_STYLE_INPUT, ""GREEN"Bronie na arenê", ""WHITE"Wpisz poni¿ej trzy ID broni, które bêd¹ na arenie!\n"WHITE"Wpisuj wed³ug wzoru: "LY"24 25 34", "Ustaw", "Cofnij");
            
                case 5: {
                    if(strlen(pWarGameName[playerid]) < 2) return dialogBox(playerid, C_RED, "Nie ustalono nazwy areny.");
                    if(pWarGamePosition[playerid][WAR_GAME_ZONE][0] == 0.0) return dialogBox(playerid, C_RED, "Nie ustalono strefy zabawy.");
                    if(pWarGamePosition[playerid][WAR_GAME_TEAM1][0] == 0.0) return dialogBox(playerid, C_RED, "Nie ustalono pozycji spawnowania siê atakuj¹cych.");
                    if(pWarGamePosition[playerid][WAR_GAME_TEAM2][0] == 0.0) return dialogBox(playerid, C_RED, "Nie ustalono pozycji spawnowania siê broni¹cy.");
                    if(pWarGameWeapons[playerid][0] == -1) return dialogBox(playerid, C_RED, "Nie ustalono broni na zabawê.");

                    inline onArenaAdd() {
                        new arenaid = cache_insert_id();
                        msg(playerid, C_LGREEN, "Dodano arenê {c}%s (ID: %d){/c}.", pWarGameName[playerid], arenaid);

                        pWarGameZone[playerid] = -1;
                        pWarGame[playerid] = false;
                        for(new i = 0; i < 3; i++) pWarGamePosition[playerid][i] = Float:{0.0, 0.0, 0.0, 0.0};
                        pWarGameWeapons[playerid] = {-1, -1, -1};
                        return strcat(pWarGameZone[playerid], " "), 1;
                    }
                    m_tquery_inline(dbconn, using inline onArenaAdd, "INSERT INTO es_fun_wg (name, zone, att, def, weap) VALUES ('%s', '%f, %f, %f, %f', '%f, %f, %f, %f', '%f, %f, %f, %f', '%d, %d, %d');", pWarGameName[playerid], unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]), unpackXYZR(pWarGamePosition[playerid][WAR_GAME_TEAM1]), unpackXYZR(pWarGamePosition[playerid][WAR_GAME_TEAM2]), unpackXYZ(pWarGameWeapons[playerid]));
                }
            }
            return 1;
        }
    }
    return 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(pWarGame[playerid] && PRESSED(KEY_SECONDARY_ATTACK)) {
        TogglePlayerControllable(playerid, true);
        pWarGame[playerid] = false;

        inline onDialogData(pp, dd, rr, ll, string:ii[]) {
            #pragma unused pp, dd, ii

            if(!rr) {
                pWarGame[playerid] = false;
                if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
                return callcmd::wgc(playerid), 1;
            }

            switch(ll) {
                case 0: msg(playerid, C_LIME, "Zapisano strefê areny! Kontynuuj tworzenie areny.");
                case 1: msg(playerid, C_LIME, "Tworzenie strefy zosta³o anulowane! Kontynuuj tworzenie areny.");
            }

            pWarGame[playerid] = false;
            if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
            return callcmd::wgc(playerid), 1;
        }
        return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_LIST, ""YELLOW"Strefa areny", ""WHITE"Zapisz strefê\n"WHITE"Usuñ strefê\n", "Wybierz", "Anuluj"), 1;
    }
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if(pWarGame[playerid]) {
        new keys, ud, lr;
        GetPlayerKeys(playerid, keys, ud, lr); 
        TogglePlayerControllable(playerid, false);

		if(lr == KEY_LEFT) {
			pWarGamePosition[playerid][WAR_GAME_ZONE][0] -= 8.0;
  			if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
   			pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
     		GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
       	}
	    else if(lr & KEY_LEFT && keys & KEY_FIRE) {
   			pWarGamePosition[playerid][WAR_GAME_ZONE][0] += 8.0;
     		if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
     		pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
	       	GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
		}
  		else if(lr == KEY_RIGHT) {
    		pWarGamePosition[playerid][WAR_GAME_ZONE][2] += 8.0;
     		if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
   			pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
     		GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
		}
  		else if(lr & KEY_RIGHT && keys & KEY_FIRE) {
    		pWarGamePosition[playerid][WAR_GAME_ZONE][2] -= 8.0;
      		if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
   			pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
     		GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
		}
		else if(ud == KEY_UP) {
  			pWarGamePosition[playerid][WAR_GAME_ZONE][3] += 8.0;
    		if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
   			pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
     		GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
		}
		else if(ud & KEY_UP && keys & KEY_FIRE) {
		    pWarGamePosition[playerid][WAR_GAME_ZONE][3] -= 8.0;
	       	if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
	   		pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
	       	GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
		}
		else if(ud == KEY_DOWN) {
		    pWarGamePosition[playerid][WAR_GAME_ZONE][1] -= 8.0;
	       	if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
      		pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
	       	GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
		}
		else if(ud & KEY_DOWN && keys & KEY_FIRE) {
		    pWarGamePosition[playerid][WAR_GAME_ZONE][1] += 8.0;
	       	if(pWarGameZone[playerid] != -1) GangZoneDestroy(pWarGameZone[playerid]);
      		pWarGameZone[playerid] = GangZoneCreate(unpackXYZR(pWarGamePosition[playerid][WAR_GAME_ZONE]));
         	GangZoneShowForPlayer(playerid, pWarGameZone[playerid], setAlpha(C_RED, 160));
		}
    }
    return 1;
}