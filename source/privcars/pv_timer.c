stock PrivCarTimer(playerid)
{
    if(pVehID[playerid] != -1) {
        new privcarid = pVehID[playerid];
        if(IsPlayerInVehicle(playerid, pVeh[privcarid]) && pVehSpawned[privcarid] && !pVehSell[privcarid]) {
            new Float:speed = GetVehSpeed(pVeh[privcarid]), Float:dist = 0.0;
            if(speed <= 300) {
                dist = speed / ((pVehUlepszenie[privcarid][ULEPSZENIE_DIST]) ? (1000) : (2000));
                pVehDist[privcarid] += dist;

                new Float:fuel = 0.0;
                switch(pVehEngine[privcarid]) {
                    case GASOLINE_ENGINE: {
                        if(pVehGas[privcarid] && pVehGasEnabled[privcarid]) fuel = speed / ((pVehUlepszenie[privcarid][ULEPSZENIE_FUEL]) ? (3000) : (4000));
                        else fuel = speed / ((pVehUlepszenie[privcarid][ULEPSZENIE_FUEL]) ? (4000) : (5000));
                    }
                    case DIESEL_ENGINE: fuel = speed / ((pVehUlepszenie[privcarid][ULEPSZENIE_FUEL]) ? (6000) : (7000));
                }
                pVehFuel[privcarid][(pVehGasEnabled[privcarid]) ? (GAS_AMOUNT) : (FUEL_AMOUNT)] -= fuel;
            }

            if(pVehFuel[privcarid][(pVehGasEnabled[privcarid]) ? (GAS_AMOUNT) : (FUEL_AMOUNT)] <= 0.0) SwitchVehicleEngine(pVeh[privcarid], false);
            UpdateDynamic3DTextLabelText(pVehLabel[privcarid], -1, UpdatePrivCarLabel(privcarid));
        }
    }
}