#include YSI_Coding\y_hooks

enum e_taxiPositionData {
    Float:taxi_actor[4],
    Float:taxi_cp[4]
};

new 
    taxiPositionData[][e_taxiPositionData] = {
        {{-1984.9602, 154.0700, 27.2553, 90.1102},      {-1988.8297, 154.0624, 27.1059, 90.1102}},
        {{-1818.6117, 596.3712, 34.7397, 357.8159},     {-1818.4735, 600.0026, 34.5842, 357.8158}},
        {{-1767.1327, 831.6763, 24.4507, 2.1571},       {-1767.2833, 835.7074, 24.3032, 2.1571}},
        {{-1664.7567, 1234.5291, 6.7493, 317.5651},     {-1661.7721, 1237.7937, 6.6115, 317.5652}},
        {{-2488.6699, 1223.4529, 34.7397, 318.6686},    {-2486.3176, 1226.1268, 34.5841, 318.6678}},
        {{-2275.6619, 703.3955, 49.0129, 268.5093},     {-2270.4519, 703.2606, 48.8653, 268.5096}},
        {{-2583.7783, 574.6621, 14.1818, 181.6006},     {-2583.6597, 570.3460, 14.0275, 181.6027}},
        {{-2077.1274, -77.1826, 34.8881, 5.0846},       {-2077.0261, -73.3004, 34.7385, 358.6233}}
    },

    TaxiNamesWoman[][] = {"Karolina", "Kornelia", "Kasia", "Basia", "Ola", "Nikola", "Roksana", "Anita", "Mariola", "¯aneta", "Klaudia", "Marta", "Martyna", "Marzena", "Agnieszka", "Halina", "Gra¿yna", "Ania", "Patrycja"},
    TaxiNamesMan[][] = {"Mateusz", "Kacper", "Borys", "Krystian", "Damian", "Piotr", "Przemek", "Stachu", "Sebastian", "Alek", "Bartek", "Filip", "Eryk", "Daniel", "Mi³osz", "Krzychu", "Kuba", "Ziemowit", "Miko³aj", "Konrad"},

    TaxiClient[MAX_PLAYERS],
    TaxiActor[MAX_PLAYERS],
    Text3D:TaxiLabel[MAX_PLAYERS],
    TaxiCP[MAX_PLAYERS],
    TaxiVehicle[MAX_PLAYERS],
    TaxiReward[MAX_PLAYERS],
    TaxiSwitch[MAX_PLAYERS]
;

#define TAXI_EMPTY      0
#define TAXI_CLIENT     1

hook function ResetPlayerData(playerid)
{
    TaxiActor[playerid] = TaxiCP[playerid] = TaxiVehicle[playerid] = -1;
    TaxiLabel[playerid] = Text3D:-1;
    TaxiReward[playerid] = 0;
    TaxiSwitch[playerid] = TaxiClient[playerid] = TAXI_EMPTY;
    return continue(playerid);
}

stock Taxi_Start(playerid)
{
    TaxiSwitch[playerid] = TAXI_EMPTY;
    TaxiReward[playerid] = 0;
    TaxiClient[playerid] = 0;

    switch(random(7)) {
        case 0: TaxiVehicle[playerid] = CreateVehicle(420, -2624.3811, -20.3573, 3.7330, 182.3381, 6, 6, -1);
        case 1: TaxiVehicle[playerid] = CreateVehicle(420, -2634.7388, -19.6801, 3.7328, 181.2853, 6, 6, -1);
        case 2: TaxiVehicle[playerid] = CreateVehicle(420, -2645.1675, -20.3702, 3.7328, 179.6205, 6, 6, -1);
        case 3: TaxiVehicle[playerid] = CreateVehicle(420, -2655.6665, -20.3468, 3.7340, 178.6502, 6, 6, -1);
        case 4: TaxiVehicle[playerid] = CreateVehicle(420, -2665.4563, -20.3113, 3.7320, 180.5607, 6, 6, -1);
        case 5: TaxiVehicle[playerid] = CreateVehicle(420, -2675.6677, -19.6135, 3.7353, 180.3867, 6, 6, -1);
        case 6: TaxiVehicle[playerid] = CreateVehicle(420, -2686.4487, -20.0350, 3.7337, 179.9200, 6, 6, -1);
    }
    SetVehicleVirtualWorld(TaxiVehicle[playerid], VW_TAXI);
    PutPlayerInVehicle(playerid, TaxiVehicle[playerid], 0);
    return Taxi_Random(playerid), 1;
}

