#define MAX_GANGZONES   50

#include YSI_Coding\y_hooks

new 
    pGangZoneOwner[MAX_GANGZONES],
    Float:pGangZonePosition[MAX_GANGZONES][4],
    pGangZone[MAX_GANGZONES],
    pGangZoneName[MAX_GANGZONES][50],
    Text3D:pGangZoneLabel[MAX_GANGZONES],
    pGangZoneRectangle[MAX_GANGZONES],
    Float:pGangZonePoint[MAX_GANGZONES][3],
    pGangWarAttacker[MAX_GANGZONES],
    pGangWarDefender[MAX_GANGZONES],
    pGangWarTime[MAX_GANGZONES],
    pGangWarTimer[MAX_GANGZONES],
    Text:pGangZoneDraw[MAX_GANGZONES][8],
    pGangWarPoints[MAX_GANGZONES][2],
    bool:pGangWarDefenderDef[MAX_GANGZONES],
    bool:pGangWarDefenderExit[MAX_GANGZONES],
    Float:pGangWarDamage[MAX_GANGZONES][2],
    pGangZoneDamageId[MAX_GANGZONES],

    pGangZoneArea[MAX_PLAYERS] = -1,
    Iterator:GangZones<MAX_GANGZONES>
;

stock Init_GangZones()
{
    Iter_Init(GangZones);

    inline onLoadGangZones()
    {
        for(new i = 0; i < cache_num_rows(); i++) {
            cache_get_value_name_int(i, "ownerid", pGangZoneOwner[i]);

            new buffer[128];
            cache_get_value_name(i, "position", buffer);
            sscanf(buffer, "p<,>a<f>[4]", pGangZonePosition[i]);

            cache_get_value_name(i, "checkpoint", buffer);
            sscanf(buffer, "p<,>a<f>[3]", pGangZonePoint[i]);

            pGangZone[i] = CreateZone(unpackXYZR(pGangZonePosition[i]));
            CreateZoneBorders(pGangZone[i]);
            CreateZoneNumber(pGangZone[i], i + 1);
            CreateGangZoneDraw(i);

            if(pGangZoneOwner[i] != -1 && !Iter_Contains(Gangs, pGangZoneOwner[i])) {
                m_pquery_format("UPDATE es_gangs_zones SET ownerid=-1 WHERE ownerid=%d", pGangZoneOwner[i]);
                printf(" >>> [GANG-ZONES]: Zresetowano pomyœlnie strefê %s (%s, #%d).", getZoneName(unpackXYZ(pGangZonePoint[i])), getCityName(unpackXYZ(pGangZonePoint[i])), i + 1);
                pGangZoneOwner[i] = -1;
            }

            pGangZoneRectangle[i] = CreateDynamicRectangle(unpackXYZR(pGangZonePosition[i]), 0, 0, -1);
            Streamer_SetIntData(STREAMER_TYPE_AREA, pGangZoneRectangle[i], E_STREAMER_EXTRA_ID, i + ZONE_MAGIC);

            pGangZoneName[i] = getZoneName(unpackXYZ(pGangZonePoint[i]));
            pGangZoneLabel[i] = CreateDynamic3DTextLabel(UpdateGangZoneLabel(i), -1, unpackXYZ(pGangZonePoint[i]), 50.0, .worldid=0);
            CreateDynamicPickup(19134, 0, unpackXYZ(pGangZonePoint[i]), .worldid=0);

            Iter_Add(GangZones, i);
        }
        printf(" >>> [GANG-ZONES]: Wczytanych stref gangowych: %d", Iter_Count(GangZones));
    }

    m_tquery_inline(dbconn, using inline onLoadGangZones, "SELECT * FROM es_gangs_zones ORDER BY id ASC");
    return 1;
}

