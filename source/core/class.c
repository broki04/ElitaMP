public OnPlayerConnect(playerid)
{
    ResetPlayerData(playerid);

    GetPlayerName(playerid, playerData[playerid][player_nickname], MAX_PLAYER_NAME);
    GetPlayerIp(playerid, playerData[playerid][player_ipaddress], 18);
    GPCI(playerid, playerData[playerid][player_serial], 65);

    GetPlayerCountry(playerid, playerData[playerid][player_country], MAX_COUNTRY_LENGTH);
    GetPlayerCity(playerid, playerData[playerid][player_city], MAX_CITY_LENGTH);
    GetPlayerAutonomousSystem(playerid, playerData[playerid][player_isp], MAX_AUTONOMOUS_SYSTEM_LENGTH);

    CreatePlayerDesign(playerid);
    RemoveObjects(playerid);
    connects ++;

    for(new i = 0; i < sizeof(animationList); i++) ApplyAnimation(playerid, animationList[i], "null", 4.0, false, false, false, false, 0, 1);

    GangBar[playerid] = CreatePlayerProgressBar(playerid, 430.0, 21.0, 54.0, 3.20, C_BLUE, 100.0, 0);
    LevelBar[playerid] = CreatePlayerProgressBar(playerid, 281.0, 5.0, 110.5, 3.2, C_BLUE, 100.0, 0);
    FishBar[playerid] = CreatePlayerProgressBar(playerid, 284.0, 343.0, 73.0, 3.69, C_BLUE, 100.0, 0);
    FuelBar[playerid] = CreatePlayerProgressBar(playerid, 275.0, 381.0, 98.5, 5.20, C_BLUE, 100.0, 0);
    SlotsBar[playerid] = CreatePlayerProgressBar(playerid, 258.0, 288.0, 123.5, 4.7, C_BLUE, 100.0, 0);
    PoolBar[playerid] = CreatePlayerProgressBar(playerid, 291.0, 395.0, 76.0, 3.7, C_BLUE, 100.0, 0);

    FindAccount(playerid);
    return SetPlayerColor(playerid, KoloryGraczy[random(sizeof(KoloryGraczy))]), 1;
}
public OnIncomingConnection(playerid, ip_address[], port) return SendRconCommand("reloadbans");

stock GetQuitReason(reason) {
    new name[15];
    switch(reason) {
        case 0: name = "Crash";
        case 1: name = "Quit";
        case 2: name = "Kick";
    }
    return name;
}

public OnPlayerDisconnect(playerid, reason)
{
    if(playerData[playerid][player_logged]) {
        SavePlayerStats(playerid);
        SetPlayerDraw(playerid, false);
    
        foreach(new i : Player) {
            if(playerData[i][player_teleportRequest] == playerid) {
                playerData[i][player_teleportRequest] = -1;
                playerData[i][player_teleportTimer] = 0;
            }
        }

        if(pVehCount[playerid] > 0) { 
            for(new i = 0; i < pVehCount[playerid]; i++) {
                if(pVehHave[playerid][i] != -1 && pVehSpawned[pVehHave[playerid][i]]) {
                    DestroyPrivCar(pVehHave[playerid][i]);
                }
            }
        }
        msgAdmins(C_ORANGE, ""LY"[%s]: "ORANGE"%s(%d): "GREY"(IP: %s, %s - %s, %s)", toUpperCase(GetQuitReason(reason)), getNick(playerid), playerid, getIP(playerid), playerData[playerid][player_country], playerData[playerid][player_city], playerData[playerid][player_isp]);
    }
    Log(LOG_DEBUG, "[%s]: Roz³¹czono gracza %s(%d).", toLowerCase(GetQuitReason(reason)), getNick(playerid), playerid);
    if(IsValidDynamic3DTextLabel(playerData[playerid][player_label])) DestroyDynamic3DTextLabel(playerData[playerid][player_label]);

    new gangid = pGang[playerid];
    ResetPlayerData(playerid);

    if(gangid != -1) {
        SaveGang(gangid);
        UpdateGangBox(gangid);
    }
    return HidePlayerDialog(playerid), 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    for(new i = 0; i < 4; i++) togglePlayerTextDraw(tdLicznik[i], playerid, false);

    if(killerid != INVALID_PLAYER_ID) {
        TogglePlayerSpectating(playerid, true);
        PlayerSpectatePlayer(playerid, killerid);
        SetTimerEx("StopKillCamera", 1000 * 4, false, "d", playerid);

        CheckDuelKiller(playerid, killerid);
        CheckQuest(killerid, DAILY_KILLS);

        achievement(playerid, ACH_DEATHS);
        achievement(killerid, ACH_KILLS);

        playerData[killerid][player_killstreak] ++; // to do

        if(pUlepszeniaLevel[killerid][ULEPSZENIE_KILLER] > 0) giveMoney(killerid, (pUlepszeniaLevel[killerid][ULEPSZENIE_KILLER] * 10));

        if(pGang[killerid] != -1 && pGang[playerid] != -1) {
            new gangid_killerid = pGang[killerid], gangid_playerid = pGang[playerid];      
            gangData[gangid_killerid][gang_kills] ++;
            gangData[gangid_playerid][gang_deaths] ++;

            if(pGang[playerid] != pGang[killerid]) {
                giveGangBudget(gangid_killerid, math_random(100, 300));
                CheckGangAchievement(gangid_killerid, GANG_ACHIEVEMENT_KILLA);
                if(GangUlepszeniaLevel[gangid_killerid][GANG_ULEP_KIBICE] > 0) addGangScore(gangid_killerid, GangUlepszeniaLevel[gangid_killerid][GANG_ULEP_KIBICE]); 
            }
            
            if(Iter_Contains(Sparing, killerid) && Iter_Contains(Sparing, playerid) && pGangSpar[gangid_killerid] && pGangSpar[gangid_playerid] && (pGangSparInitiator[gangid_killerid] == gangid_playerid || pGangSparInitiator[gangid_playerid] == gangid_killerid)) {
                pGangSparPoints[gangid_killerid] ++;
                sparingKillerData[killerid][sparing_kills] ++;
            }
        }
        Log(LOG_DEBUG, "[kill]: %s(%d) -> %s(%d): %s", getNick(killerid), killerid, getNick(playerid), playerid, WeaponNames[reason]);
    }

    playerData[playerid][player_battleTimer] = 0; 
    return SendDeathMessage(killerid, playerid, reason), 1;
}
function StopKillCamera(playerid) return TogglePlayerSpectating(playerid, false);

