#include YSI_Coding\y_hooks

new 
    GunShopActor[MAX_BUSINESS], 
    GunShopCP[MAX_BUSINESS],
    GunShopDay[MAX_BUSINESS][2]
;

stock FetchGunShop(businessid)
{
    if(businessid == -1) return 1;
    
    GunShop_RandomGun(businessid);

    businessData[businessid][business_icon] = CreateDynamicMapIcon(unpackXYZ(businessData[businessid][business_position]), 18, -1, 0, 0, .streamdistance=99999.0);
    CreateDynamic3DTextLabel(""BLUE2"SKLEP Z BRONI¥\n"WHITE"Wyjœcie", -1, -730.6880, 1449.5654, -90.4973 + 0.25, 30.0, .worldid=GetBusinessWorld(businessid));

    businessData[businessid][business_pickupExit] = CreatePickup(1318, 1, -730.6880, 1449.5654, -90.4973, GetBusinessWorld(businessid));
    SetPickupVirtualWorld(businessData[businessid][business_pickupExit], GetBusinessWorld(businessid));

    GunShopActor[businessid] = CreateDynamicActor(30, -720.3087, 1462.8923, -90.5046, 179.0494, .worldid=GetBusinessWorld(businessid));
    GunShopCP[businessid] = CreateDynamicCP(-720.0908, 1461.3250, -90.5046, 1.0, GetBusinessWorld(businessid));
    return 1;
}

stock GunShop_RandomGun(businessid)
{
    if(businessid == -1) return 1;

    new 
        cost = 0, 
        randomGun[] = {24, 25, 26, 27, 28, 29, 30, 31, 32, 34}, 
        selectedid = randomGun[random(sizeof(randomGun))];

    for(new i = 0; i < sizeof(weaponShopData); i++) { if(weaponShopData[i][weapon_id] == selectedid) cost = weaponShopData[i][weapon_money]; }

    GunShopDay[businessid][0] = selectedid;
    GunShopDay[businessid][1] = (cost - floatround(cost * math_random_float(0.25, 0.65)));
    return 1;
}

stock GunShop_Panel(playerid, businessid)
{
    if(businessid == -1) return 1;

    s[0] = EOS;
    s = ""LY"Broñ\t"GREEN"Ko"RED"szt\n";

    for(new i = 0; i < sizeof(weaponShopData); i++) {
        if(weaponShopData[i][weapon_bonus] && !businessData[businessid][business_license]) continue;
        if(GunShopDay[businessid][0] == weaponShopData[i][weapon_id]) format(s, sizeof(s), "%s"BLUE2"%s\t%s$%s\n", s, WeaponNames[weaponShopData[i][weapon_id]], (getMoney(playerid) < GunShopDay[businessid][1]) ? (RED) : (GREEN), strdot(GunShopDay[businessid][1]));
        else format(s, sizeof(s), "%s"LY"%s\t%s$%s\n", s, WeaponNames[weaponShopData[i][weapon_id]], (getMoney(playerid) < weaponShopData[i][weapon_money]) ? (RED) : (GREEN), strdot(weaponShopData[i][weapon_money]));
    }

    inline onWeaponBuy(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, ii
        if(!rr) return 1;

        new bool:gunDay = (weaponShopData[ll][weapon_id] == GunShopDay[businessid][0]) ? (true) : (false);
        if(getMoney(playerid) < ((!gunDay) ? (weaponShopData[ll][weapon_money]) : (GunShopDay[businessid][1]))) {
            msg(playerid, C_DRED, "Brakuje Ci {c}%s${/c} do zakupu: {c}%s{/c}.", strdot((!gunDay) ? (weaponShopData[ll][weapon_money]) : (GunShopDay[businessid][1])), WeaponNames[weaponShopData[ll][weapon_id]]);
            return GunShop_Panel(playerid, businessid), 1;
        }

        new type = weaponShopData[ll][weapon_type];
        pWeapons[playerid][type] = weaponShopData[ll][weapon_id];

        new str[128];
        for(new i = 0; i < 7; i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pWeapons[playerid][i]);
        m_pquery_format("UPDATE es_accounts SET weapon='%s' WHERE id=%d LIMIT 1;", str, playerData[playerid][player_accountID]);
        
        if(weaponShopData[ll][weapon_bonus]) {
            giveWeapon(playerid, weaponShopData[ll][weapon_id], 1);
            SetPlayerAmmo(playerid, weaponShopData[ll][weapon_id], 10);
        }
        else giveWeapon(playerid, weaponShopData[ll][weapon_id], 1000);
        giveMoney(playerid, -((!gunDay) ? (weaponShopData[ll][weapon_money]) : (GunShopDay[businessid][1])));

        AddBusinessMoney(businessid, floatround(weaponShopData[ll][weapon_money] * 0.20));
        return msg(playerid, C_ORANGE2, "Kupi³eœ(aœ) broñ {c}%s{/c} za: {c}%s${/c}.", WeaponNames[weaponShopData[ll][weapon_id]], strdot((!gunDay) ? (weaponShopData[ll][weapon_money]) : (GunShopDay[businessid][1]))), 1;
    }
    return Dialog_ShowCallback(playerid, using inline onWeaponBuy, DIALOG_STYLE_TABLIST_HEADERS, ""CORAL"Sklep z broni¹", s, "Zakup", "Zamknij"), 1;
}

hook OnPlayerEnterDynamicCP(playerid, STREAMER_TAG_CP:checkpointid)
{
    new businessid = playerData[playerid][player_business];
    if(businessid != -1 && checkpointid == GunShopCP[businessid]) return GunShop_Panel(playerid, businessid);
    return 1;
}