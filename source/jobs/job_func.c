#include YSI_Coding\y_hooks

stock ExitPraca(playerid)
{
    new jobid = pJob[playerid];
    switch(pJob[playerid]) {
        case JOB_MAGAZYNIER: {
            if(IsValidDynamic3DTextLabel(magazynLabel[playerid])) DestroyDynamic3DTextLabel(magazynLabel[playerid]);
            if(IsValidDynamicCP(magazynCP[playerid])) DestroyDynamicCP(magazynCP[playerid]);
        }

        case JOB_GRABARZ: {
            if(IsValidVehicle(grabarzVehicle[playerid])) DestroyVehicle(grabarzVehicle[playerid]);
            if(IsValidDynamicMapIcon(grabarzMapIcon[playerid])) DestroyDynamicMapIcon(grabarzMapIcon[playerid]);
            if(IsValidDynamicCP(grabarzCP[playerid])) DestroyDynamicCP(grabarzCP[playerid]);
            if(IsValidDynamic3DTextLabel(grabarzLabel[playerid])) DestroyDynamic3DTextLabel(grabarzLabel[playerid]);
        }

        case JOB_ZRZUT: {
            if(IsValidVehicle(ZrzutVehicle[playerid])) DestroyVehicle(ZrzutVehicle[playerid]);
            if(IsValidDynamicRaceCP(ZrzutCP[playerid])) DestroyDynamicRaceCP(ZrzutCP[playerid]);
            if(IsValidDynamicMapIcon(ZrzutMapIcon[playerid])) DestroyDynamicMapIcon(ZrzutMapIcon[playerid]);
        }

        case JOB_TAXI: {
            if(IsValidVehicle(TaxiVehicle[playerid])) DestroyVehicle(TaxiVehicle[playerid]);
            if(IsValidDynamicActor(TaxiActor[playerid])) DestroyDynamicActor(TaxiActor[playerid]);
            if(IsValidDynamicCP(TaxiCP[playerid])) DestroyDynamicCP(TaxiCP[playerid]);
            if(IsValidDynamic3DTextLabel(TaxiLabel[playerid])) DestroyDynamic3DTextLabel(TaxiLabel[playerid]);
        }

        case JOB_MYSLIWY: {
            for(new i = 0; i < MAX_HUNTING; i++) {
                if(HuntingAlive[playerid][i]) {
                    if(IsValidDynamicObject(HuntingObject[playerid][i])) DestroyDynamicObject(HuntingObject[playerid][i]);
                    if(IsValidDynamic3DTextLabel(HuntingLabel[playerid][i])) DestroyDynamic3DTextLabel(HuntingLabel[playerid][i]);
                }

                HuntingAlive[playerid][i] = false;
                HuntingHealth[playerid][i] = 0.0;
            }
        }

        case JOB_GRZYBIARZ: {
            for(new i = 0; i < MAX_GRZYBY; i++) {
                if(GrzybAlive[playerid][i]) {
                    if(IsValidDynamic3DTextLabel(GrzybLabel[playerid][i])) DestroyDynamic3DTextLabel(GrzybLabel[playerid][i]);
                    if(IsValidDynamicObject(GrzybObject[playerid][i])) DestroyDynamicObject(GrzybObject[playerid][i]);
                }
                GrzybAlive[playerid][i] = false;
            }
        }

        case JOB_DRWAL: {
            for(new i = 0; i < sizeof(DrwalTreePosition); i++) {
                if(!DrwalCutted[playerid][i]) {
                    if(IsValidDynamicObject(DrwalObject[playerid][i])) DestroyDynamicObject(DrwalObject[playerid][i]);
                    if(IsValidDynamic3DTextLabel(DrwalLabel[playerid][i])) DestroyDynamic3DTextLabel(DrwalLabel[playerid][i]);
                    if(IsValidDynamicMapIcon(DrwalMapIcon[playerid][i])) DestroyDynamicMapIcon(DrwalMapIcon[playerid][i]);
                }
            
                DrwalCutted[playerid][i] = false;
                DrwalHealth[playerid][i] = 0.0;
            }

            if(IsValidVehicle(DrwalVehicle[playerid])) DestroyVehicle(DrwalVehicle[playerid]);
        }
    }
    m_pquery_format("UPDATE es_accounts SET job_level='%s', job_score='%s' WHERE id=%d LIMIT 1;", getJobLevel(playerid), getJobScore(playerid), playerData[playerid][player_accountID]);

    ClearAnimations(playerid);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Zrezygnowa³eœ(aœ) z pracy jako: "YELLOW"%s\n"WHITE"Mo¿esz ponownie siê podj¹æ roboty, wchodz¹c w "RED"ten marker"WHITE".", getJobName(jobid));
    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Rezygnacja z pracy", s, "OK", #);

    pJob[playerid] = -1;
    for(new x = 0; x < 3; x++) PlayerTextDrawHide(playerid, tdInfo[x]);

    DisableRemoteVehicleCollisions(playerid, false);
    return LoadPlayerPosition(playerid), 1;
}

