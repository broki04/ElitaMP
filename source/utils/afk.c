#include YSI_Coding\y_hooks

new 
    bool:pAFK[MAX_PLAYERS], 
    pAFKTime[MAX_PLAYERS], 
    Float:pAFKHealth[MAX_PLAYERS][2],
    Float:pAFKPosition[MAX_PLAYERS][3]
;

hook function ResetPlayerData(playerid) {
    pAFK[playerid] = false;
    pAFKTime[playerid] = 0;
    pAFKHealth[playerid] = Float:{0.0, 0.0};
    pAFKPosition[playerid] = Float:{0.0, 0.0, 0.0};
    return continue(playerid);
}

hook OnPlayerUpdate(playerid) {
    if(!pAFK[playerid] && pAFKTime[playerid] > 0) {
        new Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
        if(x != pAFKPosition[playerid][0] && y != pAFKPosition[playerid][1] && z != pAFKPosition[playerid][2]) pAFKTime[playerid] = 0;
    }
    return 1;
}

stock CheckPlayerAFK(playerid) {
    if(!pAFK[playerid] && playerData[playerid][player_logged]) {
        GetPlayerPos(playerid, pAFKPosition[playerid][0], pAFKPosition[playerid][1], pAFKPosition[playerid][2]);

        pAFKTime[playerid] ++;
        if(pAFKTime[playerid] >= 300) {
            callcmd::zw(playerid, "Brak aktywnoœci przez 5 minut");
            dialogBox(playerid, C_RED, "Serwer uzna³, ¿e przez {c}brak aktywnoœci{/c} jesteœ AFK!\n"RED"Aby wróciæ do gry, wpisz: {c}/jj{/c}.");
        }
    }
    return 1;
}

alias:zw("afk")
CMD:zw(playerid, params[])
{
    if(pAFK[playerid]) return 1;

    pAFK[playerid] = true;
    pAFKTime[playerid] = 0;

    GetPlayerHealth(playerid, pAFKHealth[playerid][0]);
    GetPlayerArmour(playerid, pAFKHealth[playerid][1]);

    msgAll(C_NICK, "Gracz {c}%s(%d){/c} zaraz wraca!%s", getNick(playerid), playerid, (!isnull(params)) ? (form(" Powód: {c}%s{/c}.", params)) : (""));
    return TogglePlayerControllable(playerid, false), 1;
}

CMD:jj(playerid)
{
    if(!pAFK[playerid]) return dialogBox(playerid, C_ORANGE, "Nie jesteœ na {c}AFK{/c}! Aby byæ AFK, u¿yj: {c}/zw{/c}.");

    SetPlayerHealth(playerid, pAFKHealth[playerid][0]);
    SetPlayerArmour(playerid, pAFKHealth[playerid][1]);

    pAFKHealth[playerid] = Float:{0.0, 0.0};
    pAFK[playerid] = false;

    msgAll(C_NICK, "Gracz {c}%s(%d){/c} ju¿ jest.", getNick(playerid), playerid);
    return TogglePlayerControllable(playerid, true), 1;
}