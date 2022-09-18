#include YSI_Coding\y_hooks 

stock giveWeapon(playerid, weaponid, ammo) {
	if(pCheatDetected[playerid] >= 1) pCheatDetected[playerid] --;
	pCheatTemp[playerid] = true;
	SetTimerEx("AC_ResetWH", 300, false, "d", playerid);
	return GivePlayerWeapon(playerid, weaponid, ammo);
}

stock showDialog(playerid, dialogid, style, caption_string[], info_string[], button1[], button2[], perpage = 0) 
{
	new dialog_caption_string[180]; format(dialog_caption_string, sizeof(dialog_caption_string), "%s", caption_string);
	new dialog_info_string[2800]; format(dialog_info_string, sizeof(dialog_info_string), "%s", info_string);

	if(perpage == 0) return ShowPlayerDialog(playerid, dialogid, style, dialog_caption_string, dialog_info_string, button1, button2);
	else return ShowPlayerDialog(playerid, dialogid, style, dialog_caption_string, dialog_info_string, button1, button2, perpage, ""RED">>>", ""RED"<<<");
}

hook function SetPlayerColor(playerid, color)
{
	if(playerData[playerid][player_logged] && IsPlayerSpawned(playerid)) {
		PlayerTextDrawHide(playerid, tdPosition);
		PlayerTextDrawColor(playerid, tdPosition, color);
		PlayerTextDrawShow(playerid, tdPosition);
	}
	return continue(playerid, color);
}

stock strmatch(const text[], const compare[], bool:truth) 
{
	if(!strcmp(text, compare, truth, strlen(compare)) && strlen(compare) == strlen(text)) return 1;
	else return 0;
}

stock strreplace(string[], const search[], const replacement[], bool:ignorecase = false, pos = 0, limit = -1, maxlength = sizeof(string))
{
	if(limit == 0) return 0;

	new sublen = strlen(search), replen = strlen(replacement), bool:packed = ispacked(string), maxlen = maxlength, len = strlen(string), count = 0;
	if(packed) maxlen *= 4;
	if(!sublen) return 0;
	
	while(-1 != (pos = strfind(string, search, ignorecase, pos))) {
		strdel(string, pos, pos + sublen);
		len -= sublen;
		
		if(replen && len + replen < maxlen) {
			strins(string, replacement, pos, maxlength);

			pos += replen;
			len += replen;
		}
		if(limit != -1 && ++count >= limit)	break;
	}
	return count;
}

stock GetColorRGBA(color)
{
	new str[10];
	format(str, sizeof(str), "{%06x}", RGBAToRGB(color));
	return str;
}

stock strdot(number) 
{
    new len[32], count = -1;
    valstr(len, number);

    for(new i = strlen(len); i > 0; i--) {
        if((count++) == 2) {
            strins(len, ".", i);
            count = 0;
        }
    }
    return len;
}

stock syntaxError(playerid, const string[])
{
	s[0] = EOS;
	format(s, sizeof(s), ""WHITE"Wprowadzono nieprawid³owe parametry.\n"WHITE"Poprawne u¿ycie: "YELLOW"%s", string);
	return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Nieprawid³owe u¿ycie", s, "OK", #);
}

stock CheckPlayerOnline(aid)
{
	new id = -1;
	foreach(new i : Player) {
		if(playerData[i][player_accountID] == aid) {
			id = i;
			break;
		}
	}
	return id;
}

stock _catstr(variable[], color, secondcolor, text[], lenght = sizeof(variable)) {
	new str[4098]; format(str, sizeof(str), "{%06x}%s", color >>> 8, text);

	new _clamp = strfind(str, "{c}");
	while(_clamp > 0){
		new _color[20]; format(_color, sizeof(_color), "{%06x}", secondcolor >>> 8);
		strdel(str, _clamp, _clamp + 3);
		strins(str, _color, _clamp);
		_clamp = strfind(str, "{c}");
	}
	_clamp = strfind(str, "{/c}");

	while(_clamp > 0) {
		new _color[20]; format(_color, sizeof(_color), "{%06x}", color >>> 8);
		strdel(str, _clamp, _clamp + 4);
		strins(str, _color, _clamp);
		_clamp = strfind(str, "{/c}");
	}
	return strcat(variable, str, lenght);
}

stock catstr(variable[], color, secondcolor, text[], {Float, _}:...) {
	new out[2048]; va_format(out, sizeof(out), text, va_start<4>);
	return _catstr(variable, color, secondcolor, out, sizeof(out));
}

stock newMessage(playerid, color, text[]) 
{
    new str[2048]; format(str, sizeof(str), "{%06x}%s", color >>> 8, text);

    new _clamp = strfind(str, "{c}");
    while(_clamp > 0) {
        strdel(str, _clamp, _clamp + 3);
        strins(str, "{FFFFFF}", _clamp);
        _clamp = strfind(str, "{c}");
    }
	
    _clamp = strfind(str, "{/c}");
    while(_clamp > 0) {
		new _color[20]; format(_color, sizeof(_color), "{%06x}", color >>> 8);
        strdel(str, _clamp, _clamp + 4);
        strins(str, _color, _clamp);
        _clamp = strfind(str, "{/c}");
    }

    if(playerid != INVALID_PLAYER_ID) SendClientMessage(playerid, color, str);
    else SendClientMessageToAll(color, str);
    return 1;
}

stock msg(playerid, colour, const text[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), text, va_start<3>);
	return newMessage(playerid, colour, out);
}

stock msgAll(colour, const text[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), text, va_start<2>);
	return newMessage(INVALID_PLAYER_ID, colour, out);
}

stock dialogBox(playerid, colour, const text[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), text, va_start<3>);

	s[0] = EOS; catstr(s, colour, -1, "%s", out);
	return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""GOLD"Informacja", s, "OK", #), 1;
}

stock sendError(playerid, const text[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), text, va_start<2>);

	if(!pAccountSettings[playerid][0]) msg(playerid, -1, "(b³¹d)*  "RED"%s", out);
	else {
		s[0] = EOS; catstr(s, C_RED, C_YELLOW, "%s", out);
		showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"B³¹d", s, "OK", #);
	}
	return 1;
}

stock msgAdmins(colour, const text[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), text, va_start<2>);
	foreach(new i : Permission) { if(playerData[i][player_admin] >= MODERATOR) msg(i, colour, out); }
	return 1;
}

stock textDrawSetString(Text:draw, const text[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), text, va_start<2>);
	return TextDrawSetString(draw, out);
}

stock playerTextDrawSetString(playerid, PlayerText:draw, const text[], {Float, _}:...)
{
	new out[1024];
	va_format(out, sizeof(out), text, va_start<3>);
	return PlayerTextDrawSetString(playerid, draw, out);
}

stock getReasonName(reasonid)
{
	new name[15];
	switch(reasonid) {
		case 0: name = "crash";
		case 1: name = "disconnect";
		case 2: name = "kick/ban";
	}
	return name;
}