stock Taxi_Random(playerid)
{
    new skinid = random(311);
    if(skinid == 74) return Taxi_Random(playerid);

    new rand = random(sizeof(taxiPositionData));
    if(GetPVarInt(playerid, "player_taxi_random") == rand) return Taxi_Random(playerid);
    SetPVarInt(playerid, "player_taxi_random", rand);

    if(IsValidDynamicActor(TaxiActor[playerid])) DestroyDynamicActor(TaxiActor[playerid]);
    if(IsValidDynamicCP(TaxiCP[playerid])) DestroyDynamicCP(TaxiCP[playerid]);
    if(IsValidDynamic3DTextLabel(TaxiLabel[playerid])) DestroyDynamic3DTextLabel(TaxiLabel[playerid]);

    TaxiActor[playerid] = CreateDynamicActor(skinid, unpackXYZR(taxiPositionData[rand][taxi_actor]), .worldid=VW_TAXI, .playerid=playerid);
    TaxiCP[playerid] = CreateDynamicCP(unpackXYZ(taxiPositionData[rand][taxi_cp]), 3.0, .worldid=VW_TAXI, .playerid=playerid, .priority=1, .streamdistance=99999.0);
    TaxiLabel[playerid] = CreateDynamic3DTextLabel(" ", -1, unpackXYZ(taxiPositionData[rand][taxi_actor]) + 0.25, 30.0, .worldid=VW_TAXI, .playerid=playerid);

    new tstr[128]; format(tstr, sizeof(tstr), ""BLUE"%s "GREEN"TAXI\n"WHITE"%s", (SkinKobiecy(skinid)) ? ("KLIENTKA") : ("KLIENT"), (SkinKobiecy(skinid)) ? (TaxiNamesWoman[random(sizeof(TaxiNamesWoman))]) : (TaxiNamesMan[random(sizeof(TaxiNamesMan))]));
    UpdateDynamic3DTextLabelText(TaxiLabel[playerid], -1, tstr);
   
    msg(playerid, C_CHAT, "Klient ju¿ czeka na Ciebie! Lokalizacja: {c}%s, %s{/c}.", getZoneName(unpackXYZ(taxiPositionData[rand][taxi_actor])), getCityName(unpackXYZ(taxiPositionData[rand][taxi_actor])));
    return UpdateJobDraw(playerid, JOB_TAXI), 1;
}

hook OnPlayerEnterDynamicCP(playerid, checkpointid)
{
    if(pJob[playerid] == JOB_TAXI && IsPlayerInVehicle(playerid, TaxiVehicle[playerid]) && checkpointid == TaxiCP[playerid]) {
        switch(TaxiSwitch[playerid]) {
            case TAXI_EMPTY: {
                TaxiReward[playerid] = (math_random(700, 1000) * pJobLevel[playerid][JOB_TAXI]) + (pJobReward[playerid][JOB_TAXI] * 100) + ((JobDay == JOB_TAXI) ? (1000) : (0));

                if(IsValidDynamicActor(TaxiActor[playerid])) DestroyDynamicActor(TaxiActor[playerid]);
                if(IsValidDynamicCP(TaxiCP[playerid])) DestroyDynamicCP(TaxiCP[playerid]);
                if(IsValidDynamic3DTextLabel(TaxiLabel[playerid])) DestroyDynamic3DTextLabel(TaxiLabel[playerid]);

                validTaxiDestination:
                new rand = random(sizeof(taxiPositionData));
                if(GetPVarInt(playerid, "player_taxi_random") == rand) goto validTaxiDestination;
                SetPVarInt(playerid, "player_taxi_random", rand);

                addJobScore(playerid, math_random(5, 10));
                TaxiCP[playerid] = CreateDynamicCP(unpackXYZ(taxiPositionData[rand][taxi_cp]), 3.0, .worldid=VW_TAXI, .playerid=playerid, .priority=1, .streamdistance=99999.0);

                msg(playerid, C_CHAT, "Klient odebrany! Cel podró¿y: {c}%s, %s{/c}.", getZoneName(unpackXYZ(taxiPositionData[rand][taxi_cp])), getCityName(unpackXYZ(taxiPositionData[rand][taxi_cp])));
                TaxiSwitch[playerid] = TAXI_CLIENT;
            }

            case TAXI_CLIENT: {
                if(IsValidDynamicCP(TaxiCP[playerid])) DestroyDynamicCP(TaxiCP[playerid]);

                TaxiClient[playerid] ++;
                msg(playerid, C_CHAT, "Dostarczy³eœ(aœ) {c}%d{/c} klienta! Nagroda: {c}%s${/c}.", TaxiClient[playerid], strdot(TaxiReward[playerid]));

                giveMoney(playerid, TaxiReward[playerid]);
                addJobScore(playerid, math_random(30, 50));
                achievement(playerid, ACH_TAXI);

                TaxiSwitch[playerid] = TAXI_EMPTY;
                Taxi_Random(playerid);
            }
        }
        UpdateJobDraw(playerid, JOB_TAXI);
    }
    return 1;
}