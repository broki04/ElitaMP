#include YSI_Coding\y_hooks 

new bool:pGlitchArena[MAX_PLAYERS], Float:pGlitchRecord[MAX_PLAYERS], Float:pGlitch[MAX_PLAYERS][4];
hook function ResetPlayerData(playerid) {
    pGlitchArena[playerid] = false;
    pGlitch[playerid] = Float:{0, 0, 0, 0};
    pGlitchRecord[playerid] = 100.0;
    return continue(playerid);    
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(pGlitchArena[playerid] && GetPlayerWeapon(playerid) == WEAPON_DEAGLE) {
        if(GetPlayerAmmo(playerid) == 7 && PRESSED(KEY_FIRE)) pGlitch[playerid][0] = GetTickCount();
        else if(RELEASED(KEY_FIRE) && GetPlayerAmmo(playerid) == 0) {
            pGlitch[playerid][1] = GetTickCount();
            pGlitch[playerid][2] = pGlitch[playerid][1] - pGlitch[playerid][0];
            pGlitch[playerid][3] = pGlitch[playerid][2] / 1000;

            if(pGlitch[playerid][3] < 0.4 || pGlitch[playerid][3] > 10.0) dialogBox(playerid, C_RED, "Wykonany {c}glitch{/c} nie móg³ zostaæ zaliczony! Spróbuj ponownie!");
            else {
                if(pGlitch[playerid][3] < pGlitchRecord[playerid]) {
                    pGlitchRecord[playerid] = pGlitch[playerid][3];

                    dbstr[0] = EOS;
                    format(dbstr, sizeof(dbstr), "SELECT nick FROM es_accounts ORDER BY glitch ASC LIMIT 10;");
                    m_pquery(dbstr, "OnGlitchRecord", "df", playerid, pGlitchRecord[playerid]);
                }
                else showGameDraw(playerid, form("~y~Czas: ~w~~h~%.3fs~n~~r~~h~Twoj rekord: ~w~~h~%.3fs", pGlitch[playerid][3], pGlitchRecord[playerid]), 5);
            }

            ResetPlayerWeapons(playerid);
            pGlitch[playerid] = Float:{0, 0, 0, 0};
            giveWeapon(playerid, 24, 7);
        }
    }
    return 1;
}

function OnGlitchRecord(playerid, Float:record)
{
    new nick[25], place;
    for(new i = 0; i < cache_num_rows(); i++) {
        cache_get_value_name(i, "nick", nick, sizeof(nick)); place ++;
        if(!strcmp(nick, getNick(playerid))) { place --; break; }
    }

    if(place < 10) showGameDraw(playerid, form("~y~Czas: ~w~~h~%.3fs~n~~p~Miejsce: ~w~~h~%d", record, place + 1), 5);
    else showGameDraw(playerid, form("~y~Czas: ~w~~h~%.3fs~n~~p~Miejsce: ~w~~h~---", record), 5);
    return msgAll(C_GREEN, "Gracz {c}%s(%d){/c} ustanowi³(a) swój nowy rekord glitcha: {c}%.3f{/c} sek.", getNick(playerid), playerid, record), 1;
}

stock JoinGlitchArena(playerid, bool:respawn = false)
{
    if(!respawn) SavePlayerPosition(playerid);
    ResetPlayerWeapons(playerid);
    Teleport(playerid, false, 1388.68, -19.81, 1000.91, 0.0, 1, VW_ARENA + 1000 + 100 + playerid); // XD

    pGlitchArena[playerid] = true;
    pGlitch[playerid] = {0, 0, 0, 0};
    return giveWeapon(playerid, 24, 7), 1;
}

stock ExitGlitchArena(playerid)
{
    LoadPlayerPosition(playerid);
    pGlitchArena[playerid] = false;
    return msg(playerid, C_CHAT, "Opuœci³eœ(aœ) arenê do {c}glitcha{/c}."), 1;
}

CMD:glitch(playerid) 
{
    if(CheckPlayerGame(playerid)) return sendError(playerid, ""RED"Jesteœ ju¿ zapisany(a) na jak¹œ atrakcjê!");

    SavePlayerPosition(playerid);
    ResetPlayerWeapons(playerid);
    Teleport(playerid, false, 1388.68, -19.81, 1000.91, 0.0, 1, VW_ARENA + 1000 + 100 + playerid); 

    pGlitchArena[playerid] = true;
    pGlitch[playerid] = Float:{0, 0, 0, 0};
    return giveWeapon(playerid, 24, 7), 1;
}
alias:glitch("tgl", "gl", "cbug")