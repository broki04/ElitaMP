alias:kick("wyjeb")
CMD:kick(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id, reason[50];
    if(sscanf(params, "ds[50]", id, reason)) return syntaxError(playerid, "/kick [id] [pow�d]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(strlen(reason) < 3 || strlen(reason) > 20) return sendError(playerid, ""RED"Przedzia� znak�w w powodzie to: "WHITE"3-20 "RED"znaki.");
    if(BlockAdminTroll(playerid, id)) return sendError(playerid, ""RED"Nie mo�esz wyrzuci� osoby, kt�ra ma rang� wy�sz�/r�wn� twojej.");
    return KickPlayer(id, playerid, reason), 1;
}
flags:kick(MODERATOR)

CMD:mute(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id, time, reason[50];
    if(sscanf(params, "dds[50]", id, time, reason)) return syntaxError(playerid, "/mute [id] [czas w minutach] [pow�d]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(playerData[id][player_mutetime] > 0) return sendError(playerid, ""RED"Ten gracz jest ju� wyciszony(a).");
    if(time < 1 || time > 60) return sendError(playerid, ""RED"Dozwolony przedzia� czasu: "WHITE"1-60 "RED"min.");
    if(strlen(reason) < 3 || strlen(reason) > 20) return sendError(playerid, ""RED"Przedzia� znak�w w powodzie to: "WHITE"3-20 "RED"znaki.");
    if(BlockAdminTroll(playerid, id)) return sendError(playerid, ""RED"Nie mo�esz uciszy� osoby, kt�ra ma rang� wy�sz�/r�wn� twojej.");
    return MutePlayer(id, playerid, time, reason), 1;
}
flags:mute(MODERATOR)

alias:unmute("delmute")
CMD:unmute(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/unmute [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(playerData[id][player_mutetime] < 1) return sendError(playerid, ""RED"Ten gracz nie jest uciszony.");
    playerData[id][player_mutetime] = 0;

    msg(playerid, C_MINT, "Odciszy�e�(a�) gracza {c}%s(%d){/c}.", getNick(id), id);
    return msg(id, C_MINT, "%s {c}%s(%d){/c} odciszy�(a) Ciebie!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid), 1;
}
flags:unmute(MODERATOR)

alias:tt("tpto")
CMD:tt(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/tt [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(NieMozeTeraz(id)) return sendError(playerid, ""RED"Nie mo�esz si� teraz teleportowa� do tego gracza.");

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(id, false, x, y, z, a);
    Teleport(playerid, false, x, y, z, a, GetPlayerInterior(id), GetPlayerVirtualWorld(id));

    msg(playerid, C_MINT, "Teleportowa�e�(a�) si� do gracza: {c}%s(%d){/c}.", getNick(id), id);
    return msg(id, C_MINT, "%s {c}%s(%d){/c} teleportowa�(a) si� do Ciebie!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid), 1;
}
flags:tt(MODERATOR)

alias:th("tphere")
CMD:th(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/th [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(NieMozeTeraz(id)) return sendError(playerid, ""RED"Nie mo�esz teraz teleportowa� tego gracza do siebie.");

    new Float:x, Float:y, Float:z, Float:a;
    getPosition(playerid, false, x, y, z, a);
    Teleport(id, false, x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid));

    msg(playerid, C_MINT, "Teleportowa�e�(a�) gracza {c}%s(%d){/c} do siebie.", getNick(id), id);
    return msg(id, C_MINT, "%s {c}%s(%d){/c} teleportowa�(a) Ciebie do niego!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid), 1;
}
flags:th(MODERATOR)

CMD:ip(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/th [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    return msg(playerid, C_MINT, "Adres IP gracza {c}%s(%d){/c} to: {c}%s{/c}.", getNick(id), id, getIP(id));
}
flags:ip(MODERATOR)

CMD:warn(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id, reason[90];
    if(sscanf(params, "ds[90]", id, reason)) return syntaxError(playerid, "/warn [id] [pow�d]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(id == playerid) return sendError(playerid, ""RED"Nie mo�esz ostrzec samego siebie!");
    if(strlen(reason) < 3 || strlen(reason) > 20) return sendError(playerid, ""RED"Przedzia� znak�w w powodzie to: "WHITE"3-20 "RED"znaki.");
    if(BlockAdminTroll(playerid, id)) return sendError(playerid, ""RED"Nie mo�esz ostrzec osoby, kt�ra ma rang� wy�sz�/r�wn� twojej.");
    return WarnPlayer(id, playerid, reason);
}
flags:warn(MODERATOR)

alias:unwarn("delwarn")
CMD:unwarn(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/unmute [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(playerData[id][player_warns] < 1) return sendError(playerid, ""RED"Ten gracz nie posiada ostrze�e�.");

    playerData[id][player_warns] --;
    m_pquery_format("UPDATE es_accounts SET warns=%d WHERE id=%d LIMIT 1;", playerData[id][player_warns], playerData[id][player_accountID]);
    
    msg(playerid, C_LRED, "Usun��e�(a�) ostrze�enie gracza {c}%s(%d){/c} - stan: {c}%d/5{/c}.", getNick(id), id, playerData[id][player_warns]);
    return msg(id, C_LRED, "%s {c}%s(%d){/c} usun��(�a) Ci jedno ostrze�enie! Stan: {c}%d/5{/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, playerData[id][player_warns]), 1;
}
flags:unwarn(MODERATOR)

CMD:jail(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id, time, reason[90];
    if(sscanf(params, "dds[50]", id, time, reason)) return syntaxError(playerid, "/jail [id] [czas w minutach] [pow�d]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(playerData[id][player_jailtime] > 0) return sendError(playerid, ""RED"Ten gracz jest ju� we wi�zieniu.");
    if(time < 1 || time > 60) return sendError(playerid, ""RED"Dozwolony przedzia� czasu: "WHITE"1-60 "RED"min.");
    if(strlen(reason) < 3 || strlen(reason) > 20) return sendError(playerid, ""RED"Przedzia� znak�w w powodzie to: "WHITE"3-20 "RED"znaki.");
    if(BlockAdminTroll(playerid, id)) return sendError(playerid, ""RED"Nie mo�esz uwi�zi� osoby, kt�ra ma rang� wy�sz�/r�wn� twojej.");
    return JailPlayer(id, playerid, time, reason), 1;
}
flags:jail(MODERATOR)

alias:unjail("deljail")
CMD:unjail(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/unjail [id]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(playerData[id][player_jailtime] < 1) return sendError(playerid, ""RED"Ten gracz nie jest we wi�zieniu.");
    
    playerData[id][player_jailtime] = 0;
    LoadPlayerPosition(id);

    msg(playerid, C_MINT, "Uwolni�e�(a�) gracza {c}%s(%d){/c}.", getNick(id), id);
    return msg(id, C_MINT, "%s {c}%s(%d){/c} uwolni�(a) Ciebie!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid), 1;
}
flags:unjail(MODERATOR)

alias:fake("fakenick")
CMD:fake(playerid, params[])
{
    if(playerData[playerid][player_adminLoad] < MODERATOR) return dialogBox(playerid, C_ORANGE, ""ORANGE"Komenda "WHITE"/fake "ORANGE"jest dost�pna od rangi: {%06x}%s"WHITE".", getRankColor(MODERATOR) >>> 8, getRankName(MODERATOR)), 0;
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    if(playerData[playerid][player_isFake]) {
        format(playerData[playerid][player_nickname], 25, "%s", playerData[playerid][player_fakeNick]);
        SetPlayerName(playerid, playerData[playerid][player_nickname]);

        playerData[playerid][player_isFake] = false;
        playerData[playerid][player_admin] = playerData[playerid][player_adminLoad];
        Iter_Add(Permission, playerid);
        return msg(playerid, C_BLUE2, "Fa�szywy nick zosta� {c}wy��czony{/c}.");
    } 

    new nick[25];
    if(sscanf(params, "s[25]", nick)) return syntaxError(playerid, "/fake [nick]");
    if(strlen(nick) < 3 || strlen(nick) > 20 || !IsValidNickName(nick)) return sendError(playerid, ""RED"Wprowadzony nick nie jest prawid�owy.");

    new bool:found = false;
    foreach(new i : Player) {
        if(!strcmp(playerData[i][player_nickname], nick, true)) {
            found = true;
            break;
        } 
    }

    new bool:admin_found = false;
    foreach(new i : Player) {
        if(!strcmp(playerData[i][player_fakeNick], nick, true) && playerData[i][player_isFake]) {
            admin_found = true;
            break;
        }
    }

    if(found) return sendError(playerid, ""RED"Ju� kto� korzysta z takiego nicku.");
    if(admin_found) return sendError(playerid, ""RED"Poda�e�(a�) nick admina, kt�ry jest na fejku.");

    format(playerData[playerid][player_fakeNick], 25, "%s", getNick(playerid));
    format(playerData[playerid][player_nickname], 25, "%s", nick);
    SetPlayerName(playerid, nick);

    playerData[playerid][player_isFake] = true;
    playerData[playerid][player_admin] = 0;
    Iter_Remove(Permission, playerid);
    return msg(playerid, C_BLUE2, "Fa�szywy nick ustawiony - nick: {c}%s{/c}. Aby wr�ci� do normalnego nicku, wpisz: {c}/fake{/c}.", nick);
}

alias:dajmoney("dajcash", "givemoney", "givecash")
CMD:dajmoney(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new check[32], respect;
    if(sscanf(params, "s[32]d", check, respect)) return syntaxError(playerid, "/dajmoney [id/all] [money]");
    if(respect < 10000 || respect > 1000000) return sendError(playerid, "Warto�� musi mie�ci� si� w przedziale: {c}10.000-1.000.000${/c}!");

    if(strmatch(check, "all", true)) {
        foreach(new i : Player) giveMoney(i, respect);
        msgAll(C_MINT, "%s {c}%s(%d){/c} da�(a) ka�demu {c}%s${/c}!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, strdot(respect));
    } else {
        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);

        giveMoney(targetid, respect);
        msg(playerid, C_MINT, "Da�e�(a�) {c}%s${/c} graczowi: {c}%s(%d){/c}.", strdot(respect), getNick(targetid), targetid);
        msg(targetid, C_MINT, "%s {c}%s(%d){/c} da�(a) Ci: {c}%s${/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, strdot(respect));
    }
    return 1;
}
flags:dajmoney(MODERATOR)

alias:cls("clear", "cchat", "cc")
CMD:cls(playerid)
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    for(new i = 0; i < 69; i++) msgAll(-1, " ");
    return msgAll(C_MINT, "%s {c}%s(%d){/c} wyczy�ci�(a) czat!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid), 1;
}
flags:cls(MODERATOR)

CMD:jetpack(playerid)
{   
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK), 1;
}
flags:jetpack(MODERATOR)

new annTime, annTimer;
CMD:ann(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new time, message[128];
    if(sscanf(params, "ds[128]", time, message)) return syntaxError(playerid, "/ann [czas (max 30 sek)] [tekst]");
    if(time < 1 || time > 30) return sendError(playerid, "Dost�pny przedzia� czasowy: {c}1-30{/c} sekund.");
    if(strlen(message) < 3 || strlen(message) > 50) return sendError(playerid, "Dozwolony limit znak�w to: {c}3-50{/c} znaki!");
    if(annTime > 0) return sendError(playerid, "Obecnie jest og�oszenie na ekranie. Poczekaj, a� zniknie!");

    annTime = time;
    
    new _color[20];
    switch(playerData[playerid][player_admin]) {
        case MODERATOR: _color = "~g~~h~";
        case ADMIN: _color = "~b~~h~";
        case VICE: _color = "~b~~h~~h~";
        case HEAD: _color = "~r~~h~";
    }
    textDrawSetString(tdAnnounce, "%s%s:~n~~w~~h~%s", _color, getNick(playerid), message);
    TextDrawShowForAll(tdAnnounce);

    inline AnnounceTimer() {
        annTime = 0;
        TextDrawHideForAll(tdAnnounce);
        Timer_KillCallback(annTimer);
    }
    annTimer = Timer_CreateCallback(using inline AnnounceTimer, time * 1000, 0);
    return 1;
}
flags:ann(MODERATOR)