stock UpdateGangBar(playerid, gangid) return SetTimerEx("_updateGangBarEx", 300, false, "dd", playerid, gangid);
function _updateGangBarEx(playerid, gangid) 
{
    HidePlayerProgressBar(playerid, GangBar[playerid]);
    SetPlayerProgressBarColour(playerid, GangBar[playerid], getGangColor(gangid));
    ShowPlayerProgressBar(playerid, GangBar[playerid]);

    SetPlayerProgressBarValue(playerid, GangBar[playerid], gangData[gangid][gang_respect]);
    SetPlayerProgressBarMaxValue(playerid, GangBar[playerid], gangData[gangid][gang_respect_needed]);
}

stock addGangScore(gangid, amount) 
{
    gangData[gangid][gang_respect] += amount;

    new reszta = 0, old_level = gangData[gangid][gang_level];
    if(gangData[gangid][gang_respect] >= gangData[gangid][gang_respect_needed]) {
        reszta = gangData[gangid][gang_respect] - gangData[gangid][gang_respect_needed];
        gangData[gangid][gang_respect_needed] = gangData[gangid][gang_respect_needed] + 100;
        gangData[gangid][gang_respect] = reszta;
        gangData[gangid][gang_level] ++;

        if(old_level < gangData[gangid][gang_level]) foreach(new i : GangMember[gangid]) { if(pGang[i] == gangid) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ awansowa³ na {c}%s{/c} poziom.", getGangTag(gangid), strdot(gangData[gangid][gang_level])); }
    }

    m_pquery_format("UPDATE es_gangs SET respect=%d, respect_needed=%d, level=%d WHERE id=%d LIMIT 1;", gangData[gangid][gang_respect], gangData[gangid][gang_respect_needed], gangData[gangid][gang_level], gangid);
    foreach(new i : GangMember[gangid]) { if(pGang[i] == gangid) UpdateGangBar(i, gangid); }
    return UpdateGangBox(gangid), 1;
}