public OnPlayerRequestClass(playerid, classid)
{
    Teleport(playerid, false, 1950.0698, 1178.6680, 1151.3988, 322.9739, VW_WYBIERALKA, VW_WYBIERALKA);
    SetPlayerCameraPos(playerid, 1953.6459, 1182.4248, 1152.0829);
    SetPlayerCameraLookAt(playerid, 1953.0323, 1181.6375, 1151.9786);
    Streamer_UpdateEx(playerid, 1950.0698, 1178.6680, 1151.3988, VW_WYBIERALKA, VW_WYBIERALKA, STREAMER_TYPE_OBJECT);

	SetPlayerWeather(playerid, 2);
	SetPlayerTime(playerid, 12, 0);

	switch(random(5)) {
		case 0: ApplyAnimation(playerid, "DANCING", "DAN_Down_A", 4.0, true, true, true, true, 1, 0);
		case 1: ApplyAnimation(playerid, "DANCING", "dance_loop", 4.1, true, true, true, true, 1, 0);
		case 2: ApplyAnimation(playerid, "DANCING", "DAN_Right_A", 4.1, true, true, true, true, 1, 0);
		case 3: ApplyAnimation(playerid, "DANCING", "DAN_Up_A", 4.1, true, true, true, true, 1, 0);
		case 4: ApplyAnimation(playerid, "DANCING", "dnce_M_a", 4.1, true, true, true, true, 1, 0);
	}
    return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    if(!playerData[playerid][player_logged]) return Kick(playerid), 0;

    for(new i = 0; i < 4; i++) TextDrawHideForPlayer(playerid, tdWelcome[i]);
    PlayerTextDrawHide(playerid, tdLoginTime);

    foreach(new i : GangZones) ShowZoneForPlayer(playerid, pGangZone[i], setAlpha((pGangZoneOwner[i] == -1) ? (C_GREY) : (getGangColor(pGangZoneOwner[i])), 160));
    foreach(new i : GangBases) ShowZoneForPlayer(playerid, GangBaseZone[i], setAlpha((GangBaseOwner[i] == -1) ? (C_GREY) : (getGangColor(GangBaseOwner[i])), 160));
    for(new i = 0; i < sizeof(zonesNoDM); i++) ShowZoneForPlayer(playerid, noDMZone[i], setAlpha(C_YELLOW, 160));

    if(KurczakEnabled) {
        msg(playerid, -1, " ");
        msg(playerid, C_CORAL, "Na mapie znajduje siê {c}kurczak{/c}, obecna wytrzyma³oœæ: {c}%.2f HP{/c}.", KurczakHealth);
        msg(playerid, C_CORAL, "Za jego zabójstwo, ka¿dy w pobli¿u otrzymuje: {c}%s${/c}.", strdot(KurczakReward));
    }

    if(IsValidDynamic3DTextLabel(playerData[playerid][player_label])) DestroyDynamic3DTextLabel(playerData[playerid][player_label]);
    playerData[playerid][player_label] = CreateDynamic3DTextLabel(" ", -1, 0.0, 0.0, 0.0, 12.5, .attachedplayer=playerid, .worldid=0);
    return SetPlayerDraw(playerid, pDrawVisible[playerid]), 1;
}