stock CreateGangZoneDraw(zoneid)
{
    pGangZoneDraw[zoneid][0] = TextDrawCreate(264.333129, 360.236968, "LD_OTB2:butnc");
    TextDrawTextSize(pGangZoneDraw[zoneid][0], 107.000000, 77.000000);
    TextDrawAlignment(pGangZoneDraw[zoneid][0], 1);
    TextDrawColor(pGangZoneDraw[zoneid][0], 120);
    TextDrawSetShadow(pGangZoneDraw[zoneid][0], 0);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][0], 255);
    TextDrawFont(pGangZoneDraw[zoneid][0], 4);
    TextDrawSetProportional(pGangZoneDraw[zoneid][0], false);

    pGangZoneDraw[zoneid][1] = TextDrawCreate(318.333465, 354.266693, "GROVE STREET");
    TextDrawLetterSize(pGangZoneDraw[zoneid][1], 0.147665, 1.002664);
    TextDrawTextSize(pGangZoneDraw[zoneid][1], 0.000000, 1001.000000);
    TextDrawAlignment(pGangZoneDraw[zoneid][1], 2);
    TextDrawColor(pGangZoneDraw[zoneid][1], -1);
    TextDrawSetShadow(pGangZoneDraw[zoneid][1], 0);
    TextDrawSetOutline(pGangZoneDraw[zoneid][1], 1);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][1], 255);
    TextDrawFont(pGangZoneDraw[zoneid][1], 2);
    TextDrawSetProportional(pGangZoneDraw[zoneid][1], true);

    pGangZoneDraw[zoneid][2] = TextDrawCreate(318.333374, 364.222076, "~r~~h~gang1 ~w~~h~vs ~b~~h~~h~gang2");
    TextDrawLetterSize(pGangZoneDraw[zoneid][2], 0.121997, 0.927997);
    TextDrawTextSize(pGangZoneDraw[zoneid][2], 0.000000, 346.000000);
    TextDrawAlignment(pGangZoneDraw[zoneid][2], 2);
    TextDrawColor(pGangZoneDraw[zoneid][2], -1);
    TextDrawSetShadow(pGangZoneDraw[zoneid][2], 0);
    TextDrawSetOutline(pGangZoneDraw[zoneid][2], 1);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][2], 255);
    TextDrawFont(pGangZoneDraw[zoneid][2], 1);
    TextDrawSetProportional(pGangZoneDraw[zoneid][2], true);

    pGangZoneDraw[zoneid][3] = TextDrawCreate(317.333404, 374.177795, "~r~~h~100 ~w~~h~: ~b~~h~100");
    TextDrawLetterSize(pGangZoneDraw[zoneid][3], 0.205665, 1.147850);
    TextDrawTextSize(pGangZoneDraw[zoneid][3], 0.000000, 971.000000);
    TextDrawAlignment(pGangZoneDraw[zoneid][3], 2);
    TextDrawColor(pGangZoneDraw[zoneid][3], -1);
    TextDrawSetShadow(pGangZoneDraw[zoneid][3], 0);
    TextDrawSetOutline(pGangZoneDraw[zoneid][3], 1);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][3], 255);
    TextDrawFont(pGangZoneDraw[zoneid][3], 1);
    TextDrawSetProportional(pGangZoneDraw[zoneid][3], true);

    pGangZoneDraw[zoneid][4] = TextDrawCreate(374.000061, 355.511047, "25");
    TextDrawLetterSize(pGangZoneDraw[zoneid][4], 0.172999, 0.849184);
    TextDrawAlignment(pGangZoneDraw[zoneid][4], 3);
    TextDrawColor(pGangZoneDraw[zoneid][4], -1);
    TextDrawSetShadow(pGangZoneDraw[zoneid][4], 0);
    TextDrawSetOutline(pGangZoneDraw[zoneid][4], 1);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][4], 255);
    TextDrawFont(pGangZoneDraw[zoneid][4], 1);
    TextDrawSetProportional(pGangZoneDraw[zoneid][4], true);

    pGangZoneDraw[zoneid][5] = TextDrawCreate(318.666748, 378.740600, "Przejmujesz teren neutralny.~n~Licznik obrazen wylaczony!");
    TextDrawLetterSize(pGangZoneDraw[zoneid][5], 0.133999, 1.064886);
    TextDrawTextSize(pGangZoneDraw[zoneid][5], 0.000000, 354.000000);
    TextDrawAlignment(pGangZoneDraw[zoneid][5], 2);
    TextDrawColor(pGangZoneDraw[zoneid][5], -1);
    TextDrawSetShadow(pGangZoneDraw[zoneid][5], 0);
    TextDrawSetOutline(pGangZoneDraw[zoneid][5], 1);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][5], 255);
    TextDrawFont(pGangZoneDraw[zoneid][5], 1);
    TextDrawSetProportional(pGangZoneDraw[zoneid][5], true);

    pGangZoneDraw[zoneid][6] = TextDrawCreate(318.000030, 394.088745, "Rekord obrazen: broki (1000 dmg)");
    TextDrawLetterSize(pGangZoneDraw[zoneid][6], 0.101332, 0.928000);
    TextDrawTextSize(pGangZoneDraw[zoneid][6], 0.000000, 335.000000);
    TextDrawAlignment(pGangZoneDraw[zoneid][6], 2);
    TextDrawColor(pGangZoneDraw[zoneid][6], -1);
    TextDrawSetShadow(pGangZoneDraw[zoneid][6], 0);
    TextDrawSetOutline(pGangZoneDraw[zoneid][6], 1);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][6], 255);
    TextDrawFont(pGangZoneDraw[zoneid][6], 1);
    TextDrawSetProportional(pGangZoneDraw[zoneid][6], true);

    pGangZoneDraw[zoneid][7] = TextDrawCreate(317.666748, 385.377807, "~r~~h~1000 dmg ~w~~h~| ~b~~h~~h~1000 dmg");
    TextDrawLetterSize(pGangZoneDraw[zoneid][7], 0.106666, 0.770369);
    TextDrawTextSize(pGangZoneDraw[zoneid][7], 0.000000, 319.000000);
    TextDrawAlignment(pGangZoneDraw[zoneid][7], 2);
    TextDrawColor(pGangZoneDraw[zoneid][7], -1);
    TextDrawSetShadow(pGangZoneDraw[zoneid][7], 0);
    TextDrawSetOutline(pGangZoneDraw[zoneid][7], 1);
    TextDrawBackgroundColor(pGangZoneDraw[zoneid][7], 255);
    TextDrawFont(pGangZoneDraw[zoneid][7], 1);
    TextDrawSetProportional(pGangZoneDraw[zoneid][7], true);
}

