stock OnDialogGielda(playerid, dialogid, response, listitem, inputtext[])
{
    OnDialogGieldaBuy(playerid, dialogid, response);

    new privcarid = GetPVarInt(playerid, "gielda_privcarid"), sellid = GetPVarInt(playerid, "gielda_point");
    switch(dialogid) {
        case DIALOG_GIELDA_MAIN: {
            if(!response) return callcmd::pp(playerid);

            switch(listitem) {
                case 0: {
                    s[0] = EOS;
                    s = ""VIOLET2"Punkt sprzeda¿y\t"WHITE"Pojazd\t"BLUE"W³aœciciel\n";

                    for(new i = 0; i < sizeof(gieldaSellPos); i++) {
                        if(gieldaSellOwned[i] != -1) {
                            new pid = gieldaSellOwned[i];
                            catstr(s, -1, -1, ""VIOLET2"%d.\t"WHITE"%s\t"BLUE"%s\n", i + 1, GetVehicleName(pVehModel[pid]), pVehOwnerName[pid]);
                        } else {
                            catstr(s, -1, -1, ""VIOLET2"%d.\t"WHITE"---\t"GREY"---\n", i + 1);
                        }
                    }
                    showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Rynek pojazdów", s, "OK", #);
                }

                case 1: {

                }
                
                case 2: EnterTeleport(playerid, "gielda");
            }
            return 1;
        }

        case DIALOG_GIELDA_SELL: {
            if(!response || pVehSell[privcarid]) return 1;            

            new amount;
            if(sscanf(inputtext, "d", amount) || amount < 1000 || amount > 10000000) return showDialog(playerid, DIALOG_GIELDA_SELL, DIALOG_STYLE_INPUT, ""PINK"Wystawianie pojazdu na sprzeda¿", ""WHITE"Wpisz poni¿ej kwotê, za któr¹ chcesz "RED"wystawiæ pojazd "WHITE"na sprzeda¿:\n\n"RED"Nieprawid³owa wartoœæ pojazdu! "LY"(100.000$ - 10.000.000$)", "Wystaw", "Zamknij");
            
            pVehSell[privcarid] = true;
            pVehSellCost[privcarid] = amount;
            pVehSellPoint[privcarid] = sellid;
            gieldaSellOwned[sellid] = privcarid;

            m_pquery_format("UPDATE es_privcars SET sell=1, sell_cost=%d WHERE id=%d LIMIT 1;", pVehSellCost[privcarid], privcarid);

            setPosition(playerid, true, unpackXYZR(gieldaSellPos[sellid]));
            SwitchVehicleEngine(pVeh[privcarid], false);
            RemovePlayerFromVehicle(playerid);

            new gstr[256]; catstr(gstr, C_ZAJEBISTY, -1, ""DRED"PUNKT SPRZEDA¯Y "YELLOW"#%d\n"LRED"Koszt sprzeda¿y: {c}%s${/c}\n"LRED"Aby odkupiæ, wpisz: {c}/pbuy %d{/c}", sellid + 1, strdot(pVehSellCost[privcarid]), sellid + 1);
            return UpdateDynamic3DTextLabelText(gieldaSellLabel[sellid], -1, gstr), 1;
        }
    }
    return 0;
}