public OnPlayerSpawn(playerid)
{
    if(!playerData[playerid][player_logged]) return Kick(playerid), 0;
    SetPlayerSkin(playerid, playerData[playerid][player_skin]);

    if(Iter_Contains(Sparing, playerid) && pGang[playerid] != -1 && pGangSpar[pGang[playerid]]) return SparingRespawn(playerid, pGang[playerid]), 0;
    if(pDuel[playerid] && pDuelEnemy[playerid] != -1 && pDuelEnemy[pDuelEnemy[playerid]] == playerid) return DuelRespawn(playerid, pDuelEnemy[playerid]), 0;
    if(pArena[playerid] != -1) return JoinPlayerArena(playerid, pArena[playerid]), 0;
    if(pGame[playerid] == GAME_DA && Iter_Contains(Game[GAME_DA], playerid) && GameStarted[GAME_DA]) return RespawnDA(playerid), 0;
    if(pGame[playerid] == GAME_GG && Iter_Contains(Game[GAME_GG], playerid) && GameStarted[GAME_GG]) return RespawnGG(playerid), 0;
    if(pGame[playerid] == GAME_WG && Iter_Contains(Game[GAME_WG], playerid) && GameStarted[GAME_WG]) return RespawnWG(playerid), 0;
    if(playerData[playerid][player_jailtime] > 0) return JailPlayer(playerid, -1, playerData[playerid][player_jailtime], "Kara do przesiedzenia"), 0;
//    if(pGang[playerid] != -1 && gangData[pGang[playerid]][gang_battleZone] != -1) callcmd::gtp(playerid);
    
    EnableSpawnKill(playerid);
    ShowNameTagForAll(playerid);
    playerData[playerid][player_battleTimer] = 0; 

    switch(playerData[playerid][player_spawnData]) {
        case 0: RandomSpawn(playerid);
        case 1: Teleport(playerid, false, unpackXYZR(pStalyPos[playerid]), pStalyInterior[playerid], 0);
        case 2: Teleport(playerid, false, unpackXYZR(playerData[playerid][player_lastPos]), playerData[playerid][player_lastInterior], 0);
    }

    if(pGang[playerid] != -1 && CheckGangBase(pGang[playerid]) != -1) {
        new baseid = CheckGangBase(pGang[playerid]);
        if(GangBaseSettings[baseid][1]) Teleport(playerid, false, unpackXYZR(gangBasesData[baseid][base_spawn]), 0, 0, false);
    }

    if(playerData[playerid][player_respawn]) {
        playerData[playerid][player_respawn] = false;
        return LoadPlayerPosition(playerid), 1;
    }

    Log(LOG_DEBUG, "[spawn]: Wywo³ano callback `OnPlayerSpawn` na graczu: %s(%d).", getNick(playerid), playerid);
    return 1;
}

