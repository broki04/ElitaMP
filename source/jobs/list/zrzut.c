#include YSI_Coding\y_hooks

new 
    Float:ZrzutPositions[][] = {
        {105.8937, -150.7736, 200.0000},
        {1435.5105, -1679.1832, 200.0000},
        {1890.6768, -2334.0369, 200.0000},
        {-1966.3369, -941.9847, 200.0000},
        {-2607.7759, -56.4022, 200.0000},
        {-2066.1238, 407.2636, 200.0000},
        {-1407.7262, 486.8845, 200.0000},
        {-296.6862, 1096.4586, 200.0000},
        {170.2444, 1838.0859, 200.0000}
    },
    ZrzutPaczkaName[][] = {"Bronie", "Surowce", "Medykamenty", "Materialy", "Zywnosc", "Odziez", "Elektronika"},

    Float:ZrzutWaga[MAX_PLAYERS],
    ZrzutSwitch[MAX_PLAYERS],
    ZrzutPaczka[MAX_PLAYERS],
    ZrzutVehicle[MAX_PLAYERS],
    ZrzutReward[MAX_PLAYERS],
    ZrzutCP[MAX_PLAYERS],
    ZrzutMapIcon[MAX_PLAYERS],
    ZrzutPaczkaID[MAX_PLAYERS],
    ZrzutPaczkaCount[MAX_PLAYERS]
;

#define DROP_LOADED     0   // za³adowany
#define DROP_EMPTY      1   // pusty

hook function ResetPlayerData(playerid)
{
    ZrzutWaga[playerid] = 0.0;
    ZrzutReward[playerid] = 0;
    ZrzutSwitch[playerid] = ZrzutPaczka[playerid] = ZrzutPaczkaCount[playerid] = 0;
    ZrzutVehicle[playerid] = ZrzutPaczkaID[playerid] = -1;
    return continue(playerid);
}

stock Zrzut_Start(playerid)
{
    ZrzutWaga[playerid] = 0.0;
    ZrzutReward[playerid] = 0;
    ZrzutSwitch[playerid] = DROP_EMPTY;
    ZrzutPaczkaCount[playerid] = 0;

    ZrzutVehicle[playerid] = CreateVehicle(553, 409.8317, 2502.6594, 18.0052, 90.2832, -1, -1, -1);
    SetVehicleVirtualWorld(ZrzutVehicle[playerid], GetPVarInt(playerid, "player_zrzut_world"));
    PutPlayerInVehicle(playerid, ZrzutVehicle[playerid], 0);

    TogglePlayerControllable(playerid, false);
    GameTextForPlayer(playerid, "~r~~h~ZALADOWYWANIE TOWARU~y~...", 3000, 3);
    SetTimerEx("ZrzutLoaded", 1000 * 3, false, "d", playerid);
    return UpdateJobDraw(playerid, JOB_ZRZUT), 1;
}

function ZrzutLoaded(playerid)
{
    if(IsValidDynamicRaceCP(ZrzutCP[playerid])) DestroyDynamicRaceCP(ZrzutCP[playerid]);
    if(IsValidDynamicMapIcon(ZrzutMapIcon[playerid])) DestroyDynamicMapIcon(ZrzutMapIcon[playerid]);

    new rand = random(sizeof(ZrzutPositions));
    ZrzutCP[playerid] = CreateDynamicRaceCP(4, unpackXYZ(ZrzutPositions[rand]), 0.0, 0.0, 0.0, 10.0, .worldid=GetPVarInt(playerid, "player_zrzut_world"), .streamdistance=99999.0, .playerid=playerid);
    ZrzutMapIcon[playerid] = CreateDynamicMapIcon(unpackXYZ(ZrzutPositions[rand]), 19, -1, .worldid=GetPVarInt(playerid, "player_zrzut_world"), .streamdistance=99999.0, .playerid=playerid, .style=MAPICON_GLOBAL);

    SetPVarInt(playerid, "zrzut_last_point", rand);

    ZrzutPaczkaID[playerid] = random(sizeof(ZrzutPaczkaName));
    ZrzutPaczka[playerid] = math_random(3, 5);
    ZrzutPaczkaCount[playerid] = 0;
    ZrzutSwitch[playerid] = DROP_LOADED;
    ZrzutReward[playerid] = (math_random(1000, 3000) * pJobLevel[playerid][JOB_ZRZUT]) + (pJobReward[playerid][JOB_ZRZUT] * 100) + ((JobDay == JOB_ZRZUT) ? (1000) : (0));

    msg(playerid, C_GREEN, "Za³adowano {c}%d{/c} %s do samolotu! Obecny cel podró¿y: {c}%s, %s{/c}.", ZrzutPaczka[playerid], ConvertTimeFormat(ZrzutPaczka[playerid], "paczek", "paczka", "paczki", "paczek"), getZoneName(unpackXYZ(ZrzutPositions[rand])), getCityName(unpackXYZ(ZrzutPositions[rand])));
    TogglePlayerControllable(playerid, true);
    UpdateJobDraw(playerid, JOB_ZRZUT);

    s[0] = EOS;
    format(s, sizeof(s), ""WHITE"Za³adowano pomyœlnie "RED"%d "WHITE"%s.\n"WHITE"Mi³ego lotu, kole¿ko!\n\n", ZrzutPaczka[playerid], ConvertTimeFormat(ZrzutPaczka[playerid], "paczek", "paczka", "paczki", "paczek"));
    format(s, sizeof(s), ""WHITE"Typ paczki: "BLUE"%s\n"WHITE"Nagroda za paczkê: "LY"%s$\n"WHITE"Cel podró¿y: "MINT"%s, %s", ZrzutPaczkaName[ZrzutPaczkaID[playerid]], strdot(ZrzutReward[playerid]), getZoneName(unpackXYZ(ZrzutPositions[rand])), getCityName(unpackXYZ(ZrzutPositions[rand])));
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"Towar za³adowany", s, "OK", #), 1;
}

