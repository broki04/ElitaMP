stock addJobScore(playerid, amount)
{
    new jobid = pJob[playerid];
    pJobScore[playerid][jobid] += amount;

    if(pJobScore[playerid][jobid] >= JOB_SCORE_NEEDED) {
        pJobLevel[playerid][jobid] ++;
        pJobScore[playerid][jobid] = 0;

        playerData[playerid][player_job_points] ++;
        msg(playerid, C_BROWN, "Awansowa³eœ(aœ) na {c}%s{/c} poziom pracy! "LY"(%d pkt)", strdot(pJobLevel[playerid][jobid]), playerData[playerid][player_job_points]);
    }
    m_pquery_format("UPDATE es_accounts SET job_level='%s', job_score='%s', job_points=%d WHERE id=%d LIMIT 1;", getJobLevel(playerid), getJobScore(playerid), playerData[playerid][player_job_points], playerData[playerid][player_accountID]);
    return 1;
}

stock getJobLevel(playerid)
{
    new jobstr[90];
    for(new i = 0; i < MAX_JOBS; i++) format(jobstr, sizeof(jobstr), "%s%s%d", jobstr, i == 0 ? ("") : (","), pJobLevel[playerid][i]);
    return jobstr;
}

stock getJobScore(playerid)
{
    new jobstr[90];
    for(new i = 0; i < MAX_JOBS; i++) format(jobstr, sizeof(jobstr), "%s%s%d", jobstr, i == 0 ? ("") : (","), pJobScore[playerid][i]);
    return jobstr;
}