public OnPlayerText(playerid, text[])
{
    if(!playerData[playerid][player_logged] || isnull(text)) return 0;
    if(pAFK[playerid]) return dialogBox(playerid, C_RED, "Jesteœ {c}AFK{/c}, wiêc nie mo¿esz pisaæ na czacie!\n"RED"Aby wróciæ do gry, wpisz: {c}/jj{/c}."), 0;

    if(playerData[playerid][player_mutetime] > 0) {
        foreach(new i : Permission) {
            if(playerData[i][player_admin] >= MODERATOR && playerData[i][player_checkMuted]) {
                msg(i, C_ORANGE, ""BROWN"[MUTE]: "ORANGE"%s(%d): {c}%s{/c}", getNick(playerid), playerid, text);
            }
        }

        Log(LOG_CHAT, "[muted]: %s(%d): %s", getNick(playerid), playerid, text);
        return CheckPlayerMuted(playerid), 0;
    }

    if(playerData[playerid][player_admin] < MODERATOR) {
        if(playerData[playerid][player_antySpam] > gettime()) return msg(playerid, C_CORAL, "Nastêpn¹ wiadomoœæ bêdziesz móg³ wys³aæ za: {c}%s{/c}.", ConvertTime(playerData[playerid][player_antySpam], TIME_TYPE_UNIX)), 0;
        if(CompareEx(text, playerData[playerid][player_lastmessage])) return msg(playerid, C_CORAL, "Nie mo¿esz {c}drugi raz{/c} wys³aæ tej samej wiadomoœci."), 0;

        playerData[playerid][player_antySpam] = gettime() + 2;
        format(playerData[playerid][player_lastmessage], 144, "%s", text);

        new chars[] = "§", count, temp[2], string[255];
        strcopy(string, text, strlen(text));

        for(new i, x = strlen(string); i != x; i++) {
            string[i] = tolower(string[i]);
            strmid(temp, string, 0 + i, 1 + i);

            if(strlen(temp)) {
                if(strfind(temp, chars, false) != -1) count ++;
                else format(chars, sizeof(chars), "%s", temp), count = 0;

                if(count > 15 && playerData[playerid][player_mutetime] < 1) { count = 0; return MutePlayer(playerid, -1, 3, "Flood"), 0; }
            }
        }
    }

    if(quizData[quiz_active] && !playerData[playerid][player_quizDone] && CompareEx(text, quizData[quiz_buffer], (quizData[quiz_type] == QUIZ_WORD) ? (2) : (0))) {        
        quizData[quiz_count] ++;

        new money = 0, time = gettime() - quizData[quiz_tick];
        switch(quizData[quiz_count]) {
            case 1: money = math_random(7000, 10000);
            case 2: money = math_random(5000, 7000);
            case 3: money = math_random(3000, 5000);
        }
        playerData[playerid][player_quizDone] = true;

        giveMoney(playerid, money);
        addLevelScore(playerid, math_random(10, 30));

        achievement(playerid, ACH_QUIZ);
        CheckQuest(playerid, DAILY_QUIZ);

        switch(quizData[quiz_type]) {
            case QUIZ_WORD: msgAll(C_ORANGE2, "Gracz {c}%s(%d) {/c}odgad³(a) jako %s wyraz w czasie {c}%s{/c}.", getNick(playerid), playerid, getPodiumText(quizData[quiz_count], "pierwszy", "drugi", "trzeci"), checkTime(time));
            case QUIZ_MATH: msgAll(C_ORANGE2, "Gracz {c}%s(%d) {/c}poda³(a) jako %s prawid³owy wynik w czasie {c}%s{/c}.", getNick(playerid), playerid, getPodiumText(quizData[quiz_count], "pierwszy", "drugi", "trzeci"), checkTime(time));
            case QUIZ_CODE: msgAll(C_ORANGE2, "Gracz {c}%s(%d) {/c}przepisa³(a) jako %s poprawnie kod w czasie {c}%s{/c}.", getNick(playerid), playerid, getPodiumText(quizData[quiz_count], "pierwszy", "drugi", "trzeci"), checkTime(time));
        }

        if(quizData[quiz_count] >= 3) StopQuiz(true);
        return 0;
    }

    if(Iter_Contains(Game[GAME_ZP], playerid) && pGame[playerid] == GAME_ZP && GameStarted[GAME_ZP] && CompareEx(text, VehicleNames[VehicleModelZP - 400], 2)) {
        StatusZP[playerid] ++;

        new modelid = VehicleModelZP;
        if(StatusZP[playerid] >= MaxVehicleZP) { 
            foreach(new i : Game[GAME_ZP]) {
                if(i != playerid) CheckBet(i, BET_GAME, false);
                msg(i, C_CHAT, "Gracz {c}%s(%d){/c} odgad³(a) ostatni¹ nazwê pojazdu {c}\"%s\"{/c} i wygrywa zabawê!", getNick(playerid), playerid, VehicleNames[modelid - 400]); 
            }

            achievement(playerid, ACH_ZP);
            CheckBet(playerid, BET_GAME, true);

            new money = math_random(30000, 50000);
            giveMoney(playerid, money);
            showGameDraw(playerid, form("~y~ODGADLES(AS) WSZYSTKIE ~r~%d ~y~POJAZDOW!~n~~y~NAGRODA: ~r~%s$", MaxVehicleZP,  strdot(money)));
            return StopZP(playerid), 0;
        } else { 
            foreach(new i : Game[GAME_ZP]) msg(i, C_CHAT, "Gracz {c}%s(%d){/c} odgad³(a) %d nazwê pojazdu - {c}%s{/c}.", getNick(playerid), playerid, StatusZP[playerid], VehicleNames[modelid - 400]); 
            giveMoney(playerid, 100);

            if(IsValidVehicle(VehicleZP)) DestroyVehicle(VehicleZP);
            VehicleModelZP = -1;
            
            GameCountDown[GAME_ZP] = 10;
            SwitchZP = ZP_VEHICLE_WAIT;
            return UpdateZP(), 0;
        }
    }

	new hour, minute; getdate(hour, minute);
	if(hour == 21 && minute == 37 && GetPVarInt(playerid, "player_papieska") == 0 && papieskaCount < 3 && (CompareEx(text, "2137") || CompareEx(text, "21:37"))) {
		SetPVarInt(playerid, "player_papieska", 1);
		papieskaCount ++;

		msgAll(C_PAPIEZ, "Gracz {c}%s(%d){/c} napisa³(a) jako %s papie¿ow¹ godzinê!", getNick(playerid), playerid, getPodiumText(papieskaCount, "pierwszy(a)", "drugi(a)", "trzeci(a)"));
		switch(random(2)) {
			case 0: {
				giveMoney(playerid, 30000);
				msg(playerid, C_PAPIEZ, "Jako nagrodê otrzymujesz: {c}30.000${/c}.");
			}
			case 1: {
				givePremium(playerid, 1);
				msg(playerid, C_PAPIEZ, "Jako nagrodê otrzymujesz: {c}jednodniowe konto "YELLOW"Premium{/c}.");
			}
		}
        return 0;
	}

    if(text[0] == 'a' && text[1] == '!' && playerData[playerid][player_admin] > GRACZ && Iter_Contains(Permission, playerid)) {
        foreach(new i : Permission) msg(i, C_RED, "(A) {c}%s(%d){/c}: %s", getNick(playerid), playerid, text[2]);
        return Log(LOG_CHAT, "[a-chat]: %s (%s, %d'): %s", getNick(playerid), getRankName(playerData[playerid][player_admin]), playerid, text[2]), 0;
    }

    if(text[0] == '!' && pGang[playerid] != -1) {
        new gangid = pGang[playerid];
        foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "(%s)*  {c}%s(%d){/c}: %s", getGangTag(gangid), getNick(playerid), playerid, text[1]);
        return Log(LOG_CHAT, "[%s]: %s(%d): %s", getGangTag(gangid), getNick(playerid), playerid, text[1]), 0;
    }

    new string[256];
    if(playerData[playerid][player_admin] > GRACZ) format(string, sizeof(string), ""WHITE"%d' {%06x}%s {%06x}(%s)"WHITE": %s", playerid, GetPlayerColor(playerid) >>> 8, getNick(playerid), getRankColor(playerData[playerid][player_admin]) >>> 8, getRankName(playerData[playerid][player_admin]), text);
    else {
        if(!IsPremium(playerid)) {
            if(strmatch(getNick(playerid), "ES", true)) format(string, sizeof(string), ""WHITE"%d' {%06x}%s "GOLD"(Elitowicz)"WHITE": %s", playerid, GetPlayerColor(playerid) >>> 8, getNick(playerid), text);
            else if(Przegral(playerid) >= 50) format(string, sizeof(string), ""WHITE"%d' {%06x}%s "GOLD"(Sta³y gracz)"WHITE": %s", playerid, GetPlayerColor(playerid) >>> 8, getNick(playerid), text);
            else format(string, sizeof(string), ""WHITE"%d' {%06x}%s "GREY"(Gracz)"WHITE": %s", playerid, GetPlayerColor(playerid) >>> 8, getNick(playerid), text);
        } else {
            if(pOwnRankHave[playerid]) format(string, sizeof(string), ""WHITE"%d' {%06x}%s {%06x}(%s)"WHITE": %s", playerid, GetPlayerColor(playerid) >>> 8, getNick(playerid), pOwnRankColor[playerid] >>> 8, pOwnRank[playerid], text);
            else format(string, sizeof(string), ""WHITE"%d' {%06x}%s "YELLOW"(Premium)"WHITE": %s", playerid, GetPlayerColor(playerid) >>> 8, getNick(playerid), text);
        }
    }
    Log(LOG_CHAT, "[chat]: %s (%s, %d'): %s", getNick(playerid), getRankName(playerData[playerid][player_admin]), playerid, text);

    getMentioned(playerid, string, sizeof(string));
    switch(playerData[playerid][player_chatID]) {
        case CHAT_TYPE_GLOBAL: SendGlobalMessage(-1, string);
        case CHAT_TYPE_LOCAL: {
            format(string, sizeof(string), "{AFAFAF}[local]: {858585}%s'%d: {AFAFAF}%s", getNick(playerid), playerid, text);
            SendLocalMessage(playerid, 0xAFAFAFFF, string); 
        }

        case CHAT_TYPE_GANGS: {
            new gangid = pGang[playerid];
            format(string, sizeof(string), "(%s)*  {c}%s(%d){/c}: %s", getGangTag(gangid), getNick(playerid), playerid, text);            
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), string);
        }
    }

    messages ++;
    return 0;
}

