new bool:pDuel[MAX_PLAYERS], pDuelEnemy[MAX_PLAYERS], pDuelWeapon[MAX_PLAYERS][2], pDuelTick[MAX_PLAYERS];
new pDuelPoints[MAX_PLAYERS], pDuelRounds[MAX_PLAYERS], bool:pDuelSync[MAX_PLAYERS], bool:pDuelInvite[MAX_PLAYERS];
new pDuelTimer[MAX_PLAYERS], pDuelRoundCount[MAX_PLAYERS], pDuelRank[MAX_PLAYERS];

stock ResetDuelData(playerid)
{
    pDuel[playerid] = pDuelSync[playerid] = pDuelInvite[playerid] = false;
    pDuelEnemy[playerid] = -1;
    pDuelWeapon[playerid] = {-1, -1};
    pDuelRounds[playerid] = pDuelRoundCount[playerid] = pDuelTick[playerid] = 0;
}

enum {
    DUEL_RANK_BRONZE = 0,
    DUEL_RANK_SILVER = 1,
    DUEL_RANK_GOLD,
    DUEL_RANK_DIAMOND,
    DUEL_RANK_CHAMPION
};

stock getDuelPoints(rankid)
{
    new amount;
    switch(rankid) {
        case DUEL_RANK_BRONZE: amount = 100;
        case DUEL_RANK_SILVER: amount = 200;
        case DUEL_RANK_GOLD: amount = 500;
        case DUEL_RANK_DIAMOND: amount = 700;
        case DUEL_RANK_CHAMPION: amount = 1000;
    }
    return amount;
}

stock getDuelName(rankid)
{
    new name[25];
    switch(rankid) {
        case DUEL_RANK_BRONZE: name = "Bronze";
        case DUEL_RANK_SILVER: name = "Silver";
        case DUEL_RANK_GOLD: name = "Gold";
        case DUEL_RANK_DIAMOND: name = "Diamond";
        case DUEL_RANK_CHAMPION: name = "Champion";
    }
    return name;
}

alias:duel("solo")
CMD:duel(playerid, params[])
{
    if(pDuel[playerid] || NieMozeTeraz(playerid)) return 1;
    if(pDuelEnemy[playerid] != -1) return sendError(playerid, ""RED"Zosta³eœ(aœ) ju¿ zaproszony(a) na duel.");
    if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");

    new enemyid, weap1, weap2, rounds, bool:sync;
    if(sscanf(params, "ddddd", enemyid, weap1, weap2, sync, rounds)) {
        rounds = 1;
        if(sscanf(params, "dddd", enemyid, weap1, weap2, sync)) {
            sync = false;
            if(sscanf(params, "ddd", enemyid, weap1, weap2)) {
                return syntaxError(playerid, "/duel [id] [broñ 1] [broñ 2] [0=5/5, 1=7/7], [rundy (opcjonalnie, max: 10)]");
            }
        }
    }

    if(!IsPlayerConnected(enemyid)) return Polaczenie(playerid);
    if(enemyid == playerid) return sendError(playerid, ""RED"Nie mo¿esz zaprosiæ samego siebie na duel.");
    if(!IsValidWeapon(weap1) || !IsValidWeapon(weap2)) return sendError(playerid, ""RED"Podano nieprawid³owe ID broni.");
    if(rounds < 1 || rounds > 10) return sendError(playerid, ""RED"Dozwolona liczba rund to: "WHITE"1-10.");
    if(sync < bool:0 || sync > bool:1) return sendError(playerid, ""RED"Dozwolona synchronizacja to "WHITE"0(5/5) "RED"oraz "WHITE"1(7/7).");

    if(pDuel[enemyid]) return sendError(playerid, ""RED"Ten gracz jest ju¿ na duelu.");
    if(pDuelEnemy[enemyid] != -1) return sendError(playerid, ""RED"Ten gracz jest ju¿ zaproszony na duel.");
    if(!pAccountSettings[enemyid][1]) return sendError(playerid, ""RED"Ten gracz ma wy³¹czone duele.");
    if(NieMozeTeraz(enemyid) || CheckPlayerGame(enemyid)) return sendError(playerid, ""RED"Ten gracz nie mo¿e teraz przyj¹æ zaproszenia na duel.");

    pDuelEnemy[playerid] = enemyid;
    pDuelEnemy[enemyid] = playerid;

    pDuelInvite[playerid] = true;
    pDuelInvite[enemyid] = false;

    pDuelWeapon[playerid][0] = pDuelWeapon[enemyid][0] = weap1;
    pDuelWeapon[playerid][1] = pDuelWeapon[enemyid][1] = weap2;

    pDuelRounds[playerid] = pDuelRounds[enemyid] = rounds;
    pDuelSync[playerid] = pDuelSync[enemyid] = sync;
    pDuelTimer[playerid] = SetTimerEx("RejectDuelTimer", 1000 * 30, false, "ddd", playerid, enemyid, 0);

    if(!pAccountSettings[enemyid][3]) {
        s[0] = EOS;
        catstr(s, C_BLUE, -1, "Gracz {c}%s(%d){/c} wyzywa Ciê na duel na bronie: {c}%s + %s{/c}.\n", getNick(playerid), playerid, WeaponNames[weap1], WeaponNames[weap2]);
        catstr(s, C_BLUE, -1, "Synchronizacja strza³ów: {c}%s{/c}, rundy: {c}%d{/c}.\n", (sync) ? ("7/7") : ("5/5"), rounds);
        catstr(s, C_LY, -1, "Kliknij poni¿ej odpowiedni przycisk, by zaakceptowaæ pojedynek!");

        inline onDialogData(pp, dd, rr, ll, string:ii[]) {
            #pragma unused dd, ll, ii
            if(!rr) return RejectDuelTimer(playerid, enemyid, 1);
            return callcmd::aduel(pp), 1;  }
        Dialog_ShowCallback(enemyid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""YELLOW"Zaproszenie na Duel", s, "Akceptuj", "Odrzuæ");
    }
    else msg(enemyid, C_BLUE, "{c}%s(%d){/c} wyzywa Ciê na duel! "LY"%s + %s, %s, %d rund. "GREEN"(/aduel)", getNick(playerid), playerid, WeaponNames[weap1], WeaponNames[weap2], (sync) ? ("7/7") : ("5/5"), rounds);
    return msg(playerid, C_BLUE, "Wyzwa³eœ(aœ) gracza {c}%s(%d){/c} na duel! "LY"%s + %s, %s, %d rund.", getNick(enemyid), enemyid, WeaponNames[weap1], WeaponNames[weap2], (sync) ? ("7/7") : ("5/5"), rounds), 1;
}

