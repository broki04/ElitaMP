#include YSI_Coding\y_hooks

#define JOB_MAGAZYNIER      0
#define JOB_GRABARZ         1
#define JOB_ZRZUT           2
#define JOB_TAXI            3
#define JOB_MYSLIWY         4
#define JOB_DRWAL           5
#define JOB_GRZYBIARZ       6

#define MAX_JOBS            (JOB_GRZYBIARZ + 1)
#define JOB_SCORE_NEEDED    500

enum e_jobUlepszeniaData {
    job_ulep_id,
    job_ulep_name[25],
    job_ulep_points
};

#define JOB_NITRO           0
#define JOB_FIX             1
#define JOB_CHEATS          2

new const jobUlepszeniaData[][e_jobUlepszeniaData] = {
    {JOB_NITRO,     "Nitro",        6},
    {JOB_FIX,       "/Napraw",      8},
    {JOB_CHEATS,    "/Cheats",      10}
};

new 
    pJob[MAX_PLAYERS] = -1, 

    pJobLevel[MAX_PLAYERS][MAX_JOBS], 
    pJobScore[MAX_PLAYERS][MAX_JOBS],
    pJobReward[MAX_PLAYERS][MAX_JOBS],
    bool:pJobUlepszenia[MAX_PLAYERS][sizeof(jobUlepszeniaData)],

    JobDay,
    magazynierPickup
;

hook function ResetPlayerData(playerid)
{
    pJob[playerid] = -1;
    for(new i = 0; i < sizeof(jobUlepszeniaData); i++) pJobUlepszenia[playerid][i] = false;

    for(new i = 0; i < MAX_JOBS; i++) {
        pJobLevel[playerid][i] = 1;
        pJobScore[playerid][i] = pJobReward[playerid][i] = 0;
    }
    return continue(playerid);
}

stock getJobName(jobid)
{
    new name[25];
    switch(jobid) {
        case JOB_MAGAZYNIER:    name = "Magazynier";
        case JOB_GRABARZ:       name = "Grabarz";
        case JOB_ZRZUT:         name = "Zrzut paczki";
        case JOB_TAXI:          name = "Taksowkarz";
        case JOB_MYSLIWY:       name = "Mysliwy";
        case JOB_DRWAL:         name = "Drwal";
        case JOB_GRZYBIARZ:     name = "Grzybiarz";
    }
    return name;
}

stock GetJobUlepszenia(playerid)
{
    new jstr[128];
    for(new i = 0; i < sizeof(jobUlepszeniaData); i++) format(jstr, sizeof(jstr), "%s%s%i", jstr, i == 0 ? ("") : (","), pJobUlepszenia[playerid][i]);
    return jstr;
}

stock Init_Jobs()
{
    JobDay = random(MAX_JOBS);

    // urz¹d pracy
    CreateDynamicActor(17, 2640.1563, 1969.1239, 10.8319, 0.1270, .worldid=0, .interiorid=0);
    CreateDynamic3DTextLabel(""GOLD"Pracodawca\n"WHITE"Aby podj¹æ siê pracy, kliknij: "YELLOW"SPACJA + Y", -1, 2640.1563, 1969.1239, 10.8319 + 0.35, 30.0, .worldid=0, .interiorid=0);

    // magazynier
    magazynierPickup = CreateDynamicPickup(1575, 1, 1346.6949, 1034.9030, 1190.5111);
    CreateDynamic3DTextLabel(""BLUE"Punkty zaopatrzenia\n"WHITE"WejdŸ w "RED"czerwone kó³ko"WHITE", aby zebraæ towar.", -1, 1346.6949, 1034.9030, 1190.5111, 30.0);
    return printf(" >>> [JOBS]: Wczytanych prac: %d (praca dnia: %s)", MAX_JOBS, getJobName(JobDay)), 1;
}

stock CountJobUlepszenia(playerid)
{
    new count = 0;
    for(new i = 0; i < sizeof(jobUlepszeniaData); i++) { if(pJobUlepszenia[playerid][i]) count ++; }
    return count;
}

stock Task:RunJobPanel(playerid)
{
    new Task:t = task_new();

    s[0] = EOS;
    s = ""WHITE"Polecenie\t"BLUE"Rezultat\n";
    catstr(s, C_WHITE, -1, "Lista dostêpnych prac\n");
    catstr(s, C_WHITE, -1, "Podium najlepszych pracowników\n");
    catstr(s, C_WHITE, -1, "Premiowana praca dnia:\t"YELLOW"%s\n", getJobName(JobDay));
    catstr(s, C_WHITE, -1, "Zwiêksz wynagrodzenie:\t"RED"%d pkt.\n", playerData[playerid][player_job_points]);
    catstr(s, C_WHITE, -1, "Ulepszenia pracy:\t"YELLOW"%d/%d\n", CountJobUlepszenia(playerid), sizeof(jobUlepszeniaData));
    strcat(s, " \n");

    for(new i = 0; i < MAX_JOBS; i++) catstr(s, C_WHITE, -1, ""LGREEN"%d. "WHITE"%s\t"LY"(poz. %d, %d/%d exp)\n", i + 1, getJobName(i), pJobLevel[playerid][i], pJobScore[i], JOB_SCORE_NEEDED);
    return showDialog(playerid, DIALOG_PRACA_PANEL, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Panel pracy", s, "Wybierz", "Zamknij"), t;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) 
{
    if(PRESSED(KEY_YES | KEY_SPRINT) && IsPlayerInRangeOfPoint(playerid, 3.0, 2640.1563, 1969.1239, 10.8319)) return task_await(RunJobPanel(playerid));
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason) 
{
    if(pJob[playerid] != -1) return ExitPraca(playerid);
    return 1;
}