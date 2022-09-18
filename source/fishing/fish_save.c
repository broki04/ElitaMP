stock GetFishBaits(playerid)
{
    new bstr[128];
    for(new i = 0; i < sizeof(fishBaitData); i++) format(bstr, sizeof(bstr), "%s%s%i", bstr, i == 0 ? ("") : (","), pFishBait[playerid][i]);
    return bstr;
}

