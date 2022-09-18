new pMoneyTimer[MAX_PLAYERS] = -1;

stock getMoney(playerid) return playerData[playerid][player_money];
stock giveMoney(playerid, amount, bool:updateTD = true) {
    new _amount = amount;
    if(pCredit[playerid] > 0 && _amount > 0) {
        _amount = amount - floatround(amount * 0.2);
        pCredit[playerid] -= floatround(amount * 0.2);
        m_pquery_format("UPDATE es_accounts SET credit=%d WHERE id=%d LIMIT 1;", pCredit[playerid], playerData[playerid][player_accountID]);
    }
    playerData[playerid][player_money] += _amount;

    playerTextDrawSetString(playerid, tdMoney[0], "$~w~~h~%s", strdot(playerData[playerid][player_money]));
    if(percent(40)) addLevelScore(playerid, math_random(10, 30));

    if(updateTD) {
        if(pMoneyTimer[playerid] != -1) KillTimer(pMoneyTimer[playerid]);
        playerTextDrawSetString(playerid, tdMoney[1], "%s~w~~h~$%s", (_amount > 0) ? ("~g~~h~~h~+") : ("~r~~h~-"), strdot(_amount));
        PlayerTextDrawShow(playerid, tdMoney[1]);

        if(_amount > 0) playSound(playerid, 4201);
        pMoneyTimer[playerid] = SetTimerEx("ResetMoneyTD", 1000 * 3, false, "d", playerid);
    }
    return 1;
}
function ResetMoneyTD(playerid) return PlayerTextDrawHide(playerid, tdMoney[1]), KillTimer(pMoneyTimer[playerid]);

stock resetMoney(playerid) {
    playerData[playerid][player_money] = 0;
    return playerTextDrawSetString(playerid, tdMoney[0], "$~w~~h~%s", strdot(playerData[playerid][player_money])), 1;
}

stock setMoney(playerid, amount) {    
    playerData[playerid][player_money] = amount;
    playerTextDrawSetString(playerid, tdMoney[0], "$~w~~h~%s", strdot(playerData[playerid][player_money]));
}