stock UpdateGangZoneLabel(zoneid)
{
    new str[256], ownerid = pGangZoneOwner[zoneid];
    format(str, sizeof(str), ""GOLD"STREFA #%d "LBLUE"(%s)\n"WHITE"Aby zaatakowaæ, wpisz: "RED"/atakuj.\n\n"WHITE"W³aœciciel: "GREY"%s", zoneid + 1, pGangZoneName[zoneid], (ownerid == -1) ? ("BRAK") : (form("{%06x}%s [%s]", getGangColor(ownerid) >>> 8, getGangName(ownerid), getGangTag(ownerid))));
    return str;
}

stock UpdateGangZones(gangid)
{
    foreach(new i : GangZones) {
        if(pGangZoneOwner[i] == gangid) {
            UpdateDynamic3DTextLabelText(pGangZoneLabel[i], -1, UpdateGangZoneLabel(i));

            HideZoneForAll(pGangZone[i]);
            ShowZoneForAll(pGangZone[i], setAlpha(getGangColor(pGangZoneOwner[i]), 160));
        }
    }
}

stock GetZoneDamageId(zoneid)
{
    new Float:damage = 0.0;
    foreach(new i : Player) {
        if(IsPlayerInDynamicArea(i, pGangZoneRectangle[zoneid]) && pGangZoneArea[i] == zoneid && (pGang[i] == pGangWarAttacker[zoneid] || (pGangWarDefender[zoneid] != -1 && pGang[i] == pGangWarDefender[zoneid]))) {
            if(playerData[i][player_zone_damage] > damage) {
                pGangZoneDamageId[zoneid] = i;
                damage = playerData[i][player_zone_damage];
            }
        }
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(killerid != INVALID_PLAYER_ID) {
        if(pGang[playerid] != -1 && pGang[killerid] != -1) {
            new zoneid = pGangZoneArea[playerid];
            if(zoneid != -1 && pGangWarTime[zoneid] > 0 && IsPlayerInDynamicArea(playerid, pGangZoneRectangle[zoneid]) && IsPlayerInDynamicArea(killerid, pGangZoneRectangle[zoneid])) {
                pGangWarPoints[zoneid][(pGang[killerid] == pGangWarAttacker[zoneid]) ? (0) : (1)] ++;
            }
        }
    }
    return 1;
}

CMD:atakuj(playerid)
{
    if(pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym zespole.");

    new zoneid = pGangZoneArea[playerid];
    if(zoneid == -1) return sendError(playerid, ""RED"Nie znajdujesz siê na ¿adnym terytorium.");

    new gangid = pGang[playerid];
    if(gangData[gangid][gang_battleZone] != -1) return sendError(playerid, ""RED"Twój zespó³ przejmuje ju¿ jakieœ terytorium. Aby pomóc, wpisz: "WHITE"/ttp."); 
    if(pGangZoneOwner[zoneid] == gangid) return sendError(playerid, ""RED"Nie mo¿esz przej¹æ terytorium zajêtego przez Twój zespó³.");
    if(pGangWarTime[zoneid] > 0) return sendError(playerid, ""RED"Terytorium, które próbujesz zaatakowaæ, jest ju¿ atakowane przez inny zespó³.");
    return SetTimerEx("StartGangWar", 500, false, "ddd", playerid, gangid, zoneid), 1;
}

CMD:ttp(playerid)
{
    new gangid = pGang[playerid], zoneid = gangData[gangid][gang_battleZone];
    if(pGang[playerid] == -1) return sendError(playerid, ""RED"Nie jesteœ w ¿adnym zespo³e.");
    if(zoneid == -1) return sendError(playerid, ""RED"Twój zespó³ nie przejmuje ¿adnego terytorium.");

    randomPositionArea(unpackXYZR(pGangZonePosition[zoneid]), pGangZonePoint[zoneid][0], pGangZonePoint[zoneid][1]);
    CA_FindZ_For2DCoord(pGangZonePoint[zoneid][0], pGangZonePoint[zoneid][1], pGangZonePoint[zoneid][2]);
    return Teleport(playerid, false, unpackXYZ(pGangZonePoint[zoneid]) + 1.0, 0.0, 0, 0);
}

function StartGangWar(playerid, gangid, zoneid)
{
    if(zoneid == -1 || !Iter_Contains(Gangs, gangid)) return 0;
    ZoneFlashForAll(pGangZone[zoneid], setAlpha(getGangColor(gangid), 160));

    pGangWarAttacker[zoneid] = gangid;
    pGangWarDefender[zoneid] = pGangZoneOwner[zoneid];
    pGangWarTime[zoneid] = 30;
    pGangWarPoints[zoneid] = {0, 0};
    pGangWarDefenderDef[zoneid] = pGangWarDefenderExit[zoneid] = false;
    pGangWarDamage[zoneid] = Float:{0.0, 0.0};
    pGangZoneDamageId[zoneid] = -1;

    TextDrawSetString(pGangZoneDraw[zoneid][1], form("%s", toUpperCase(getZoneName(unpackXYZ(pGangZonePoint[zoneid])))));
    TextDrawSetString(pGangZoneDraw[zoneid][2], form("~r~~h~%s ~w~~h~vs ~b~~h~~h~%s", getGangName(gangid), (pGangZoneOwner[zoneid] == -1) ? ("Teren neutralny") : (form("%s", getGangName(pGangZoneOwner[zoneid])))));
    TextDrawSetString(pGangZoneDraw[zoneid][4], form("~w~~h~%d", pGangWarTime[zoneid]));

    gangData[gangid][gang_battleZone] = zoneid;
    foreach(new i : GangMember[gangid]) { 
        if(IsPlayerInDynamicArea(i, pGangZoneRectangle[zoneid])) { for(new x = 0; x < 8; x++) TextDrawShowForPlayer(i, pGangZoneDraw[zoneid][x]); }
        playerData[i][player_zone_damage] = 0.0;
    }

    if(pGangWarDefender[zoneid] != -1) {
        new enemyid = pGangWarDefender[zoneid];
        gangData[enemyid][gang_battleZone] = zoneid;

        foreach(new i : GangMember[enemyid]) {
            if(IsPlayerInDynamicArea(i, pGangZoneRectangle[zoneid])) { for(new x = 0; x < 8; x++) TextDrawShowForPlayer(i, pGangZoneDraw[zoneid][x]); }
            playerData[i][player_zone_damage] = 0.0;
        }

        TextDrawSetString(pGangZoneDraw[zoneid][3], form("~r~~h~%d ~w~~h~: ~b~~h~~h~%d", pGangWarPoints[zoneid][0], pGangWarPoints[zoneid][1]));
        TextDrawSetString(pGangZoneDraw[zoneid][5], "_");
        TextDrawSetString(pGangZoneDraw[zoneid][6], "~w~~h~Rekord obrazen: ~r~~h~---");
        TextDrawSetString(pGangZoneDraw[zoneid][7], "~r~~h~0.0 dmg ~w~~h~| ~b~~h~~h~0.0 dmg");

        msgAll(C_BLUE2, "Zespó³ {c}%s{/c} zaatakowa³ teren {c}%s{/c} zespo³u: {c}%s{/c}.", getGangName(gangid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])), getGangName(enemyid));
    } else {
        TextDrawSetString(pGangZoneDraw[zoneid][3], "_");
        TextDrawSetString(pGangZoneDraw[zoneid][5], "~w~~h~Przejmujesz teren ~y~~h~neutralny~w~~h~.~n~~w~~h~Licznik ~r~~h~obrazen ~w~~h~wylaczony.");
        TextDrawSetString(pGangZoneDraw[zoneid][6], "_");
        TextDrawSetString(pGangZoneDraw[zoneid][7], "_");

        msgAll(C_BLUE2, "Zespó³ {c}%s{/c} zaatakowa³ neutralny teren {c}%s{/c}.", getGangName(gangid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])));
    }

    pGangWarTimer[zoneid] = SetTimerEx("GangWarTimer", 1000, true, "ddd", pGangWarAttacker[zoneid], pGangWarDefender[zoneid], zoneid);
    return 1;
}