stock setPosition(playerid, bool:inVeh, Float:x, Float:y, Float:z, Float:a) 
{
	if(inVeh) {
		new vehicleid = GetPlayerVehicleID(playerid);
		SetVehiclePos(vehicleid, x, y, z + 0.30);
		SetVehicleZAngle(vehicleid, a);
	} else {
		SetPlayerPos(playerid, x, y, z);
		SetPlayerFacingAngle(playerid, a);
	}
	return 1;
}

stock getPosition(playerid, bool:inVeh, &Float:x, &Float:y, &Float:z, &Float:a) 
{
	if(inVeh) {
		new vehicleid = GetPlayerVehicleID(playerid);
		GetVehiclePos(vehicleid, x, y, z);
		GetVehicleZAngle(vehicleid, a);
	} else {
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, a);
	}
	return 1;
}

stock Teleport(playerid, bool:inVeh, Float:x, Float:y, Float:z, Float:a, interiorid, worldid, bool:freeze = false) 
{
	SetPlayerInterior(playerid, interiorid);
	SetPlayerVirtualWorld(playerid, worldid);
	setPosition(playerid, inVeh, x, y, z, a);

	if(freeze) FreezePlayer(playerid, 2);
	Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    SetTimerEx("RefreshSpec", 100, false, "d", playerid);
	return Log(LOG_DEBUG, "[%s-tp]: Teleportowano gracza %s(%d)! (x: %.2f, y: %.2f, z: %.2f, a: %.2f, int: %d, vworld: %d)", (inVeh) ? ("veh") : ("ped"), getNick(playerid), playerid, x, y, z, a, interiorid, worldid), 1;
}

stock GetVehicleNameByID(const modelid[])
{
	for(new i = 400; i < 611; i++) { if(strfind(VehicleNames[i - 400], modelid, true) != -1) return i; }
	return 0;
}
stock Polaczenie(playerid) return sendError(playerid, ""RED"Ten gracz nie jest po³¹czony z serwerem.");
stock rankError(playerid, level) return dialogBox(playerid, C_RED, ""RED"Twój poziom rangi administracyjnej jest zbyt niski. Musisz mieæ przynajmniej "WHITE"%d "RED"poziom.", level);

stock UnixTime(type, time)
{
	switch(tolower(type)) {
		case 'd': time = time * 86400;
		case 'h': time = time * 3600;
		case 'm': time = time * 60;
	}
	return time;
}

stock strcopy(variable[], const data[], length = sizeof(data)) return strins((variable[0] = EOS, variable), data, 0, length);
stock ConvertTimeFormat(time, const format0[], const format1[], const format2[], const format3[]) 
{
	new name[32];
	switch(time) {
		case 0: strcopy(name, format0, 32);
		case 1: strcopy(name, format1, 32);
		case 2..4: strcopy(name, format2, 32);
		default: strcopy(name, format3, 32);
	}
	return name;
}

stock ConvertTime(time, type) 
{
    new _time;
    switch(type) {
        case TIME_TYPE_MS:  _time = time;
        case TIME_TYPE_UNIX: _time = time - gettime();
    }
 
    new _years = _time / 31556926;
    _time -= _years * 31556926;
 
    new string[128];
    if(_years > 0) format(string, sizeof(string), "%s", form("%d %s", _years, ConvertTimeFormat(_years, "lat", "rok", "lata", "lat")));
 
    new _months = _time / 2678400;
    _time -= _months * 2678400;
 
    if(_months > 0) {
        if(isnull(string)) format(string, sizeof(string), "%s%s", string, form("%d %s", _months, ConvertTimeFormat(_months, "miesiêcy", "miesi¹c", "miesi¹ce", "miesiêcy")));
        else format(string, sizeof(string), "%s, %s", string, form("%d %s", _months, ConvertTimeFormat(_months, "miesiêcy", "miesi¹c", "miesi¹ce", "miesiêcy")));
    }
 
    new _days = _time / 86400;
    _time -= _days * 86400;
 
    if(_days > 0) {
        if(isnull(string)) format(string, sizeof(string), "%s%s", string, form("%d %s", _days, ConvertTimeFormat(_days, "dni", "dzieñ", "dni", "dni")));
        else format(string, sizeof(string), "%s, %s", string, form("%d %s", _days, ConvertTimeFormat(_days, "dni", "dzieñ", "dni", "dni")));
    }
 
    new _hours = _time / 3600;
    _time -= _hours * 3600;
 
    if(_hours > 0) {
        if(isnull(string)) format(string, sizeof(string), "%s%s", string, form("%d %s", _hours, ConvertTimeFormat(_hours, "godzin", "godzinê", "godziny", "godzin")));
        else format(string, sizeof(string), "%s, %s", string, form("%d %s", _hours, ConvertTimeFormat(_hours, "godzin", "godzinê", "godziny", "godzin")));
    }
 
    new _minutes = _time / 60;
    _time -= _minutes * 60;
 
    if(_minutes > 0) {
        if(isnull(string)) format(string, sizeof(string), "%s%s", string, form("%d %s", _minutes, ConvertTimeFormat(_minutes, "minut", "minutê", "minuty", "minut")));
        else format(string, sizeof(string), "%s, %s", string, form("%d %s", _minutes, ConvertTimeFormat(_minutes, "minut", "minutê", "minuty", "minut")));
    }
 
    new _seconds = _time % 60;
    if(_seconds > 0) {
        if(isnull(string)) format(string, sizeof(string), "%s%s", string, form("%d %s", _time, ConvertTimeFormat(_time, "sekund", "sekundê", "sekundy", "sekund")));
        else format(string, sizeof(string), "%s, %s", string, form("%d %s", _time, ConvertTimeFormat(_time, "sekund", "sekundê", "sekundy", "sekund")));
    }
    return string;
}

stock IsBlockCommand(playerid, const nameblock[], time)
{
	new name[50]; format(name, sizeof(name), "%s", nameblock);
	if(gettime() < GetPVarInt(playerid, name)) return msg(playerid, C_ORANGE, "Bêdziesz móg³ {c}tej komendy{/c} u¿yæ za {c}%s{/c}.", ConvertTime(GetPVarInt(playerid, name) - gettime(), TIME_TYPE_MS)), 1;
	return SetPVarInt(playerid, name, gettime() + time), 0;
}

new pFreezeTimer[MAX_PLAYERS];
stock FreezePlayer(playerid, time)
{
	KillTimer(pFreezeTimer[playerid]);
    TogglePlayerControllable(playerid, false);
	pFreezeTimer[playerid] = SetTimerEx("UnFreezePlayer", 1000 * time, false, "d", playerid);
	return Log(LOG_DEBUG, "[freeze]: Zamro¿ono gracza na %d sekundy!", time);
}
function UnFreezePlayer(playerid) { KillTimer(pFreezeTimer[playerid]); return TogglePlayerControllable(playerid, true); }

stock foundPlayer(playerName[])
{
    new result, foundCount = 0;
  	if(isNumeric(playerName)) {
    	result = strval(playerName);
    	if(!IsPlayerConnected(result)) return ERROR_NOT_FOUND;
  	} else {
 		foreach(new i : Player) {
      		if(foundString(getNick(i), playerName)) {
        		foundCount++;
        		result = i;
          	}
      	}
        
    	if(foundCount < 1) return ERROR_NOT_FOUND;
    	else if(foundCount > 1) return ERROR_FOUND_MULTIPLE;
  	}
  	return result;
}

