enum e_gangAchievementData {
    gang_achievement_id,
    gang_achievement_name[50],
    gang_achievement_description[255],
    gang_achievement_needed,
    gang_achievement_reward
};

#define GANG_ACHIEVEMENT_RANKED     0
#define GANG_ACHIEVEMENT_KILLA      1
#define GANG_ACHIEVEMENT_ZONE       2
#define GANG_ACHIEVEMENT_DEFENDER   3
#define GANG_ACHIEVEMENT_SLOTS      4

new gangAchievementData[][e_gangAchievementData] = {
    {GANG_ACHIEVEMENT_RANKED,       "Rankingowe sparingi",  "Musicie wygraæ 10 sparingów rankingowych.",        10,     50000},
    {GANG_ACHIEVEMENT_KILLA,        "Urodzeni zabojcy",     "Musicie zabiæ 100 osób, które nale¿¹ do zespo³u.", 100,    30000},
    {GANG_ACHIEVEMENT_ZONE,         "Maniacy stref",        "Musicie przej¹æ 5 stref zespo³owych.",             5,      15000},
    {GANG_ACHIEVEMENT_DEFENDER,     "The Last Stand",       "Musicie obroniæ 5 swoich stref zespo³owych.",      5,      20000},
    {GANG_ACHIEVEMENT_SLOTS,        "Wiecej ludzi",         "Musicie zakupiæ dodatkowe 10 miejsc do zespo³u.",  10,     12000}
};
new bool:pGangAchievement[MAX_GANGS][sizeof(gangAchievementData)], pGangAchievementStatus[MAX_GANGS][sizeof(gangAchievementData)];

stock CheckGangAchievement(gangid, achievementid)
{
    if(!Iter_Contains(Gangs, gangid)) return 1;

    if(!pGangAchievement[gangid][achievementid] && pGangAchievementStatus[gangid][achievementid] ++ >= gangAchievementData[achievementid][gang_achievement_needed]) {
        pGangAchievement[gangid][achievementid] = true;

        addGangScore(gangid, math_random(100, 300));
        giveGangBudget(gangid, gangAchievementData[achievementid][gang_achievement_reward]);

        foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ wykona³ osi¹gniêcie: {c}\"%s\"{/c}. Nagroda: {c}"EURO"%s{/c}.", getGangTag(gangid), gangAchievementData[achievementid][gang_achievement_name], strdot(gangAchievementData[achievementid][gang_achievement_reward]));
    }

    m_pquery_format("UPDATE es_gangs SET ach_done='%s', ach_status='%s' WHERE id=%d LIMIT 1;", GetGangAchievements(gangid), GetGangAchievementStatus(gangid), gangid);
    return 1;
}

stock GetGangAchievements(gangid)
{
    new str[256];
    for(new i = 0; i < sizeof(gangAchievementData); i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pGangAchievement[gangid][i]);
    return str;
}

stock GetGangAchievementStatus(gangid)
{
    new str[256];
    for(new i = 0; i < sizeof(gangAchievementData); i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pGangAchievementStatus[gangid][i]);
    return str;
}

stock OnDialogGangAchievements(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_GANG_ACHIEVEMENTS: {
            if(!response) return callcmd::team(playerid);

            new 
                gangid = pGang[playerid];

            s[0] = EOS;
            format(s, sizeof(s), ""WHITE"Nazwa osi¹gniêcia:\t\t"RED"%s\n", gangAchievementData[listitem][gang_achievement_name]);
            format(s, sizeof(s), "%s"WHITE"Postêp osi¹gniêcia:\t\t%s\n", s, (!pGangAchievement[gangid][listitem]) ? (form(""BLUE"%d/%d", pGangAchievementStatus[gangid][listitem], gangAchievementData[listitem][gang_achievement_needed])) : (""BLUE"WYKONANE"));
            format(s, sizeof(s), "%s"WHITE"Nagroda za osi¹gniêcie:\t"PINK""EURO"%s\n", s, strdot(gangAchievementData[listitem][gang_achievement_reward]));
            strcat(s, " \n");
            format(s, sizeof(s), "%s"WHITE"Opis osi¹gniêcia:\n"GREY"%s", s, gangAchievementData[listitem][gang_achievement_description]);

            inline onDialogData(pid, dd, rr, ll, string:ii[]) { 
                #pragma unused pid, dd, rr, ll,ii 
                return OnDialogResponse(playerid, DIALOG_GANG, 1, 9, ""); }
            Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BLUE2"Osi¹gniêcie zespo³owe", s, "OK", #);
            return 1;
        }
    }
    return 0;
}