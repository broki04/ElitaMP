enum {
    GAME_CH = 0,
    GAME_ZP = 1,
    GAME_DA,
    GAME_RC,
    GAME_BTR,
    GAME_SM,
    GAME_GG,
    GAME_WP,
    GAME_SK,
    GAME_WG,
};
#define GAME_START_TIME     20

#define SUMO_ONE_PLAYER     0
#define SUMO_TIME_OUT       1

#define SK_CANCEL_GAME      0
#define SK_WINNER_FIRST     1

new
    gameName[][] = {"Chowany", "Zgadnij pojazd", "Death Area", "Wyscig", "Battle Royale", "Sumo", "Gun Game", "Wipeout", "Sianko", "War Game"},
    gameCMD[][] = {"/ch", "/zp", "/da", "/rc", "/btr", "/sm", "/gg", "/wp", "/sk", "/wg"},

    bool:GameStarted[sizeof(gameName)],
    GameTimer[sizeof(gameName)],
    GameCountDown[sizeof(gameName)],
    GameDrawShowed,

    pGame[MAX_PLAYERS],
    Iterator:Game[sizeof(gameName)]<MAX_PLAYERS>
;

#define GAME_STOP_TIMER         0
#define GAME_STOP_PLAYERS       1
#define GAME_STOP_NULL          2

stock Init_Games()
{
    GameDrawShowed = -1;
    for(new i = 0; i < sizeof(gameName); i++) Iter_Init(Game[i]);
    UpdateGameDraw();
}
 
stock RefreshGameList()
{
    new gamestr[256];
    for(new i = 0; i < sizeof(gameName); i++) format(gamestr, sizeof(gamestr), "%s%s~n~", gamestr, gameCMD[i]);
    return gamestr;
}

stock GetGameLimit(gameid)
{
    new limit = 0;
    switch(gameid) {
        case GAME_CH:   limit = 3;
        case GAME_ZP:   limit = 3;
        case GAME_DA:   limit = 2;
        case GAME_RC:   limit = 1;
        case GAME_BTR:  limit = 2;
        case GAME_SM:   limit = 3;
        case GAME_GG:   limit = 2;
        case GAME_WP:   limit = 1;
        case GAME_SK:   limit = 2;
        case GAME_WG:   limit = 2;
    }
    return limit;
}

stock ResetGameData(playerid)
{
    for(new i = 0; i < sizeof(gameName); i++) Iter_Remove(Game[i], playerid);
    pGame[playerid] = -1;
}

stock UpdateGameDraw()
{
    new gamestr[256];
    for(new gameid = 0; gameid < sizeof(gameName); gameid++) {
        if(GameStarted[gameid]) format(gamestr, sizeof(gamestr), "%s~r~~h~(---)~n~", gamestr);
        else if(!GameStarted[gameid] && GameCountDown[gameid] > 0) format(gamestr, sizeof(gamestr), "%s~y~~h~(%d sek)~n~", gamestr, GameCountDown[gameid]);
        else format(gamestr, sizeof(gamestr), "%s~w~~h~(%d/%d)~n~", gamestr, Iter_Count(Game[gameid]), GetGameLimit(gameid));
    }
    return TextDrawSetString(tdGameLimit, gamestr), 1;
}

stock AddPlayerGame(playerid, gameid)
{
    if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");
    if(GameStarted[gameid]) return sendError(playerid, ""RED"Zabawa {c}%s{/c} wystartowa³a!", gameName[gameid]);
    if(gameid == GAME_SM && !GameStarted[GAME_SM] && GameCountDown[GAME_SM] > 0 && Iter_Count(Game[GAME_SM]) >= sumoVehicleCount) return sendError(playerid, ""RED"Wylosowana arena na {c}%s (%s){/c} mo¿e pomieœciæ maksymalnie {c}%d{/c} osób!", gameName[GAME_SM], gameCMD[GAME_SM], sumoVehicleCount);

    pGame[playerid] = gameid;
    Iter_Add(Game[gameid], playerid);
    
    switch(gameid) {
        case GAME_CH:   checkRecord(RECORD_CH, Iter_Count(Game[gameid]));
        case GAME_ZP:   checkRecord(RECORD_ZP, Iter_Count(Game[gameid]));
        case GAME_DA:   checkRecord(RECORD_DA, Iter_Count(Game[gameid]));
        case GAME_RC:   checkRecord(RECORD_RC, Iter_Count(Game[gameid]));
        case GAME_BTR:  checkRecord(RECORD_BTR, Iter_Count(Game[gameid]));
        case GAME_GG:   checkRecord(RECORD_GG, Iter_Count(Game[gameid]));
        case GAME_WP:   checkRecord(RECORD_WP, Iter_Count(Game[gameid]));
        case GAME_SM:   checkRecord(RECORD_SM, Iter_Count(Game[gameid]));
        case GAME_SK:   checkRecord(RECORD_SK, Iter_Count(Game[gameid]));
        case GAME_WG:   checkRecord(RECORD_WG, Iter_Count(Game[gameid]));
    }

    if(Iter_Count(Game[gameid]) == GetGameLimit(gameid) && GameCountDown[gameid] < 1) {
        GameCountDown[gameid] = GAME_START_TIME;
        GameTimer[gameid] = SetTimerEx("ZapisyGameTimer", 1000, true, "d", gameid);
        GameStarted[gameid] = false;

        if(gameid == GAME_SM) {
            sumoArenaID = random(3);
            switch(sumoArenaID) {
                case 0: sumoVehicleCount = 40;
                case 1: sumoVehicleCount = 20;
                case 2: sumoVehicleCount = 16;
            }
        }

        if(GameDrawShowed == -1) {
            textDrawSetString(tdGameZapisy[1], "~w~~h~Startuje zabawa: ~y~~h~%s (%s)~w~~h~.~n~~w~~h~Zapisy wciaz trwaja - ~r~~h~30 sekund~w~~h~.~n~~w~~h~Zapisanych osob: ~b~~h~~h~0 osob~w~~h~.", gameName[gameid], gameCMD[gameid]);
            for(new i = 0; i < 3; i++) TextDrawShowForAll(tdGameZapisy[i]);
            GameDrawShowed = gameid;
        }
        msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} wystartuje za: {c}%d {/c}sekund!", gameName[gameid], gameCMD[gameid], GAME_START_TIME);
    }

    msg(playerid, C_CHAT, "Zapisa³eœ(aœ) siê na: {c}%s (%s){/c}.", gameName[gameid], gameCMD[gameid]);
    return UpdateGameDraw(), 1;
}

