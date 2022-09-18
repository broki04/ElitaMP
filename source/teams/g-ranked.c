stock getRankingPoints(rankid) {
    new amount = 0;
    switch(rankid) {
        case SPARING_RANK_BRONZE: amount = 100;
        case SPARING_RANK_SILVER: amount = 200;
        case SPARING_RANK_GOLD: amount = 300;
        case SPARING_RANK_PLATINUM: amount = 500;
        case SPARING_RANK_DIAMOND: amount = 700;
        case SPARING_RANK_CHAMPION: amount = 1000;
    }
    return amount;
}

stock getRankingName(rankid) {
    new name[25];
    switch(rankid) {
        case SPARING_RANK_BRONZE: name = "Bronze";
        case SPARING_RANK_SILVER: name = "Silver";
        case SPARING_RANK_GOLD: name = "Gold";
        case SPARING_RANK_PLATINUM: name = "Platinum";
        case SPARING_RANK_DIAMOND: name = "Diamond";
        case SPARING_RANK_CHAMPION: name = "Champion";
    }
    return name;
}

stock CheckRankedLevel(gangid, score, lost_score)
{
    if(pGangSparRankedScore[gangid] >= getRankingPoints(pGangSparRank[gangid]) && pGangSparRank[gangid] != SPARING_RANK_CHAMPION) {
        new result = pGangSparRankedScore[gangid] - getRankingPoints(pGangSparRank[gangid]);
        pGangSparRankedScore[gangid] = result;
        pGangSparRank[gangid] ++;

        foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ awansowa³ do rangi: {c}%s{/c}. {c}(%d/%d)", getGangTag(gangid), getRankingName(pGangSparRank[gangid]), pGangSparRankedScore[gangid], getRankingPoints(pGangSparRank[gangid]));
    }
    else if(pGangSparRankedScore[gangid] < 1 && pGangSparRank[gangid] != SPARING_RANK_BRONZE) {
        pGangSparRank[gangid] --;

        new result = getRankingPoints(pGangSparRank[gangid]) + score;
        pGangSparRankedScore[gangid] = result - lost_score;

        foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ spad³ do rangi: {c}%s{/c}. {c}(%d/%d)", getGangTag(gangid), getRankingName(pGangSparRank[gangid]), pGangSparRankedScore[gangid], getRankingPoints(pGangSparRank[gangid]));
    }
    else if(pGangSparRankedScore[gangid] < 1 && pGangSparRank[gangid] == SPARING_RANK_BRONZE) {
        pGangSparRank[gangid] = SPARING_RANK_BRONZE;
        pGangSparRankedScore[gangid] = 0;
    }

    m_pquery_format("UPDATE es_gangs SET sparing_points=%d, sparing_rank=%d WHERE id=%d LIMIT 1;", pGangSparRankedScore[gangid], pGangSparRank[gangid], gangid);
    return 1;
}

stock CheckRankedMatch(winner, losser, win_score, lost_score)
{
    new winner_old_score = pGangSparRankedScore[winner];
    new losser_old_score = pGangSparRankedScore[losser];

    pGangSparRankedScore[winner] += win_score;
    pGangSparRankedScore[losser] -= lost_score;

    foreach(new i : GangMember[winner]) {
        textDrawSetString(gangData[winner][gang_budgetDraw][0], "~g~~h~~h~+~w~~h~%d pkt.", win_score);
        TextDrawShowForPlayer(i, gangData[winner][gang_budgetDraw][0]);
        playSound(i, 6401);
    }

    foreach(new i : GangMember[losser]) {
        textDrawSetString(gangData[losser][gang_budgetDraw][0], "~r~~h~-~w~~h~%d pkt.", win_score);
        TextDrawShowForPlayer(i, gangData[losser][gang_budgetDraw][0]);
        playSound(i, 1190);
    }
    SetTimerEx("HideRankedPoints", 1000 * 3, false, "dd", winner, losser);

    CheckGangAchievement(winner, GANG_ACHIEVEMENT_RANKED);
    CheckRankedLevel(winner, winner_old_score, lost_score);
    CheckRankedLevel(losser, losser_old_score, lost_score);
}
function HideRankedPoints(gangid, enemyid) {
    foreach(new i : GangMember[gangid]) TextDrawHideForPlayer(i, gangData[gangid][gang_budgetDraw][0]);
    foreach(new i : GangMember[enemyid]) TextDrawHideForPlayer(i, gangData[enemyid][gang_budgetDraw][0]);
    return 1;
}