function GangWarTimer(gangid, enemyid, zoneid)
{
    if(!Iter_Contains(Gangs, gangid) || zoneid == -1) return 0;

    new countMembers[MAX_GANGS][MAX_GANGZONES];
    foreach(new i : GangMember[gangid]) { if(IsPlayerInDynamicArea(i, pGangZoneRectangle[zoneid]) && pGang[i] == gangid) { countMembers[gangid][zoneid] ++; } }
    if(countMembers[gangid][zoneid] < 1) return CancelGangWar(gangid, enemyid, zoneid);

    textDrawSetString(pGangZoneDraw[zoneid][4], "~w~~h~%d", pGangWarTime[zoneid]);
    if(enemyid != -1) {
        foreach(new i : GangMember[enemyid]) { if(IsPlayerInDynamicArea(i, pGangZoneRectangle[zoneid]) && pGang[i] == enemyid) { countMembers[enemyid][zoneid] ++; } }

        if(countMembers[enemyid][zoneid] < 1 && pGangWarDefenderDef[zoneid]) {
            pGangWarDefenderDef[zoneid] = false;
            pGangWarDefenderExit[zoneid] = true;
            //pGangWarTime[zoneid] = pGangWarTime[zoneid] / 2;
        }

        textDrawSetString(pGangZoneDraw[zoneid][3], "~r~~h~%d ~w~~h~: ~b~~h~~h~%d", pGangWarPoints[zoneid][0], pGangWarPoints[zoneid][1]);
        textDrawSetString(pGangZoneDraw[zoneid][7], "~r~~h~%.1f dmg ~w~~h~| ~b~~h~~h~%.1f dmg", pGangWarDamage[zoneid][0], pGangWarDamage[zoneid][1]);

        if(pGangWarDefenderDef[zoneid]) {
            new damage_id = pGangZoneDamageId[zoneid], Float:damage = playerData[pGangZoneDamageId[zoneid]][player_zone_damage];
            textDrawSetString(pGangZoneDraw[zoneid][6], "~w~~h~Rekord obrazen: ~r~~h~%s (%.2f dmg)", getNick(damage_id), damage);

            /*SortDeepArray(playerZoneData, player_zone_damage, .order=SORT_DESC);
            for(new i = 0; i < Iter_Count(GangMember[gangid]) + Iter_Count(GangMember[enemyid]); i++) {
                if(!IsPlayerConnected(playerZoneData[i][player_zone_id])) continue;
                if(i == 1) break;

                new playerid = playerZoneData[i][player_zone_id], Float:damage = playerZoneData[i][player_zone_damage];
                TextDrawSetString(pGangZoneDraw[zoneid][6], form("~w~~h~Rekord obrazen: ~r~~h~%s (%.2f dmg)", getNick(playerid), damage));
            }*/
        }
    }

    pGangWarTime[zoneid] --;
    if(pGangWarTime[zoneid] < 0) CheckGangWar(gangid, enemyid, zoneid);
    else GetZoneDamageId(zoneid);
    return 1;
}

