#include YSI_Coding\y_hooks

enum e_RaceStatusData {
    race_id,
    race_status
};

new 
    RaceVehicle[MAX_PLAYERS], 
    RacePlacement[MAX_PLAYERS],
    RaceStatusData[MAX_PLAYERS][e_RaceStatusData],
    RaceCheckpoint[MAX_PLAYERS],
    RaceMapIcon[MAX_PLAYERS],
    RaceVehicleBack[MAX_PLAYERS],
    
    RaceArenaName[50],
    Float:RaceMatchCP[MAX_RACE_CP][3],
    RaceArenaID,
    RaceVehicleModel,
    Float:RaceVehicleSpawn[4],
    RaceCountCPs,
    RaceRecordID,
    RaceRecordTime,
    RaceTimer,
    RaceMeta
;

function StartRC()
{
    if(!GameStarted[GAME_RC]) return 1;
    GameStarted[GAME_RC] = true;
    GameCountDown[GAME_RC] = 200 + 5;

    RaceMeta = RaceTimer = 0;
    if(cache_num_rows() > 0) {
        cache_get_value_name_int(0, "id", RaceArenaID);
        cache_get_value_name(0, "name", RaceArenaName, sizeof(RaceArenaName));
        cache_get_value_name_int(0, "modelid", RaceVehicleModel);
        cache_get_value_name_int(0, "checkpoints", RaceCountCPs);
        cache_get_value_name_int(0, "record_id", RaceRecordID);
        cache_get_value_name_int(0, "record_score", RaceRecordTime);

        new buffer[90];
        cache_get_value_name(0, "position", buffer);
        sscanf(buffer, "p<,>a<f>[4]", RaceVehicleSpawn);

        inline onLoadRaceCP()
        {
            msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartowa³a! "LY"(%s, %d %s)", gameName[GAME_RC], gameCMD[GAME_RC], RaceArenaName, Iter_Count(Game[GAME_RC]), ConvertTimeFormat(Iter_Count(Game[GAME_RC]), "osób", "osoba", "osoby", "osób"));
            for(new i = 0; i < cache_num_rows(); i++) {
                cache_get_value_name_float(i, "x", RaceMatchCP[i][0]);
                cache_get_value_name_float(i, "y", RaceMatchCP[i][1]);
                cache_get_value_name_float(i, "z", RaceMatchCP[i][2]);

                printf(" >>> [RC-%d]: Dodano checkpoint #%d - (%.2f, %.2f, %.2f)", RaceArenaID, i + 1, unpackXYZ(RaceMatchCP[i]));
            }

            foreach(new i : Game[GAME_RC]) {
                SavePlayerPosition(i);
                ResetPlayerWeapons(i);
                DisableRemoteVehicleCollisions(i, true);

                SetPlayerHealth(i, 100.0);
                SetPlayerArmour(i, 0.0);
                SetPlayerVirtualWorld(i, VW_RC);

                FreezePlayer(i, 5);
                setOdliczanie(i, 5);
                SetTimer("ResetRaceTimer", 1000 * 5, false);

                RaceStatusData[i][race_id] = i;
                RaceStatusData[i][race_status] = RacePlacement[i] = RaceVehicleBack[i] = 0;

                UpdateGameStatus(i, GAME_RC);
                for(new x = 0; x < 3; x++) PlayerTextDrawShow(i, tdInfo[x]);

                RaceVehicle[i] = CreateVehicle(RaceVehicleModel, unpackXYZR(RaceVehicleSpawn), -1, -1, -1);
                SetVehicleNumberPlate(RaceVehicle[i], ""RED"WYŒCIG");
                SetVehicleVirtualWorld(RaceVehicle[i], VW_RC);
                PutPlayerInVehicle(i, RaceVehicle[i], 0);

                if(IsValidDynamicRaceCP(RaceCheckpoint[i])) DestroyDynamicRaceCP(RaceCheckpoint[i]);
                if(IsValidDynamicMapIcon(RaceMapIcon[i])) DestroyDynamicMapIcon(RaceMapIcon[i]);

                RaceCheckpoint[i] = CreateDynamicRaceCP(0, unpackXYZ(RaceMatchCP[0]), unpackXYZ(RaceMatchCP[1]), 5.0, .worldid=VW_RC, .playerid=i);
                RaceMapIcon[i] = CreateDynamicMapIcon(unpackXYZ(RaceMatchCP[1]), 56, -1, .worldid=VW_RC, .playerid=i);

                if(RaceRecordID != -1) {
                    new Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", RaceRecordID));
                    if(cache_num_rows() > 0) {
                        new nick[25]; cache_get_value(0, 0, nick, sizeof(nick));
                        msg(i, C_CHAT, "Rekord trasy pobity przez {c}%s{/c}. "LY"(%s)", nick, checkTime(RaceRecordTime));
                    }
                    cache_delete(result);
                }
            }
        }
        m_tquery_inline(dbconn, using inline onLoadRaceCP, "SELECT * FROM es_fun_rc_cp WHERE arenaid=%d LIMIT %d;", RaceArenaID, RaceCountCPs);
    }
    return 1;  
}
function ResetRaceTimer(playerid) return RaceTimer = 0;

