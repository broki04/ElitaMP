#include YSI_Coding\y_hooks

enum {
    EVENT_SETTING_REPAIR = 0,
    EVENT_SETTING_FLIP = 1,
    EVENT_SETTING_VEHICLE,
};

enum {
    EVENT_TYPE_NONE,
    EVENT_TYPE_DM,
    EVENT_TYPE_NODM,
    EVENT_TYPE_10HP,
    EVENT_TYPE_1V1,
    EVENT_TYPE_TDM
};

new 
    bool:EventStarted,
    EventLeader,
    EventName[25],
    EventType,
    Float:EventPosition[4],
    EventInterior,
    EventTimer,
    EventCountDown,
    Float:EventSpawnTDM[2][4],
    Float:EventSpawnDuel[2][4],
    EventDuelCount = 0,
    bool:EventDuelStarted,
    EventDuelEnemy[2],
    EventZone,
    EventRectangle,

    Float:pEventZonePos[MAX_PLAYERS][4],
    bool:pEventZone[MAX_PLAYERS],
    pEventZoneCreator[MAX_PLAYERS],

    eventSettingData[][] = {"Naprawianie pojazdów", "Odwracanie pojazdów", "Spawnowanie wybranego pojazdu"},
    bool:EventSettings[sizeof(eventSettingData)],
    EventVehicle,

    pEventVehicle[MAX_PLAYERS],
    Iterator:Event<MAX_PLAYERS>
;

#define EVENT_TIME_START    30
#define EVENT_MINIMUM       1
#define EVENT_TYPE_LAST     EVENT_TYPE_TDM

hook function ResetPlayerData(playerid) {
    pEventZonePos[playerid] = Float:{0.0, 0.0, 0.0, 0.0};
    pEventZone[playerid] = false;
    pEventZoneCreator[playerid] = -1;
    return continue(playerid);
}

stock CheckEvent(playerid) {
    if(EventLeader == playerid && EventStarted) return true;
    return false;
}
stock EventError(playerid) return sendError(playerid, ""RED"Event nie wystartowa³, b¹dŸ nie jesteœ {c}prowadz¹cym{/c} tego wydarzenia.");

