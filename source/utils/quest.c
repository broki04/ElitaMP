enum e_questListData {
    daily_id,
    daily_name[255],
    daily_required,
};

#define DAILY_PRACA             0
#define DAILY_KILLS             1
#define DAILY_DUEL              2
#define DAILY_ARENA             3
#define DAILY_QUIZ              4
#define DAILY_DISKS             5
#define DAILY_FIGURKA           6

new const questListData[][e_questListData] = {
    {DAILY_PRACA,           "Wykonaj 30 razy dowolna prace",        30},
    {DAILY_KILLS,           "Zabij 100 osob dowolna bronia",        100},
    {DAILY_DUEL,            "Wygraj 20 dueli dowolna bronia",       20},
    {DAILY_ARENA,           "Zabij 70 graczy na dowolnej arenie",   70},
    {DAILY_QUIZ,            "Rozwiaz 15 testow reakcji",            15},
    {DAILY_DISKS,           "Znajdz 30 dyskietek na mapie",         30},
    {DAILY_FIGURKA,         "Znajdz figurke na mapie",              1}
};

new 
    pDailyQuest[MAX_PLAYERS] = -1,
    pDailyMoney[MAX_PLAYERS],
    pDailyStatus[MAX_PLAYERS],
    bool:pDailyDone[MAX_PLAYERS]
;

stock ResetQuestData(playerid)
{
    pDailyQuest[playerid] = -1;
    pDailyMoney[playerid] = pDailyStatus[playerid] = 0;
    pDailyDone[playerid] = false;
}

stock GetQuestName(playerid)
{
    new name[128];
    if(pDailyDone[playerid]) name = "Zadanie wykonane";
    else format(name, sizeof(name), "%s", questListData[pDailyQuest[playerid]][daily_name]);
    return name;
}

stock SetRandomQuest(playerid)
{
    if(pDailyQuest[playerid] == -1 && !pDailyDone[playerid]) {
        pDailyQuest[playerid] = random(sizeof(questListData));
        
        if(pUlepszeniaLevel[playerid][ULEPSZENIE_QUEST] > 0) {
            new money = math_random(10000, 30000);

            new Float:chuj = 0.0;
            switch(pUlepszeniaLevel[playerid][ULEPSZENIE_QUEST]) {
                case 1: chuj = 0.10;
                case 2: chuj = 0.25;
                case 3: chuj = 0.35;
                case 4: chuj = 0.50;
                case 5: chuj = 0.75;
            }
            pDailyMoney[playerid] = floatround(money * chuj) + money;
        } 
        else pDailyMoney[playerid] = math_random(10000, 30000);
        pDailyStatus[playerid] = 0;

        PlayerTextDrawSetString(playerid, tdQuest, form("ZADANIE DNIA:~n~~w~~h~%s", GetQuestName(playerid)));
        msg(playerid, C_BLUE2, "Wylosowano nowe zadanie dnia: {c}%s{/c}. Nagroda: {c}%s${/c}.", GetQuestName(playerid), strdot(pDailyMoney[playerid]));
        m_pquery_format("UPDATE es_accounts SET daily=%d, daily_done=0, daily_status=0, daily_reward=%d WHERE id=%d LIMIT 1;", pDailyQuest[playerid], pDailyMoney[playerid], playerData[playerid][player_accountID]);
    }
}

stock CheckQuest(playerid, questid)
{
    if(questid == -1 || pDailyQuest[playerid] != questid) return 1;

    pDailyStatus[playerid] ++;
    if(pDailyStatus[playerid] >= questListData[questid][daily_required] && !pDailyDone[playerid]) {
        msg(playerid, C_LORANGE, "Wykona³eœ(aœ) zadanie dnia: {c}%s{/c}. Nagroda: {c}%s${/c}.", GetQuestName(playerid), strdot(pDailyMoney[playerid]));
        
        pDailyDone[playerid] = true;
        giveMoney(playerid, pDailyMoney[playerid]);
        addLevelScore(playerid, math_random(100, 500));

        PlayerTextDrawHide(playerid, tdQuest);
        m_pquery_format("UPDATE es_accounts SET daily_done=1 WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID]);
    }
    return 1;
}

alias:quest("daily", "challenge", "zadanie")
CMD:quest(playerid)
{
    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Zadanie dnia: "BLUE"%s\n", (pDailyDone[playerid]) ? ("Zadanie wykonane") : (form("%s "LRED"[%d/%d]", GetQuestName(playerid), pDailyStatus[playerid], questListData[pDailyQuest[playerid]][daily_required])));
    format(s, sizeof(s), "%s"WHITE"Nagroda za zadanie: "LGREEN"%s\n", s, (pDailyDone[playerid]) ? ("---") : (form("%s$", strdot(pDailyMoney[playerid]))));
    strcat(s, " \n");
    strcat(s, ""WHITE"»  "LY"Zmieñ swoje zadanie dnia "YELLOW"(100.000$)\n");
    format(s, sizeof(s), "%s"WHITE"»  "LY"Postêp osi¹gniêcia: "BLUE2"%s\n", s, (pAchievementDone[playerid][ACH_QUEST]) ? (""GREEN"Wykonane") : (form("%d/%d (poz. %d)", pAchievementStatus[playerid][ACH_QUEST], achievementData[ACH_QUEST][achievement_required][pAchievementLevel[playerid][ACH_QUEST]], pAchievementLevel[playerid][ACH_QUEST])));
    return showDialog(playerid, DIALOG_QUEST, DIALOG_STYLE_LIST, ""BLUE"Zadanie dnia", s, "Wybierz", "Zamknij"), 1;
}

stock OnDialogQuest(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_QUEST: {
            if(!response) return 1;

            switch(listitem) {
                case 0..2: callcmd::quest(playerid);

                case 3: {
                    if(pDailyDone[playerid]) return sendError(playerid, ""RED"Masz ju¿ wykonane zadanie dnia. Wróc tutaj jutro!");
                    if(getMoney(playerid) < 100000) return valueError(playerid, 100000);
                    giveMoney(playerid, -100000);

                    pDailyQuest[playerid] = random(sizeof(questListData));
                    pDailyMoney[playerid] = math_random(10000, 30000);
                    pDailyStatus[playerid] = 0;

                    PlayerTextDrawSetString(playerid, tdQuest, form("ZADANIE DNIA:~n~~w~~h~%s", GetQuestName(playerid)));
                    msg(playerid, C_BLUE2, "Zmieniono zadanie dnia na: {c}%s{/c}. Nagroda: {c}%s${/c}.", GetQuestName(playerid), strdot(pDailyMoney[playerid]));
                    m_pquery_format("UPDATE es_accounts SET daily=%d, daily_done=0, daily_status=0, daily_reward=%d WHERE id=%d LIMIT 1;", pDailyQuest[playerid], pDailyMoney[playerid], playerData[playerid][player_accountID]);
                }

                case 4: callcmd::quest(playerid);
            }
        }
    }
    return 0;
}
