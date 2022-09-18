enum e_gangData {
    gang_name[50],
    gang_tag[20],
    gang_color,
    gang_ownerid,
    Text:gang_draw[3],
    gang_level,
    gang_respect,
    gang_respect_needed,
    gang_slots,
    gang_place,
    gang_rankDefault[50],
    gang_battleZone,
    gang_spar_win,
    gang_spar_loss,
    gang_spar_remis,
    gang_kills,
    gang_deaths,
    gang_budget,
    Text:gang_budgetDraw[2]
};

enum e_gangUlepszeniaData {
    gang_ulep_name[25],
    gang_ulep_cost
};

new 
    pGang[MAX_PLAYERS],
    pGangInviteTimer[MAX_PLAYERS],

    pTransferData[MAX_PLAYERS][MAX_GANGS],
    pGangTransferID[MAX_PLAYERS],
    pGangTransferCost[MAX_PLAYERS],
    pGangTransferTimer[MAX_PLAYERS],

    pGangCreatorName[MAX_PLAYERS][50],
    pGangCreatorTag[MAX_PLAYERS][20],
    pGangCreatorColor[MAX_PLAYERS],
    PlayerBar:GangBar[MAX_PLAYERS],
    Iterator:Sparing<MAX_PLAYERS>,
    gangRankingPlace = 0,

    pGangSparRankedScore[MAX_GANGS],
    pGangSparRank[MAX_GANGS],

    GangUlepszeniaData[][e_gangUlepszeniaData] = {
        {"Gangowi kibice",      30000}, // za zabójstwo wiêcej expa
        {"Wsparcie szejków",    50000}, // za posiadane strefy gangowe, graffiti - wiêcej euro
        {"Tereny gangowe",      10000}  // za przejêcie strefy gangowej - wiêksza nagroda w ojro
    },
    GangUlepszeniaLevel[MAX_GANGS][sizeof(GangUlepszeniaData)],

    gangRankPermissions[][] = {"Zarz¹dzanie cz³onkami zespo³u", "Mo¿liwoœæ zapraszania osób do zespo³u", "Mo¿liwoœæ wykupywania ulepszeñ zespo³u"},
    pGangRankName[MAX_PLAYERS][50], pGangRankColor[MAX_PLAYERS], bool:pGangRankPerm[MAX_PLAYERS][sizeof(gangRankPermissions)],
    pGangInvites[MAX_PLAYERS][5],

    gangData[MAX_GANGS][e_gangData], 
    Iterator:GangMember[MAX_GANGS]<MAX_PLAYERS>,
    Iterator:Gangs<MAX_GANGS>
;

enum {
    GANG_ULEP_KIBICE = 0,
    GANG_ULEP_SZEJK = 1,
    GANG_ULEP_ZONES,
}

enum { // odpowiednia numeracja
    GANG_DIALOG_BUDGET = 8,
    GANG_DIALOG_SPARING = 9,
    GANG_DIALOG_ACHIEVEMENT,
    GANG_DIALOG_MARKET,
    GANG_DIALOG_ULEP,
    GANG_DIALOG_BASE,
    GANG_DIALOG_INVITE,
    GANG_DIALOG_MANAGE,
    GANG_DIALOG_SETTINGS,
};

#define GANG_PERM_MEMBERS       0
#define GANG_PERM_INVITE        1
#define GANG_PERM_ULEP          2

enum e_sparingKillerData {
    sparing_playerid,
    sparing_kills,
    Float:sparing_damage
};
new sparingKillerData[MAX_PLAYERS][e_sparingKillerData];

#define getGangName(%0)         gangData[%0][gang_name] 
#define getGangTag(%0)          gangData[%0][gang_tag]
#define getGangColor(%0)        gangData[%0][gang_color]

#define GANG_PRICE_RESPECT      10000
#define GANG_PRICE_MONEY        100000

#define MAX_GANG_SLOTS          30
#define GANG_PRICE_SLOTS        100

stock ResetGangData(playerid)
{
    if(pGang[playerid] != -1) Iter_Remove(GangMember[pGang[playerid]], playerid);
    Iter_Remove(Sparing, playerid);

    static const resetSparingData[e_sparingKillerData];
    sparingKillerData[playerid] = resetSparingData;

    pGang[playerid] = pGangTransferID[playerid] = -1;
    pGangInvites[playerid] = {-1, -1, -1, -1, -1};
    for(new i = 0; i < MAX_GANGS; i++) pTransferData[playerid][i] = -1;
    pGangTransferCost[playerid] = 0;

    strcat(pGangRankName[playerid], "");
    pGangRankColor[playerid] = C_WHITE;
    for(new i = 0; i < sizeof(gangRankPermissions); i++) pGangRankPerm[playerid][i] = false;

    strcat(pGangCreatorName[playerid], "");
    strcat(pGangCreatorTag[playerid], "");
    pGangCreatorColor[playerid] = -1;
}

