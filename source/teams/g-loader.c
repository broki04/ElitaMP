stock FetchGang(gangid, loopid, bool:query = false)
{
    new Cache:result;
    if(query) result = mysql_query(dbconn, form("SELECT * FROM es_gangs WHERE id=%d LIMIT 1;", gangid));

    cache_get_value_name(loopid, "name", gangData[gangid][gang_name]);
    cache_get_value_name(loopid, "tag", gangData[gangid][gang_tag]);
    cache_get_value_name_int(loopid, "color", gangData[gangid][gang_color]);
    cache_get_value_name_int(loopid, "ownerid", gangData[gangid][gang_ownerid]);
    cache_get_value_name_int(loopid, "respect", gangData[gangid][gang_respect]);
    cache_get_value_name_int(loopid, "respect_needed", gangData[gangid][gang_respect_needed]);
    cache_get_value_name_int(loopid, "level", gangData[gangid][gang_level]);
    cache_get_value_name_int(loopid, "slots", gangData[gangid][gang_slots]);
    cache_get_value_name(loopid, "default_rank", gangData[gangid][gang_rankDefault]);
    cache_get_value_name_int(loopid, "sparing_points", pGangSparRankedScore[gangid]);
    cache_get_value_name_int(loopid, "sparing_rank", pGangSparRank[gangid]);
    cache_get_value_name_int(loopid, "kills", gangData[gangid][gang_kills]);
    cache_get_value_name_int(loopid, "deaths", gangData[gangid][gang_deaths]);
    cache_get_value_name_int(loopid, "budget", gangData[gangid][gang_budget]);

    new buffer[90];
    cache_get_value_name(loopid, "ach_done", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(gangAchievementData)), pGangAchievement[gangid]);

    cache_get_value_name(loopid, "ach_status", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(gangAchievementData)), pGangAchievementStatus[gangid]);

    cache_get_value_name(loopid, "sparing", buffer);
    sscanf(buffer, "p<,>ddd", gangData[gangid][gang_spar_win], gangData[gangid][gang_spar_loss], gangData[gangid][gang_spar_remis]);

    cache_get_value_name(loopid, "ulepszenia", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(GangUlepszeniaData)), GangUlepszeniaLevel[gangid]);

    gangRankingPlace ++;
    gangData[gangid][gang_place] = gangRankingPlace;
    gangData[gangid][gang_battleZone] = -1;

    CreateGangDesign(gangid);
    Sparing_Init(gangid);

    Iter_Add(Gangs, gangid);
    return (query) ? (cache_delete(result)) : (1);
}

stock Init_Gangs()
{
    Iter_Init(Gangs);
    inline onLoadGangs()
    {
        new id = -1;
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "id", id);
            FetchGang(id, i, false);
        }

        Init_GangZones();
        
        printf(" >>> [GANGS]: Wczytanych gangów: %d", Iter_Count(Gangs));
    }

    m_tquery_inline(dbconn, using inline onLoadGangs, "SELECT * FROM es_gangs ORDER BY respect DESC");
    return 1;
}

