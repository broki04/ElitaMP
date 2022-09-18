#include YSI_Coding\y_hooks

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    new mineid = GetNearestOre(playerid);
    if(mineid != -1 && PRESSED(KEY_SPRINT | KEY_YES) && !MineStarted[playerid] && MineBag[playerid] && MineBagLevel[playerid] > 0) {
        if(!MinePickaxe[playerid] || MinePickaxeHealth[playerid][PICKAXE_HEALTH_STATUS] <= 0.0) return dialogBox(playerid, C_RED, ""RED"Nie posiadasz kilofa, b¹dŸ twój kilof jest {c}zniszczony{/c}."); 
        if(GetBagWeight(playerid) >= GetBagAmount(playerid)) return sendError(playerid, ""RED"Twój plecak górniczy jest pe³ny! Sprzedaj jego zawartoœæ na {c}/kopalnia{/c}.");
        if(MineCounter[mineid] >= MineMiners[mineid]) return dialogBox(playerid, C_RED, ""RED"Przekroczono {c}limit{/c} osób, które mog¹ wydobywaæ tutaj!");

        MineStarted[playerid] = true;    
        MineCounter[mineid] ++;

        giveWeapon(playerid, WEAPON_SHOVEL, 1);
        SetPlayerArmedWeapon(playerid, WEAPON_SHOVEL);

        GameTextForPlayer(playerid, "~y~WYCIAGA KILOF~r~~h~...", 2000, 3);
        return SetTimerEx("PrepareMining", 1000 * 2, false, "dd", playerid, mineid);
    }   
    return 1;
}

function PrepareMining(playerid, mineid)
{
    MineStarted[playerid] = true;    

    new time = 0;
    switch(MinePickaxeLevel[playerid]) {
        case 1: time = math_random(14, 17);
        case 2: time = math_random(11, 14);
        case 3: time = math_random(9, 11);
        case 4: time = math_random(7, 9);
        default: time = math_random(5, 7);
    }

    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "GANGS", "HNDSHKAA", 4.1, true, false, false, true, 0, 1);
    GameTextForPlayer(playerid, "~y~TRWA KOPANIE~r~~h~...", 1000 * time, 3);
    return SetTimerEx("OnPlayerMining", (1000 * time) + 1000, false, "dd", playerid, mineid);
}

stock CancelMining(playerid)
{
    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);
    RemovePlayerWeapon(playerid, WEAPON_SHOVEL);
    if(pWeapons[playerid][0] > 0) giveWeapon(playerid, pWeapons[playerid][0], 1);
    return MineStarted[playerid] = false, 1;
}

function OnPlayerMining(playerid, mineid)
{
    if(!MineCreated[mineid]) return CancelMining(playerid);

    if(percent(40)) MineLevel[mineid] --;
    MineCounter[mineid] --;

    if(MineLevel[mineid] == 5) { if(IsValidDynamicObject(MineObject[mineid][2])) DestroyDynamicObject(MineObject[mineid][2]); }
    if(MineLevel[mineid] == 2) { if(IsValidDynamicObject(MineObject[mineid][3])) DestroyDynamicObject(MineObject[mineid][3]); }

    new weight = math_random(1, 5);
    MineWeight[playerid][MineType[mineid]] += weight;
    MinePickaxeHealth[playerid][PICKAXE_HEALTH_STATUS] -= math_random_float(2.0, 8.0);
    m_pquery_format("UPDATE es_accounts SET mine_weight='%s', pickaxe_health='%.2f, %.2f, %d' WHERE id=%d LIMIT 1;", GetMineWeight(playerid), MinePickaxeHealth[playerid][PICKAXE_HEALTH_STATUS], MinePickaxeHealth[playerid][PICKAXE_HEALTH_LIMIT], playerData[playerid][player_accountID]);

    s[0] = EOS;
    catstr(s, mineData[MineType[mineid]][mine_color], -1, "Wydoby³eœ(aœ) {c}%d.0 kg{/c} %s{/c}.\n", weight, GetMineName(MineType[mineid]));
    catstr(s, mineData[MineType[mineid]][mine_color], -1, "Posiadasz aktualnie {c}%d.0 kg{/c} tego surowca!\n", MineWeight[playerid][MineType[mineid]]);
    catstr(s, mineData[MineType[mineid]][mine_color], -1, "Sprzedaj¹c surowce, mo¿esz zarobiæ: {c}%s${/c}.", strdot(GetOreCost(playerid)));

    if(oreEvent) {
        if(percent(40) || OreDay == MineType[mineid]) {
            catstr(s, mineData[MineType[mineid]][mine_color], -1, "\n\nOtrzymujesz dodatkowo {c}punkt górniczy{/c} za wykopanie {c}surowca dnia{/c}!");
            MinePoints[playerid] ++;
        }
    } else {
        if(percent(35) && OreDay == MineType[mineid]) {
            catstr(s, mineData[MineType[mineid]][mine_color], -1, "\n\nOtrzymujesz dodatkowo {c}punkt górniczy{/c} za wykopanie {c}surowca dnia{/c}!");
            MinePoints[playerid] ++;
        }
    }
    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"Wydobycie ukoñczone", s, "OK", #);

    if(MineLevel[mineid] < 1) {
        if(IsValidDynamic3DTextLabel(MineLabel[mineid])) DestroyDynamic3DTextLabel(MineLabel[mineid]);
        for(new j = 0; j < 4; j++) { if(IsValidDynamicObject(MineObject[mineid][j])) DestroyDynamicObject(MineObject[mineid][j]); }
        MineCreated[mineid] = false;
    }
    else UpdateDynamic3DTextLabelText(MineLabel[mineid], -1, UpdateMineLabel(mineid));

    achievement(playerid, ACH_GORNIK);
    addLevelScore(playerid, math_random(10, 30));
    return CancelMining(playerid), 1;
}