stock UpdateRC()
{
    if(!GameStarted[GAME_RC]) return 1;
    if(Iter_Count(Game[GAME_RC]) < 1) return StopRC();
    RaceTimer ++;

    new place = 0;
    SortDeepArray(RaceStatusData, race_status, .order=SORT_DESC);
    for(new i = 0; i < Iter_Count(Game[GAME_RC]); i++) {
        if(!IsPlayerConnected(RaceStatusData[i][race_id])) continue;
        new playerid = RaceStatusData[i][race_id];
        RacePlacement[playerid] = place ++;
    }

    new iterate_max;
    foreach(new i : Game[GAME_RC]) {
        if(RaceVehicleBack[i] > 0) {
            RaceVehicleBack[i] --;
            if(RaceVehicleBack[i] < 1) {
                if(IsValidVehicle(RaceVehicle[i])) DestroyVehicle(RaceVehicle[i]);
                if(IsValidDynamicRaceCP(RaceCheckpoint[i])) DestroyDynamicRaceCP(RaceCheckpoint[i]);
                if(IsValidDynamicMapIcon(RaceMapIcon[i])) DestroyDynamicMapIcon(RaceMapIcon[i]);

                pGame[i] = -1;
                Iter_SafeRemove(Game[GAME_RC], i, iterate_max);

                DisableRemoteVehicleCollisions(i, false);
                SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
                for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
                LoadPlayerPosition(i);

                msg(i, C_CHAT, "Nie wróci³eœ(aœ) na czas do {c}pojazdu{/c} - zosta³eœ(aœ) wyrzucony(a).");
            }
            showGameDraw(i, form("~y~MASZ ~r~%d ~y~%s, BY WROCIC DO POJAZDU!", RaceVehicleBack[i], ConvertTimeFormat(RaceVehicleBack[i], "SEKUND", "SEKUNDE", "SEKUNDY", "SEKUND")), 5);
        }
        UpdateGameStatus(i, GAME_RC);
    }
    return 1;
}