alias:aduel("asolo")
CMD:aduel(playerid)
{
    if(pDuel[playerid] || pDuelInvite[playerid]) return 1;
    if(!pAccountSettings[playerid][1]) return sendError(playerid, ""RED"Masz wy³¹czone duele. W³¹cz je: "WHITE"/konto.");
    if(pDuelEnemy[playerid] == -1) return sendError(playerid, ""RED"Nie otrzyma³eœ(aœ) ¿adnego zaproszenia na duel.");

    new enemyid = pDuelEnemy[playerid];
    if(!pAccountSettings[enemyid][1]) return sendError(playerid, ""RED"Ten gracz ma wy³¹czone duele.");
    if(pDuel[enemyid]) return sendError(playerid, ""RED"Ten gracz jest ju¿ na duelu.");
    return RunDuelMatch(enemyid, playerid);
}

stock RunDuelMatch(playerid, enemyid)
{
    if(pDuel[playerid] || pDuel[enemyid]) return 1;
    pDuel[playerid] = pDuel[enemyid] = true;

    KillTimer(pDuelTimer[playerid]);
    KillTimer(pDuelTimer[enemyid]);

    SavePlayerPosition(playerid);       SavePlayerPosition(enemyid);
    SetPlayerHealth(playerid, 100.0);   SetPlayerHealth(enemyid, 100.0);
    SetPlayerArmour(playerid, 100.0);   SetPlayerArmour(enemyid, 100.0);
    ResetPlayerWeapons(playerid);       ResetPlayerWeapons(enemyid);
    SetCameraBehindPlayer(playerid);    SetCameraBehindPlayer(enemyid);
    UpdateDuelBox(playerid);            UpdateDuelBox(enemyid);

    giveWeapon(playerid, pDuelWeapon[playerid][0], 1000);
    giveWeapon(playerid, pDuelWeapon[playerid][1], 1000);

    giveWeapon(enemyid, pDuelWeapon[enemyid][0], 1000);
    giveWeapon(enemyid, pDuelWeapon[enemyid][1], 1000);

    playerData[playerid][player_god] = false;
    playerData[enemyid][player_god] = false;

    new world = math_random(10000, 15000);
    Teleport(playerid, false, 1413.93, 3.22, 1000.92, 135.0, 1, world, false);
    Teleport(enemyid, false, 1372.25, -35.94, 1000.92, 315.0, 1, world, false);

    for(new i = 0; i < 3; i++) {
        PlayerTextDrawShow(playerid, tdInfo[i]);
        PlayerTextDrawShow(enemyid, tdInfo[i]);
    }

    pDuelTick[playerid] = pDuelTick[enemyid] = GetTickCount();
    return msgAll(C_GOLD, "Duel {c}%s vs %s{/c} rozpoczêty! "LY"(%s + %s, %s, %d rund)", getNick(playerid), getNick(enemyid), WeaponNames[pDuelWeapon[playerid][0]], WeaponNames[pDuelWeapon[playerid][1]], (pDuelSync[playerid]) ? ("7/7") : ("5/5"), pDuelRounds[playerid]);
}

