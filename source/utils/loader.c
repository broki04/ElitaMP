stock LoadPlayerStats(playerid)
{
    new Cache:result = mysql_query(dbconn, form("SELECT * FROM es_accounts WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID])), buffer[255];

    cache_get_value_name(0, "password", playerData[playerid][player_password]);
    cache_get_value_name_int(0, "visits", playerData[playerid][player_visits]);
    cache_get_value_name_int(0, "admin", playerData[playerid][player_adminLoad]);
    cache_get_value_name_int(0, "admin_level", playerData[playerid][player_adminLevel]);
    cache_get_value_name_int(0, "admin_online", playerData[playerid][player_adminTime]);
    cache_get_value_name(0, "admin_pass", playerData[playerid][player_adminPass]);
    cache_get_value_name_int(0, "money", playerData[playerid][player_money]);
    cache_get_value_name_int(0, "suicides", playerData[playerid][player_suicides]);
    cache_get_value_name_int(0, "session", playerData[playerid][player_session]);
    cache_get_value_name_int(0, "gang", pGang[playerid]);
    cache_get_value_name_int(0, "gang_rank_color", pGangRankColor[playerid]);
    cache_get_value_name(0, "gang_rank", pGangRankName[playerid]);
    cache_get_value_name_int(0, "level", playerData[playerid][player_level]);
    cache_get_value_name_int(0, "score", playerData[playerid][player_score]);
    cache_get_value_name_int(0, "score_needed", playerData[playerid][player_score_needed]);
    cache_get_value_name_int(0, "duel_rank", pDuelRank[playerid]);
    cache_get_value_name_int(0, "duel_points", pDuelPoints[playerid]);
    cache_get_value_name_int(0, "skin", playerData[playerid][player_skin]);
    cache_get_value_name_int(0, "nick_timer", playerData[playerid][player_nickTimer]);
    cache_get_value_name_int(0, "spawn_data", playerData[playerid][player_spawnData]);
    cache_get_value_name_int(0, "premium", playerData[playerid][player_premiumTime]);
    cache_get_value_name(0, "own_rank", pOwnRank[playerid]);
    cache_get_value_name_int(0, "own_rank_color", pOwnRankColor[playerid]);
    cache_get_value_name_float(0, "glitch", pGlitchRecord[playerid]);
    cache_get_value_name_int(0, "level_points", playerData[playerid][player_levelPoints]);
    cache_get_value_name_int(0, "warns", playerData[playerid][player_warns]);
    cache_get_value_name_int(0, "jail", playerData[playerid][player_jailtime]);
    cache_get_value_name_int(0, "mute", playerData[playerid][player_mutetime]);
    cache_get_value_name_bool(0, "bonus_vcxz", playerData[playerid][player_bonus_vcxz]);
    cache_get_value_name_int(0, "job_points", playerData[playerid][player_job_points]);
    cache_get_value_name_int(0, "chat", playerData[playerid][player_chatID]);
    cache_get_value_name_int(0, "player_gang_loan", playerData[playerid][player_gang_loan_expire]);
    cache_get_value_name_int(0, "fightstyle", playerData[playerid][player_fightstyle]);
    cache_get_value_name_bool(0, "draw_visible", pDrawVisible[playerid]);
    cache_get_value_name_int(0, "fortune_block", pFortuneBlock[playerid]);
    cache_get_value_name_int(0, "casino_tokens", pCasinoTokens[playerid]);
    cache_get_value_name_int(0, "bank", pBank[playerid]);
    cache_get_value_name_int(0, "credit", pCredit[playerid]);

    // other systems
    cache_get_value_name(0, "gang_rank_permission", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(gangRankPermissions)), pGangRankPerm[playerid]);

    cache_get_value_name(0, "hud", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(hudOptions)), pHud[playerid]);

    cache_get_value_name(0, "staly_pos", buffer);
    sscanf(buffer, "p<,>a<f>[4]", pStalyPos[playerid]);
    cache_get_value_name_int(0, "staly_int", pStalyInterior[playerid]);

    cache_get_value_name(0, "last_pos", buffer);
    sscanf(buffer, "p<,>a<f>[4]", playerData[playerid][player_lastPos]);
    cache_get_value_name_int(0, "last_int", playerData[playerid][player_lastInterior]);

    cache_get_value_name(0, "weapon", buffer);
    sscanf(buffer, "p<,>a<d>[7]", pWeapons[playerid]);

    cache_get_value_name(0, "admin_penalty", buffer);
    sscanf(buffer, "p<,>a<d>[5]", playerData[playerid][player_adminPenalty]);

    cache_get_value_name(0, "license", buffer);
    sscanf(buffer, "p<,>a<d>[3]", pDriveLicense[playerid]);

    // job system
    cache_get_value_name(0, "job_level", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", MAX_JOBS), pJobLevel[playerid]);

    cache_get_value_name(0, "job_score", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", MAX_JOBS), pJobScore[playerid]);

    cache_get_value_name(0, "job_reward", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", MAX_JOBS), pJobReward[playerid]);

    cache_get_value_name(0, "job_ulep", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(jobUlepszeniaData)), pJobUlepszenia[playerid]);

    // arena system
    cache_get_value_name(0, "arena_kills", buffer);
    sscanf(buffer, "p<,>a<d>[4]", pArenaKills[playerid]);

    cache_get_value_name(0, "arena_deaths", buffer);
    sscanf(buffer, "p<,>a<d>[4]", pArenaDeaths[playerid]);

    // achievement system
    cache_get_value_name(0, "ach_count", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(achievementData)), pAchievement[playerid]);

    cache_get_value_name(0, "ach_status", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(achievementData)), pAchievementStatus[playerid]);

    cache_get_value_name(0, "ach_level", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(achievementData)), pAchievementLevel[playerid]);

    cache_get_value_name(0, "ach_done", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(achievementData)), pAchievementDone[playerid]);

    // quest system
    cache_get_value_name_int(0, "daily", pDailyQuest[playerid]);
    cache_get_value_name_bool(0, "daily_done", pDailyDone[playerid]);
    cache_get_value_name_int(0, "daily_status", pDailyStatus[playerid]);
    cache_get_value_name_int(0, "daily_reward", pDailyMoney[playerid]);

    // ulepszenia system
    cache_get_value_name(0, "ulepszenia", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(ulepszeniaData)), pUlepszenia[playerid]);

    cache_get_value_name(0, "ulepszenia_level", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(ulepszeniaLevelData)), pUlepszeniaLevel[playerid]);

    // fish system
    cache_get_value_name_int(0, "fish_rode", pFishRode[playerid]);
    cache_get_value_name_float(0, "fish_zylka", pFishZylka[playerid]);

    cache_get_value_name(0, "fish_bait", buffer);
    sscanf(buffer, "p<,>a<d>[5]", pFishBait[playerid]);

    cache_get_value_name(0, "fish_ulep", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(fishUlepszeniaData)), pFishUlepszenia[playerid]);

    // mine system
    cache_get_value_name_int(0, "pickaxe", MinePickaxeLevel[playerid]);
    cache_get_value_name_int(0, "mine_points", MinePoints[playerid]);
    cache_get_value_name_int(0, "mine_bag", MineBagLevel[playerid]);
    cache_get_value_name(0, "pickaxe_name", MinePickaxeName[playerid]);

    if(MinePickaxeLevel[playerid] > 0) MinePickaxe[playerid] = true;
    if(MineBagLevel[playerid] > 0) MineBag[playerid] = true;

    cache_get_value_name(0, "pickaxe_health", buffer);
    sscanf(buffer, "p<,>a<f>[2]", MinePickaxeHealth[playerid]);

    cache_get_value_name(0, "mine_weight", buffer);
    sscanf(buffer, form("p<,>a<d>[%d]", sizeof(mineData)), MineWeight[playerid]);

    // truck system
    cache_get_value_name_float(0, "truck_dist", pTruckDist[playerid]);
    return cache_delete(result), 1;
}