stock foundString(string1[], string2[], &start = 0, &end = 0)
{
  	if(!strlen(string1) || !strlen(string2)) return false;
  	new num = strfind(string1, string2, true);

  	if(num != -1) {
   		start = num;
   		end = start + strlen(string2);
   		return true;
  	}
  	return false;
}

stock randomPositionArea(Float:minx, Float:miny, Float:maxx, Float:maxy, &Float:fDestX, &Float:fDestY) 
{
    new iMin, iMax, Float:mul = floatpower(10.0, 4);
    iMin = floatround(minx * mul);
    iMax = floatround(maxx * mul);
    fDestX = float(random(iMax - iMin) + iMin) / mul;

    iMin = floatround(miny * mul);
    iMax = floatround(maxy * mul);
    fDestY = float(random(iMax - iMin) + iMin) / mul; 
}

stock IsValidGangBuffer(buffer[], min, max) 
{
	new len = strlen(buffer);
	if(!(min <= len <= max)) return 0;

	for(new i; i != len; i++) {
		switch(buffer[i]) {
			case 'a' .. 'z': continue;
			case 'A' .. 'Z': continue;
			case '0' .. '9': continue;
			case ' ', '.', '-', '[', ']': continue;
			default: return 0;
		}
	}
	return 1;
}

stock DeletePolish(buffer[])
{
	for(new x, j = strlen(buffer); x != j; x++) {
		switch(buffer[x]) {
			case 165: buffer[x] = 0x41;
			case 185: buffer[x] = 0x61;
			case 202: buffer[x] = 0x45;
			case 234: buffer[x] = 0x65;
			case 211: buffer[x] = 0x4f;
			case 243: buffer[x] = 0x6f;
			case 198: buffer[x] = 0x43;
			case 230: buffer[x] = 0x63;
			case 163: buffer[x] = 0x4c;
			case 179: buffer[x] = 0x6c;
			case 209: buffer[x] = 0x4e;
			case 241: buffer[x] = 0x6e;
			case 140: buffer[x] = 0x53;
			case 156: buffer[x] = 0x73;
			case 143, 175: buffer[x] = 0x5a;
			case 159, 191: buffer[x] = 0x7a;
		}
	}

	new out[256];
	format(out, sizeof(out), "%s", buffer);
	return out;
}

stock valueError(playerid, amount)
{
	s[0] = EOS;
	format(s, sizeof(s), ""WHITE"Nie staæ ciê na zakup!\n"WHITE"Brakuje Ci: "YELLOW"%s$", strdot(amount - getMoney(playerid)));
	return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""BLUE"Brak finansów", s, "OK", #);
}

stock toggleTextDraw(Text:draw, playerid, bool:toggle) 
{
    if(playerid != -1) {
        if(toggle) TextDrawShowForPlayer(playerid, draw);
        else TextDrawHideForPlayer(playerid, draw);
    } else {
        if(toggle) TextDrawShowForAll(draw);
        else TextDrawHideForAll(draw);
    }
    return 1;
}

stock togglePlayerTextDraw(PlayerText:draw, playerid, bool:toggle) 
{
    if(toggle) PlayerTextDrawShow(playerid, draw);
    else PlayerTextDrawHide(playerid, draw);
    return 1;
}

stock setTextDrawColor(Text:draw, bool:box, colour) {
	if(box) TextDrawBoxColor(draw, colour);
	else TextDrawColor(draw, colour);
	return 1;
}

stock setPlayerTextDrawColor(PlayerText:draw, playerid, bool:box, colour) {
	if(box) PlayerTextDrawBoxColor(playerid, draw, colour);
	else PlayerTextDrawColor(playerid, draw, colour);
	return 1;
}
stock percent(r) return (random(100) <= r) ? (true) : (false);
stock percent_float(Float:r) return (float(random(100) <= r)) ? (true) : (false);

stock SavePlayerPosition(playerid)
{
	GetPlayerHealth(playerid, playerData[playerid][player_saveHealthArmour][0]);
	GetPlayerArmour(playerid, playerData[playerid][player_saveHealthArmour][1]);

	if(playerData[playerid][player_saveHealthArmour][0] > 100) playerData[playerid][player_saveHealthArmour][0] = 100.0;
	if(playerData[playerid][player_saveHealthArmour][1] > 100) playerData[playerid][player_saveHealthArmour][1] = 100.0;

	getPosition(playerid, (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) ? (true) : (false), unpackXYZR(playerData[playerid][player_savePosition]));

	playerData[playerid][player_saveSkin] = GetPlayerSkin(playerid);
	playerData[playerid][player_saveColor] = GetPlayerColor(playerid);
	playerData[playerid][player_saveWorld] = GetPlayerVirtualWorld(playerid);
    playerData[playerid][player_saveInterior] = GetPlayerInterior(playerid);
	return SaveWeaponData(playerid), 1;
}

stock SaveWeaponData(playerid) {
	for(new a; a != 13; a++) GetPlayerWeaponData(playerid, a, playerData[playerid][player_saveWeaponData][a], playerData[playerid][player_saveAmmoData][a]);
	return 1;
}

stock LoadWeaponData(playerid) {
	for(new i; i != 12; i++) if(!(i == 38 || i == 16 || i == 35 || i == 36 || i == 37)) giveWeapon(playerid, playerData[playerid][player_saveWeaponData][i], playerData[playerid][player_saveAmmoData][i]);
	return 1;
}

stock SQLInjection(const str[])
{
	if(strfind(str, "INSERT", false) != -1) return 1;
 	if(strfind(str, "UPDATE", false) != -1) return 1;
  	if(strfind(str, "DELETE", false) != -1) return 1;
   	if(strfind(str, "FROM", false) != -1) return 1;
   	if(strfind(str, "TRUNCATE", false) != -1) return 1;
    if(strfind(str, "DROP", false) != -1) return 1;
    if(strfind(str, "ALTER", false) != -1) return 1;
    if(strfind(str, "TABLE", false) != -1) return 1;
    if(strfind(str, "WHERE", false) != -1) return 1;
    if(strfind(str, "SELECT", false) != -1) return 1;
    if(strfind(str, "NULL", false) != -1) return 1;
    if(strfind(str, "EXISTS", false) != -1) return 1;
    if(strfind(str, "EMPTY", false) != -1) return 1;
    if(strfind(str, "TRUNCATE", false) != -1) return 1;
	return 0;
}