public OnPlayerCommandPerformed(playerid, cmd[], params[], result, flags)
{
    if(!playerData[playerid][player_logged]) return 0;

    if(result == -1) {
        new teleportid = CheckPlayerTeleport(cmd);
        if(teleportid != -1) Teleport(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), teleportData[teleportid][teleport_x], teleportData[teleportid][teleport_y], teleportData[teleportid][teleport_z], teleportData[teleportid][teleport_a], teleportData[teleportid][teleport_interior], 0);
        else msg(playerid, C_RED, "Nie znaleziono komendy {c}/%s{/c}. SprawdŸ: {c}/help{/c}.", cmd);
    } else { 
        foreach(new i : Permission) if(playerData[i][player_checkCMD]) msg(i, C_LBLUE, "[cmd]: "CORAL"%s(%d): "LRED"/%s %s", getNick(playerid), playerid, cmd, params); 
        Log(LOG_CMD, "[cmd]: %s(%d): /%s %s", getNick(playerid), playerid, cmd, params);
    }
    return 1;
}

public OnPlayerCommandReceived(playerid, cmd[], params[], flags)
{
    if(!playerData[playerid][player_logged] || CheckPlayerBattle(playerid) || GetPlayerState(playerid) == PLAYER_STATE_WASTED) return 0;
    if(flags && playerData[playerid][player_admin] < flags) return dialogBox(playerid, C_ORANGE, ""ORANGE"Komenda "WHITE"/%s "ORANGE"jest dostêpna od rangi: {%06x}%s"WHITE".", cmd, getRankColor(flags) >>> 8, getRankName(flags)), 0;

    AllowEventCMD(playerid, cmd);

    if(strmatch(cmd, "exit", true) || strmatch(cmd, "return", true) || strmatch(cmd, "leave", true) || strmatch(cmd, "losexp", true)) return 1;
    if(strmatch(cmd, "skin", true) || strmatch(cmd, "skiny", true) || strmatch(cmd, "skins", true) || strmatch(cmd, "losresp", true) || strmatch(cmd, "biznes", true)) return 1;
    if(strmatch(cmd, "naj", true) || strmatch(cmd, "best", true) || strmatch(cmd, "pm", true) || strmatch(cmd, "re", true) || strmatch(cmd, "msg", true)) return 1;
    if(strmatch(cmd, "w", true) || strmatch(cmd, "pw", true)) return 1;

    if(playerData[playerid][player_admin] >= MODERATOR) {
        if(strmatch(cmd, "mute", true) || strmatch(cmd, "kick", true) || strmatch(cmd, "ban", true)) return 1;
        if(strmatch(cmd, "offban", true) || strmatch(cmd, "delmute", true) || strmatch(cmd, "unmute", true)) return 1;
        if(strmatch(cmd, "warn", true) || strmatch(cmd, "delwarn", true) || strmatch(cmd, "unwarn", true)) return 1;
    }

    if(playerData[playerid][player_business] != -1) {
        if(strmatch(cmd, "shoot", true)) return 1;
        return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend, bêd¹c w œrodku lokalu biznesowego!"), 0; // to do
    }

    if(pAFK[playerid]) {
        if(strmatch(cmd, "jj", true)) return 1;
        return dialogBox(playerid, C_RED, "Jesteœ {c}AFK{/c}, wiêc nie mo¿esz u¿ywaæ komend!\n"RED"Aby wróciæ do gry, wpisz: {c}/jj{/c}."), 0;
    }

    if(MineStarted[playerid]) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend, gdy wydobywasz surowiec!"), 0;
    if(Iter_Contains(Event, playerid) && EventStarted) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend, bêd¹c na wydarzeniu! Wyj¹tek: {c}/exit{/c}."), 0;
    if(GetPlayerVirtualWorld(playerid) == VW_HEIST) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend, bêd¹c w skarbcu!"), 0;
    if(playerData[playerid][player_jailtime] > 0) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend, bêd¹c we wiêzieniu! Wyjdziesz za: {c}%s{/c}.", ConvertTime(playerData[playerid][player_jailtime], TIME_TYPE_MS)), 0;
    if(pGang[playerid] != -1 && Iter_Contains(Sparing, playerid) && pGangSpar[pGang[playerid]]) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend na sparingu. Wyj¹tek: {c}/exit{/c}."), 0;
    if(pDuel[playerid] && pDuelEnemy[playerid] != -1) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend na duelu. Wyj¹tek: {c}/exit{/c}."), 0;
    if(pJob[playerid] != -1) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend w pracy."), 0;
    if(pTruck[playerid]) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend, gdy jesteœ na spedycji. Wyj¹tek: {c}/exit{/c}"), 0;
    if(pArena[playerid] != -1 || pGlitchArena[playerid]) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend na arenie. Wyj¹tek: {c}/exit{/c}."), 0;  
    if(pGame[playerid] != -1 && Iter_Contains(Game[pGame[playerid]], playerid) && GameStarted[pGame[playerid]]) return msg(playerid, C_DRED, "Nie mo¿esz u¿ywaæ komend na zabawie. Wyj¹tek: {c}/exit{/c}."), 0;
    return 1;
}