hook OnPlayerEnterDynRaceCP(playerid, checkpointid)
{
    if(pGame[playerid] == GAME_RC && Iter_Contains(Game[GAME_RC], playerid) && GameStarted[GAME_RC]) {
        if(IsValidDynamicRaceCP(RaceCheckpoint[playerid])) DestroyDynamicRaceCP(RaceCheckpoint[playerid]);
        if(IsValidDynamicMapIcon(RaceMapIcon[playerid])) DestroyDynamicMapIcon(RaceMapIcon[playerid]);
        
        RaceStatusData[playerid][race_status] ++;
        playSound(playerid, 1139);

        if(RaceStatusData[playerid][race_status] == RaceCountCPs) {
            new money = 0;

            RaceMeta ++;
            msgAll(C_BROWN, "Gracz {c}%s(%d){/c} dojecha³ #%d do mety! "LY"(%s)", getNick(playerid), playerid, RaceMeta, checkTime(RaceTimer));

            switch(RaceMeta) {
                case 1: { 
                    money = math_random(10000, 30000); 
                    CheckBet(playerid, BET_GAME, true);

                    if(RaceRecordTime > RaceTimer || RaceRecordID == -1) {
                        m_pquery_format("UPDATE es_fun_rc SET record_id=%d, record_score=%d WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID], RaceTimer, RaceArenaID);

                        msgAll(-1, " ");
                        msgAll(C_VIOLET, "Gracz {c}%s(%d){/c} pobi³(a) rekord trasy: {c}%s{/c}.", getNick(playerid), playerid, RaceArenaName);
                        msgAll(C_VIOLET, "Nowy rekord trasy wynosi: {c}%s{/c}.", checkTime(RaceTimer));
                        msgAll(-1, " ");

                        giveMoney(playerid, 10000);
                        msg(playerid, C_VIOLET, "Za pobicie rekordu trasy otrzymujesz: {c}10.000${/c}.");
                    }
                }
                case 2: money = math_random(5000, 10000); 
                case 3: money = math_random(3000, 5000); 
                default: money = math_random(1000, 3000); 
            }

            if(RaceMeta <= 3) achievement(playerid, ACH_RC);
            if(RaceMeta != 1) CheckBet(playerid, BET_GAME, false);
            giveMoney(playerid, money);

            if(IsValidVehicle(RaceVehicle[playerid])) DestroyVehicle(RaceVehicle[playerid]);
            if(IsValidDynamicRaceCP(RaceCheckpoint[playerid])) DestroyDynamicRaceCP(RaceCheckpoint[playerid]);
            if(IsValidDynamicMapIcon(RaceMapIcon[playerid])) DestroyDynamicMapIcon(RaceMapIcon[playerid]);

            DisableRemoteVehicleCollisions(playerid, false);
            SetPlayerColor(playerid, setAlpha(GetPlayerColor(playerid), 0xFF));
            for(new i = 0; i < 3; i++) PlayerTextDrawHide(playerid, tdInfo[i]);
            LoadPlayerPosition(playerid);

            pGame[playerid] = -1;
            return Iter_Remove(Game[GAME_RC], playerid);
        }

        new count = RaceStatusData[playerid][race_status];
        RaceCheckpoint[playerid] = CreateDynamicRaceCP((count == RaceCountCPs - 1) ? (1) : (0), unpackXYZ(RaceMatchCP[count]), unpackXYZ(RaceMatchCP[count + 1]), 5.0, .worldid=VW_RC, .playerid=playerid);
        RaceMapIcon[playerid] = CreateDynamicMapIcon(unpackXYZ(RaceMatchCP[count + 1]), 56, -1, .worldid=VW_RC, .playerid=playerid);
    }
    return 1;
}

stock StopRC()
{
    if(!GameStarted[GAME_RC]) return 1;
    GameStarted[GAME_RC] = false;
    GameCountDown[GAME_RC] = 0;

    foreach(new i : Game[GAME_RC]) {
        if(IsValidVehicle(RaceVehicle[i])) DestroyVehicle(RaceVehicle[i]);
        if(IsValidDynamicRaceCP(RaceCheckpoint[i])) DestroyDynamicRaceCP(RaceCheckpoint[i]);
        if(IsValidDynamicMapIcon(RaceMapIcon[i])) DestroyDynamicMapIcon(RaceMapIcon[i]);

        pGame[i] = -1;
        DisableRemoteVehicleCollisions(i, false);
        SetPlayerColor(i, setAlpha(GetPlayerColor(i), 0xFF));
        for(new x = 0; x < 3; x++) PlayerTextDrawHide(i, tdInfo[x]);
        LoadPlayerPosition(i);
    }
    Iter_Clear(Game[GAME_RC]);

    UpdateGameDraw();
    KillTimer(GameTimer[GAME_RC]);
    return msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} zakoñczona.", gameName[GAME_RC], gameCMD[GAME_RC]), 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(Iter_Contains(Game[GAME_RC], playerid) && pGame[playerid] == GAME_RC && GameStarted[GAME_RC] && RaceVehicle[playerid] == vehicleid && RaceVehicleBack[playerid] > 0) {
        RaceVehicleBack[playerid] = 0;
        showGameDraw(playerid, "_", 1); // XD
    }
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
    if(Iter_Contains(Game[GAME_RC], playerid) && pGame[playerid] == GAME_RC && GameStarted[GAME_RC]) {
        RaceVehicleBack[playerid] = 10;
        showGameDraw(playerid, "~y~MASZ ~r~10 ~y~SEKUND, BY WROCIC DO POJAZDU!", 5);
    }
    return 1;
}