#include YSI_Coding\y_hooks
    
#define TRYG3D_MOD_PLAYER       true

stock CountPlayerBaits(playerid) 
{
    new count = 0;
    for(new i = 0; i < sizeof(fishBaitData); i++) count += pFishBait[playerid][i];
    return count;
}

stock GetFishClass(fishid, bool:alt = false)
{
    new name[20];
    switch(fishData[fishid][fish_class]) {
        case FISH_CLASS_LEGENDARY:  name = (alt) ? ("legendarnej") : ("legendarna");
        case FISH_CLASS_RARE:       name = (alt) ? ("rzadkiej") : ("rzadka");
        case FISH_CLASS_REGULAR:    name = (alt) ? ("pospolitej") : ("pospolita");
    }
    return name;
}

alias:low("fish")
CMD:low(playerid)
{
    if(!CA_IsPlayerNearWater(playerid, 1.0)) return sendError(playerid, ""RED"Nie jesteœ w pobli¿u wody.");
    if(IsPlayerSwimming(playerid)) return sendError(playerid, ""RED"Nie mo¿esz ³owiæ ryb, gdy jesteœ w wodzie.");    
    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return sendError(playerid, ""RED"Musisz byæ na nogach.");

    if(pFishing[playerid]) return sendError(playerid, ""RED"£owisz ju¿ ryby.");
    if(pFishRode[playerid] == -1) return sendError(playerid, ""RED"Nie posiadasz wêdki! Zakupisz j¹ na: "YELLOW"/lowisko"RED".");
    if(pFishZylka[playerid] <= 0.0) return sendError(playerid, ""RED"Twoja ¿y³ka jest zu¿yta. Naprawisz j¹ na: "YELLOW"/lowisko"RED".");
    if(CountPlayerBaits(playerid) < 1) return sendError(playerid, ""RED"Nie posiadasz ¿adnych przynêt. Zakupisz je na: "YELLOW"/lowisko"RED".");

    new x = 0;
    s = ""WHITE"Przynêta\t"LY"Iloœæ\n";
    for(new i = 0; i < sizeof(fishBaitData); i++) {
        if(pFishBait[playerid][i] > 0) {
            format(s, sizeof(s), "%s"WHITE"%s\t"LY"x%d\n", s, fishBaitData[i][fish_bait_name], pFishBait[playerid][i]);
            playerData[playerid][player_saveData][x++] = i;
        }
    }

    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, ii

        if(!rr) return 1;
        new baitid = playerData[playerid][player_saveData][ll];
        if(pFishBait[playerid][baitid] < 1) return sendError(playerid, ""RED"Nie posiadasz tej przynêty. Zakupisz j¹ na: "YELLOW"/lowisko"RED".");
        if(pFishBaitUsed[playerid] != -1) return sendError(playerid, ""RED"Wykorzysta³eœ(aœ) ju¿ jak¹œ przynêtê.");

        pFishBait[playerid][baitid] --;
        pFishBaitUsed[playerid] = baitid;

        TogglePlayerControllable(playerid, false);
        GameTextForPlayer(playerid, "~n~~n~~n~~y~ZACIAGANIE ZYLKI~r~...", 2000, 3);
        return SetTimerEx("OnPlayerFishing", 1000 * 2, false, "d", playerid);
    }
    return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Wybierz przynêtê", s, "Wybierz", "Anuluj"), 1;
}

