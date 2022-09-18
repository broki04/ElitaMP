stock PremiumError(playerid) return dialogBox(playerid, C_RED, ""RED"Nie posiadasz konta "YELLOW"Premium"RED".\n"RED"Wiêcej informacji o koncie pod: "YELLOW"/premium"RED".");
stock givePremium(playerid, days)
{
    if(!playerData[playerid][player_logged]) return 1;
    playerData[playerid][player_premiumTime] = (playerData[playerid][player_premiumTime] > gettime()) ? (playerData[playerid][player_premiumTime] + UnixTime('d', days)) : (gettime() + UnixTime('d', days));
    
    Iter_Add(Premium, playerid);
    return Log(LOG_DEBUG, "[vip]: %s(%d) -> %d dni", getNick(playerid), playerid, days);
}

CMD:vpozostalo(playerid)
{
    if(!IsPremium(playerid)) return PremiumError(playerid);
    return msg(playerid, C_LY, "Twoje {c}konto premium{/c} jest wa¿ne przez: {c}%s{/c}.", ConvertTime(playerData[playerid][player_premiumTime], TIME_TYPE_UNIX)), 1;
}

CMD:vsay(playerid, params[])
{
    if(!IsPremium(playerid)) return PremiumError(playerid);

    new _text[128];
    if(sscanf(params, "s[128]", _text)) return syntaxError(playerid, "/vsay [tekst]");
    if(strlen(_text) < 3 || strlen(_text) > 30) return sendError(playerid, ""RED"Dozwolona d³ugoœæ znaków to: "WHITE"3-30 "RED"znaki.");
    return msgAll(C_LY, "[VIP]: {%06x}%s(%d): "WHITE"%s", GetPlayerColor(playerid) >>> 8, getNick(playerid), playerid, _text), 1;
}
