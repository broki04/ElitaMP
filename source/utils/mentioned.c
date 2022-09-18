#define MAX_ALLOWED_MENTIONED   0

stock getMentioned(playerid, text[], max_len = sizeof(text))
{
    #pragma unused playerid

	new len = strlen(text), pos = 0, mentiones_start, mentiones_stop, p_name[MAX_PLAYER_NAME], o_name[48], n_name[48], used_tags = 0;
	while((mentiones_stop != -1 && pos < len && pos != -1 && (mentiones_start = strfind(text, "@", false, pos)) != -1) && (MAX_ALLOWED_MENTIONED == 0 || (used_tags < MAX_ALLOWED_MENTIONED))) {
        p_name = "";
        mentiones_stop = strfind(text, " ", false, mentiones_start);
        strmid(p_name, text, mentiones_start + 1, (mentiones_stop == -1) ? (len) : (mentiones_stop));

        new targetid = foundPlayer(p_name);
        if(targetid != ERROR_NOT_FOUND && targetid != ERROR_FOUND_MULTIPLE) {
            format(o_name, sizeof(o_name), "@%s", p_name);
            GetPlayerName(targetid, p_name, MAX_PLAYER_NAME);

            format(n_name, sizeof(n_name), "{%06x}%s"WHITE"", GetPlayerColor(targetid) >>> 8, p_name);
            strreplace(text, o_name, n_name, true, pos, 1, max_len);
            playSound(targetid, 1057);

            pos += strlen(n_name);
            len = strlen(text);
            used_tags ++;
        }
        pos ++;
    }
}
