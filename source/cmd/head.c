alias:setrank("setadmin")
CMD:setrank(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new id, admin, level;
    if(sscanf(params, "ddd", id, admin, level)) return syntaxError(playerid, "/setrank [id] [ranga] [poziom rangi]");
    if(!IsPlayerConnected(id)) return Polaczenie(playerid);
    if(admin < 0 || admin > HEAD) return sendError(playerid, ""RED"Podano z�y numer rangi! 0-Gracz, 1-Moderator, 2-Admin, 3-Head Admin");
    if(level < 0 || level > 2) return sendError(playerid, ""RED"Podano z�y numer poziomu! Maksymalny dopuszczalny poziom rangi to: "WHITE"2");
    if(admin > 0 && level == 0) return sendError(playerid, ""RED"Nadaj�c rang� administracyjn�, poziom rangi nie mo�e by� zerowy!");
    if(BlockAdminTroll(playerid, id)) return sendError(playerid, ""RED"Nie mo�esz zarz�dza� osobami, kt�re maj� rang� wy�sz�/r�wn� Tobie!");
    if(playerData[id][player_accountID] == 1) return sendError(playerid, ""RED"Wykurwiaj �mieciu jebany kurwo <3333");
    if(playerData[id][player_accountID] == playerData[playerid][player_accountID]) return sendError(playerid, ""RED"Nie mo�esz zarz�dza� swoim kontem XD");

    playerData[id][player_adminLoad] = admin;
    playerData[id][player_adminLevel] = level;
    
    if(strmatch(playerData[id][player_adminPass], "@NULL", true)) return showDialog(id, DIALOG_ADMIN_PASSWORD, DIALOG_STYLE_PASSWORD, ""YELLOW"Ustawianie has�a na s�u�b�", ""WHITE"Wpisz poni�ej has�o, kt�rym b�dziesz si� logowa� na rang� administracyjn�.", "Ustaw", "Anuluj");
    if(admin == GRACZ) {
        strcat(playerData[playerid][player_adminPass], "@NULL");
        m_pquery_format("UPDATE es_accounts SET admin_pass='@NULL' WHERE id=%d LIMIT 1;", playerData[id][player_accountID]);
        Iter_Remove(Permission, id);
    }
    m_pquery_format("UPDATE es_accounts SET admin=%d, admin_level=%d WHERE id=%d LIMIT 1;", admin, level, playerData[id][player_accountID]);

    msg(playerid, C_VIOLET2, "Ustawi�e�(a�) graczowi {c}%s(%d){/c} rang�: {c}%s poz. %d{/c}.", getNick(id), id, getRankName(admin), level);
    return msg(id, C_VIOLET2, "%s {c}%s(%d){/c} ustawi�(a) Ci rang�: {c}%s poz. %d{/c}.", getRankName(playerData[playerid][player_admin]), getNick(playerid), playerid, getRankName(admin), level);
}
flags:setrank(HEAD)

CMD:tpxyz(playerid, params[])
{
    if(!checkAdminLevel(playerid, 1)) return rankError(playerid, 1);

    new Float:x, Float:y, Float:z;
    if(sscanf(params, "fff", x, y, z)) return syntaxError(playerid, "/tpxyz [x] [y] [z]");
    return SetPlayerPos(playerid, x, y, z), 1;
}
flags:tpxyz(HEAD)

CMD:csound(playerid, params[])
{
    if(!checkAdminLevel(playerid, 2)) return rankError(playerid, 2);

    new soundid;
    if(sscanf(params, "d", soundid)) return syntaxError(playerid, "/csound [soundid]");
    if(soundid < 1) return sendError(playerid, ""RED"ID d�wi�ku nie mo�e by� poni�ej zera!");
    return playSound(playerid, soundid), 1;
}
flags:csound(HEAD)