#include YSI_Coding\y_hooks

new 
	wpObjectData[50],
	wpCountPeople,
	wpArenaID,
	bool:wpSwitch,
	wpWinCP,
	wpMeta,
	wpTimer,
	wpRecordUID,
	wpRecordTime,

	wpFallCount[MAX_PLAYERS]
;

stock StartWP()
{
    if(!GameStarted[GAME_WP]) return 1;
    GameStarted[GAME_WP] = true;

    wpCountPeople = Iter_Count(Game[GAME_WP]);
	wpArenaID = random(4);
	wpMeta = wpTimer = 0;

	if(IsValidDynamicCP(wpWinCP)) DestroyDynamicCP(wpWinCP);
	switch(wpArenaID) {
		case 0: wpWinCP = CreateDynamicCP(-3825.7432, 71.9582, 16.1176, 2.0, .worldid=VW_WP, .streamdistance=99999.0, .priority=1);
		case 1: wpWinCP = CreateDynamicCP(1322.5627, -2914.5513, 76.0186, 2.0, .worldid=VW_WP, .streamdistance=99999.0, .priority=1);
		case 2: wpWinCP = CreateDynamicCP(3189.0029, -3118.1018, 66.1556, 2.0, .worldid=VW_WP, .streamdistance=99999.0, .priority=1);
		case 3: wpWinCP = CreateDynamicCP(3260.9214, -469.6050, 50.4406, 2.0, .worldid=VW_WP, .streamdistance=99999.0, .priority=1);
	}

	inline onFetchRecord() {
		cache_get_value_name_int(0, "record_uid", wpRecordUID);
		cache_get_value_name_int(0, "record_time", wpRecordTime);

        new time_str[90]; format(time_str, sizeof(time_str), "%02d:%02d", floatround(((wpRecordTime) / 3600)), floatround(((wpRecordTime) / 60) % 60));
		foreach(new i : Game[GAME_WP]) {
			SavePlayerPosition(i);
			ResetPlayerWeapons(i);
			
			wpFallCount[i] = 0;
			SetHealth(i, 100.0, 100.0);
			playerData[i][player_god] = true;

			UpdateGameStatus(i, GAME_WP);
			for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);

			switch(wpArenaID) {
				case 0: {
					Teleport(i, false, -3786.105224, -573.58837, 7.91103, 0.0, 0, VW_WP, false);
					SetPlayerMapIcon(i, 1, -3825.7432, 71.9582, 16.1176, 17, -1, MAPICON_GLOBAL);
				}

				case 1: {
					Teleport(i, false, 1733.9845, -2998.6887, 3.9896, 0.0, 0, VW_WP, false);
					SetPlayerMapIcon(i, 1, 1322.5627, -2914.5513, 76.0186, 17, -1, MAPICON_GLOBAL);
				}
				
				case 2: {
					Teleport(i, false, 3065.6580, -2668.2412, 13.3102, 0.0, 0, VW_WP, false);
					SetPlayerMapIcon(i, 1, 3189.0029, -3118.1018, 66.1556, 17, -1, MAPICON_GLOBAL);
				}
				
				case 3: {
					Teleport(i, false, 3291.8894, -954.2856, 20.7121, 0.0, 0, VW_WP, false);
					SetPlayerMapIcon(i, 1, 3260.9214, -469.6050, 50.4406, 17, -1, MAPICON_GLOBAL);
				}
			}

			if(wpRecordUID != -1) {
				new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", wpRecordUID));
				if(cache_num_rows() > 0) {
					new nick[25]; cache_get_value(0, 0, nick, sizeof(nick));
					msg(i, C_CHAT, "Rekord {c}areny #%d{/c} pobity przez {c}%s{/c}. "LY"(%s)", wpArenaID + 1, nick, time_str);
				}
				cache_delete(result);
			}

			FreezePlayer(i, 5);
			setOdliczanie(i, 5);
			SetTimer("ResetTimerWP", 1000 * 5, false);
		}

    	msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(Arena #%d, %d %s)", gameName[GAME_WP], gameCMD[GAME_WP], wpArenaID + 1, Iter_Count(Game[GAME_WP]), ConvertTimeFormat(Iter_Count(Game[GAME_WP]), "osób", "osoba", "osoby", "osób"));
	}
	m_tquery_inline(dbconn, using inline onFetchRecord, "SELECT * FROM es_fun_wp WHERE arenaid=%d LIMIT 1;", wpArenaID);
    return UpdateGameDraw(), 1;
}
function ResetTimerWP() return wpTimer = 0;

stock MoveWP()
{
	if(!GameStarted[GAME_WP]) return 1;
	wpSwitch = !wpSwitch;
	
	for(new i = 0; i < sizeof(wpObjectData); i++) {
		if(wpSwitch) MoveDynamicObject(wpObjectData[i], unpackXYZ(wipeoutObjectPosition[i][wipeout_start_pos]), 3);
		else MoveDynamicObject(wpObjectData[i], unpackXYZ(wipeoutObjectPosition[i][wipeout_last_pos]), 3);
	}
	return 1;	
}