alias:evcreate("evstart")
CMD:evcreate(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    if(EventStarted) return sendError(playerid, ""RED"Aktualnie trwa event prowadzony przez: {c}%s(%d){/c}.", getNick(EventLeader), EventLeader);

    new name[25];
    if(sscanf(params, "s[25]", name)) return syntaxError(playerid, "/evcreate [nazwa]");
    if(strlen(name) < 3 || strlen(name) > 20) return sendError(playerid, ""RED"Dozwolona d³ugoœæ nazwy eventu to: {c}3-20{/c} znaki!");

    new Float:pos[4];
    getPosition(playerid, false, unpackXYZR(pos));
    for(new i = 0; i < 4; i++) EventPosition[i] = pos[i];

    EventLeader = playerid;
    EventType = EVENT_TYPE_NONE;
    EventCountDown = EVENT_TIME_START;
    EventInterior = GetPlayerInterior(playerid);
    format(EventName, sizeof(EventName), "%s", name);
    EventDuelCount = 0;
    EventDuelStarted = false;
    EventDuelEnemy = {-1, -1};
    EventVehicle = EventZone = EventRectangle = -1;
    for(new i = 0; i < sizeof(eventSettingData); i++) EventSettings[i] = false;

    for(new i = 0; i < 2; i++) {
        EventSpawnDuel[i] = Float:{0.0, 0.0, 0.0, 0.0};
        EventSpawnTDM[i] = Float:{0.0, 0.0, 0.0, 0.0};
    }

    textDrawSetString(tdEvent[1], "~w~~h~STARTUJE ~y~~h~/EVENT~w~~h~!");
    textDrawSetString(tdEvent[2], "~w~~h~%s~n~~n~~w~~h~Prowadzacy: ~r~~h~%s~n~~w~~h~Zapisanych osob: ~b~~h~~h~0 osob", name, getNick(playerid));
    textDrawSetString(tdEvent[3], "~w~~h~%d", EVENT_TIME_START);
    for(new i = 0; i < 4; i++) TextDrawShowForAll(tdEvent[i]);

    EventTimer = SetTimer("EventStartTimer", 1000, true);
    msgAll(C_CORAL, "%s {c}%s(%d){/c} organizuje event: {c}%s{/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, EventName);
    msgAll(C_CORAL, "Zapisy pod {c}/ev{/c} trwaj¹ przez: {c}"#EVENT_TIME_START"{/c} sekund!");
    return msgAll(C_CORAL, "Istnieje opcja {c}obserwacji{/c} eventu po starcie! "LY"(/evspec)"), 1;
}
flags:evcreate(MODERATOR)

function EventStartTimer()
{
    if(EventStarted) return 1;

    EventCountDown --;
    textDrawSetString(tdEvent[2], "~w~~h~%s~n~~n~~w~~h~Prowadzacy: ~r~~h~%s~n~~w~~h~Zapisanych osob: ~b~~h~~h~%d %s", EventName, getNick(EventLeader), Iter_Count(Event), ConvertTimeFormat(Iter_Count(Event), "osob", "osoba", "osoby", "osob"));
    textDrawSetString(tdEvent[3], "~w~~h~%d", EventCountDown);
    
    if(EventCountDown < 1) {
       /* if(Iter_Count(Event) < EVENT_MINIMUM) {
            EventStarted = false;
            EventPosition = Float:{0.0, 0.0, 0.0, 0.0};
            EventLeader = INVALID_PLAYER_ID;

            for(new i = 0; i < 4; i++) TextDrawHideForAll(tdEvent[i]);
            msgAll(C_CORAL, "Event {c}nie móg³{/c} wystartowaæ - zapisa³o siê za ma³o osób!");
            return KillTimer(EventTimer);
        }*/

        EventStarted = true;
        EventType = EVENT_TYPE_NONE;

        new playerid = EventLeader;
        SavePlayerPosition(playerid);
        ResetPlayerWeapons(playerid);
        SetHealth(playerid, 100.0, 0.0);

        giveWeapon(playerid, WEAPON_MINIGUN, 99999);
        SetPlayerColor(playerid, setAlpha(C_RED, 0xFF));
        Teleport(playerid, false, unpackXYZR(EventPosition), EventInterior, VW_EVENT, false);
        msg(playerid, C_LBLUE, "Event wystartowa³! Zarz¹dzanie eventem pod: {c}/evpanel{/c}.");

        for(new i = 0; i < 4; i++) TextDrawHideForAll(tdEvent[i]);
        foreach(new i : Event) {
            SavePlayerPosition(i);
            ResetPlayerWeapons(i);
            SetHealth(i, 100.0, 0.0);
            
            Teleport(i, false, EventPosition[0] + math_random_float(-3.0, 3.0), EventPosition[1] + math_random_float(-3.0, 3.0), EventPosition[2], EventPosition[3], EventInterior, VW_EVENT, false);
            msg(i, C_LBLUE, "Event wystartowa³! Aby siê z niego wypisaæ, wpisz: {c}/exit{/c}.");
        }
        KillTimer(EventTimer);
    }
    return 1;
}

alias:evend("evzakoncz", "evstop")
CMD:evend(playerid, params[])
{
    if(!CheckEvent(playerid)) return EventError(playerid);

    foreach(new i : Event) LoadPlayerPosition(i);
    Iter_Clear(Event);

    EventStarted = false;
    EventLeader = INVALID_PLAYER_ID;
    EventType = EVENT_TYPE_NONE;
    EventPosition = Float:{0.0, 0.0, 0.0, 0.0};

    if(EventZone != -1) GangZoneDestroy(EventZone);
    if(IsValidDynamicArea(EventRectangle)) DestroyDynamicArea(EventRectangle);

    new targetid = -1;
    if(!isnull(params)) {
        targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(!Iter_Contains(Event, targetid)) return sendError(playerid, ""RED"Ten gracz nie jest na evencie. Mo¿esz go dodaæ komend¹: {c}/evadd{/c}.");

        CheckBet(targetid, BET_EVENT, true);
    }

    foreach(new i : Player) { if(pSpectate[i] == playerid) { SpecToggle(i, playerid, false); } }
    msgAll(C_CORAL, "Prowadz¹cy {c}%s(%d){/c} zakoñczy³ event!%s", getNick(playerid), playerid, (targetid != -1) ? (form(" Zwyciêzca: {c}%s(%d){/c}.", getNick(targetid), targetid)) : (""));
    return LoadPlayerPosition(playerid), 1;
}

stock AllowEventCMD(playerid, const cmd[])
{
    if(EventLeader == playerid && EventStarted) {
        if(strmatch(cmd, "evend", true) || strmatch(cmd, "evzakoncz", true) || strmatch(cmd, "evstop", true) || strmatch(cmd, "evusun", true)) return 1;
        if(strmatch(cmd, "evnagroda", true) || strmatch(cmd, "evgift", true) || strmatch(cmd, "evpanel", true) || strmatch(cmd, "evdelete", true)) return 1;
        if(strmatch(cmd, "evtype", true) || strmatch(cmd, "evtyp", true) || strmatch(cmd, "evadd", true) || strmatch(cmd, "evdodaj", true)) return 1;
        if(strmatch(cmd, "evtdmspawn", true) || strmatch(cmd, "evweapon", true) || strmatch(cmd, "evgun", true) || strmatch(cmd, "evfreeze", true)) return 1;
        if(strmatch(cmd, "evunfreeze", true) || strmatch(cmd, "evdisarm", true) || strmatch(cmd, "evgod", true) || strmatch(cmd, "evdelgod", true)) return 1;
        if(strmatch(cmd, "evungod", true) || strmatch(cmd, "evcolor", true) || strmatch(cmd, "evinvisible", true) || strmatch(cmd, "evczas", true)) return 1;
        if(strmatch(cmd, "evtime", true) || strmatch(cmd, "evpogoda", true) || strmatch(cmd, "evweather", true) || strmatch(cmd, "evhealth", true)) return 1;
        if(strmatch(cmd, "evsethp", true) || strmatch(cmd, "evheal", true) || strmatch(cmd, "evarmor", true) || strmatch(cmd, "evarmour", true)) return 1;
        if(strmatch(cmd, "evsetar", true) || strmatch(cmd, "evtph", true) || strmatch(cmd, "evtphere", true) || strmatch(cmd, "evtpt", true) || strmatch(cmd, "evtpto", true)) return 1;
    }
    return 1;
}

alias:ev("event")
CMD:ev(playerid)
{
    if(EventCountDown < 1) return sendError(playerid, ""RED"Aktualnie nie ma ¿adnego odliczania do wydarzenia!");
    if(EventStarted) return sendError(playerid, ""RED"Event jest ju¿ prowadzony przez: {c}%s(%d){/c}. Poproœ prowadz¹cego o dodanie Ciê!", getNick(EventLeader), EventLeader);
    if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");
    if(NieMozeTeraz(playerid)) return sendError(playerid, ""RED"Nie mo¿esz teraz zapisaæ siê na event.");
    if(Iter_Contains(Event, playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na event.");
    if(EventLeader == playerid) return sendError(playerid, ""RED"Jesteœ prowadz¹cym eventu, nie mo¿esz siê na niego zapisaæ.");

    Iter_Add(Event, playerid);
    return msg(playerid, C_CHAT, "Zapisa³eœ(aœ) siê na {c}event{/c} organizowany przez: {c}%s(%d){/c}.", getNick(EventLeader), EventLeader), 1;
}

hook OnPlayerDisconnect(playerid, reason) 
{
    if(Iter_Contains(Event, playerid) && EventStarted && (playerid == EventDuelEnemy[0] || playerid == EventDuelEnemy[1])) {
        foreach(new i : Event) msg(i, C_CORAL, "Pojedynek {c}%s vs {/c} zakoñczy³ siê walkowerem ze strony: {c}%s(%d){/c}!", getNick(EventDuelEnemy[0]), getNick(EventDuelEnemy[1]), getNick(playerid), playerid);

        EventDuelStarted = false;
        EventDuelEnemy = {-1, -1};
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != -1 && Iter_Contains(Event, killerid) && Iter_Contains(Event, playerid) && EventStarted) {
        if(killerid == EventDuelEnemy[0] || killerid == EventDuelEnemy[1]) {
            foreach(new i : Event) msg(i, C_CORAL, "Pojedynek {c}%s vs {/c} zakoñczy³ siê wygran¹: {c}%s(%d){/c}!", getNick(EventDuelEnemy[0]), getNick(EventDuelEnemy[1]), getNick(killerid), killerid);

            EventDuelStarted = false;
            EventDuelEnemy = {-1, -1};
        }
    }

    new iterate_max;
    if(Iter_Contains(Event, playerid) && EventStarted) {
        CheckBet(playerid, BET_EVENT, false);
        msg(playerid, C_DRED, "Zgin¹³eœ(aœ) na {c}evencie{/c}, wiêc zosta³eœ(aœ) z {c}niego{/c} wyrzucony(a).");

        SetPlayerColor(playerid, setAlpha(GetPlayerColor(playerid), 0xFF));
        LoadPlayerPosition(playerid);

        Iter_SafeRemove(Event, playerid, iterate_max);
        playerid = iterate_max;
    }
    return 1;
}

CMD:evspec(playerid)
{
    if(!EventStarted) return sendError(playerid, ""RED"Event nie wystartowa³!");
    if(Iter_Contains(Event, playerid)) return sendError(playerid, ""RED"Jesteœ zapisany na event!");
    if(playerid == EventLeader) return sendError(playerid, ""RED"Nie mo¿esz siebie specowaæ na evencie.");

    SpecToggle(playerid, EventLeader, true);
    return msg(playerid, C_CORAL, "Rozpoczêto specowanie eventu - lider: {c}%s(%d){/c}.", getNick(EventLeader), EventLeader), 1;
}