stock LoadPlayerPosition(playerid)
{
	if(IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
	if(playerData[playerid][player_saveWorld] == 0 && playerData[playerid][player_saveInterior] == 0) {
	    SetPlayerTeam(playerid, NO_TEAM);
	    SetPlayerSkin(playerid, playerData[playerid][player_saveSkin]);
	    SetPlayerColor(playerid, playerData[playerid][player_saveColor]);

		Teleport(playerid, false, unpackXYZR(playerData[playerid][player_savePosition]), playerData[playerid][player_saveInterior], playerData[playerid][player_saveWorld], true);

		SetPlayerHealth(playerid, playerData[playerid][player_saveHealthArmour][0]);
		SetPlayerArmour(playerid, playerData[playerid][player_saveHealthArmour][1]);

		ResetPlayerWeapons(playerid);
		LoadWeaponData(playerid);

	    SetCameraBehindPlayer(playerid);
		TogglePlayerControllable(playerid, true);
		SetPlayerDrunkLevel(playerid, 0);
	}
	else SpawnPlayer(playerid);
	return SetTimerEx("RefreshSpec", 100, false, "d", playerid), 1;
}

stock toUpperCase(string[])
{
	new str[2], concat[256];
	for(new i = 0; i < strlen(string); i++) {
		if(string[i] >= 97 && string[i] <= 122) string[i] -= 32;
		format(str, sizeof(str), "%c", string[i]);
		strcat(concat, str);
	}
	return concat;
}

stock toLowerCase(string[])
{
	new str[2], concat[256];
	for(new i = 0; i < strlen(string); i++) {
		if(string[i] >= 65 && string[i] <= 90) string[i] += 32;
		format(str, sizeof(str), "%c", string[i]);
		strcat(concat, str);
	}
	return concat;
}

stock CheckPlayerGame(playerid)
{
	if(
		pGame[playerid] != -1 && Iter_Contains(Game[pGame[playerid]], playerid) || pArena[playerid] != -1 || 
		pGlitchArena[playerid] || pDuelEnemy[playerid] != -1 && !pDuel[playerid] || Iter_Contains(Sparing, playerid) ||
		Iter_Contains(Event, playerid)
	) return true;
	return false;
}

stock NieMozeTeraz(playerid)
{
	if(
		pDuel[playerid] && pDuelEnemy[playerid] != -1 || Iter_Contains(Sparing, playerid) && pGang[playerid] != -1 && pGangSpar[pGang[playerid]] ||
		pJob[playerid] != -1 || playerData[playerid][player_vault] || playerData[playerid][player_jailtime] > 0 || Iter_Contains(Event, playerid) && EventStarted ||
		pArena[playerid] != -1 || pGlitchArena[playerid] || pGame[playerid] != -1 && Iter_Contains(Game[pGame[playerid]], playerid) && GameStarted[pGame[playerid]] ||
		playerData[playerid][player_business] != -1 || pFishing[playerid] || MineStarted[playerid]
	) return true;
	return false;
}

stock DeniedVehicle(model) {
	if(model == 447 || model == 592 || model == 577 || model == 432 || model == 425 || model == 520 || model == 476 || model == 464 || model == 564 || model == 590) return 1;
    return 0;
}

stock AddCasinoTokens(playerid, tokens)
{
	if(tokens > 0) pCasinoTokens[playerid] += tokens;
	else pCasinoTokens[playerid] -= tokens;
	return 1;
}
stock GetCasinoTokens(playerid) return pCasinoTokens[playerid];

stock mix_string(string[], Return[], max_swap = 0, dest_size = sizeof Return)
{
	new tmp_str[200], swap, size = strlen(string);
	strcat(tmp_str, string);

	for(new i; tmp_str[i]; i ++) {
		if(i > size || (max_swap > 0 && max_swap == swap)) break;
		
		new tmp = tmp_str[i], swit = random(size - 1);
		tmp_str[i] = tmp_str[swit];
		tmp_str[swit] = tmp;
		
		++ swap;
	}
	return format(Return, dest_size, tmp_str);
}

stock getLocalDate()
{
	new date[128], hour, minute, day, month, year;
	getdate(year, month, day), gettime(hour, minute);
	format(date, sizeof(date), "%02d.%02d.%d, %02d:%02d", day, month, year, hour, minute);
	return date;
}

stock getDay(day, month, year) {
	if(month < 3) {
		month += 12;
		year --;
	}
	return (((13 * month + 3) / 5 + day + year + year / 4 - year / 100 + year / 400) % 7) + 1;
}

stock getDayName(day) {
	new name[20];
	switch(day) {
		case 1: name = "Poniedzialek";
		case 2: name = "Wtorek";
		case 3: name = "Sroda";
		case 4: name = "Czwartek";
		case 5: name = "Piatek";
		case 6: name = "Sobota";
		case 7: name = "Niedziela";
	}
	return name;
}

stock timediff(seconds, &dd, &hh, &mm, &ss)
{
  	if(seconds < 0) {
		dd = hh = mm = ss = 0;
		return; 
	}
  	dd = seconds;
  	ss = dd % 60;
	dd -= ss;
  	mm = dd % 3600;
 	dd -= mm;
  	hh = dd % 86400;
  	dd -= hh;
	mm /= 60;
	hh /= 3600;
	dd /= 86400;
}

stock saveAll()
{
	foreach(new gangid : Gangs) SaveGang(gangid);
	foreach(new playerid : Player) {
		SavePlayerStats(playerid);
		
		
	}
	return 1;
}

new pGameDrawTimer[MAX_PLAYERS] = -1;
stock showGameDraw(playerid, const string[], time = 5)
{
	KillTimer(pGameDrawTimer[playerid]);

	PlayerTextDrawSetString(playerid, tdGameDraw, form("%s", string));
	PlayerTextDrawShow(playerid, tdGameDraw);
	
	pGameDrawTimer[playerid] = SetTimerEx("hideGameDraw", 1000 * time, false, "d", playerid);
	return 1;
}

function hideGameDraw(playerid) {
	KillTimer(pGameDrawTimer[playerid]);
	return PlayerTextDrawHide(playerid, tdGameDraw), 1;
}

stock EnableSpawnKill(playerid)
{
    SetPlayerHealth(playerid, 1000.0);
    SetPlayerArmour(playerid, 1000.0);

    playerData[playerid][player_antySK] = 5;
    playerData[playerid][player_god] = true;
	return showGameDraw(playerid, "~n~~n~~n~~n~~n~~n~~n~~n~~y~ANTY ~y~~h~SPAWN-KILL:~n~~g~~h~~h~AKTYWNY!", 3), 1;
}

stock UpdatePlayerLabel(playerid)
{
	new str[350];
	if(!GetPlayerVirtualWorld(playerid)) {
		strcat(str, "\n\n\n\n\n");
		if(pAFK[playerid]) format(str, sizeof(str), "%s\n\n\n\n\n"GOLD"AFK", str);

		if(playerData[playerid][player_admin] < MODERATOR) {
			if(pOwnRankHave[playerid]) format(str, sizeof(str), "\n{%06x}%s", pOwnRankColor[playerid] >>> 8, pOwnRank[playerid]);
			else {
				if(IsPremium(playerid)) format(str, sizeof(str), "\n"YELLOW"Premium");
				else format(str, sizeof(str), "\n{%06x}%s", getRankColor(playerData[playerid][player_admin]) >>> 8, getRankName(playerData[playerid][player_admin]));
			}
		} 
		else format(str, sizeof(str), "\n{%06x}%s", getRankColor(playerData[playerid][player_admin]) >>> 8, getRankName(playerData[playerid][player_admin]));

		if(pGang[playerid] != -1) format(str, sizeof(str), "%s\n{%06x}%s [%s]", str, getGangColor(pGang[playerid]) >>> 8, getGangName(pGang[playerid]), getGangTag(pGang[playerid]));
		format(str, sizeof(str), "%s\n"GREEN"FPS: "WHITE"%d", str, playerData[playerid][player_fps]);
		UpdateDynamic3DTextLabelText(playerData[playerid][player_label], -1, str);
	}
	else UpdateDynamic3DTextLabelText(playerData[playerid][player_label], -1, " ");
	return 1;
}

stock IsPlayerInArea(playerid, Float:minx, Float:miny, Float:maxx, Float:maxy)
{
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	return (x > minx && x < maxx && y > miny && y < maxy) ? (true) : (false);
}

stock SwitchVehicleEngine(vehicleid, bool:engine_status)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, _:engine_status, lights, alarm, doors, bonnet, boot,objective);
}

