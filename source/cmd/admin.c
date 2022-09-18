CMD:ban(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new id, time, reason[50];
    if(sscanf(params, "dds[50]", id, time, reason)) {
        time = -1;
        if(sscanf(params, "ds[50]", id, reason)) {
            return syntaxError(playerid, "/ban [id] [czas (-1=perm)] [powód]");
        }
    }
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(id == playerid) return sendError(playerid, ""RED"Nie mo¿esz siebie zbanowaæ.");
    if(BlockAdminTroll(playerid, id)) return sendError(playerid, ""RED"Nie mo¿esz zbanowaæ osoby, która ma rangê wy¿sz¹/równ¹ Tobie.");
    if(time != -1 && (time < 1 || time > 30)) return sendError(playerid, ""RED"Dozwolony przedzia³ czasowy bana to: "WHITE"1-30 "RED"dni.");
    if(strlen(reason) < 3 || strlen(reason) > 20) return sendError(playerid, ""RED"Dozwolony przedzia³ znaków w powodzie to: "WHITE"3-20 "RED"znaki.");
    return BanPlayer(id, playerid, time, reason), 1;
}
flags:ban(ADMIN)

CMD:offban(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new id, time, reason[50];
    if(sscanf(params, "dds[50]", id, time, reason)) {
        time = -1;
        if(sscanf(params, "ds[50]", id, reason)) {
            return syntaxError(playerid, "/offban [id konta] [czas (-1=perm)] [powód]");
        }
    }
    if(CheckPlayerOnline(id) != -1) return sendError(playerid, ""RED"Ten gracz jest online.");
    if(time != -1 && (time < 1 || time > 30)) return sendError(playerid, ""RED"Dozwolony przedzia³ czasowy bana to: "WHITE"1-30 "RED"dni.");
    if(strlen(reason) < 3 || strlen(reason) > 20) return sendError(playerid, ""RED"Dozwolony przedzia³ znaków w powodzie to: "WHITE"3-20 "RED"znaki.");

    inline onAdminOffBan()
    {
        new serial[65], ip[18];
        if(cache_num_rows() > 0) {
            cache_get_value_name(0, "serial_last", serial, sizeof(serial));
            cache_get_value_name(0, "ip_last", ip, sizeof(ip));

            new nick[25], Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", id));
            if(cache_num_rows() > 0) cache_get_value(0, 0, nick, sizeof(nick));
            cache_delete(result);

            msgAll(C_LRED, "Gracz {c}%s{/c} zbanowany(a) przez: {c}%s(%d){/c} %s.", nick, getNick(playerid), playerid, (time == -1) ? ("permanentnie") : (form("czasowo na: {c}%d{/c} dni", time)));
            msgAll(C_LRED, "Powód zbanowania: {c}%s{/c}.", reason);

            m_pquery_format("INSERT INTO es_bans (userid, adminid, reason, date, ip, gpci, expire) VALUES (%d, %d, '%s', '%s', '%s', '%s', %d);", id, playerData[playerid][player_accountID], reason, getLocalDate(), ip, serial, (time == -1) ? (-1) : (gettime() + UnixTime('d', time)));
        }
        else sendError(playerid, ""RED"Konto o takim identyfikatorze nie istnieje.");
    }

    m_tquery_inline(dbconn, using inline onAdminOffBan, "SELECT serial_last, ip_last FROM es_accounts WHERE id=%d LIMIT 1;", id);
    return 1;
}
flags:offban(ADMIN)

CMD:god(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    
    if(isnull(params)) {
        playerData[playerid][player_god] = !playerData[playerid][player_god];
        return msg(playerid, C_CORAL, "%s sobie nieœmiertelnoœæ.", (playerData[playerid][player_god]) ? ("Da³eœ(aœ)") : ("Zabra³eœ(aœ)")), 1;
    } else {
        if(!isNumeric(params)) return sendError(playerid, ""RED"Podane ID gracza musi byæ numeryczne.");

        new targetid = strval(params);
        if(!IsPlayerConnected(targetid)) return Polaczenie(playerid);
        if(NieMozeTeraz(targetid)) return sendError(playerid, ""RED"Nie mo¿esz teraz zarz¹dzaæ nieœmiertelnoœci¹ tego gracza.");

        playerData[targetid][player_god] = !playerData[targetid][player_god];
        msg(playerid, C_CORAL, "%s graczowi {c}%s(%d){/c} nieœmiertelnoœæ.", (playerData[targetid][player_god]) ? ("Da³eœ(aœ)") : ("Zabra³eœ(aœ)"), getNick(targetid), targetid);
        msg(targetid, C_CORAL, "%s {c}%s(%d){/c} %s Tobie nieœmiertelnoœæ.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, (playerData[targetid][player_god]) ? ("da³(a)") : ("zabra³(a)"));
    }
    return 1;
}
flags:god(ADMIN)