function ZapisyGameTimer(gameid)
{
    GameCountDown[gameid] --;
    if(GameCountDown[gameid] < 1 && !GameStarted[gameid]) {
        if(Iter_Count(Game[gameid]) < GetGameLimit(gameid)) {
            GameStarted[gameid] = false;
            GameCountDown[gameid] = 0;
            GameDrawShowed = -1;

            for(new i = 0; i < 3; i++) TextDrawHideForAll(tdGameZapisy[i]);
            KillTimer(GameTimer[gameid]);

            foreach(new i : Game[gameid]) pGame[i] = -1;
            Iter_Clear(Game[gameid]);
            UpdateGameDraw();
            return msgAll(C_CHAT, "Zabawa {c}%s (%s){/c} nie wystartowa³a, bo zapisa³o siê za ma³o osób!", gameName[gameid], gameCMD[gameid]), 1;
        }

        GameCountDown[gameid] = 0;
        GameStarted[gameid] = true;
        GameDrawShowed = -1;

        for(new i = 0; i < 3; i++) TextDrawHideForAll(tdGameZapisy[i]);
        KillTimer(GameTimer[gameid]);

        switch(gameid) {
            case GAME_CH: StartChowany();
            case GAME_ZP: StartZP();
            case GAME_DA: StartDA();
            case GAME_RC: m_pquery("SELECT * FROM es_fun_rc ORDER BY RAND() LIMIT 1;", "StartRC");
            case GAME_BTR: StartBTR();
            case GAME_SM: StartSM();
            case GAME_GG: StartGG();
            case GAME_WP: StartWP();
            case GAME_SK: StartSK();
            case GAME_WG: m_pquery("SELECT * FROM es_fun_wg ORDER BY RAND() LIMIT 1;", "StartWG");
        }
        GameTimer[gameid] = SetTimerEx("GameStartedTimer", 1000, true, "d", gameid);
    }

    if(GameDrawShowed == gameid) textDrawSetString(tdGameZapisy[1], "~w~~h~Startuje zabawa: ~y~~h~%s (%s)~w~~h~.~n~~w~~h~Zapisy wciaz trwaja - ~r~~h~%d %s~w~~h~.~n~~w~~h~Zapisanych osob: ~b~~h~~h~%d %s~w~~h~.", gameName[gameid], gameCMD[gameid], GameCountDown[gameid], ConvertTimeFormat(GameCountDown[gameid], "sekund", "sekunda", "sekundy", "sekund"), Iter_Count(Game[gameid]), ConvertTimeFormat(Iter_Count(Game[gameid]), "osob", "osoba", "osoby", "osob"));
    return UpdateGameDraw(), 1;
}

function GameStartedTimer(gameid)
{
    if(GameCountDown[gameid] > 0) {
        GameCountDown[gameid] --;
        if(GameCountDown[gameid] < 1 && GameStarted[gameid]) {        
            switch(gameid) {
                case GAME_CH: StopChowany(GAME_STOP_TIMER);
                case GAME_ZP: CreateVehicleZP();
                case GAME_DA: StopDA(true);
                case GAME_RC: StopRC();
                case GAME_SM: StopSM(SUMO_TIME_OUT);
                case GAME_SK: StopSK(INVALID_PLAYER_ID);
                case GAME_WG: StopWG(false);
            }
        }
    }

    switch(gameid) {
        case GAME_CH: UpdateChowany();
        case GAME_ZP: UpdateZP();
        case GAME_DA: UpdateDA();
        case GAME_RC: UpdateRC();
        case GAME_BTR: UpdateBTR();
        case GAME_SM: UpdateSM();
        case GAME_GG: UpdateGG();
        case GAME_WP: UpdateWP();
        case GAME_SK: UpdateSK();
        case GAME_WG: UpdateWG();
    }
    return 1;
}

// 

CMD:ch(playerid) return AddPlayerGame(playerid, GAME_CH);
CMD:zp(playerid) return AddPlayerGame(playerid, GAME_ZP);
CMD:da(playerid) return AddPlayerGame(playerid, GAME_DA);
CMD:rc(playerid) return AddPlayerGame(playerid, GAME_RC);
CMD:btr(playerid) return AddPlayerGame(playerid, GAME_BTR);
CMD:sm(playerid) return AddPlayerGame(playerid, GAME_SM);
CMD:gg(playerid) return AddPlayerGame(playerid, GAME_GG);
CMD:wp(playerid) return AddPlayerGame(playerid, GAME_WP);
CMD:sk(playerid) return AddPlayerGame(playerid, GAME_SK);
CMD:wg(playerid) return AddPlayerGame(playerid, GAME_WG);

alias:rc("ws")
alias:ch("hns")