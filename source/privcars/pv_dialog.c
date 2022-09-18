stock OnDialogPrivCars(playerid, dialogid, response, listitem, inputtext[])
{
    OnDialogPrivCarBuy(playerid, dialogid, response, listitem);
    OnDialogPrivCarManage(playerid, dialogid, response, listitem, inputtext);
    OnDialogPrivCarKeys(playerid, dialogid, response, listitem, inputtext);
    OnDialogPrivCarUlepszenia(playerid, dialogid, response, listitem);
    OnDialogGielda(playerid, dialogid, response, listitem, inputtext);

    switch(dialogid) {
        case DIALOG_PRIVCAR_TELEPORT: {
            if(!response) return 1;
            new slotid = GetPrivCarSlot(playerid, pVehFetch[playerid][listitem]);
            if(GetPVarInt(playerid, "privcar_teleport_type") == 1) callcmd::pth(playerid, form("%d", slotid));
            else callcmd::ptp(playerid, form("%d", slotid));
            return 1;
        }

        case DIALOG_PRIVCAR_NEWBIE: {
            if(!response) return 1;
            
            switch(listitem) {
                case 0: {

                }

                case 1: {
                    if(pVehCount[playerid] >= ((pUlepszenia[playerid][ULEPSZENIE_PRIVCAR]) ? (5) : (3))) return dialogBox(playerid, C_RED, ""RED"Posiadasz ju¿ maksymaln¹ iloœæ prywatnych pojazdów.");
                    
                    new count = (pVehCount[playerid] == 0) ? (1) : (pVehCount[playerid]);
                    if(getMoney(playerid) < (PP_COST_MONEY * count)) return valueError(playerid, (PP_COST_MONEY * count));
                    showDialog(playerid, DIALOG_PRIVCAR_BUY, DIALOG_STYLE_LIST, ""BLUE"Zakup prywatny pojazd", ""WHITE"Jednoœlady\n"WHITE"Kabriolety\n"WHITE"Przemys³owe\n"WHITE"Lowridery\n"WHITE"Off-Roads\n"WHITE"Publiczne\n"WHITE"Salonowe\n"WHITE"Sportowe\n"WHITE"Unikalne\n", "Dalej", "Cofnij");
                }

                case 2: ShowGieldaPanel(playerid);
                case 3: callcmd::pp(playerid);

                default: {
                    new privcarid = pVehFetch[playerid][listitem - 4];
                    if(!pVehSpawned[privcarid]) CreatePrivCar(playerid, privcarid, false, 0);
                    else task_await(RunPrivCarManage(playerid, privcarid));
                }
            }
            return 1;
        }
    }
    return 0;
}