function OnPlayerFishing(playerid)
{
    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || !CA_IsPlayerNearWater(playerid)) return 1;

    pFishing[playerid] = true;
    pFishStatus[playerid] = 10.0;

    new fishid = random(sizeof(fishData));
    playerData[playerid][player_saveData][0] = fishid;

    SetPlayerProgressBarMaxValue(playerid, FishBar[playerid], 100.0);
    SetPlayerProgressBarValue(playerid, FishBar[playerid], pFishStatus[playerid]);
    ShowPlayerProgressBar(playerid, FishBar[playerid]);

    new Float:chance = 0.0, baitid = pFishBaitUsed[playerid];
    switch(fishData[fishid][fish_class]) {
        case FISH_CLASS_LEGENDARY:  chance = (fishEvent) ? (math_random_float(80.0, 95.0) + fishBaitData[baitid][fish_bait_chance]) : (((pFishUlepszenia[playerid][FISH_LEGENDARY]) ? (math_random_float(10.0, 20.0)) : (math_random_float(5.0, 10.0))) + fishBaitData[baitid][fish_bait_chance]);
        case FISH_CLASS_RARE:       chance = math_random_float(30.0, 50.0) + fishBaitData[baitid][fish_bait_chance];
        case FISH_CLASS_REGULAR:    chance = math_random_float(70.0, 90.0) + fishBaitData[baitid][fish_bait_chance];
    }
    SetPVarFloat(playerid, "player_fish_float", chance);

    playerTextDrawSetString(playerid, tdFishing[0], "~y~PROCES LOWIENIA");
    playerTextDrawSetString(playerid, tdFishing[1], "~w~~h~Aby lowic, klikaj ~r~~h~LPM~n~~w~~h~Lowisz teraz: ~r~~h~%s ~r~~h~~h~(kl. %s)~n~~w~~h~Szansa na wylowienie: ~y~~h~%.0f%", DeletePolish(fishData[fishid][fish_name]), GetFishClass(fishid), chance);
    for(new i = 0; i < 2; i++) PlayerTextDrawShow(playerid, tdFishing[i]);

    SetPlayerAttachedObject(playerid, 5, 18632, 6, 0.079376, 0.037070, 0.007706, 181.482910, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000);
    ApplyAnimation(playerid, "SWORD", "sword_block", 50.0, false, true, false, true, 1);
    return TogglePlayerControllable(playerid, false), 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_YES) && !IsPlayerInAnyVehicle(playerid) && CA_IsPlayerNearWater(playerid, 1.0) && !pFishing[playerid]) return callcmd::low(playerid);
    
    if(PRESSED(KEY_FIRE) && pFishing[playerid]) {
        new rodeid = pFishRode[playerid];
        pFishStatus[playerid] += fishRodeData[rodeid][fish_rode_amount];

        SetPlayerProgressBarMaxValue(playerid, FishBar[playerid], 100.0);
        SetPlayerProgressBarValue(playerid, FishBar[playerid], pFishStatus[playerid]);
        ApplyAnimation(playerid, "SWORD", "sword_block", 50.0, false, true, false, true, 1);

        if(pFishStatus[playerid] >= 100.0) {
            new fishid = playerData[playerid][player_saveData][0], Float:chance = GetPVarFloat(playerid, "player_fish_float");
            playerTextDrawSetString(playerid, tdFishing[1], "~w~~h~Aby lowic, klikaj ~r~~h~LPM~n~~w~~h~Lowisz teraz: ~r~~h~%s ~r~~h~~h~(kl. %s)~n~~w~~h~Szansa na wylowienie: ~y~~h~%.0f%", DeletePolish(fishData[fishid][fish_name]), GetFishClass(fishid), chance);

            if(percent_float(chance)) {
                new Float:weight = math_random_float(fishData[fishid][fish_weight][0], fishData[fishid][fish_weight][1]);
                new money = fishData[fishid][fish_money] + ((pFishUlepszenia[playerid][FISH_REWARD]) ? (floatround(fishData[fishid][fish_money] * 0.50)) : (0)) + ((fishEvent) ? (floatround(fishData[fishid][fish_money] * 0.50)) : (0));

                giveMoney(playerid, money);
                addLevelScore(playerid, math_random(30, 50));
                achievement(playerid, ACH_RYBAK);

                s[0] = EOS;
                format(s, sizeof(s), ""WHITE"Wy³owi³eœ(aœ) %d rybê: "RED"%s "LY"(%.2f kg, kl. %s)\n"WHITE"Wartoœæ ryby: "BLUE"%s$.", GetAchievement(playerid, ACH_RYBAK), fishData[fishid][fish_name], weight, GetFishClass(fishid), strdot(money));
                showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"Udany po³ów", s, "OK", #);
            }
            else dialogBox(playerid, C_RED, ""RED"Niestety, nie uda³o Ci siê z³owiæ ryby! Spróbuj ponownie.");

            pFishZylka[playerid] -= math_random_float(3.0, 7.0);
            pFishing[playerid] = false;
            pFishStatus[playerid] = 0.0;
            pFishBaitUsed[playerid] = -1;

            for(new i = 0; i < 2; i++) PlayerTextDrawHide(playerid, tdFishing[i]);
            HidePlayerProgressBar(playerid, FishBar[playerid]);

            ClearAnimations(playerid);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            RemovePlayerAttachedObject(playerid, 5);
            return TogglePlayerControllable(playerid, true), 1;
        }
    }
    return 1;
}

stock FishTimer(playerid)
{
    if(!pFishing[playerid]) return 1;
    pFishStatus[playerid] -= 3.0;

    SetPlayerProgressBarMaxValue(playerid, FishBar[playerid], 100.0);
    SetPlayerProgressBarValue(playerid, FishBar[playerid], pFishStatus[playerid]);

    if(pFishStatus[playerid] <= 0.0) {
        pFishing[playerid] = false;
        pFishStatus[playerid] = 0.0;
        pFishBaitUsed[playerid] = -1;

        for(new i = 0; i < 2; i++) PlayerTextDrawHide(playerid, tdFishing[i]);
        HidePlayerProgressBar(playerid, FishBar[playerid]);

        ClearAnimations(playerid);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        RemovePlayerAttachedObject(playerid, 5);
        TogglePlayerControllable(playerid, true);
        return dialogBox(playerid, C_RED, ""RED"Ryba, któr¹ próbowa³eœ(aœ) z³owiæ, uciek³a Ci!");    
    }
    return 1;
}