public OnPlayerUpdate(playerid)
{
    new drunkLevel = GetPlayerDrunkLevel(playerid);
    if(drunkLevel < 100) SetPlayerDrunkLevel(playerid, 2000);
    else {
        if(playerData[playerid][player_drunk] != drunkLevel) {
            new fps = playerData[playerid][player_drunk] - drunkLevel;
            if(fps > 0 && fps < 400) playerData[playerid][player_fps] = fps;
            playerData[playerid][player_drunk] = drunkLevel;
        }
    }

    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER && IsPlayerInAnyVehicle(playerid)) {
        new vehicleid = GetPlayerVehicleID(playerid), Float:health;
        GetVehicleHealth(vehicleid, health);

        health = (health - 250.0) / 7.5;
        if(health > 100) health = 100;

        PlayerTextDrawSetString(playerid, tdLicznik[2], form("HP: ~w~~h~%.0f", health));
        PlayerTextDrawSetString(playerid, tdLicznik[3], form("KM: ~w~~h~%.0f", GetVehSpeed(vehicleid))); 
    }
    if(playerData[playerid][player_god] && IsPlayerInAnyVehicle(playerid)) RepairVehicle(GetPlayerVehicleID(playerid));

    new keys, updown, leftright;
    GetPlayerKeys(playerid, keys, updown, leftright);
    
    if(pVehEdit[playerid]) UpdateEditorAttachment(playerid, keys, updown, leftright);
    return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
    switch(newstate) {
        case PLAYER_STATE_DRIVER: {
            for(new i = 0; i < 4; i++) togglePlayerTextDraw(tdLicznik[i], playerid, true);
        
            new vehicleid = GetPlayerVehicleID(playerid);
            PlayerTextDrawSetString(playerid, tdLicznik[1], form("%s", VehicleNames[GetVehicleModel(vehicleid) - 400]));

            if(VehiclePrivate[vehicleid]) {
                new privcarid = VehiclePrivateID[vehicleid], keys = FindKeyPlayer(privcarid, playerid);
                if(!pVehSell[privcarid]) {
                    pVehID[playerid] = privcarid;
                    if(vehicleid == pVeh[privcarid] && (pVehOwner[privcarid] != playerData[playerid][player_accountID] && !keys)) {
                        ClearAnimations(playerid);
                        pVehID[playerid] = -1;

                        s[0] = EOS;
                        format(s, sizeof(s), ""WHITE"Ten pojazd nale¿y do: "BROWN"%s"WHITE".\n"WHITE"Nie mo¿esz do niego wsi¹œæ.\n"LY"Poproœ w³aœciciela, aby da³(a) Ci klucze!", pVehOwnerName[privcarid]);
                        return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Prywatny pojazd", s, "OK", #), 1;
                    } 
                } else {
                    ClearAnimations(playerid);

                    s[0] = EOS;
                    catstr(s, C_GITOWY, -1, "Ten pojazd zosta³ {c}wystawiony{/c} na sprzeda¿.\n");
                    catstr(s, C_GITOWY, -1, "Koszt pojazdu wynosi: {c}%s${/c}.\n\n", strdot(pVehSellCost[privcarid]));
                    catstr(s, C_LY, -1, "Aby odkupiæ ten pojazd, wpisz: {c}/pbuy %d{/c}.", pVehSellPoint[privcarid] + 1);
                    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""CORAL"Prywatny pojazd", s, "OK", #), 1;
                }
            }
        }

        case PLAYER_STATE_ONFOOT: {
            for(new i = 0; i < 4; i++) togglePlayerTextDraw(tdLicznik[i], playerid, false);
        }
    }
    return SetTimerEx("RefreshSpec", 100, false, "d", playerid), 1;
}

