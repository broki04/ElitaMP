stock EnterBusiness(playerid, businessid)
{
    if(businessid == -1) return 1;
    playerData[playerid][player_business] = businessid;

    new bstr[128];
    switch(businessData[businessid][business_type]) {
        case BUSINESS_CASINO: {
            format(bstr, sizeof(bstr), "~n~~n~~w~~h~Posiadane zetony: ~r~~h~%s", strdot(pCasinoTokens[playerid]));
            if(businessData[businessid][business_ownerid] == playerData[playerid][player_accountID]) format(bstr, sizeof(bstr), "%s~n~~w~~h~Dochod biznesu: ~y~~h~%s$", bstr, strdot(businessData[businessid][business_reward]));

            Teleport(playerid, false, 2008.3939, 1017.8072, 994.4688, 91.4314, 10, GetBusinessWorld(businessid));
        }

        case BUSINESS_GUNSHOP: {
            format(bstr, sizeof(bstr), "~n~~n~~w~~h~Bron dnia: ~r~~h~%s ~r~~h~~h~(%s$)~n~", WeaponNames[GunShopDay[businessid][0]], strdot(GunShopDay[businessid][1]));
            if(businessData[businessid][business_ownerid] == playerData[playerid][player_accountID]) format(bstr, sizeof(bstr), "%s~n~~w~~h~Dochod biznesu: ~y~~h~%s$", bstr, strdot(businessData[businessid][business_reward]));

            Teleport(playerid, false, -728.1599, 1449.2866, -90.5046, 271.6507, 1, GetBusinessWorld(businessid), true);
        }
    }

    playerTextDrawSetString(playerid, tdInfo[1], "~p~~h~%s", DeletePolish(GetBusinessType(businessData[businessid][business_type])));
    playerTextDrawSetString(playerid, tdInfo[0], bstr);

    SaveWeaponData(playerid);
    for(new i = 0; i < 2; i++) PlayerTextDrawShow(playerid, tdInfo[i]);
    return ResetPlayerWeapons(playerid), 1;
}

stock AddBusinessMoney(businessid, money)
{
    if(businessid == -1 || businessData[businessid][business_ownerid] == -1) return 1;

    businessData[businessid][business_reward] += money;
    m_pquery_format("UPDATE es_business SET reward=%d WHERE id=%d LIMIT 1;", businessData[businessid][business_reward], businessid);

    new playerid = CheckPlayerOnline(businessData[businessid][business_ownerid]);
    if(playerid != -1 && playerData[playerid][player_business] == businessid) {
        if(pBusinessTimer[playerid] != -1) KillTimer(pBusinessTimer[playerid]);

        new bstr[128];
        switch(businessData[businessid][business_type]) {
            case BUSINESS_CASINO: format(bstr, sizeof(bstr), "~n~~n~~w~~h~Posiadane zetony: ~r~~h~%s~n~~w~~h~Dochod biznesu: ~y~~h~%s$ ~g~~h~~h~(+%s$)", strdot(pCasinoTokens[playerid]), strdot(businessData[businessid][business_reward]), strdot(money));
            case BUSINESS_GUNSHOP: format(bstr, sizeof(bstr), "~n~~n~~w~~h~Bron dnia: ~r~~h~%s ~r~~h~~h~(%s$)~n~~w~~h~Dochod biznesu: ~y~~h~%s$ ~g~~h~~h~(+%s$)", WeaponNames[GunShopDay[businessid][0]], strdot(GunShopDay[businessid][1]), strdot(businessData[businessid][business_reward]), strdot(money));
        }

        playerTextDrawSetString(playerid, tdInfo[0], bstr);
        pBusinessTimer[playerid] = SetTimerEx("RefreshInfoTD", 1000 * 3, false, "dd", playerid, businessid);
    } 
    return 1;
}

function RefreshInfoTD(playerid, businessid)
{
    if(businessid == -1 || playerData[playerid][player_business] != businessid) return KillTimer(pBusinessTimer[playerid]);

    switch(businessData[businessid][business_type]) {
        case BUSINESS_CASINO: playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Posiadane zetony: ~r~~h~%s~n~~w~~h~Dochod biznesu: ~y~~h~%s$", strdot(pCasinoTokens[playerid]), strdot(businessData[businessid][business_reward]));
        case BUSINESS_GUNSHOP: playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Bron dnia: ~r~~h~%s ~r~~h~~h~(%s$)~n~~w~~h~Dochod biznesu: ~y~~h~%s$", WeaponNames[GunShopDay[businessid][0]], strdot(GunShopDay[businessid][1]), strdot(businessData[businessid][business_reward]));
    }
    return KillTimer(pBusinessTimer[playerid]), 1;
}