stock SwitchVehicleDoors(vehicleid, bool:door_status)
{
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehicleid, engine, lights, alarm, _:door_status, bonnet, boot,objective);
}

stock IsPosInWater(Float:x, Float:y, Float:z, Float:dist = 1.0, Float:height = 3.0)
{
	new Float:tmp;
	for(new i; i < 6; i++)
		if(CA_RayCastLine(x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z + height, x + (dist * floatsin(i * 60.0, degrees)), y + (dist * floatcos(i * 60.0, degrees)), z - height, tmp, tmp, tmp) == WATER_OBJECT)
			return 1;
	return 0;
}

stock GetVehiclePosition(vehicleid, &Float:x, &Float:y, &Float:z, &Float:a)
{
	GetVehiclePos(vehicleid, x, y, z);
	GetVehicleZAngle(vehicleid, a);
}

stock SetVehiclePosition(vehicleid, Float:x, Float:y, Float:z, Float:a)
{
	SetVehiclePos(vehicleid, x, y, z);
	SetVehicleZAngle(vehicleid, a);
}

stock HideNameTagForAll(playerid)
{
	foreach(new i : Player) ShowPlayerNameTagForPlayer(playerid, i, false);
	return 1;
}

stock ShowNameTagForAll(playerid)
{
	foreach(new i : Player) ShowPlayerNameTagForPlayer(playerid, i, true);
	return 1;
}

stock CheckPlayerBattle(playerid)
{
	if(gettime() < playerData[playerid][player_battleTimer]) return msg(playerid, C_CORAL, "Nie mo¿esz u¿ywaæ komend przez {c}%s{/c} z powodu {c}walki{/c}!", ConvertTime(playerData[playerid][player_battleTimer], TIME_TYPE_UNIX));
	return 0;
}

stock CompareEx(comp[], with[], up = 0)
{
	new comp_len = strlen(comp), with_len = strlen(with), character[2];
	
	if(comp_len != with_len) return false;
	for(new i = 0; i < comp_len; i++) { if(comp[i] == with[i]) character[0] ++; }
	if(comp_len == character[0]) return true;
	else {
		if(up > 0) {
			toLowerCase(comp), toLowerCase(with);
			for(new i = 0; i < comp_len; i++) { if(comp[i] == with[i]) character[1] ++; }
			if(comp_len == character[1]) return true;
		}
		else return false;
	}
	return false;
}

stock getPodiumText(variable, const one[], const two[], const three[])
{
	new name[32];
	switch(variable) {
		case 1: strcopy(name, one, 32);
		case 2: strcopy(name, two, 32);
		case 3: strcopy(name, three, 32);
	}
	return name;
}

stock GetWeaponModelById(modelid) 
{
	switch(modelid) {
		case 1: return 331;
		case 2: return 333;
		case 3: return 334;
		case 4: return 335;
		case 5: return 336;
		case 6: return 337;
		case 7: return 338;
		case 8: return 339;
		case 9: return 341;
		case 10: return 321;
		case 11: return 322;
		case 12: return 323;
		case 13: return 324;
		case 14: return 325;
		case 15: return 326;
		case 16: return 342;
		case 17: return 343;
		case 18: return 344;
		case 22: return 346;
		case 23: return 347;
		case 24: return 348;
		case 25: return 349;
		case 26: return 350;
		case 27: return 351;
		case 28: return 352;
		case 29: return 353;
		case 30: return 355;
		case 31: return 356;
		case 32: return 372;
		case 33: return 357;
		case 34: return 358;
		case 35: return 359;
		case 36: return 360;
		case 37: return 361;
		case 38: return 362;
		case 39: return 363;
		case 40: return 364;
		case 41: return 365;
		case 42: return 366;
		case 43: return 367;
		case 44: return 368;
		case 45: return 369;
		case 46: return 371;
		case 1337: return 1242;
		case 2137: return 1240;
	}
	return 0;
}

stock ShowRegulamin(playerid)
{
	s[0] = EOS;
	strcat(s, ""VIOLET"1. Informacje ogólne:\n");
	strcat(s, ""LY"§. "WHITE"Rejestracja na serwerze jest obowi¹zkowa, tak samo jak znajomoœæ regulaminu.\n");
	strcat(s, ""LY"§. "WHITE"Gracz, który otrzyma³ bana, mo¿e siê odwo³aæ od niego na serwerze Discord "BLUE"("DISCORD")"WHITE".\n");
	strcat(s, ""LY"§. "WHITE"W wyj¹tkowych sytuacjach, gracz mo¿e zostaæ ukarany za aspekty nie zawarte w regulaminie.\n");
	strcat(s, ""LY"§. "WHITE"Administracja nie bêdzie tolerowa³a postów, które Ÿle wp³ywaj¹ na wygl¹d serwera przez inne osoby.\n\n");

	strcat(s, ""VIOLET"2. Zabrania siê:\n");
	strcat(s, ""LY"§. "WHITE"Posiadania wszelkiego oprogramowania ingeruj¹cego w grê i daj¹cego przewagê nad innymi.\n");
	strcat(s, ""LY"§. "WHITE"Obra¿ania innych graczy oraz administracji serwera, celowego wywo³ywania k³ótni, floodowania, itp.\n");
	strcat(s, ""LY"§. "WHITE"Wykorzystywania bugów serwera dla korzyœci w³asnej. Napotkany bug nale¿y odrazu zg³osiæ administracji.\n");
	strcat(s, ""LY"§. "WHITE"Unikania walki w jakikolwiek sposób, m.in. poprzez teleportacjê, wyjœcie z serwera.\n");
	strcat(s, ""LY"§. "WHITE"Uleczania w trakcie walki, co te¿ siê odwo³uje do ucieczki, bo gracz musia³by siê oddaliæ.\n");
	strcat(s, ""LY"§. "WHITE"Przeszkadzania innym graczom oraz administracji na ró¿norakie sposoby.\n");
	strcat(s, ""LY"§. "WHITE"Zabijania graczy z wykorzystaniem pojazdów, np. poprzez celowe potr¹cenie.\n");
	strcat(s, ""LY"§. "WHITE"Masowego spawnowania pojazdów, w szczególnoœci w miejscach ze sporym zagêszczeniem graczy.\n");
	strcat(s, ""LY"§. "WHITE"Udostêpniania, sprzeda¿y innym graczom swojego konta, czy te¿ has³a do niego.\n");
	strcat(s, ""LY"§. "WHITE"Reklamowania innych serwisów, itp. - tyczy siê to równie¿ anty-reklamy.\n");
	strcat(s, ""LY"§. "WHITE"Wymuszania wszelakich korzyœci materialnych (respekt, gotówka, ranga) od administracji/graczy.\n");
	strcat(s, ""LY"§. "WHITE"Wykorzystywania programów do maskowania rzeczywistego adresu IP (VPN).\n");
	strcat(s, ""LY"§. "WHITE"Sk³adania bezsensownych raportów, b¹dŸ takich, które s¹ trudne do odczytu przez administracjê.");
	return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""RED"Regulamin", s, "OK", #), 1; 
}