stock UpdateDuelBox(playerid)
{
    new enemyid = pDuelEnemy[playerid];
    playerTextDrawSetString(playerid, tdInfo[1], "~r~~h~~h~POJEDYNEK DUEL");
    playerTextDrawSetString(playerid, tdInfo[2], "~y~~h~%d", pDuelRounds[playerid]);

    new rankstr[128];
    if(pDuelRank[playerid] == DUEL_RANK_CHAMPION) format(rankstr, sizeof(rankstr), "~w~~h~%s ~p~~h~#%d", getDuelName(pDuelRank[playerid]), pDuelPoints[playerid]);
    else format(rankstr, sizeof(rankstr), "~w~~h~%s ~p~~h~(%d/%d)", getDuelName(pDuelRank[playerid]), pDuelPoints[playerid], getDuelPoints(pDuelRank[playerid]));

    playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Punktacja: ~r~~h~%d~w~~h~:~b~~h~~h~%d~n~~w~~h~%s", pDuelRoundCount[playerid], pDuelRoundCount[enemyid], rankstr);
    return 1; 
}

stock CheckDuelKiller(playerid, killerid)
{
    if(pDuel[playerid] && pDuel[killerid] && pDuelEnemy[killerid] == playerid) {
        pDuelRounds[playerid] --;
        pDuelRounds[killerid] --;

        pDuelRoundCount[killerid] ++;
        if(pDuelRounds[killerid] < 1) CheckDuel(playerid, killerid);
        else { UpdateDuelBox(killerid); UpdateDuelBox(playerid); } 
    }
    return 1;
}

stock DuelRespawn(playerid, enemyid)
{
    SetPlayerHealth(playerid, 100.0);   SetPlayerHealth(enemyid, 100.0);
    SetPlayerArmour(playerid, 100.0);   SetPlayerArmour(enemyid, 100.0);
    ResetPlayerWeapons(playerid);       ResetPlayerWeapons(enemyid);
    SetCameraBehindPlayer(playerid);    SetCameraBehindPlayer(enemyid);
    UpdateDuelBox(playerid);            UpdateDuelBox(enemyid);

    giveWeapon(playerid, pDuelWeapon[playerid][0], 1000);
    giveWeapon(playerid, pDuelWeapon[playerid][1], 1000);

    giveWeapon(enemyid, pDuelWeapon[enemyid][0], 1000);
    giveWeapon(enemyid, pDuelWeapon[enemyid][1], 1000);

    new world = math_random(10000, 15000);
    Teleport(playerid, false, 1413.93, 3.22, 1000.92, 135.0, 1, world, false);
    Teleport(enemyid, false, 1372.25, -35.94, 1000.92, 315.0, 1, world, false);
    return 1;
}

