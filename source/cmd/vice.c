alias:checkban("getban")
CMD:checkban(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id;
    if(sscanf(params, "d", id)) return syntaxError(playerid, "/checkban [userid (nie znasz uid? /getuid [nick])]");
    if(CheckPlayerOnline(id) != -1) return sendError(playerid, ""RED"Ten gracz jest online.");
    
    inline onCheckBan()
    {
        new bid;
        if(cache_num_rows() > 0) {
            cache_get_value_name_int(0, "id", bid);
            task_await(ShowBanManage(playerid, bid));
        }
        else sendError(playerid, ""RED"Nie znaleziono takiego bana.");
    }

    m_pquery_inline(dbconn, using inline onCheckBan, "SELECT * FROM es_bans WHERE userid=%d LIMIT 1;", id);
    return 1;
}
flags:checkban(VICE)

alias:getuid("getid", "getuserid")
CMD:getuid(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new nick[25];
    if(sscanf(params, "s[25]", nick)) return syntaxError(playerid, "/getuid [nick]");

    inline onGetUserID() 
    {
        new id;
        if(cache_num_rows() > 0) {
            cache_get_value_name_int(0, "id", id);
            msg(playerid, C_CHAT, "Identyfikator konta {c}%s{/c} to: {c}%s{/c}.", nick, strdot(id));
        }
        else sendError(playerid, ""RED"Takie konto nie istnieje!");
    }
    m_pquery_inline(dbconn, using inline onGetUserID, "SELECT id FROM es_accounts WHERE nick='%s' LIMIT 1;", nick);
    return 1;
}
flags:getuid(VICE)

alias:setvip("dajvip", "setpremium", "dajpremium")
CMD:setvip(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new check[32], days;
    if(sscanf(params, "s[32]d", check, days)) return syntaxError(playerid, "/setvip [id/all] [days]");
    if(days < 1 || days > 30) return sendError(playerid, ""RED"Dozwolona d³ugoœæ wa¿noœci konta premium to {c}1-30{/c} dni!");

    if(strmatch(check, "all", true)) {
        foreach(new i : Player) givePremium(i, days);
        msgAll(C_MINT, "%s {c}%s(%d){/c} da³(a) ka¿demu {c}Konto Premium{/c} wa¿ne przez {c}%d{/c} %s!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, days, ConvertTimeFormat(days, "dni", "dzieñ", "dni", "dni"));
    } else {
        new targetid = strval(params);
        givePremium(targetid, days);

        msg(playerid, C_MINT, "Da³eœ(aœ) graczowi {c}%s(%d){/c} konto premium wa¿ne przez: {c}%d{/c} %s.", getNick(targetid), targetid, days, ConvertTimeFormat(days, "dni", "dzieñ", "dni", "dni"));
        msg(targetid, C_MINT, "%s {c}%s(%d){/c} da³(a) Ci {c}konto premium{/c} wa¿ne przez: {c}%d{/c} %s.",getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, days, ConvertTimeFormat(days, "dni", "dzieñ", "dni", "dni"));
    }
    return 1;
}
flags:setvip(VICE)