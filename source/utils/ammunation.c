new const Float:ammunationDataPos[][] = {
    {2400.5310, -1981.8948, 13.5469},
    {1368.6572, -1279.8928, 13.5469},
    {2159.3611, 943.1766, 10.8203}
};
new ammunationLeavePickup, ammunationWeaponPickup;
new pAmmunationID[MAX_PLAYERS] = -1, pWeapons[MAX_PLAYERS][6];

stock ResetWeaponData(playerid)
{
    pAmmunationID[playerid] = -1;
    pWeapons[playerid] = {0, WEAPON_DEAGLE, WEAPON_SHOTGUN, WEAPON_MP5, 0, 0};
}

stock Init_Ammunation()
{
    for(new i = 0; i < sizeof(ammunationDataPos); i++) {
        CreateDynamicPickup(1318, 1, ammunationDataPos[i][0], ammunationDataPos[i][1], ammunationDataPos[i][2]);
        CreateDynamic3DTextLabel(""YELLOW"Sklep z broni¹\n"GREEN"Wejœcie do sklepu.", -1, ammunationDataPos[i][0], ammunationDataPos[i][1], ammunationDataPos[i][2] + 0.17, 50.0);
        CreateDynamicMapIcon(ammunationDataPos[i][0], ammunationDataPos[i][1], ammunationDataPos[i][2], 6, -1, .streamdistance=99999.0);
    }

    ammunationWeaponPickup = CreateDynamicPickup(1254, 1, -720.0908, 1461.3250, -90.5046, VW_AMMUNATION);
    CreateDynamic3DTextLabel(""BLUE"Sklep z broni¹\n"LY"PodejdŸ, by zakupiæ broñ.", -1, -720.0908, 1461.3250, -90.5046 + 0.15, 30.0, .worldid=VW_AMMUNATION);
    CreateDynamicActor(30, -720.3087, 1462.8923, -90.5046, 179.0494, .worldid=VW_AMMUNATION);

    ammunationLeavePickup = CreateDynamicPickup(1318, 1, -730.6888, 1449.5366, -90.4973, VW_AMMUNATION);
    CreateDynamic3DTextLabel(""YELLOW"Sklep z broni¹\n"GREEN"Wyjœcie ze sklepu.", -1, -720.0908, 1461.3250, -90.5046 + 0.15, 30.0, .worldid=VW_AMMUNATION);
    return 1;
}

stock IsPlayerInGunShop(playerid)
{
    new id = -1;
    for(new i = 0; i < sizeof(ammunationDataPos); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.0, ammunationDataPos[i][0], ammunationDataPos[i][1], ammunationDataPos[i][2])) {
            id = i;
            break;
        }
    }
    return id;
}

stock GunShop_Enter(playerid, pickupid)
{
    if(pAmmunationID[playerid] != -1) {
        if(pickupid == ammunationLeavePickup) {
            new i = pAmmunationID[playerid];
            Teleport(playerid, false, ammunationDataPos[i][0], ammunationDataPos[i][1], ammunationDataPos[i][2], 0.0, 0, 0, false);
            pAmmunationID[playerid] = -1;
        }

        if(pickupid == ammunationWeaponPickup) {
            s[0] = EOS;
            s = ""WHITE"Broñ\t"BLUE"Respekt\t"ORANGE"Gotówka\n";
            for(new i = 0; i < sizeof(weaponShopData); i++) format(s, sizeof(s), "%s"WHITE"%s\t"BLUE"%s pkt. respektu\t"ORANGE"%s$\n", s, weaponShopData[i][weapon_name], strdot(weaponShopData[i][weapon_respect]), strdot(weaponShopData[i][weapon_money]));

            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ii
                if(!rr) return playerData[playerid][player_pickupTimer] = 3;

                if(getRespect(playerid) < weaponShopData[ll][weapon_respect]) return valueError(playerid, weaponShopData[ll][weapon_respect] - getRespect(playerid), " pkt. respektu");
                if(getMoney(playerid) < weaponShopData[ll][weapon_money]) return valueError(playerid, weaponShopData[ll][weapon_money] - getMoney(playerid), "$");

                new type = weaponShopData[ll][weapon_type];
                pWeapons[playerid][type] = weaponShopData[ll][weapon_id];

                new str[128];
                for(new i = 0; i < 6; i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pWeapons[playerid][i]);
                m_pquery_format("UPDATE es_accounts SET weapon='%s' WHERE id=%d LIMIT 1;", str, playerData[playerid][player_accountID]);

                giveRespect(playerid, -weaponShopData[ll][weapon_respect]);
                giveMoney(playerid, -weaponShopData[ll][weapon_money]);

                GivePlayerWeapon(playerid, weaponShopData[ll][weapon_id], 1000);
                msg(playerid, C_ORANGE2, "Kupi³eœ(aœ) broñ {c}%s{/c} za: {c}%s pkt. respektu + %s${/c}.", WeaponNames[weaponShopData[ll][weapon_id]], strdot(weaponShopData[ll][weapon_respect]), strdot(weaponShopData[ll][weapon_money]));
                return playerData[playerid][player_pickupTimer] = 3;
            }

            Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Sklep z broni¹", s, "Zakup", "Anuluj");
        }
    } else {
        new shopid = IsPlayerInGunShop(playerid);
        if(shopid != -1) {
            pAmmunationID[playerid] = shopid;
            return Teleport(playerid, false, -728.1599, 1449.2866, -90.5046, 271.6507, 1, VW_AMMUNATION, true);
        }
    }
    return 1;
}