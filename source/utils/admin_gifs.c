new 
    walizkaPickup,
    walizkaOwner,

    bombaPickup,
    bombaOwner,
    bombaTimer
;

CMD:walizka(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);
    
    if(walizkaPickup != INVALID_PLAYER_ID) {
        if(walizkaOwner == playerid) {
            s[0] = EOS;
            catstr(s, C_LBLUE, -1, "Obecnie walizka {c}znajduje si�{/c} na mapie!\n"LBLUE"Kliknij poni�ej {c}odpowiedni{/c} przycisk, aby usun�� walizk�.");

            inline onWalizkaDialog(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ll, ii
                if(!rr) return 1;
                if(IsValidDynamicPickup(walizkaPickup)) DestroyDynamicPickup(walizkaPickup);
                
                walizkaOwner = INVALID_PLAYER_ID;
                msgAll(C_BROWN, "%s {c}%s(%d){/c} usun��(�a) walizke z mapy.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid);
                return TextDrawHideForAll(tdWalizka), 1;
            }
            Dialog_ShowCallback(playerid, using inline onWalizkaDialog, DIALOG_STYLE_MSGBOX, ""BLUE2"Usuwanie walizki", s, "Usu�", "Anuluj");
        } 
        else sendError(playerid, ""RED"Obecnie walizka jest postawiona na mapie.");
        return 1;
    }

    new name[50];
    if(sscanf(params, "s[50]", name)) return syntaxError(playerid, "/walizka [podpowied�]");
    if(strlen(name) < 3 || strlen(name) > 15) return sendError(playerid, ""RED"Podpowied� musi mie� {c}3-15{/c} znaki!");
    
    new Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
    walizkaPickup = CreateDynamicPickup(1210, 1, x, y, z, .worldid=0, .interiorid=0);
    walizkaOwner = playerid;

    msgAll(C_BROWN, "%s {c}%s(%d){/c} zgubi�(a) walizk�! Podpowied�: {c}%s{/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, name);
    return textDrawSetString(tdWalizka, "~g~~h~Walizka: ~w~~h~%s", name), 1;
}
flags:walizka(MODERATOR)

CMD:bomba(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    if(bombaPickup != INVALID_PLAYER_ID) {
        if(bombaOwner == playerid) {
            s[0] = EOS;
            catstr(s, C_LBLUE, -1, "Obecnie bomba {c}znajduje si�{/c} na mapie!\n"LBLUE"Kliknij poni�ej {c}odpowiedni{/c} przycisk, aby usun�� bomb�.");

            inline onWalizkaDialog(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ll, ii
                if(!rr) return 1;
                if(IsValidDynamicPickup(bombaPickup)) DestroyDynamicPickup(bombaPickup);
                
                bombaOwner = INVALID_PLAYER_ID;
                KillTimer(bombaTimer);
                msgAll(C_BROWN, "%s {c}%s(%d){/c} usun��(�a) bomb� z mapy.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid);
                return TextDrawHideForAll(tdBomba), 1;
            }
            Dialog_ShowCallback(playerid, using inline onWalizkaDialog, DIALOG_STYLE_MSGBOX, ""BLUE2"Usuwanie walizki", s, "Usu�", "Anuluj");
        } 
        else sendError(playerid, ""RED"Obecnie bomba jest pod�o�ona na mapie.");
        return 1;
    }

    new name[50];
    if(sscanf(params, "s[50]", name)) return syntaxError(playerid, "/bomba [podpowied�]");
    if(strlen(name) < 3 || strlen(name) > 15) return sendError(playerid, ""RED"Podpowied� musi mie� {c}3-15{/c} znaki!");
    
    new Float:x, Float:y, Float:z; GetPlayerPos(playerid, x, y, z);
    bombaPickup = CreateDynamicPickup(1210, 1, x, y, z, .worldid=0, .interiorid=0);
    bombaOwner = playerid;
    bombaTimer = SetTimer("BombaExplode", 1000 * 60 * 5, false);

    msgAll(C_LRED, "%s {c}%s(%d){/c} pod�o�y�(a) bomb�, kt�ra za {c}5 minut{/c} wybuchnie! Podpowied�: {c}%s{/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, name);
    return textDrawSetString(tdBomba, "~r~~h~Bomba: ~w~~h~%s", name), 1;
}
flags:bomba(MODERATOR)

function BombaExplode()
{
    if(IsValidDynamicPickup(bombaPickup)) DestroyDynamicPickup(bombaPickup);

    new Float:x, Float:y, Float:z;
    Streamer_GetItemPos(STREAMER_TYPE_PICKUP, bombaPickup, x, y, z);
    for(new i = 0; i < 3; i++) CreateExplosion(x, y, z, 1, 10.0);

    bombaOwner = INVALID_PLAYER_ID;
    KillTimer(bombaTimer);
    return msgAll(C_LRED, "Bomba zosta�a {c}zdetonowana{/c}, bo nikt jej nie znalaz� w ci�gu {c}5 minut{/c}!"), 1;
}

hook OnPlayerPickUpDynPickup(playerid, STREAMER_TAG_PICKUP:pickupid)
{
    if(pickupid == walizkaPickup) {
        if(IsValidDynamicPickup(walizkaPickup)) DestroyDynamicPickup(walizkaPickup);
        walizkaOwner = INVALID_PLAYER_ID;
        TextDrawHideForAll(tdWalizka);

        new money = math_random(3000, 6000);
        giveMoney(playerid, money);
        addLevelScore(playerid, math_random(100, 300));
        return msgAll(C_CORAL, "Gracz {c}%s(%d){/c} znalaz�(a) walizk�! Nagroda: {c}%s${/c}.", getNick(playerid), playerid, strdot(money));
    }

    if(pickupid == bombaPickup) {
        s[0] = EOS;
        catstr(s, C_LY, -1, "Odnalaz�e�(a�) bomb�, kt�r� musisz rozbroi�!\n"LY"Kliknij poni�ej {c}odpowiedni{/c} kolor kabelka, kt�ry musisz przeci��.");

        inline onBombaDialog(pp, dd, rr, ll, string:ii[]) {
            #pragma unused pp, dd, rr, ll, ii

            switch(random(2)) {
                case 0: {
                    new money = math_random(3000, 6000);
                    giveMoney(playerid, money);
                    addLevelScore(playerid, math_random(100, 300));
                    msgAll(C_CORAL, "Gracz {c}%s(%d){/c} rozbroi�(a) bomb�! Nagroda: {c}%s${/c}.", getNick(playerid), playerid, strdot(money));
                }
                case 1: msgAll(C_CORAL, "Gracz {c}%s(%d){/c} znalaz�(a) bomb�, lecz nie rozbroi�(a) jej!");
            }

            if(IsValidDynamicPickup(bombaPickup)) DestroyDynamicPickup(bombaPickup);
            bombaOwner = INVALID_PLAYER_ID;
            TextDrawHideForAll(tdBomba);
            return KillTimer(bombaTimer), 1;
        }
        Dialog_ShowCallback(playerid, using inline onBombaDialog, DIALOG_STYLE_MSGBOX, ""GREEN"Bomba", s, ""RED"Czerwony", ""BLUE"Niebieski");
    }
    return 1;
}