stock checkRankPermission(playerid, permID) return (pGangRankPerm[playerid][permID]) ? (true) : (false);
stock UpdateGangBox(gangid)
{
    TextDrawSetString(gangData[gangid][gang_draw][0], form("%s [%s]", getGangName(gangid), getGangTag(gangid)));
    TextDrawSetString(gangData[gangid][gang_draw][1], form("%s/%s exp ~w~~h~(%s lvl)", strdot(gangData[gangid][gang_respect]), strdot(gangData[gangid][gang_respect_needed]), strdot(gangData[gangid][gang_level])));

    new gangstr[350];
    foreach(new i : GangMember[gangid]) { if(pGang[i] == gangid) format(gangstr, sizeof(gangstr), "%s~g~~h~~h~%s~n~", gangstr, getNick(i)); }
    return TextDrawSetString(gangData[gangid][gang_draw][2], gangstr), 1;
}

stock LoadPlayerGang(playerid, bool:created = false)
{
    new gangid = pGang[playerid];
    if(pGang[playerid] != -1) {
        if(Iter_Contains(Gangs, gangid)) {
            if(!created) {
                msg(playerid, getGangColor(gangid), "{c}(%s)*{/c}  Nale¿ysz do zespo³u: {c}%s [%s]{/c} - twoja ranga: {c}%s{/c}.", getGangTag(gangid), getGangName(gangid), getGangTag(gangid), pGangRankName[playerid]);
                msg(playerid, getGangColor(gangid), "{c}(%s)*{/c}  Napisz coœ na czacie zespo³owym: {c}!SIEMA KURWa{/c}.", getGangTag(gangid));
            }
            Iter_Add(GangMember[gangid], playerid);

            for(new i = 0; i < 3; i++) TextDrawShowForPlayer(playerid, gangData[gangid][gang_draw][i]);
            ShowPlayerProgressBar(playerid, GangBar[playerid]);

            UpdateGangBar(playerid, gangid);
            UpdateGangBox(gangid);
        } else {
            msg(playerid, -1, " ");
            msg(playerid, C_DRED, "Wczytywanie Twojego {c}zespo³u{/c} zakoñczone {c}niepowodzeniem{/c}!");
            msg(playerid, C_DRED, "Powodem takiego {c}rezultatu{/c} mo¿e byæ fakt, ¿e {c}zespó³{/c} móg³ zostaæ rozwi¹zany.");
            msg(playerid, -1, " ");

            m_pquery_format("UPDATE es_accounts SET gang=-1 WHERE id=%d LIMIT 1;", playerData[playerid][player_accountID]);
            pGang[playerid] = -1;
        }
    }
    return 1;
}

function DelayedGangInvite(playerid, gangid) 
{
    KillTimer(pGangInviteTimer[playerid]);

    for(new i = 0; i < 5; i++) {
        if(pGangInvites[playerid][i] == gangid) {
            pGangInvites[playerid][i] = -1;
            break;
        }
    }

    foreach(new i : GangMember[gangid]) { if(pGang[i] == gangid) msg(i, getGangColor(gangid), "(%s)*  {c}Gracz{/c} %s(%d){c} nie zd¹¿y³(a) zaakceptowaæ zaproszenia.{/c}", getGangTag(gangid), getNick(playerid), playerid); }
    return msg(playerid, getGangColor(gangid), "Nie zd¹¿y³eœ(aœ) zaakceptowaæ zaproszenia od zespo³u {c}%s [%s]{/c}.", getGangName(gangid), getGangTag(gangid)), 1;
}

stock countGangMembers(gangid)
{
    new count;
    foreach(new i : GangMember[gangid]) { if(pGang[i] == gangid) count++; }
    return count;
}

stock SaveGang(gangid) 
{
    dbstr[0] = EOS;
    format(dbstr, sizeof(dbstr), "UPDATE es_gangs SET slots=%d, respect=%d, respect_needed=%d, level=%d, sparing_points=%d, sparing_rank=%d, sparing='%d, %d, %d', kills=%d, deaths=%d, ulepszenia='%s' WHERE id=%d LIMIT 1;",
        gangData[gangid][gang_slots],
        gangData[gangid][gang_respect],
        gangData[gangid][gang_respect_needed],
        gangData[gangid][gang_level],
        pGangSparRankedScore[gangid],
        pGangSparRank[gangid],
        gangData[gangid][gang_spar_win],
        gangData[gangid][gang_spar_loss],
        gangData[gangid][gang_spar_remis],
        gangData[gangid][gang_kills],
        gangData[gangid][gang_deaths],
        GetGangUlepszenia(gangid),
        gangid
    );
    m_pquery(dbstr);
    return 1;
}