stock SavePlayerStats(playerid) 
{
    inline onSavePlayerData() return printf(" >>> [SAVE]: Zapisano statystyki %s(%d).", getNick(playerid), playerid);

    if(playerData[playerid][player_admin] > GRACZ) {
        m_pquery_format("UPDATE es_accounts SET admin_online=admin_online+%d WHERE id=%d LIMIT 1;", playerData[playerid][player_online], playerData[playerid][player_accountID]);
    }

    dbstr[0] = EOS;
    format(dbstr, sizeof(dbstr), "\
    UPDATE es_accounts SET \
    visits=%d, ip_last='%s', date_last='%s', money=%d, suicides=%d, hud='%s', session=session+%d, \
    gang=%d, level=%d, score=%d, score_needed=%d, skin=%d, nick_timer=%d, premium=%d, serial_last='%s', arena_kills='%s', \
    arena_deaths='%s', level_points=%d, ach_count='%s', ach_status='%s', ach_level='%s', ach_done='%s', daily_status=%d, warns=%d, jail=%d, mute=%d, \
    bonus_vcxz=%d, admin_penalty='%d, %d, %d, %d, %d', fish_rode=%d, fish_zylka=%.1f, fish_bait='%s', \
    mine_weight='%s', pickaxe=%d, pickaxe_health='%.2f, %.2f', mine_points=%d, truck_dist=%.1f, casino_tokens=%d \
    WHERE id=%d LIMIT 1;", 
        playerData[playerid][player_visits],
        playerData[playerid][player_ipaddress],
        getLocalDate(),
        playerData[playerid][player_money],
        playerData[playerid][player_suicides],
        getHud(playerid),
        playerData[playerid][player_online],
        pGang[playerid],
        playerData[playerid][player_level],
        playerData[playerid][player_score],
        playerData[playerid][player_score_needed],
        playerData[playerid][player_skin],
        playerData[playerid][player_nickTimer],
        playerData[playerid][player_premiumTime],
        getSerial(playerid),
        getArenaKills(playerid),
        getArenaDeaths(playerid),
        playerData[playerid][player_levelPoints],
        GetAchievementCount(playerid), GetAchievementStatus(playerid), GetAchievementLevel(playerid), GetAchievementDone(playerid),
        pDailyStatus[playerid],
        playerData[playerid][player_warns],
        playerData[playerid][player_jailtime],
        playerData[playerid][player_mutetime],
        playerData[playerid][player_bonus_vcxz],
        unpackXYZR(playerData[playerid][player_adminPenalty]), playerData[playerid][player_adminPenalty][4], 
        pFishRode[playerid], pFishZylka[playerid], GetFishBaits(playerid),
        GetMineWeight(playerid), MinePickaxeLevel[playerid], MinePickaxeHealth[playerid][0], MinePickaxeHealth[playerid][1], MinePoints[playerid],
        pTruckDist[playerid], pCasinoTokens[playerid],
        playerData[playerid][player_accountID]
    );

    m_pquery_inline(dbconn, using inline onSavePlayerData, dbstr);
    return 1;
}