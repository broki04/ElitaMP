#include YSI_Coding\y_hooks

new bool:IsKicked[MAX_PLAYERS] = false;
hook function ResetPlayerData(playerid) { IsKicked[playerid] = false; return continue(playerid); }

stock KickEx(playerid) {
    inline onKickPlayer() return Kick(playerid), 1;
    return Timer_CreateCallback(using inline onKickPlayer, 50 + GetPlayerPing(playerid), 1);
}
#define Kick KickEx

stock KickPlayer(playerid, adminid, const reason[])
{
    if(IsKicked[playerid]) return 1;

    new admin_name[25]; format(admin_name, sizeof(admin_name), "%s", (adminid != -1) ? (form("%s(%d)", getNick(adminid), adminid)) : ("Serwer"));
    msgAll(C_LRED, "Gracz {c}%s(%d){/c} wyrzucony(a) przez: {c}%s{/c}.", getNick(playerid), playerid, admin_name);
    msgAll(C_LRED, "Powód wyrzucenia: {c}%s{/c}.", reason);

    if(adminid != -1) {
        playerData[adminid][player_adminPenalty][ADMIN_PENALTY_KICK] ++;
        playerData[adminid][player_adminPoints] += math_random(3, 5);
    }
    Log(LOG_PUNISH, "[kick]: %s -> %s(%d): %s", admin_name, getNick(playerid), playerid, reason);

    s[0] = EOS;
    strcat(s, ""RED"ZOSTA£EŒ(AŒ) WYRZUCONY(A)!\n\n");
    format(s, sizeof(s), "%s"WHITE"Admin wyrzucaj¹cy: "LRED"%s\n", s, admin_name);
    format(s, sizeof(s), "%s"WHITE"Data wyrzucenia: "LRED"%s\n", s, getLocalDate());
    format(s, sizeof(s), "%s"WHITE"Powód wyrzucenia: "LRED"%s", s, reason);
    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Wyrzucenie", s, "OK", #);

    kicks ++;
    IsKicked[playerid] = true;
    return Kick(playerid), 1;
}

