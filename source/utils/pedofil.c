#include YSI_Coding\y_hooks

new pedofilSelected = -1;
new pedofilReward = 0;

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID) {
        if(pedofilSelected == playerid) {
            pedofilSelected = -1;
        
            giveMoney(killerid, pedofilReward);
            achievement(killerid, ACH_PEDOFIL);

            msgAll(C_LRED, "Gracz {c}%s(%d){/c} zabija pedofila: {c}%s(%d){/c}.", getNick(killerid), killerid, getNick(playerid), playerid);
            msgAll(C_LRED, "Nagroda za zabójstwo to: {c}%s${/c}.", strdot(pedofilReward));
        }
    }
    return 1;
}

CMD:pedofil(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    if(pedofilSelected != -1) return sendError(playerid, ""RED"Ju¿ ktoœ jest pedofilem!");

    new money;
    if(sscanf(params, "d", money)) return syntaxError(playerid, "/pedofil [money]");
    if(money < 3000 || money > 25000) return sendError(playerid, ""RED"Dozwolony przedzia³ nagrody to: "WHITE"3.000$-25.000$.");

    pedofilSelected = playerid;
    pedofilReward = money;

    msgAll(C_NICK, "%s {c}%s(%d){/c} sta³(a) siê najbardziej poszukiwanym(¹) pedofilem(lk¹) na serwerze!", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid);
    msgAll(C_NICK, "Nagroda za z³apanie to: {c}%s${/c}.", strdot(money));

    new const pedofilSkiny[] = {32, 33, 34, 35, 37, 15, 8, 47, 48, 269, 264};
    giveWeapon(playerid, WEAPON_DILDO, 1);
    return SetPlayerSkin(playerid, pedofilSkiny[random(sizeof(pedofilSkiny))]), 1;
}
flags:pedofil(MODERATOR)