stock CreateGangDesign(i) 
{
    gangData[i][gang_draw][0] = TextDrawCreate(458.000030, 9.140820, "Paris Samp-Germain [PSG]");
    TextDrawLetterSize(gangData[i][gang_draw][0], 0.144666, 0.890666);
    TextDrawTextSize(gangData[i][gang_draw][0], 0.000000, 670.000000);
    TextDrawAlignment(gangData[i][gang_draw][0], 2);
    TextDrawColor(gangData[i][gang_draw][0], -1);
    TextDrawSetShadow(gangData[i][gang_draw][0], 0);
    TextDrawSetOutline(gangData[i][gang_draw][0], 1);
    TextDrawBackgroundColor(gangData[i][gang_draw][0], 255);
    TextDrawFont(gangData[i][gang_draw][0], 1);
    TextDrawSetProportional(gangData[i][gang_draw][0], true);

    gangData[i][gang_draw][1] = TextDrawCreate(456.333190, 26.562932, "0/100 (1 lvl)");
    TextDrawLetterSize(gangData[i][gang_draw][1], 0.136333, 0.608592);
    TextDrawTextSize(gangData[i][gang_draw][1], 0.000000, 180.000000);
    TextDrawAlignment(gangData[i][gang_draw][1], 2);
    TextDrawColor(gangData[i][gang_draw][1], -1);
    TextDrawSetShadow(gangData[i][gang_draw][1], 0);
    TextDrawSetOutline(gangData[i][gang_draw][1], 1);
    TextDrawBackgroundColor(gangData[i][gang_draw][1], 255);
    TextDrawFont(gangData[i][gang_draw][1], 1);
    TextDrawSetProportional(gangData[i][gang_draw][1], true);

    gangData[i][gang_draw][2] = TextDrawCreate(452.999938, 36.933345, "broki~n~vester~n~lou~n~tenshin~n~seth");
    TextDrawLetterSize(gangData[i][gang_draw][2], 0.137000, 0.703999);
    TextDrawTextSize(gangData[i][gang_draw][2], 0.000000, 66.000000);
    TextDrawAlignment(gangData[i][gang_draw][2], 2);
    TextDrawColor(gangData[i][gang_draw][2], -1);
    TextDrawUseBox(gangData[i][gang_draw][2], true);
    TextDrawBoxColor(gangData[i][gang_draw][2], 35);
    TextDrawSetShadow(gangData[i][gang_draw][2], 0);
    TextDrawSetOutline(gangData[i][gang_draw][2], 1);
    TextDrawBackgroundColor(gangData[i][gang_draw][2], 255);
    TextDrawFont(gangData[i][gang_draw][2], 1);
    TextDrawSetProportional(gangData[i][gang_draw][2], true);

    TextDrawColor(gangData[i][gang_draw][0], getGangColor(i));
    TextDrawColor(gangData[i][gang_draw][1], getGangColor(i));

    gangData[i][gang_budgetDraw][0] = TextDrawCreate(480.000030, 32.370403, "+10 pkt.");
    TextDrawLetterSize(gangData[i][gang_budgetDraw][0], 0.132332, 0.650072);
    TextDrawTextSize(gangData[i][gang_budgetDraw][0], 719.000000, 0.000000);
    TextDrawAlignment(gangData[i][gang_budgetDraw][0], 1);
    TextDrawColor(gangData[i][gang_budgetDraw][0], -1);
    TextDrawSetShadow(gangData[i][gang_budgetDraw][0], 0);
    TextDrawSetOutline(gangData[i][gang_budgetDraw][0], 1);
    TextDrawBackgroundColor(gangData[i][gang_budgetDraw][0], 255);
    TextDrawFont(gangData[i][gang_budgetDraw][0], 1);
    TextDrawSetProportional(gangData[i][gang_budgetDraw][0], true);

    gangData[i][gang_budgetDraw][1] = TextDrawCreate(479.666595, 39.007442, "+1.000.000$");
    TextDrawLetterSize(gangData[i][gang_budgetDraw][1], 0.132332, 0.650072);
    TextDrawTextSize(gangData[i][gang_budgetDraw][1], 719.000000, 0.000000);
    TextDrawAlignment(gangData[i][gang_budgetDraw][1], 1);
    TextDrawColor(gangData[i][gang_budgetDraw][1], -1);
    TextDrawSetShadow(gangData[i][gang_budgetDraw][1], 0);
    TextDrawSetOutline(gangData[i][gang_budgetDraw][1], 1);
    TextDrawBackgroundColor(gangData[i][gang_budgetDraw][1], 255);
    TextDrawFont(gangData[i][gang_budgetDraw][1], 1);
    TextDrawSetProportional(gangData[i][gang_budgetDraw][1], true);
    return 1;
}