stock UpdateWP()
{
    if(!GameStarted[GAME_WP]) return 1;
	if(Iter_Count(Game[GAME_WP]) < 1) return StopWP();
	wpTimer ++;

	new iterate_max;
	foreach(new i : Game[GAME_WP]) {
		new Float:pos[3]; GetPlayerPos(i, unpackXYZ(pos));
		if(pos[2] < 0.0) {
			wpFallCount[i] ++;
			if(wpFallCount[i] < 3) {
				switch(wpArenaID) {
					case 0: Teleport(i, false, -3786.105224, -573.58837, 7.91103, 0.0, 0, VW_WP, false);
					case 1: Teleport(i, false, 1733.9845, -2998.6887, 3.9896, 0.0, 0, VW_WP, false);
					case 2: Teleport(i, false, 3065.6580, -2668.2412, 13.3102, 0.0, 0, VW_WP, false);
					case 3: Teleport(i, false, 3291.8894, -954.2856, 20.7121, 0.0, 0, VW_WP, false);
				} 
				showGameDraw(i, form("~y~SPADLES(AS) ~w~~h~%d/3 ~y~RAZ DO WODY!~n~~y~PO PRZEKROCZENIU ~w~~h~ILOSCI PROB ~y~ZOSTANIESZ WYRZUCONY(A)!", wpFallCount[i]), 5);
			} else {
				for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
				LoadPlayerPosition(i);
				RemovePlayerMapIcon(i, 1);

				pGame[i] = -1;
				wpFallCount[i] = 0;

				Iter_SafeRemove(Game[GAME_WP], i, iterate_max);
				i = iterate_max;
			}
		}

		UpdateGameStatus(i, GAME_WP);
        for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);
	}
	return 1;
}

stock StopWP()
{
	if(!GameStarted[GAME_WP]) return 1;
	GameStarted[GAME_WP] = false;
	GameCountDown[GAME_WP] = 0;

	foreach(new i : Game[GAME_WP]) {
		pGame[i] = -1;
		LoadPlayerPosition(i);
		RemovePlayerMapIcon(i, 1);
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
	}
	Iter_Clear(Game[GAME_WP]);

	UpdateGameDraw();
	msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zosta³a zakoñczona!", gameName[GAME_WP], gameCMD[GAME_WP]);
	return KillTimer(GameTimer[GAME_WP]);
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	if(pGame[playerid] == GAME_WP && Iter_Contains(Game[GAME_WP], playerid) && GameStarted[GAME_WP] && checkpointid == wpWinCP) {
		wpMeta ++;
		if(wpMeta >= 3) {
			new money = math_random(1000, 3000);
			giveMoney(playerid, money);
			showGameDraw(playerid, form("~y~WIPEOUT ~r~ZAKONCZONE!~n~~y~NAGRODA: ~r~%s$", strdot(money)));
			StopWP();
		} else {
            new time_str[90]; format(time_str, sizeof(time_str), "%02d:%02d", floatround(((wpTimer) / 3600)), floatround(((wpTimer) / 60) % 60));
			new money = (wpMeta == 1) ? (math_random(17000, 30000)) : (wpMeta == 2) ? (math_random(7000, 15000)) : (math_random(5000, 10000));
			giveMoney(playerid, money);
			
			achievement(playerid, ACH_WP);
			CheckBet(playerid, BET_GAME, true);

			if(wpMeta == 1) {
				if(wpRecordTime > wpTimer || wpRecordUID == -1) {
					m_pquery_format("UPDATE es_fun_wp SET record_uid=%d, record_time=%d WHERE arenaid=%d LIMIT 1;", wpRecordUID, wpRecordTime, wpArenaID);
			
					msgAll(-1, " ");
					msgAll(C_VIOLET, "Gracz {c}%s(%d){/c} pobi³(a) rekord {c}areny #%d{/c}.", getNick(playerid), playerid, RaceArenaName);
					msgAll(C_VIOLET, "Nowy rekord areny wynosi: {c}%s{/c}.", time_str);
					msgAll(-1, " ");

					giveMoney(playerid, 10000);
					msg(playerid, C_VIOLET, "Za pobicie rekordu areny otrzymujesz: {c}10.000${/c}.");
				}
			}
			else CheckBet(playerid, BET_GAME, false);

			pGame[playerid] = -1;
			LoadPlayerPosition(playerid);
			RemovePlayerMapIcon(playerid, 1);
			for(new x = 0; x < 3; x++) PlayerTextDrawHide(playerid, tdInfo[x]);
			Iter_Remove(Game[GAME_WP], playerid);

			msgAll(C_BROWN, "Gracz {c}%s(%d){/c} przeszed³(a) {c}%s (%s){/c} jako: {c}#%d{/c}.", getNick(playerid), playerid, gameName[GAME_WP], gameCMD[GAME_WP], wpMeta);
			showGameDraw(playerid, form("~y~DOSZEDLES(AS) ~r~#%d ~y~DO METY!~n~~y~NAGRODA: ~r~%s$", wpMeta, strdot(money)));
		}
	}
	return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
	if(pGame[playerid] == GAME_WP && GameStarted[GAME_WP] && Iter_Contains(Game[GAME_WP], playerid)) {
		pGame[playerid] = -1;
		msg(playerid, C_CHAT, "Zgin¹³eœ(aœ) na: {c}%s (%s){/c}. Spróbuj ponownie póŸniej!", gameName[GAME_WP], gameCMD[GAME_WP]);

		LoadPlayerPosition(playerid);
		RemovePlayerMapIcon(playerid, 1);

		for(new x = 0; x < 3; x++) PlayerTextDrawHide(playerid, tdInfo[x]);
		Iter_Remove(Game[GAME_WP], playerid);
	}
	return 1;
}