hook OnPlayerEnterDynRaceCP(playerid, checkpointid)
{
    if(pJob[playerid] == JOB_ZRZUT && IsPlayerInVehicle(playerid, ZrzutVehicle[playerid]) && ZrzutCP[playerid] == checkpointid) {
        switch(ZrzutSwitch[playerid]) {
            case DROP_LOADED: {
                ZrzutPaczka[playerid] --;
                ZrzutPaczkaCount[playerid] ++;

                if(IsValidDynamicRaceCP(ZrzutCP[playerid])) DestroyDynamicRaceCP(ZrzutCP[playerid]);
                if(IsValidDynamicMapIcon(ZrzutMapIcon[playerid])) DestroyDynamicMapIcon(ZrzutMapIcon[playerid]);

                if(ZrzutPaczka[playerid] < 1) {
                    ZrzutSwitch[playerid] = DROP_EMPTY;
                    ZrzutPaczka[playerid] = ZrzutPaczkaCount[playerid] = 0;

                    ZrzutCP[playerid] = CreateDynamicRaceCP(1, 409.8317, 2502.6594, 18.0052, 0.0, 0.0, 0.0, 10.0, .worldid=GetPVarInt(playerid, "player_zrzut_world"), .streamdistance=99999.0, .playerid=playerid);
                    ZrzutMapIcon[playerid] = CreateDynamicMapIcon(409.8317, 2502.6594, 18.0052, 20, -1, .worldid=GetPVarInt(playerid, "player_zrzut_world"), .streamdistance=99999.0, .playerid=playerid, .style=MAPICON_GLOBAL);
                    
                    addJobScore(playerid, math_random(50, 100));
                    RepairVehicle(GetPlayerVehicleID(playerid));
                    msg(playerid, C_GREEN, "Udaj siê na {c}opuszczone lotnisko{/c}, by za³adowaæ paczki do samolotu!");
                } else {
                    validZrzutPoint:
                    new rand = random(sizeof(ZrzutPositions));
                    if(GetPVarInt(playerid, "zrzut_last_point") == rand) goto validZrzutPoint;
                    SetPVarInt(playerid, "zrzut_last_point", rand);

                    ZrzutCP[playerid] = CreateDynamicRaceCP(4, unpackXYZ(ZrzutPositions[rand]), 0.0, 0.0, 0.0, 10.0, .worldid=GetPVarInt(playerid, "player_zrzut_world"), .streamdistance=99999.0, .playerid=playerid);
                    ZrzutMapIcon[playerid] = CreateDynamicMapIcon(unpackXYZ(ZrzutPositions[rand]), 19, -1, .worldid=GetPVarInt(playerid, "player_zrzut_world"), .streamdistance=99999.0, .playerid=playerid, .style=MAPICON_GLOBAL);

                    giveMoney(playerid, ZrzutReward[playerid]);
                    addJobScore(playerid, math_random(10, 30));
                    RepairVehicle(GetPlayerVehicleID(playerid));
                    achievement(playerid, ACH_ZRZUT);

                    msg(playerid, C_GREEN, "Zrzuci³eœ(aœ) {c}%d{/c} paczkê! Nowy cel podró¿y: {c}%s, %s{/c}.", ZrzutPaczkaCount[playerid], getZoneName(unpackXYZ(ZrzutPositions[rand])), getCityName(unpackXYZ(ZrzutPositions[rand])));
                    msg(playerid, C_GREEN, "Nagroda za zrzucenie paczki to: {c}%s${/c}.", strdot(ZrzutReward[playerid]));
                }
            }

            case DROP_EMPTY: {
                if(IsValidDynamicRaceCP(ZrzutCP[playerid])) DestroyDynamicRaceCP(ZrzutCP[playerid]);
                if(IsValidDynamicMapIcon(ZrzutMapIcon[playerid])) DestroyDynamicMapIcon(ZrzutMapIcon[playerid]);

                TogglePlayerControllable(playerid, false);
                GameTextForPlayer(playerid, "~r~~h~ZALADOWYWANIE TOWARU~y~...", 3000, 3);
                SetVehiclePosition(ZrzutVehicle[playerid], 409.8317, 2502.6594, 18.0052, 90.2832);
                SetTimerEx("ZrzutLoaded", 1000 * 3, false, "d", playerid);
            }
        }
        UpdateJobDraw(playerid, JOB_ZRZUT);
    }
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid)
{
    if(vehicleid == ZrzutVehicle[playerid] && pJob[playerid] == JOB_ZRZUT) return ExitPraca(playerid), 1;
    return 1;
}