#include YSI_Coding\y_hooks

hook OnPlayerDisconnect(playerid, reason)
{
    if(pEventZone[playerid] && pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if(pEventZone[playerid]) {
        new keys, ud, lr; GetPlayerKeys(playerid, keys, ud, lr);
        TogglePlayerControllable(playerid, false);
    
		if(lr == KEY_LEFT) {
			pEventZonePos[playerid][0] -= 8.0;
  			if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
   			pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
     		GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
       	}
	    else if(lr & KEY_LEFT && keys & KEY_FIRE) {
   			pEventZonePos[playerid][0] += 8.0;
     		if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
     		pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
	       	GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
		}
  		else if(lr == KEY_RIGHT) {
    		pEventZonePos[playerid][2] += 8.0;
     		if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
   			pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
     		GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
		}
  		else if(lr & KEY_RIGHT && keys & KEY_FIRE) {
    		pEventZonePos[playerid][2] -= 8.0;
      		if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
   			pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
     		GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
		}
		else if(ud == KEY_UP) {
  			pEventZonePos[playerid][3] += 8.0;
    		if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
   			pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
     		GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
		}
		else if(ud & KEY_UP && keys & KEY_FIRE) {
		    pEventZonePos[playerid][3] -= 8.0;
	       	if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
	   		pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
	       	GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
		}
		else if(ud == KEY_DOWN) {
		    pEventZonePos[playerid][1] -= 8.0;
	       	if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
      		pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
	       	GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
		}
		else if(ud & KEY_DOWN && keys & KEY_FIRE) {
		    pEventZonePos[playerid][1] += 8.0;
	       	if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
      		pEventZoneCreator[playerid] = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
         	GangZoneShowForPlayer(playerid, pEventZoneCreator[playerid], C_DGREEN);
		}
		else if(keys & KEY_SECONDARY_ATTACK) {
			TogglePlayerControllable(playerid, true);
            pEventZone[playerid] = false;

            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ll, ii
                if(!rr) {
                    pEventZone[playerid] = false;
                    if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
                    return msg(playerid, C_CORAL, "Usun¹³eœ(aœ) strefê eventow¹."), 1;
                }

                if(EventZone != -1) GangZoneDestroy(EventZone);
                if(IsValidDynamicArea(EventRectangle)) DestroyDynamicArea(EventRectangle);

                EventZone = GangZoneCreate(unpackXYZR(pEventZonePos[playerid]));
                EventRectangle = CreateDynamicRectangle(unpackXYZR(pEventZonePos[playerid]), VW_EVENT, EventInterior);

                GangZoneShowForPlayer(playerid, EventZone, C_CORAL);
                GangZoneFlashForPlayer(playerid, EventZone, C_LRED);

                foreach(new i : Event) {
                    GangZoneShowForPlayer(i, EventZone, C_CORAL);
                    GangZoneFlashForPlayer(i, EventZone, C_LRED);
                
                    msg(i, C_CORAL, "Prowadz¹cy {c}%s(%d){/c} stworzy³(a) strefê eventu!", getNick(playerid), playerid);
                }

                pEventZone[playerid] = false;
                if(pEventZoneCreator[playerid] != -1) GangZoneDestroy(pEventZoneCreator[playerid]);
                return msg(playerid, C_CORAL, "Stworzy³eœ(aœ) strefê eventow¹!"), 1;
            }
            Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""GREEN"Strefa eventu", ""WHITE"Strefa eventu stworzona! Kliknij poni¿ej odpowiedni przycisk, czy zachowaæ strefê, czy usun¹æ.", "Zostaw", "Usuñ");
		}
    }
    return 1;
}