stock CheckGangWar(gangid, enemyid, zoneid)
{
    foreach(new i : Player) { if(IsPlayerInDynamicArea(i, pGangZoneRectangle[zoneid]) && pGangZoneArea[i] == zoneid) { for(new x = 0; x < 8; x++) TextDrawHideForPlayer(i, pGangZoneDraw[zoneid][x]); } }

    ZoneStopFlashForAll(pGangZone[zoneid]);
    gangData[gangid][gang_battleZone] = -1;

    if(enemyid == -1) {
        CheckBet(gangid, BET_ZONE, true);

        pGangZoneOwner[zoneid] = gangid;
        msgAll(C_BLUE2, "Zespó³ {c}%s{/c} przej¹³ neutralny teren {c}%s{/c}.", getGangName(gangid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])));
        giveGangBudget(gangid, ConquestGangReward(gangid, true));
    } else {
        gangData[enemyid][gang_battleZone] = -1;
        if(pGangWarPoints[zoneid][0] > pGangWarPoints[zoneid][1]) {
            pGangZoneOwner[zoneid] = pGangWarAttacker[zoneid];
            msgAll(C_BLUE2, "Zespó³ {c}%s{/c} przej¹³ teren {c}%s{/c} zajêty przez: {c}%s{/c}.", getGangName(gangid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])), getGangName(enemyid));        

            CheckBet(gangid, BET_ZONE, true);
            CheckBet(enemyid, BET_ZONE, false);

            CheckGangAchievement(pGangWarAttacker[zoneid], GANG_ACHIEVEMENT_ZONE);
            giveGangBudget(gangid, ConquestGangReward(gangid, true));
            giveGangBudget(enemyid, -ConquestGangReward(enemyid, false));
        }
        else if(pGangWarPoints[zoneid][1] > pGangWarPoints[zoneid][0]) {
            pGangZoneOwner[zoneid] = pGangWarDefender[zoneid];
            msgAll(C_BLUE2, "Zespó³ {c}%s{/c} obroni³ teren {c}%s{/c} przed gangiem: {c}%s{/c}.", getGangName(enemyid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])), getGangName(gangid));

            CheckBet(gangid, BET_ZONE, false);
            CheckBet(enemyid, BET_ZONE, true);

            CheckGangAchievement(pGangWarDefender[zoneid], GANG_ACHIEVEMENT_DEFENDER);
            giveGangBudget(gangid, -ConquestGangReward(gangid, false));
            giveGangBudget(enemyid, ConquestGangReward(enemyid, true));
        } else {
            if(pGangWarDamage[zoneid][1] > pGangWarDamage[zoneid][0]) {
                pGangZoneOwner[zoneid] = pGangWarDefender[zoneid];
                msgAll(C_BLUE2, "Zespó³ {c}%s{/c} obroni³ teren {c}%s{/c} przed gangiem: {c}%s{/c}.", getGangName(enemyid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])), getGangName(gangid));

                CheckBet(gangid, BET_ZONE, false);
                CheckBet(enemyid, BET_ZONE, true);

                CheckGangAchievement(pGangWarDefender[zoneid], GANG_ACHIEVEMENT_DEFENDER);
                giveGangBudget(gangid, -ConquestGangReward(gangid, false));
                giveGangBudget(enemyid, ConquestGangReward(enemyid, true));
            } else {
                pGangZoneOwner[zoneid] = pGangWarAttacker[zoneid];
                msgAll(C_BLUE2, "Zespó³ {c}%s{/c} przej¹³ teren {c}%s{/c} zajêty przez: {c}%s{/c}.", getGangName(gangid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])), getGangName(enemyid));

                CheckBet(gangid, BET_ZONE, true);
                CheckBet(enemyid, BET_ZONE, false);

                CheckGangAchievement(pGangWarAttacker[zoneid], GANG_ACHIEVEMENT_ZONE);
                giveGangBudget(gangid, ConquestGangReward(gangid, true));
                giveGangBudget(enemyid, -ConquestGangReward(enemyid, false));
            }
        }
    }

    HideZoneForAll(pGangZone[zoneid]);
    ShowZoneForAll(pGangZone[zoneid], setAlpha(getGangColor(pGangZoneOwner[zoneid]), 160));

    pGangWarTime[zoneid] = 0;
    pGangWarAttacker[zoneid] = pGangWarDefender[zoneid] = -1;
    pGangWarPoints[zoneid] = {0, 0};
    KillTimer(pGangWarTimer[zoneid]);

    addGangScore(pGangZoneOwner[zoneid], (enemyid == -1) ? (10) : (math_random(15, 30)));
    m_pquery_format("UPDATE es_gangs_zones SET ownerid=%d WHERE id=%d LIMIT 1;", pGangZoneOwner[zoneid], zoneid + 1);
    return UpdateDynamic3DTextLabelText(pGangZoneLabel[zoneid], -1, UpdateGangZoneLabel(zoneid)), 1;
}

