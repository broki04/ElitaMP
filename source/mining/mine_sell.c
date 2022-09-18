#include YSI_Coding\y_hooks

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_YES) && IsPlayerInRangeOfPoint(playerid, 2.0, 440.1974, -846.5050, 29.8050) && !MineSell[playerid]) {
        if(GetBagWeight(playerid) < 30) return dialogBox(playerid, C_RED, ""RED"Musisz posiadaæ przynajmniej {c}30.0 kg{/c} zebranych surowców!");

        inline onDialogData(pp, dd, rr, ll, string:ii[]) {
            #pragma unused pp, dd, ll, ii
            if(!rr) return 1;

            if(IsValidVehicle(MineSellVehicle[playerid])) DestroyVehicle(MineSellVehicle[playerid]);
            if(IsValidDynamic3DTextLabel(MineSellLabel[playerid])) DestroyDynamic3DTextLabel(MineSellLabel[playerid]);
            if(IsValidDynamicMapIcon(MineSellIcon[playerid])) DestroyDynamicMapIcon(MineSellIcon[playerid]);
            if(IsValidDynamicCP(MineSellCP[playerid])) DestroyDynamicCP(MineSellCP[playerid]);

            MineSell[playerid] = true;
            MineSellTimer[playerid] = 0;
            MineSellNagroda[playerid] = (oreEvent) ? (floatround(GetOreCost(playerid) * 0.50) + GetOreCost(playerid)) : (GetOreCost(playerid));

            MineSellVehicle[playerid] = CreateVehicle(414, 434.0357, -858.0491, 28.1053, 156.4119, -1, -1, -1);
            MineSellIcon[playerid] = CreateDynamicMapIcon(281.5060, 1411.6732, 9.9837, 24, -1, .worldid=0, .interiorid=0, .streamdistance=99999.0, .style=MAPICON_GLOBAL, .playerid=playerid);
            MineSellCP[playerid] = CreateDynamicCP(281.5060, 1411.6732, 9.9837, 3.0, .worldid=0, .interiorid=0, .priority=1, .playerid=playerid);
            MineSellLabel[playerid] = CreateDynamic3DTextLabel(" ", -1, -2000.0, 0.0, -4000.0, 30.0, .attachedvehicle=MineSellVehicle[playerid], .worldid=0, .interiorid=0);

            new mstr[256]; format(mstr, sizeof(mstr), ""YELLOW"Surowce "LY"%s(%d)\n"ORANGE"Waga ³adunku: "WHITE"%d.0 kg\n"ORANGE"Nagroda: "WHITE"%s$", getNick(playerid), playerid, GetBagWeight(playerid), strdot(MineSellNagroda[playerid]));
            UpdateDynamic3DTextLabelText(MineSellLabel[playerid], -1, mstr);

            s[0] = EOS;
            catstr(s, C_BLUE2, -1, "Rozpoczêto sprzeda¿ surowców - waga: {c}%d.0 kg{/c}!\n", GetBagWeight(playerid));
            catstr(s, C_BLUE2, -1, "Za dostarczenie otrzymasz: {c}%s${/c}.\n", strdot(MineSellNagroda[playerid]));
            catstr(s, C_BLUE2, -1, "Towar musisz dostarczyæ do: {c}%s, %s{/c}.", getZoneName(281.5060, 1411.6732, 9.9837), getCityName(281.5060, 1411.6732, 9.9837));
            showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Sprzeda¿ surowców", s, "OK", #);

            SavePlayerPosition(playerid);
            ResetPlayerWeapons(playerid);
            SetHealth(playerid, 100.0, 100.0);
            return PutPlayerInVehicle(playerid, MineSellVehicle[playerid], 0), 1;
        }
        
        s[0] = EOS;
        catstr(s, C_YELLOW, -1, "Czy chcesz sprzedaæ {c}%d.0 kg{/c} wydobytych surowców?\n", GetBagWeight(playerid));
        catstr(s, C_YELLOW, -1, "Mo¿esz na tym zyskaæ {c}%s pkt. respektu + %s${/c} oraz osi¹gniêcie!\n\n", strdot(GetOreCost(playerid)));
        catstr(s, C_LRED, -1, "Kliknij poni¿ej odpowiedni przycisk, by sprzedaæ surowce!");
        Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""VIOLET"Sprzeda¿ surowców", s, "Tak", "Nie");
    }
    return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(vehicleid == MineSellVehicle[playerid] && MineSell[playerid]) MineSellTimer[playerid] = 0;
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
    if(vehicleid == MineSellVehicle[playerid] && MineSell[playerid]) {
        MineSellTimer[playerid] = 5;
        GameTextForPlayer(playerid, "~r~~h~WROC DO POJAZDU!", 1000, 3);
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(MineSell[playerid]) {
        if(IsValidVehicle(MineSellVehicle[playerid])) DestroyVehicle(MineSellVehicle[playerid]);
        if(IsValidDynamic3DTextLabel(MineSellLabel[playerid])) DestroyDynamic3DTextLabel(MineSellLabel[playerid]);
        if(IsValidDynamicMapIcon(MineSellIcon[playerid])) DestroyDynamicMapIcon(MineSellIcon[playerid]);
        if(IsValidDynamicCP(MineSellCP[playerid])) DestroyDynamicCP(MineSellCP[playerid]);

        MineSell[playerid] = false;
        MineSellNagroda[playerid] = 0;
        ShowMissionDraw(playerid, "~r~~h~MISSION FAILED!", "~w~~h~Zginales(as) podczas sprzedazy surowcow!");
    }
    return 1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if(checkpointid == MineSellCP[playerid] && MineSell[playerid] && IsPlayerInVehicle(playerid, MineSellVehicle[playerid])) {
        if(IsValidVehicle(MineSellVehicle[playerid])) DestroyVehicle(MineSellVehicle[playerid]);
        if(IsValidDynamic3DTextLabel(MineSellLabel[playerid])) DestroyDynamic3DTextLabel(MineSellLabel[playerid]);
        if(IsValidDynamicMapIcon(MineSellIcon[playerid])) DestroyDynamicMapIcon(MineSellIcon[playerid]);
        if(IsValidDynamicCP(MineSellCP[playerid])) DestroyDynamicCP(MineSellCP[playerid]);

        new money = MineSellNagroda[playerid];
        giveMoney(playerid, money);
        LoadPlayerPosition(playerid);

        s[0] = EOS;
        catstr(s, C_CHAT, C_LY, "Sprzeda³eœ(aœ) {c}%d.0 kg{/c} surowców!\n", GetBagWeight(playerid));
        catstr(s, C_CHAT, C_LY, "Otrzymujesz: {c}%s${/c}.", strdot(money));
        showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Sprzeda¿ surowców", s, "OK", #);

        MineSell[playerid] = false;
        MineSellTimer[playerid] = 0;
        MineSellNagroda[playerid] = 0;

        for(new i = 0; i < sizeof(mineData); i++) MineWeight[playerid][i] = 0;
        m_pquery_format("UPDATE es_accounts SET mine_weight='%s' WHERE id=%d LIMIT 1;", GetMineWeight(playerid), playerData[playerid][player_accountID]);
        ShowMissionDraw(playerid, "~y~MISSION PASSED!", "~w~~h~Dostarczyles(as) towar z surowcami!", money);
    }
    return 1;
}

stock MineCheckTimer(playerid)
{
    if(MineSellTimer[playerid] > 0 && MineSell[playerid]) {
        MineSellTimer[playerid] --;
        GameTextForPlayer(playerid, "~r~~h~WROC DO POJAZDU!", 1000, 3);

        if(MineSellTimer[playerid] < 1 && !IsPlayerInVehicle(playerid, MineSellVehicle[playerid])) {
            if(IsValidVehicle(MineSellVehicle[playerid])) DestroyVehicle(MineSellVehicle[playerid]);
            if(IsValidDynamic3DTextLabel(MineSellLabel[playerid])) DestroyDynamic3DTextLabel(MineSellLabel[playerid]);
            if(IsValidDynamicMapIcon(MineSellIcon[playerid])) DestroyDynamicMapIcon(MineSellIcon[playerid]);
            if(IsValidDynamicCP(MineSellCP[playerid])) DestroyDynamicCP(MineSellCP[playerid]);

            MineSell[playerid] = false;
            MineSellNagroda[playerid] = 0;
            
            ShowMissionDraw(playerid, "~r~~h~MISSION FAILED!", "~w~~h~Nie wrociles(as) do pojazdu na czas!");
            LoadPlayerPosition(playerid);
        }
    }
    return 1;
}