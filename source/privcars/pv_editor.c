#define SLOW_EDITOR         0.05
#define FAST_EDITOR         0.10
#define ROTATION_EDITOR     75.0

stock SetEditorAttachment(playerid, bool:position)
{
    pVehEdit[playerid] = true;
    pVehEditType[playerid] = position;
    return 1;
}

stock UpdateEditorAttachment(playerid, keys, updown, leftright)
{
    new privcarid = GetPVarInt(playerid, "privcarid_attachment"), cache = GetPVarInt(playerid, "cache_attachment"), bool:position = pVehEditType[playerid];

    if(keys & KEY_NO) {
        pVehEdit[playerid] = false;

        for(new i = 0; i < 3; i++) {
            pVehObjectPos[privcarid][cache][i] = pVehEditorPos[playerid][i];
            pVehObjectRot[privcarid][cache][i] = pVehEditorRot[playerid][i];
        }

        dbstr[0] = EOS;
        format(dbstr, sizeof(dbstr), "UPDATE es_privcars_attachments SET x=%f, y=%f, z=%f, rx=%f, ry=%f, rz=%f WHERE id=%d LIMIT 1;", unpackXYZ(pVehObjectPos[privcarid][cache]), unpackXYZ(pVehObjectRot[privcarid][cache]), pVehObjectID[privcarid][cache]);
        return m_pquery(dbstr, "SaveEditorAttachment", "ddd", playerid, privcarid, cache), 1;
    }

    if(keys & KEY_SPRINT) {
        if(updown == KEY_UP) {
            if(position) pVehEditorPos[playerid][1] += (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR);
            else pVehEditorRot[playerid][1] -= (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR) * ROTATION_EDITOR;
        }
        else if(updown == KEY_DOWN) {
            if(position) pVehEditorPos[playerid][1] -= (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR);
            else pVehEditorRot[playerid][1] += (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR) * ROTATION_EDITOR;
        }
    } else {
        if(updown == KEY_UP) {
            if(position) pVehEditorPos[playerid][2] += (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR);
            else pVehEditorRot[playerid][0] += (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR) * ROTATION_EDITOR;
        }
        else if(updown == KEY_DOWN) {
            if(position) pVehEditorPos[playerid][2] -= (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR);
            else pVehEditorRot[playerid][0] -= (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR) * ROTATION_EDITOR;
        }
    }

    if(leftright == KEY_LEFT) {
        if(position) pVehEditorPos[playerid][0] -= (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR);
        else pVehEditorRot[playerid][2] += (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR) * ROTATION_EDITOR;
    }
    else if(leftright == KEY_RIGHT) {
        if(position) pVehEditorPos[playerid][0] += (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR);
        else pVehEditorRot[playerid][2] -= (keys & KEY_JUMP) ? (FAST_EDITOR) : (SLOW_EDITOR) * ROTATION_EDITOR;
    }

    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, pVehObjectData[privcarid][cache], E_STREAMER_ATTACH_OFFSET_X, pVehEditorPos[playerid][0]);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, pVehObjectData[privcarid][cache], E_STREAMER_ATTACH_OFFSET_Y, pVehEditorPos[playerid][1]);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, pVehObjectData[privcarid][cache], E_STREAMER_ATTACH_OFFSET_Z, pVehEditorPos[playerid][2]);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, pVehObjectData[privcarid][cache], E_STREAMER_ATTACH_R_X, pVehEditorRot[playerid][0]);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, pVehObjectData[privcarid][cache], E_STREAMER_ATTACH_R_Y, pVehEditorRot[playerid][1]);
    Streamer_SetFloatData(STREAMER_TYPE_OBJECT, pVehObjectData[privcarid][cache], E_STREAMER_ATTACH_R_Z, pVehEditorRot[playerid][2]);
    Streamer_Update(playerid, STREAMER_TYPE_OBJECT);

    AttachDynamicObjectToVehicle(pVehObjectData[privcarid][cache], pVeh[privcarid], unpackXYZ(pVehEditorPos[playerid]), unpackXYZ(pVehEditorRot[playerid]));

    s[0] = EOS;
    if(position) format(s, sizeof(s), "~y~X: ~w~~h~%.3f ~y~Y: ~w~~h~%.3f ~y~Z: ~w~~h~%.3f~n~~w~~h~Strzalki ~r~~h~- ~w~~h~nawigacja~n~~w~~h~SPACJA + ~r~~h~gora/dol~w~~h~: przod/tyl~n~~w~~h~SHIFT: ~r~~h~szybsza edycja~n~~w~~h~Edycje obiektu zakonczysz klawiszem: ~g~~h~N", pVehEditorPos[playerid][0], pVehEditorPos[playerid][1], pVehEditorPos[playerid][2]);
    else format(s, sizeof(s), "~y~RX: ~w~~h~%.3f ~y~RY: ~w~~h~%.3f ~y~RZ: ~w~~h~%.3f~n~~w~~h~Strzalki ~r~~h~- ~w~~h~nawigacja~n~~w~~h~SPACJA + ~r~~h~gora/dol~w~~h~: rotacja Y~n~~w~~h~SHIFT: ~r~~h~szybsza edycja~n~~w~~h~Edycje obiektu zakonczysz klawiszem: ~g~~h~N", pVehEditorRot[playerid][0], pVehEditorRot[playerid][1], pVehEditorRot[playerid][2]);
    return showGameDraw(playerid, s, 1000), 1;
}

function SaveEditorAttachment(playerid, privcarid, cache)
{
    TogglePlayerControllable(playerid, true);
    ShowPlayerObjectMenu(playerid, privcarid, cache);
    return PlayerTextDrawHide(playerid, tdGameDraw);
}