stock WarnPlayer(playerid, adminid, const reason[])
{
    if(IsKicked[playerid]) return 1;

    new admin_name[25]; format(admin_name, sizeof(admin_name), "%s", (adminid != -1) ? (form("%s(%d)", getNick(adminid), adminid)) : ("Serwer"));
    playerData[playerid][player_warns] ++;

    msgAll(C_LRED, "Gracz {c}%s(%d){/c} ostrze¿ony(a) %d/5 raz przez: {c}%s{/c}.", getNick(playerid), getNick(playerid), playerData[playerid][player_warns], admin_name);
    msgAll(C_LRED, "Powód ostrze¿enia: {c}%s{/c}.", reason);

    if(adminid != -1) {
        playerData[adminid][player_adminPenalty][ADMIN_PENALTY_WARN] ++;
        playerData[adminid][player_adminPoints] += math_random(3, 5);
    }
    Log(LOG_PUNISH, "[warn]: %s -> %s(%d): %s (%d/5)", admin_name, getNick(playerid), playerid, reason, playerData[playerid][player_warns]);

    if(playerData[playerid][player_warns] >= 5) {
        m_pquery_format("UPDATE es_accounts SET warns=0 WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID]);
        return BanPlayer(playerid, adminid, 7, "5/5 ostrzezen"), 1;
    }
    return playSound(playerid, 1053), 1;
}

stock JailPlayer(playerid, adminid, time, const reason[])
{
    if(IsKicked[playerid]) return 1;

    new admin_name[25]; format(admin_name, sizeof(admin_name), "%s", (adminid != -1) ? (form("%s(%d)", getNick(adminid), adminid)) : ("Serwer"));
    msgAll(C_LRED, "Gracz {c}%s(%d){/c} uwiêziony(a) przez: {c}%s{/c} na: {c}%d min{/c}.", getNick(playerid), playerid, admin_name, time);
    msgAll(C_LRED, "Powód uwiêzienia: {c}%s{/c}.", reason);

    if(adminid != -1) {
        playerData[adminid][player_adminPenalty][ADMIN_PENALTY_JAIL] ++;    
        playerData[adminid][player_adminPoints] += math_random(3, 5);
    }
    Log(LOG_PUNISH, "[jail]: %s -> %s(%d): %s (%dm)", admin_name, getNick(playerid), playerid, reason, time);
    if(playerData[playerid][player_jailtime] < 1) playerData[playerid][player_jailtime] = time * 60;

    SavePlayerPosition(playerid);
    Teleport(playerid, false, 1094.2321, -2545.6389, 16.0043, 0.0, VW_JAIL, VW_JAIL, true);
    achievement(playerid, ACH_JAIL);

    s[0] = EOS;
    strcat(s, ""RED"ZOSTA£EŒ(AŒ) UWIÊZIONY!\n\n");
    format(s, sizeof(s), "%s"WHITE"Admin nadaj¹cy wyrok: "LRED"%s\n", s, admin_name);
    format(s, sizeof(s), "%s"WHITE"Data nadania wyroku: "LRED"%s\n", s, getLocalDate());
    format(s, sizeof(s), "%s"WHITE"D³ugoœæ wyroku: "LRED"%d min\n", s, time);
    format(s, sizeof(s), "%s"WHITE"Powód wyroku: "LRED"%s", s, reason);
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Uwiêzienie", s, "OK", #), 1;
}

stock MutePlayer(playerid, adminid, time, const reason[])
{
    if(IsKicked[playerid]) return 1;

    new admin_name[25]; format(admin_name, sizeof(admin_name), "%s", (adminid != -1) ? (form("%s(%d)", getNick(adminid), adminid)) : ("Serwer"));
    msgAll(C_LRED, "Gracz {c}%s(%d){/c} wyciszony(a) przez: {c}%s{/c} na: {c}%d min{/c}.", getNick(playerid), playerid, admin_name, time);
    msgAll(C_LRED, "Powód uciszenia: {c}%s{/c}.", reason);

    if(adminid != -1) {
        playerData[adminid][player_adminPenalty][ADMIN_PENALTY_MUTE] ++;
        playerData[adminid][player_adminPoints] += math_random(3, 5);
    }
    Log(LOG_PUNISH, "[mute]: %s -> %s(%d): %s (%dm)", admin_name, getNick(playerid), playerid, reason, time);
    playerData[playerid][player_mutetime] = time * 60;

    s[0] = EOS;
    strcat(s, ""RED"ZOSTA£EŒ(AŒ) WYCISZONY(A)!\n\n");
    format(s, sizeof(s), "%s"WHITE"Admin wyciszaj¹cy: "LRED"%s\n", s, admin_name);
    format(s, sizeof(s), "%s"WHITE"Data wyciszenia: "LRED"%s\n", s, getLocalDate());
    format(s, sizeof(s), "%s"WHITE"D³ugoœæ wyciszenia: "LRED"%d min\n", s, time);
    format(s, sizeof(s), "%s"WHITE"Powód wyciszenia: "LRED"%s", s, reason);
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Wyciszenie", s, "OK", #), 1;
}

stock CheckPlayerMuted(playerid)
{
    s[0] = EOS;
    format(s, sizeof(s), ""RED"Jesteœ obecnie wyciszony(a)!\n"RED"Bêdziesz móg³ pisaæ za: "WHITE"%s"RED".", ConvertTime(playerData[playerid][player_mutetime], TIME_TYPE_MS));
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Wyciszenie", s, "OK", #), 1;
}

stock BanPlayer(playerid, adminid, time, const reason[])
{
    if(IsKicked[playerid]) return 1;

    new admin_name[25]; format(admin_name, sizeof(admin_name), "%s", (adminid != -1) ? (form("%s(%d)", getNick(adminid), adminid)) : ("Serwer"));
    msgAll(C_LRED, "Gracz {c}%s(%d){/c} zbanowany(a) przez: {c}%s{/c} %s.", getNick(playerid), playerid, admin_name, (time == -1) ? ("permanentnie") : (form("czasowo na: {c}%d{/c} dni", time)));
    msgAll(C_LRED, "Powód zbanowania: {c}%s{/c}.", reason);

    if(adminid != -1) {
        playerData[adminid][player_adminPenalty][ADMIN_PENALTY_BAN] ++;
        playerData[adminid][player_adminPoints] += math_random(3, 5);
    }
    Log(LOG_PUNISH, "[mute]: %s -> %s(%d): %s (%s)", admin_name, getNick(playerid), playerid, reason, (time == -1) ? ("perm") : (form("%dd", time)));
    m_pquery_format("INSERT INTO es_bans (userid, adminid, reason, date, ip, gpci, expire) VALUES (%d, %d, '%s', '%s', '%s', '%s', %d);", playerData[playerid][player_accountID], (adminid != -1) ? (playerData[adminid][player_accountID]) : (-1), reason, getLocalDate(), getIP(playerid), getSerial(playerid), (time == -1) ? (-1) : (gettime() + UnixTime('d', time)));

    s[0] = EOS;
    //strcat(s, ""RED"ZOSTA£EŒ(AŒ) ZBANOWANY(A)!\n\n");
    strcat(s, ""RED"BANIDO DO SERVIRADO\n\n");
    format(s, sizeof(s), "%s"WHITE"Admin banuj¹cy: "LRED"%s\n", s, admin_name);
    format(s, sizeof(s), "%s"WHITE"Data zbanowania: "LRED"%s\n", s, getLocalDate());
    format(s, sizeof(s), "%s"WHITE"D³ugoœæ banicji: "LRED"%s\n", s, (time == -1) ? ("Permanentny") : (form("%d dni", time)));
    format(s, sizeof(s), "%s"WHITE"Powód banicji: "LRED"%s", s, reason);
    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Zbanowanie", s, "OK", #);

    bans ++;
    IsKicked[playerid] = true;
    return Kick(playerid), 1;
}

stock CheckPlayerBanned(playerid, bool:accountID)
{
    inline onCheckPlayerBanned()
    {
        if(cache_num_rows() > 0) {
            new userid, adminid, reason[256], expire, ip[18], serial[65], date[50];
            cache_get_value_name_int(0, "userid", userid);
            cache_get_value_name_int(0, "adminid", adminid);
            cache_get_value_name(0, "reason", reason);
            cache_get_value_name(0, "ip", ip);
            cache_get_value_name(0, "serial", serial);
            cache_get_value_name_int(0, "expire", expire);
            cache_get_value_name(0, "date", date);

            if(expire != -1 && expire < gettime()) { m_pquery_format("DELETE FROM es_bans WHERE userid=%d LIMIT 1;", userid); }
            else {
                new nick[25], Cache:result = mysql_query(dbconn, form("SELECT nick FROM es_accounts WHERE id=%d LIMIT 1;", adminid));
                if(cache_num_rows() > 0) cache_get_value(0, 0, nick, sizeof(nick));
                cache_delete(result);
                
                s[0] = EOS;
                strcat(s, ""RED"ZOSTA£EŒ(AŒ) ZBANOWANY(A)!\n\n");
                format(s, sizeof(s), "%s"WHITE"Admin banuj¹cy: "LRED"%s\n", s, (adminid == -1) ? ("Serwer") : (form("%s", nick))); 
                format(s, sizeof(s), "%s"WHITE"Data zbanowania: "LRED"%s\n", s, date);
                format(s, sizeof(s), "%s"WHITE"D³ugoœæ banicji: "LRED"%s\n", s, (expire == -1) ? ("Permanentny") : (ConvertTime(expire, TIME_TYPE_UNIX)));
                format(s, sizeof(s), "%s"WHITE"Powód banicji: "LRED"%s", s, reason);
                showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Zbanowanie", s, "OK", #);

                Kick(playerid);
            }
        }
    }

    new query[256];
    if(accountID) format(query, sizeof(query), "SELECT * FROM es_bans WHERE ip='%s' OR gpci='%s' OR userid=%d", getIP(playerid), getSerial(playerid), playerData[playerid][player_accountID]);
    else format(query, sizeof(query), "SELECT * FROM es_bans WHERE ip='%s' OR gpci='%s'", getIP(playerid), getSerial(playerid));
    m_tquery_inline(dbconn, using inline onCheckPlayerBanned, query);
    return 1;
}