stock CancelGangWar(gangid, enemyid, zoneid)
{
    ZoneStopFlashForAll(pGangZone[zoneid]);
    gangData[gangid][gang_battleZone] = -1;

    if(enemyid == -1) {
        msgAll(C_BLUE2, "Zespó³ {c}%s{/c} nie przej¹³ neutralnego terenu {c}%s{/c}.", getGangName(gangid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])));
        CheckBet(gangid, BET_ZONE, false);
    } else {
        CheckBet(gangid, BET_ZONE, false);
        CheckBet(enemyid, BET_ZONE, true);

        msgAll(C_BLUE2, "Zespó³ {c}%s{/c} nie przej¹³ terenu {c}%s{/c} zespo³u: {c}%s{/c}.", getGangName(gangid), getZoneName(unpackXYZ(pGangZonePoint[zoneid])), getGangName(enemyid));
        gangData[enemyid][gang_battleZone] = -1;
    }

    pGangWarAttacker[zoneid] = pGangWarDefender[zoneid] = -1;
    pGangZoneOwner[zoneid] = enemyid;
    pGangWarTime[zoneid] = 0;
    pGangWarPoints[zoneid] = {0, 0};
    return KillTimer(pGangWarTimer[zoneid]), 1;
}

stock ShowGangZones(playerid)
{
    s[0] = EOS;
    s = ""WHITE"Terytorium\t"GREEN"W³aœciciel\n";

    foreach(new i : GangZones) {
        new ownerid = pGangZoneOwner[i];
        format(s, sizeof(s), "%s"WHITE"%s "LY"(#%d)\t%s\n", s, getZoneName(unpackXYZ(pGangZonePoint[i])), i + 1, (ownerid == -1) ? (""GREY"BRAK") : (form("{%06x}%s [%s]", getGangColor(ownerid) >>> 8, getGangName(ownerid), getGangTag(ownerid))));
    }

    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
        #pragma unused pp, dd, rr, ll, ii
        return callcmd::team(playerid), 1; }
    return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Terytoria zespo³owe", s, "OK", #), 1;
}