stock GetRandomCoord(&Float:x, &Float:y, &Float:z)
{
	new Float:rand_x, Float:rand_y;
	switch(random(4)) {
		case 0: {
			rand_x = random(2000);
			rand_y = random(2000); 
		}
		case 1: {
			rand_x = 0 - random(2000);
			rand_y = 0 - random(2000); 
		}
		case 2: {
			rand_x = random(2000);
			rand_y = 0 - random(2000); 
		}
		case 3: {
			rand_x = 0 - random(2000);
			rand_y = random(2000); 
		}
    }

	x = rand_x;
	y = rand_y;
	
	CA_FindZ_For2DCoord(x, y, z);
	z += 0.35;
	return true;
}

stock SetHealth(playerid, Float:health, Float:armour) {
	SetPlayerHealth(playerid, health);
	return SetPlayerArmour(playerid, armour), 1;
}
stock Przegral(playerid) return floatround(((playerData[playerid][player_online] + playerData[playerid][player_session]) / 3600));
stock addVehicleComponent(vehicleid, ...) {
	for(new slotid, componentid, amount = numargs(); slotid != amount; slotid++) { if((componentid = getarg(slotid, 0)) >= 1000) AddVehicleComponent(vehicleid, componentid); }
	return 1;
}

stock TuneVehicle(vehicleid)
{
	switch(GetVehicleModel(vehicleid))
	{
		case 400: addVehicleComponent(vehicleid, 1010, 1013, 1018, 1019, 1020, 1021, 1024, 1086, 1087);
		case 401: addVehicleComponent(vehicleid, 1001, 1003, 1004, 1005, 1006, 1007, 1010, 1013, 1017, 1019, 1020, 1086, 1087, 1142, 1143, 1144, 1145);
		case 404: addVehicleComponent(vehicleid, 1000, 1002, 1007, 1010, 1013, 1016, 1017, 1019, 1020, 1021, 1086, 1087);
		case 405: addVehicleComponent(vehicleid, 1000, 1001, 1010, 1014, 1018, 1019, 1020, 1021, 1023, 1086, 1087);
		case 410: addVehicleComponent(vehicleid, 1001, 1003, 1007, 1010, 1013, 1017, 1019, 1020, 1021, 1023, 1024, 1086, 1087);
		case 415: addVehicleComponent(vehicleid, 1001, 1003, 1007, 1010, 1017, 1018, 1019, 1023, 1086, 1087);
		case 418: addVehicleComponent(vehicleid, 1002, 1006, 1010, 1016, 1020, 1021, 1086, 1087);
		case 420: addVehicleComponent(vehicleid, 1001, 1003, 1004, 1005, 1010, 1019, 1021, 1086, 1087);
		case 421: addVehicleComponent(vehicleid, 1000, 1010, 1014, 1016, 1018, 1019, 1020, 1021, 1023, 1086, 1087);
		case 422: addVehicleComponent(vehicleid, 1007, 1010, 1013, 1017, 1019, 1020, 1021, 1086, 1087);
		case 426: addVehicleComponent(vehicleid, 1001, 1003, 1004, 1005, 1006, 1010, 1019, 1021, 1086, 1087);
		case 436: addVehicleComponent(vehicleid, 1001, 1003, 1006, 1007, 1010, 1013, 1017, 1019, 1020, 1021, 1022, 1086, 1087);
		case 439: addVehicleComponent(vehicleid, 1001, 1003, 1007, 1010, 1013, 1017, 1023, 1086, 1087, 1142, 1143, 1144, 1145);
		case 477: addVehicleComponent(vehicleid, 1006, 1007, 1010, 1017, 1018, 1019, 1020, 1021, 1086, 1087);
		case 478: addVehicleComponent(vehicleid, 1004, 1005, 1010, 1012, 1013, 1020, 1021, 1022, 1024, 1086, 1087);
		case 489: addVehicleComponent(vehicleid, 1000, 1002, 1004, 1005, 1006, 1010, 1013, 1016, 1018, 1019, 1020, 1024, 1086, 1087);
		case 491: addVehicleComponent(vehicleid, 1003, 1007, 1010, 1014, 1017, 1018, 1019, 1020, 1021, 1023, 1086, 1087, 1142, 1143, 1144, 1145);
		case 492: addVehicleComponent(vehicleid, 1000, 1004, 1005, 1006, 1010, 1016, 1086, 1087);
		case 496: addVehicleComponent(vehicleid, 1001, 1002, 1003, 1006, 1007, 1010, 1011, 1017, 1019, 1020, 1023, 1086, 1087);
		case 500: addVehicleComponent(vehicleid, 1010, 1013, 1019, 1020, 1021, 1024, 1086, 1087);
		case 505: addVehicleComponent(vehicleid, 1000, 1002, 1004, 1005, 1006, 1010, 1013, 1016, 1018, 1019, 1020, 1024, 1086, 1087);
		case 516: addVehicleComponent(vehicleid, 1000, 1002, 1004, 1007, 1010, 1015, 1016, 1017, 1018, 1019, 1020, 1021, 1086, 1087);
		case 517: addVehicleComponent(vehicleid, 1002, 1003, 1007, 1010, 1016, 1017, 1018, 1019, 1020, 1023, 1086, 1087, 1142, 1143, 1144, 1145);
		case 518: addVehicleComponent(vehicleid, 1001, 1003, 1005, 1006, 1007, 1010, 1013, 1017, 1018, 1020, 1023, 1086, 1087, 1142, 1143, 1144, 1145);
		case 527: addVehicleComponent(vehicleid, 1001, 1007, 1010, 1014, 1015, 1017, 1018, 1020, 1021, 1086, 1087);
		case 529: addVehicleComponent(vehicleid, 1001, 1003, 1006, 1007, 1010, 1011, 1012, 1017, 1018, 1019, 1020, 1023, 1086, 1087);
		case 534: addVehicleComponent(vehicleid, 1010, 1086, 1087, 1100, 1101, 1106, 1122, 1123, 1124, 1125, 1126, 1127, 1178, 1179, 1180, 1185);
		case 535: addVehicleComponent(vehicleid, 1010, 1086, 1087, 1109, 1110, 1111, 1112, 1113, 1114, 1115, 1116, 1117, 1118, 1119, 1120, 1121);
		case 536: addVehicleComponent(vehicleid, 1010, 1086, 1087, 1103, 1104, 1105, 1107, 1108, 1128, 1181, 1182, 1183, 1184);
		case 540: addVehicleComponent(vehicleid, 1001, 1004, 1006, 1007, 1010, 1017, 1018, 1019, 1020, 1023, 1024, 1086, 1087, 1142, 1143, 1144, 1145);
		case 542: addVehicleComponent(vehicleid, 1010, 1014, 1015, 1018, 1019, 1020, 1021, 1086, 1087, 1142, 1143, 1144, 1145);
		case 546: addVehicleComponent(vehicleid, 1001, 1002, 1004, 1006, 1007, 1010, 1017, 1018, 1019, 1023, 1024, 1086, 1087, 1142, 1143, 1144, 1145);
		case 547: addVehicleComponent(vehicleid, 1000, 1003, 1010, 1016, 1018, 1019, 1020, 1021, 1086, 1087);
		case 549: addVehicleComponent(vehicleid, 1001, 1003, 1007, 1010, 1011, 1012, 1017, 1018, 1019, 1020, 1023, 1086, 1087, 1142, 1143, 1144, 1145);
		case 550: addVehicleComponent(vehicleid, 1001, 1003, 1004, 1005, 1006, 1010, 1018, 1019, 1020, 1023, 1086, 1087, 1142, 1143, 1144, 1145);
		case 551: addVehicleComponent(vehicleid, 1002, 1003, 1005, 1006, 1010, 1016, 1018, 1019, 1020, 1021, 1023, 1086, 1087);
		case 558: addVehicleComponent(vehicleid, 1010, 1086, 1087, 1088, 1089, 1090, 1091, 1092, 1093, 1094, 1095, 1163, 1164, 1165, 1166, 1167, 1168);
		case 559: addVehicleComponent(vehicleid, 1010, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1086, 1087, 1158, 1159, 1160, 1161, 1162, 1173);
		case 560: addVehicleComponent(vehicleid, 1010, 1026, 1027, 1028, 1029, 1030, 1031, 1032, 1033, 1086, 1087, 1138, 1139, 1140, 1141, 1169, 1170);
		case 561: addVehicleComponent(vehicleid, 1010, 1055, 1056, 1057, 1058, 1059, 1060, 1061, 1062, 1063, 1064, 1086, 1087, 1154, 1155, 1156, 1157);
		case 562: addVehicleComponent(vehicleid, 1010, 1034, 1035, 1036, 1037, 1038, 1039, 1040, 1041, 1086, 1087, 1146, 1147, 1148, 1149, 1171, 1172);
		case 565: addVehicleComponent(vehicleid, 1010, 1045, 1046, 1047, 1048, 1049, 1050, 1051, 1052, 1053, 1054, 1086, 1087, 1150, 1151, 1152, 1153);
		case 567: addVehicleComponent(vehicleid, 1010, 1086, 1087, 1102, 1129, 1130, 1131, 1132, 1133, 1186, 1187, 1188, 1189);
		case 575: addVehicleComponent(vehicleid, 1010, 1042, 1043, 1044, 1086, 1087, 1099, 1174, 1175, 1176, 1177);
		case 576: addVehicleComponent(vehicleid, 1010, 1086, 1087, 1134, 1135, 1136, 1137, 1190, 1191, 1192, 1193);
		case 580: addVehicleComponent(vehicleid, 1001, 1006, 1007, 1010, 1017, 1018, 1020, 1023, 1086, 1087);
		case 585: addVehicleComponent(vehicleid, 1001, 1003, 1006, 1007, 1010, 1013, 1017, 1018, 1019, 1020, 1023, 1086, 1087, 1142, 1143, 1144, 1145);
		case 589: addVehicleComponent(vehicleid, 1000, 1004, 1005, 1006, 1007, 1010, 1013, 1016, 1017, 1018, 1020, 1024, 1086, 1087, 1142, 1143, 1144, 1145);
		case 600: addVehicleComponent(vehicleid, 1004, 1005, 1006, 1007, 1010, 1013, 1017, 1018, 1020, 1022, 1086, 1087);
		case 603: addVehicleComponent(vehicleid, 1001, 1006, 1007, 1010, 1017, 1018, 1019, 1020, 1023, 1024, 1086, 1087, 1142, 1143, 1144, 1145);
		case 402, 409, 411, 412, 419, 424, 438, 442, 445, 451, 458, 466, 467, 474, 475, 479, 480, 506, 507, 526, 533, 541, 545, 555, 566, 579, 587, 602: addVehicleComponent(vehicleid, 1010, 1086, 1087);
	}
	return 1;
}