public OnPlayerDamage(&playerid, &Float:amount, &issuerid, &weapon, &bodypart)
{
    if(issuerid != INVALID_PLAYER_ID) {
        if(
            playerData[playerid][player_god] || playerData[issuerid][player_god] || 
            GetPlayerVirtualWorld(issuerid) == VW_BANK || GetPlayerVirtualWorld(playerid) == VW_BANK ||
            GetPlayerVirtualWorld(issuerid) == VW_HEIST || GetPlayerVirtualWorld(playerid) == VW_HEIST ||
            (pGame[playerid] == GAME_CH && GameStarted[GAME_CH] && Iter_Contains(Game[GAME_CH], playerid) && playerid == ChowanySzukajacy) ||
            (pGame[playerid] == GAME_ZP && GameStarted[GAME_ZP] && Iter_Contains(Game[GAME_ZP], playerid)) || 
            playerData[playerid][player_jailtime] > 0 || playerData[issuerid][player_jailtime] > 0 || pNoDM[playerid] || pNoDM[issuerid] || pAFK[playerid] || pAFK[issuerid] ||
            ((Iter_Contains(Event, playerid) || Iter_Contains(Event, issuerid)) && EventStarted && (EventType == EVENT_TYPE_NODM || EventType == EVENT_TYPE_NONE))
        ) {
            return 0;
        }
        if(!GetPlayerVirtualWorld(playerid) && playerData[playerid][player_admin] < MODERATOR) playerData[playerid][player_battleTimer] = gettime() + 10;

        if(pGame[issuerid] == GAME_DA && Iter_Contains(Game[GAME_DA], issuerid) && GameStarted[GAME_DA] && pGame[issuerid] == pGame[playerid]) {
            deathArenaKills[issuerid][da_id] = issuerid;
            deathArenaKills[issuerid][da_damage] += amount;
        }

        if(pDuel[playerid] && pDuel[issuerid] && pDuelEnemy[playerid] == issuerid && pDuelSync[playerid] && pDuelSync[issuerid]) amount = amount - 15.0;
        if(pGang[playerid] != -1 && pGang[issuerid] != -1) {
            new gangid_playerid = pGang[playerid], gangid_issuerid = pGang[issuerid], zoneid = pGangZoneArea[playerid];
            if(pGangZoneArea[playerid] != -1 && pGangZoneArea[issuerid] != -1 && IsPlayerInDynamicArea(playerid, pGangZoneRectangle[zoneid]) && IsPlayerInDynamicArea(issuerid, pGangZoneRectangle[zoneid]) && pGangWarTime[zoneid] > 0 && gangData[gangid_playerid][gang_battleZone] == zoneid && gangData[gangid_issuerid][gang_battleZone] == zoneid) {
                pGangWarDamage[zoneid][(pGang[issuerid] == pGangWarAttacker[zoneid]) ? (0) : (1)] += amount;
                playerData[issuerid][player_zone_damage] += amount;
            }

            if(Iter_Contains(Sparing, playerid) && Iter_Contains(Sparing, issuerid) && pGangSpar[gangid_playerid] && pGangSpar[gangid_issuerid]) {
                if(pGang[playerid] == pGang[issuerid]) return 0;

                new initiator = (pGangSparInitiator[gangid_playerid] == gangid_issuerid) ? (gangid_issuerid) : (gangid_playerid);
                new rivality = (pGangSparInitiator[gangid_playerid] == gangid_issuerid) ? (gangid_playerid) : (gangid_issuerid);

                if(pGangSparSettings[initiator][rivality][0] && weapon == WEAPON_DEAGLE) amount = amount - 15.0; 
                if(pGangSparSettings[initiator][rivality][1]) { if(bodypart == 9 && weapon == 34) amount = amount + 200.0; }

                sparingKillerData[issuerid][sparing_playerid] = issuerid;
                sparingKillerData[issuerid][sparing_damage] += amount;
            }
        }
    }
    return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source) return task_await(RunPlayerStats(playerid, clickedplayerid)), 1;