stock CountGangMembers(gangid)
{
    new count = 0;
    new Cache:result = mysql_query(dbconn, form("SELECT 1 FROM es_accounts WHERE gang=%d LIMIT %d;", gangid, gangData[gangid][gang_slots]));
    count = cache_num_rows();
    cache_delete(result);
    return count;
}

stock GetPlayerGangPerms(playerid)
{
    new pstr[128];
    for(new i = 0; i < sizeof(gangRankPermissions); i++) format(pstr, sizeof(pstr), "%s%s%i", pstr, i == 0 ? ("") : (","), pGangRankPerm[playerid][i]);
    return pstr;
}

stock gangRankError(playerid, i)
{
    s[0] = EOS;
    catstr(s, C_RED, C_YELLOW, "Nie posiadasz odpowiednich {c}uprawnieñ{/c}, by móc to zrobiæ!\n");
    catstr(s, C_RED, C_YELLOW, "Poproœ o nadanie permisji: {c}\"%s\"{/c}.", gangRankPermissions[i]);
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Brak permisji", s, "OK", #), 1;
}

new pGangBudgetTimer[MAX_GANGS];
stock giveGangBudget(gangid, amount)
{
    if(!Iter_Contains(Gangs, gangid)) return 1;

    gangData[gangid][gang_budget] += amount;
    textDrawSetString(gangData[gangid][gang_budgetDraw][1], "%s~w~~h~C%s", (amount < 0) ? ("~r~~h~-") : ("~g~~h~~h~+"), strdot(amount));

    m_pquery_format("UPDATE es_gangs SET budget=%d WHERE id=%d LIMIT 1;", gangData[gangid][gang_budget], gangid);
    foreach(new i : GangMember[gangid]) TextDrawShowForPlayer(i, gangData[gangid][gang_budgetDraw][1]);
    pGangBudgetTimer[gangid] = SetTimerEx("HideBudgetDraw", 1000 * 3, false, "d", gangid);
    return 1;
}

function HideBudgetDraw(gangid) {
    foreach(new i : GangMember[gangid]) TextDrawHideForPlayer(i, gangData[gangid][gang_budgetDraw][1]); 
    return KillTimer(pGangBudgetTimer[gangid]); }
stock GetPlayerBudget(playerid) return ((playerData[playerid][player_level] + 1) * floatround((GetAchievement(playerid, ACH_KILLS) + GetAchievement(playerid, ACH_DEATHS)) / 2, floatround_round) + pDuelRank[playerid]) * 100;

stock GetGangUlepszenia(gangid)
{
    new ustr[256];
    for(new i = 0; i < sizeof(GangUlepszeniaData); i++) format(ustr, sizeof(ustr), "%s%s%i", ustr, i == 0 ? ("") : (","), GangUlepszeniaLevel[gangid][i]);
    return ustr;
}

stock RandomGangReward(gangid)
{   
    new euro = 0;
    switch(GangUlepszeniaLevel[gangid][GANG_ULEP_SZEJK]) {
        case 0: euro = math_random(10, 30);
        case 1: euro = math_random(30, 50);
        case 2: euro = math_random(50, 100);
        case 3: euro = math_random(100, 300);
        case 4: euro = math_random(300, 500);
        default: euro = math_random(500, 1000);
    }
    return euro;
}

stock ShowGangTop(playerid)
{
    s[0] = EOS;
    strcat(s, ""WHITE""ARROW"  "GREEN"Ranking wed³ug: "LY"poziomu zespo³u\n");
    strcat(s, ""WHITE""ARROW"  "GREEN"Ranking wed³ug: "LY"zabójstw\n");
    strcat(s, ""WHITE""ARROW"  "GREEN"Ranking wed³ug: "LY"œmierci\n");
    strcat(s, ""WHITE""ARROW"  "GREEN"Ranking wed³ug: "LY"wygranych sparingów\n");
    strcat(s, ""WHITE""ARROW"  "GREEN"Ranking wed³ug: "LY"bud¿etu\n");
    return showDialog(playerid, DIALOG_GANG_TOP, DIALOG_STYLE_LIST, ""YELLOW"Podium najlepszych zespo³ów", s, "Wybierz", "Cofnij"), 1;
}