stock MovePointCol(const Float:StartX, const Float:StartY, const Float:StartZ, const Float:EndX, const Float:EndY, const Float:EndZ, &Float:x, &Float:y, &Float:z)
{
	new objectid;
	x = y = z = FLOAT_INFINITY;
	objectid = CA_RayCastLine(StartX, StartY, StartZ, EndX, EndY, EndZ, x, y, z);

	if(x == FLOAT_INFINITY) x = EndX;
	if(y == FLOAT_INFINITY) y = EndY;
	if(z == FLOAT_INFINITY) z = EndZ;
	return objectid;
}

stock GetGroundRotation(const Float:x, const Float:y, const Float:size, &Float:rx, &Float:ry){
	new Float:tpx1,Float:tpy1,Float:tpz1, Float:tpx2,Float:tpy2,Float:tpz2, Float:T3D:tmp;
	GetPointInFront2D(x,y,0.0,size,tpx1,tpy1);
	GetPointInFront2D(x,y,180.0,size,tpx2,tpy2);
	CA_FindZ_For2DCoord(tpx1,tpy1,tpz1);
	CA_FindZ_For2DCoord(tpx2,tpy2,tpz2);
	GetRotationFor2Point3D(tpx1,tpy1,tpz1,tpx2,tpy2,tpz2,rx,T3D:tmp);
	
	GetPointInFront2D(x,y,90.0,size,tpx1,tpy1);
	GetPointInFront2D(x,y,270.0,size,tpx2,tpy2);
	CA_FindZ_For2DCoord(tpx1,tpy1,tpz1);
	CA_FindZ_For2DCoord(tpx2,tpy2,tpz2);
	GetRotationFor2Point3D(tpx1,tpy1,tpz1,tpx2,tpy2,tpz2,ry,T3D:tmp);
}

stock GetPointInFront2D(const Float:x, const Float:y, const Float:rz, const Float:radius, &Float:tx, &Float:ty){
	tx = x - (radius * floatsin(rz,degrees));
	ty = y + (radius * floatcos(rz,degrees));
}

stock GetPointInFront3D(const Float:x, const Float:y, const Float:z, const Float:rx, const Float:rz, const Float:radius, &Float:tx, &Float:ty, &Float:tz){
	tx = x - (radius * floatcos(rx,degrees) * floatsin(rz,degrees));
	ty = y + (radius * floatcos(rx,degrees) * floatcos(rz,degrees));
	tz = z + (radius * floatsin(rx,degrees));
}