stock CheckDuel(playerid, killerid, surrender_playerid = -1)
{
    new winner = -1; 
    if(surrender_playerid == -1) {
        if(pDuelRoundCount[killerid] > pDuelRoundCount[playerid]) winner = killerid;
        else if(pDuelRoundCount[playerid] > pDuelRoundCount[killerid]) winner = playerid;
        else winner = -1; 
    } else {
        if(surrender_playerid == playerid) winner = killerid;
        else if(surrender_playerid == killerid) winner = playerid;
    }

    new time = (GetTickCount() - pDuelTick[playerid]) / 1000;
    if(winner == -1) msgAll(C_BROWN, "{c}%s(%d){/c} remisuje duel z {c}%s(%d){/c} w czasie {c}%02d:%02d{/c}.", getNick(playerid), playerid, getNick(killerid), killerid, (time % 3600) / 60, time % 60);
    else {
        new rounds = pDuelRoundCount[winner];
        if(rounds > 1) msgAll(C_BROWN, "{c}%s(%d){/c} wygrywa duel z {c}%s(%d){/c} w czasie {c}%02d:%02d{/c}. "LY"(%d:%d)", getNick(winner), winner, getNick(pDuelEnemy[winner]), pDuelEnemy[winner], (time % 3600) / 60, time % 60, pDuelRoundCount[winner], pDuelRoundCount[pDuelEnemy[winner]]);
        else {
            new Float:health, Float:armor;
            GetPlayerHealth(winner, health);
            GetPlayerArmour(winner, armor);

            msgAll(C_BROWN, "{c}%s(%d){/c} wygrywa duel z {c}%s(%d){/c} w czasie {c}%02d:%02d{/c}.", getNick(winner), winner, getNick(pDuelEnemy[winner]), pDuelEnemy[winner], (time % 3600) / 60, time % 60);
            msgAll(C_BROWN, "Zwyciêzcy pozosta³o: {c}%.0f HP + %.0f AR{/c}.", health, armor);
        }
        giveMoney(winner, 100);
    
        new 
            old_winner_score = pDuelPoints[winner],
            old_losser_score = pDuelPoints[pDuelEnemy[winner]],

            winner_score = math_random(10, 15),
            losser_score = math_random(7, 10)
        ;

        pDuelPoints[winner] += winner_score;
        pDuelPoints[pDuelEnemy[winner]] -= losser_score;
        
        achievement(winner, ACH_DUEL);
        CheckQuest(winner, DAILY_DUEL);

        CheckBet(winner, BET_DUEL, true);
        CheckBet(pDuelEnemy[winner], BET_DUEL, false);

        CheckDuelRank(winner, old_winner_score, winner_score);
        CheckDuelRank(pDuelEnemy[winner], old_losser_score, losser_score);
    }

    for(new i = 0; i < 3; i++) {
        PlayerTextDrawHide(playerid, tdInfo[i]);
        PlayerTextDrawHide(killerid, tdInfo[i]);
    }

    ResetDuelData(playerid); ResetDuelData(killerid);
    return LoadPlayerPosition(playerid), LoadPlayerPosition(killerid);
}

function RejectDuelTimer(playerid, enemyid, reason)
{
    KillTimer(pDuelTimer[playerid]);
    KillTimer(pDuelTimer[enemyid]);

    switch(reason) {
        case 0: {
            msg(playerid, C_ORANGE, "{c}%s(%d){/c} nie zd¹¿y³(a) zaakceptowaæ propozycji duelu.", getNick(enemyid), enemyid);
            msg(enemyid, C_ORANGE, "Nie zd¹¿y³eœ(aœ) zaakceptowaæ propozycji duelu od: {c}%s(%d){/c}.", getNick(playerid), playerid);
        }

        case 1: {
            msg(playerid, C_ORANGE, "{c}%s(%d){/c} odrzuci³(a) Twoj¹ propozycjê duelu.", getNick(enemyid), enemyid);
            msg(enemyid, C_ORANGE, "Odrzuci³eœ(aœ) propozycjê duelu od: {c}%s(%d){/c}.", getNick(playerid), playerid);
        }
    }

    ResetDuelData(playerid), ResetDuelData(enemyid);
    return 1;
}

stock CheckDuelRank(playerid, score, lost_score)
{
    if(pDuelPoints[playerid] >= getDuelPoints(pDuelRank[playerid]) && pDuelRank[playerid] != DUEL_RANK_CHAMPION) {
        new result = pDuelPoints[playerid] - getDuelPoints(pDuelRank[playerid]);
        pDuelPoints[playerid] = result;
        pDuelRank[playerid] ++;

        msg(playerid, C_VIOLET, "Awansowa³eœ(aœ) do rangi {c}%s (%d/%d){/c} na duelach.", getDuelName(pDuelRank[playerid]), pDuelPoints[playerid], getDuelPoints(pDuelRank[playerid]));
    }
    else if(pDuelPoints[playerid] < 1 && pDuelRank[playerid] != DUEL_RANK_BRONZE) {
        pDuelRank[playerid] --;

        new result = getDuelPoints(pDuelRank[playerid]) + score;
        pDuelPoints[playerid] = result - lost_score;

        msg(playerid, C_VIOLET, "Spad³eœ(aœ) do rangi {c}%s (%d/%d){/c} na duelach.", getDuelName(pDuelRank[playerid]), pDuelPoints[playerid], getDuelPoints(pDuelRank[playerid]));
    }
    else if(pDuelPoints[playerid] < 1 && pDuelRank[playerid] == DUEL_RANK_BRONZE) {
        pDuelRank[playerid] = DUEL_RANK_BRONZE;
        pDuelPoints[playerid] = 0;
    }

    m_pquery_format("UPDATE es_accounts SET duel_rank=%d, duel_points=%d WHERE id=%d LIMIT 1;", pDuelRank[playerid], pDuelPoints[playerid], playerData[playerid][player_accountID]);
    return 1;
}