stock CountGangZones(gangid)
{
    new count = 0;
    foreach(new i : GangZones) { if(pGangZoneOwner[i] == gangid) count ++; }
    return count;
}

function ZonesReward()
{
    foreach(new gangid : Gangs) {
        new count = CountGangZones(gangid);
        if(count > 0 && countGangMembers(gangid) > 0) {
            new euro = RandomGangReward(gangid) * count;
            giveGangBudget(gangid, euro);
            addGangScore(gangid, math_random(50, 70));
            foreach(new i : GangMember[gangid]) msg(i, getGangColor(gangid), "{c}(%s)*{/c}  Twój zespó³ posiada {c}%d{/c} terytoriów! "LY"(+"EURO"%s)", getGangTag(gangid), count, strdot(euro));
        }
    }
    return 1;
}

stock ConquestGangReward(gangid, bool:winner)
{
    new euro = 0;
    switch(GangUlepszeniaLevel[gangid][GANG_ULEP_ZONES]) {
        case 0: euro = (winner) ? (math_random(1000, 3000)) : (math_random(2000, 3000));
        case 1: euro = (winner) ? (math_random(3000, 5000)) : (math_random(1500, 2000));
        case 2: euro = (winner) ? (math_random(5000, 8000)) : (math_random(1000, 1500));
        case 3: euro = (winner) ? (math_random(8000, 10000)) : (math_random(700, 1000));
        case 4: euro = (winner) ? (math_random(10000, 20000)) : (math_random(500, 700));
        default: euro = (winner) ? (math_random(20000, 50000)) : (math_random(300, 500));
    }
    return euro;
}