public OnPlayerPickUpDynamicPickup(playerid, pickupid)
{
    if(playerData[playerid][player_pickupTimer] > 0 && (pGame[playerid] != GAME_BTR && !GameStarted[GAME_BTR])) return 1;
    playerData[playerid][player_pickupTimer] = 2;

    Magazynier_Pickup(playerid, pickupid);
    RoyalePickup(playerid, pickupid);
    CollectDisk(playerid, pickupid);
    CollectFigure(playerid, pickupid);
    Bank_Pickup(playerid, pickupid);
    WordPickup(playerid, pickupid);
    return 1;
}

function Vacex_Bonus(playerid)
{
    new money = math_random(10000, 30000);
    msg(playerid, C_CORAL, "Otrzymujesz {c}%s${/c} dodatkowej nagrody za oszczanie pomnika {c}carbonary{/c}.", strdot(money));
    m_pquery_format("UPDATE es_accounts SET bonus_vcxz=1 WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID]);

    ClearAnimations(playerid);
    return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE), 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_CTRL_BACK) && IsPlayerInRangeOfPoint(playerid, 3.0, 2164.00, 1073.19, 12.45) && !playerData[playerid][player_bonus_vcxz]) {
        playerData[playerid][player_bonus_vcxz] = true;
        ApplyAnimation(playerid, "PAULNMAC", "Piss_in", 4.0, false, false, false, false, 0);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_PISSING);
        return SetTimerEx("Vacex_Bonus", 1000 * math_random(3, 5), false, "d", playerid);
    }
    if(PRESSED(KEY_CTRL_BACK) && pSpectate[playerid] && playerData[playerid][player_sparspec]) return ShowSparingPanel(playerid, playerData[playerid][player_sparingGangSpec]);

    if(PRESSED(KEY_SUBMISSION) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
        if(pJob[playerid] != -1 && !pJobUlepszenia[playerid][JOB_FIX]) return 1;
        return callcmd::napraw(playerid), 1;
    }
    if(PRESSED(KEY_YES) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && !PlayerFueling[playerid]) return callcmd::flip(playerid), 1;
    
    if(HOLDING(KEY_FIRE) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
        if(pJob[playerid] != -1 && !pJobUlepszenia[playerid][JOB_NITRO]) return 1;
        return AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
    }
    if(RELEASED(KEY_FIRE) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) return RemoveVehicleComponent(GetPlayerVehicleID(playerid), 1010);
    return 1;
}

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    Magazynier_CP(playerid, checkpointid);
    return 1;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
    new i = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);

    new zoneid = i - ZONE_MAGIC;
    if(0 <= zoneid < Iter_Count(GangZones) + 1) {
        pGangZoneArea[playerid] = zoneid;
        if(pGangWarTime[zoneid] > 0) {
            if(pGang[i] == pGangWarAttacker[zoneid] || pGang[i] == pGangWarDefender[zoneid]) {
                for(new x = 0; x < 8; x++) TextDrawShowForPlayer(playerid, pGangZoneDraw[zoneid][x]); 
            }
        }
    }

    new nodm = i - NODM_MAGIC;
    if(0 <= nodm < sizeof(zonesNoDM) + 1) {
        pNoDM[playerid] = true;
        TextDrawShowForPlayer(playerid, tdNoDM);
    }
    return 1;
}

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
    if(pGangZoneArea[playerid] != -1) {
        new zoneid = pGangZoneArea[playerid];
        for(new x = 0; x < 8; x++) TextDrawHideForPlayer(playerid, pGangZoneDraw[zoneid][x]);
        pGangZoneArea[playerid] = -1;
    }

    if(pNoDM[playerid]) {
        pNoDM[playerid] = false;
        TextDrawHideForPlayer(playerid, tdNoDM);
    }
    return 1;
}