stock UpdateJobDraw(playerid, jobid)
{
    if(jobid == -1) return 1;
    playerTextDrawSetString(playerid, tdInfo[1], "~p~%s", getJobName(jobid));
    playerTextDrawSetString(playerid, tdInfo[2], " ");

    switch(jobid) {
        case JOB_DRWAL:         playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Scietych drzew: ~g~~h~~h~%d~n~~w~~h~Nagroda za drzewo: ~p~~h~%s$", DrwalCount[playerid], strdot(DrwalReward[playerid]));
        case JOB_GRZYBIARZ:     playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Zebranych grzybow: ~r~~h~%d~n~~w~~h~Nagroda za sztuke: ~y~~h~%s$", GrzybCount[playerid], strdot(GrzybReward[playerid]));
        case JOB_MAGAZYNIER:    playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Dostarczonych towarow: ~b~~h~~h~%s~n~~w~~h~Towar: ~r~~h~%s", strdot(GetAchievement(playerid, ACH_MAGAZYNIER)), (magazynTowar[playerid]) ? (form("%s ~y~~h~~h~(%s$)", warePaczkaData[magazynPaczkaType[playerid]][ware_name], strdot(magazynReward[playerid]))) : ("---"));
        case JOB_MYSLIWY:       playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Zabitych saren: ~r~~h~%d~n~~w~~h~Nagroda za sztuke: ~y~~h~%s$", HuntingCount[playerid], strdot(HuntingReward[playerid]));

        case JOB_TAXI: {
            new rand = GetPVarInt(playerid, "player_taxi_random");
            switch(TaxiSwitch[playerid]) {
                case TAXI_EMPTY:    playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Udaj sie do ~r~~h~klienta~w~~h~!~n~~w~~h~Lokalizacja: ~y~~h~%s, %s", getZoneName(unpackXYZ(taxiPositionData[rand][taxi_cp])), getCityName(unpackXYZ(taxiPositionData[rand][taxi_cp])));
                case TAXI_CLIENT:   playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Cel podrozy: ~b~~h~~h~%s, %s~n~~w~~h~Nagroda: ~y~~h~%s$", getZoneName(unpackXYZ(taxiPositionData[rand][taxi_cp])), getCityName(unpackXYZ(taxiPositionData[rand][taxi_cp])), strdot(TaxiReward[playerid]));
            }
        }

        case JOB_ZRZUT: {
            if(ZrzutPaczka[playerid] > 0) playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Typ paczki: ~r~~h~%s ~r~~h~~h~(pozostalo: %d)~n~~w~~h~Nagroda: ~g~~h~%s$", ZrzutPaczkaName[ZrzutPaczkaID[playerid]], ZrzutPaczka[playerid], strdot(ZrzutReward[playerid]));
            else playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Udaj sie na ~y~~h~lotnisko~w~~h~, by uzupelnic paczki!");
        }

        case JOB_GRABARZ: {
            switch(grabarzEtap[playerid]) {
                case 0: playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Dostarczonych trumien: ~p~~h~%d~n~~w~~h~Trumna: ~y~~h~---", strdot(GetAchievement(playerid, ACH_GRABARZ)));
                case 1: playerTextDrawSetString(playerid, tdInfo[0], "~n~~n~~w~~h~Dostarczonych trumien: ~p~~h~%d~n~~w~~h~Trumna (%d/%d): ~r~~h~%s ~y~~h~~h~(%s$)", strdot(GetAchievement(playerid, ACH_GRABARZ)), grabarzCount[playerid], GetPVarInt(playerid, "player_grabarz_corps"), corpsNames[grabarzCorps[playerid]][corp_name], strdot(grabarzReward[playerid]));
            }
        }
    }
    return 1;
}