stock GetRotationFor2Point3D(const Float:x, const Float:y, const Float:z, const Float:tx, const Float:ty, const Float:tz, &Float:rx, &Float:rz){
	new Float:radius = GetDistance3D(x,y,z,tx,ty,tz);
	if(radius <= 0.0) return 0;
	rx = CompressRotation(-(acos((tz-z)/radius)-90.0));
	rz = CompressRotation(atan2(ty-y,tx-x)-90.0);
	return 1;
}

stock checkTime(time)
{
	new 
		mm = floatround(((time) / 60) % 60), 
		ss = time % 60,
		out[50];

	format(out, sizeof(out), "%02d:%02d", mm, ss);
	return out;
}

stock IsModelCar(modelid)
{
	switch(modelid)
	{
		case 400..416, 418..424, 426..429, 431, 433,
		434, 436..445, 451, 455, 459, 466, 467, 470,
		471, 474, 475, 477..480, 482, 483, 485, 486,
		489..492, 494..496, 498..500, 502..508, 514,
		515..518, 524..536, 540..547, 549..552, 554,
		555..562, 564..568, 571..576, 578..580, 582,
		583, 585, 587..589, 594, 596..605, 609: return 1;
	}
	return 0;
}

stock IsModelBoat(vehicleid)
{
	switch(GetVehicleModel(vehicleid)) {
		case 472, 473, 493, 595, 539, 484, 430, 453, 452, 446, 454: return 1;
	}
	return 0;
}

stock IsModelMotor(modelid)
{
	switch(modelid) {
		case 462, 448, 581, 522, 461, 521, 523, 463, 586, 468, 471: return 1;
	}
	return 0;
}

stock IsModelAir(modelid)
{
	switch(modelid) {
		case 417, 425, 447, 460, 464, 465, 469, 476, 487, 488, 497, 501, 511..513, 519, 520, 548, 553, 563, 577, 592, 593: return 1;
	}
	return 0;
}

stock IsCar(vehicleid) return IsModelCar(GetVehicleModel(vehicleid));
stock IsMotor(vehicleid) return IsModelMotor(GetVehicleModel(vehicleid));
stock IsAir(vehicleid) return IsModelAir(GetVehicleModel(vehicleid));
stock IsBoat(vehicleid) return IsModelBoat(GetVehicleModel(vehicleid));

new MissionTimer[MAX_PLAYERS];

stock ShowMissionDraw(playerid, header[], info[], money = 0)
{
	if(MissionTimer[playerid] != -1) KillTimer(MissionTimer[playerid]);
	MissionTimer[playerid] = SetTimerEx("HideMissionDraw", 1000 * 5, false, "d", playerid);

	if(money > 0) format(info, strlen(info), "%s~n~~w~~h~Nagroda: ~r~~h~%s$", info, strdot(money));
	playerTextDrawSetString(playerid, tdMissionDraw[0], header);
	playerTextDrawSetString(playerid, tdMissionDraw[1], info);

	for(new i = 0; i < 2; i++) PlayerTextDrawShow(playerid, tdMissionDraw[i]);
	return playSound(playerid, 1139), 1;
}
function HideMissionDraw(playerid) {
	for(new i = 0; i < 2; i++) PlayerTextDrawHide(playerid, tdMissionDraw[i]);
	return KillTimer(MissionTimer[playerid]);
}

stock getLost(playerid, &Float:loss) {
	if(!IsPlayerConnected(playerid)) return 0;

	new _str[512], _loss[20], start, end;
	GetPlayerNetworkStats(playerid, _str, sizeof(_str));
	
	start = strfind(_str, "packetloss", true);
	end = strfind(_str, "%", true, start);
	
	strmid(_loss, _str, start + 12, end, sizeof(_loss));
	return packetloss = floatstr(_loss), 1;
}

function PrepareVehicleRespawn() return RespawnVehicles(false);
stock RespawnVehicles(bool:privcars) 
{
	new bool:used[MAX_VEHICLES], c;
	foreach(new playerid : Player) {
		if(playerData[playerid][player_createdVehicle] != -1 && !IsPlayerInVehicle(playerid, playerData[playerid][player_createdVehicle])) {
			if(IsValidVehicle(playerData[playerid][player_createdVehicle])) DestroyVehicle(playerData[playerid][player_createdVehicle]);
			c ++;
		}

		new vehicleid = GetPlayerVehicleID(playerid);
		if(vehicleid > 0) {
			used[vehicleid] = true;
			new modelid = GetVehicleModel(vehicleid);
			if(modelid == 403 || modelid == 514 || modelid == 515 || modelid == 525 || modelid == 531 || modelid == 572 || modelid == 583) {
				vehicleid = GetVehicleTrailer(vehicleid);
				if(vehicleid > 0) used[vehicleid] = true;
			}
		}
	}

	for(new _vehicleid; _vehicleid < MAX_VEHICLES; _vehicleid --) {
		if(!used[_vehicleid] && IsValidVehicle(_vehicleid) && (!VehiclePrivate[_vehicleid] || privcars)) {
			SetVehicleToRespawn(_vehicleid);
			c ++;
		}
	}
	return msgAll(C_ORANGE, "Serwer zrespawnowa³ {c}%d{/c} nieu¿ywanych pojazdów.", c);
}

stock IsObjectInSphere(objectid, Float:x, Float:y, Float:z, Float:radius)
{
    new Float:pos[3], Float:dist;
    GetObjectPos(objectid, unpackXYZ(pos));
    dist = floatsqroot(floatpower(floatabs(floatsub(x, pos[0])), 2) + floatpower(floatabs(floatsub(y, pos[1])), 2) + floatpower(floatabs(floatsub(z, pos[2])), 2));
    return (dist < radius) ? (1) : (0);
}

stock Is2DPointInRangeOfPoint(Float:x, Float:y, Float:_x, Float:_y, Float:range)
{
    _x -= x; _y -= y;
    return ((_x * _x) + (_y * _y)) < (range * range);
}

stock GetAngleToPool(Float:x, Float:y, Float:_x, Float:_y, &Float:angle)
{
    angle = atan2(y - _y, x - _x);
    angle = floatsub(angle, 90.0);
    if(angle < 0.0) angle = floatadd(angle, 360.0);
}

stock Get2DInFrontOfPos(Float:x, Float:y, Float:a, &Float:_x, &Float:_y, Float:dist)
{
    if(a > 360.0) a -= 360.0;

    x += (dist * floatsin(-a, degrees));
    y += (dist * floatcos(-a, degrees));

    _x = x;
    _y = y;
}

stock Get2DBehindObjectInAngle(objectid, Float:a, &Float:x, &Float:y, Float:dist)
{
    new Float:z; GetObjectPos(objectid, x, y, z);
    x += (dist * floatsin(-a + 180.0, degrees));
    y += (dist * floatcos(-a + 180.0, degrees));
}

stock AngleInRangeOfAngle(Float:x, Float:y, Float:range)
{
	x -= y;
	return ((x < range) && (x > -range)) ? (true) : (false);
}
