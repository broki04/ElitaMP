#include YSI_Coding\y_hooks

new bool:pAnimation[MAX_PLAYERS] = false;
hook function ResetPlayerData(playerid) {
    pAnimation[playerid] = false;
    return continue(playerid); 
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if((newkeys & KEY_SPRINT) && !(oldkeys & KEY_SPRINT) && pAnimation[playerid]) return StopLoopingAnim(playerid);
    return 1;
}

stock LoopingAnim(playerid, animlib[], animname[], Float:speed, bool:looping, bool:lockx, bool:locky, bool:lockz, lp, fs = 0)
{
    pAnimation[playerid] = true;
	msg(playerid, C_GREY, "Aby przerwaæ animacjê, wciœnij "BLUE2"SPACJÊ "GREY"lub wpisz: "BLUE2"/astop.");
    return ApplyAnimation(playerid, animlib, animname, speed, looping, lockx, locky, lockz, lp, fs);
}

stock StopLoopingAnim(playerid)
{
    pAnimation[playerid] = false;
    ClearAnimations(playerid, 1);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    return ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, false, false, false, false, 0);
}

alias:anims("anim", "animacje")
CMD:anims(playerid)
{
    s[0] = EOS;
	strcat(s, ""BLUE2"Lista dostêpnych animacji:\n");
	strcat(s, ""WHITE"/rzygaj /jedz /machaj /klepnij /deal /cpun /plantuj /aresztuj /smiech /ramiona\n");
	strcat(s, ""WHITE"/wal /wytrysk /schowaj /wymiotuj /pa /daj /usiadz /robman /lez /oh\n");
	strcat(s, ""WHITE"/spij /caluj /ranny /taxi /rozgladaj /yes /no /myjrece /medyk\n");
	strcat(s, ""WHITE"/dowody /wniebo /start /naprawia /naprawil /wtf /patrz /wyjmijbutelke\n");
	strcat(s, ""WHITE"/chatauto /chatauto2 /bagaznik /krzeslouderz /krzeslowypij /krzeslokomp\n");
	strcat(s, ""WHITE"/krzeslozegarek /krzeslorozm2 /wypij /barman /barman2 /opieraj\n");
	strcat(s, ""WHITE"/nalej /polchodz /polodejdz /polstop /ratunku /carchat /zolwik\n");
	strcat(s, ""WHITE"/krzeslo /sikaj /drunk /lookout /napad /hide\n");
	strcat(s, ""WHITE"/siadaj /fucku /taichi /pij /inbedleft /inbedright /fotel /krzycz\n");
	strcat(s, ""WHITE"/adhd /raczkuj /down /scratch /beach /circle /fallback /bacznosc\n");
	strcat(s, ""WHITE"/rofl /getin /sprayuj /headbutt /lightcig /tapcig /bat /chant /fall\n");
	strcat(s, ""WHITE"/finger /shouting /cop /lowbodypush /push /rollfall /boxanim\n");
    strcat(s, ""WHITE"/smoke [1-4] /aim [1-3] /run [1-9] /lay [1-9] /wave [1-5] /walk [1-9] /rece [1-3]\n");
	strcat(s, ""WHITE"/look [1-4] /shout [1-3] /bj [1-3] /swat2 [1-6] /lean [1-2] /stance [1-9] /call [1-2]\n");
	strcat(s, ""WHITE"/jem [1-4] /rap [1-6] /trup [1-2] /pijak [1-3] /basket [1-6] /gryps [1-10]\n");
	strcat(s, ""WHITE"/hike [1-3] /face [1-6] /tancz [1-4] /chat [1-11] /siad [1-5] /yo [1-9]\n\n");

	strcat(s, ""LBLUE"Aby przerwaæ animacjê, wpisz "WHITE"/astop "LBLUE"lub kliknij "WHITE"SPACJÊ");
    return showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""VIOLET"Animacje", s, "OK", #);
}

alias:astop("animstop")
CMD:astop(playerid, params[]) return StopLoopingAnim(playerid);

CMD:plantuj(playerid, params[]) return LoopingAnim(playerid, "BOMBER", "BOM_Plant", 4.0, false, false, false, false, 0);
CMD:aresztuj(playerid, params[]) return LoopingAnim(playerid, "ped", "ARRESTgun", 4.0, false, true, true, true, -1);
CMD:smiech(playerid, params[]) return LoopingAnim(playerid, "RAPPING", "Laugh_01", 4.0, false, false, false, false, 0);
CMD:ramiona(playerid, params[]) return LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, true, false, false, false, -1);
CMD:lez(playerid, params[]) return LoopingAnim(playerid, "BEACH", "bather", 4.0, true, false, false, false, 0);
alias:wal("wank")
CMD:wal(playerid, params[]) return LoopingAnim(playerid, "PAULNMAC", "wank_loop", 4.0, true, false, false, false, 0);
CMD:wytrysk(playerid, params[]) return LoopingAnim(playerid, "PAULNMAC", "wank_out", 4.0, false, false, false, false, 0);
CMD:schowaj(playerid, params[]) return LoopingAnim(playerid, "ped", "cower", 3.0, true, false, false, false, 0);
CMD:wymiotuj(playerid, params[]) return LoopingAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, false, false, false, false, 0);
CMD:papa(playerid, params[]) return LoopingAnim(playerid, "PED", "endchat_03", 3.0, false, false, false, false, 0);
CMD:daj(playerid, params[]) return LoopingAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, false, false, false, false, 0);
CMD:usiadz(playerid, params[]) return LoopingAnim(playerid, "BEACH", "ParkSit_M_loop", 4.0, true, false, false, false, 0);
CMD:spij(playerid, params[]) return LoopingAnim(playerid, "INT_HOUSE", "BED_Loop_R", 4.0, true, false, false, false, 0);
CMD:ranny(playerid, params[]) return LoopingAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0, true, false, false, true, 0);
CMD:taxi(playerid, params[]) return LoopingAnim(playerid, "ped", "IDLE_taxi", 4.0, false, false, false, false, 0);
CMD:rozgladaj(playerid, params[]) return LoopingAnim(playerid, "ped", "roadcross", 4.0, false, false, false, false, 0);
CMD:yes(playerid, params[]) return LoopingAnim(playerid, "GANGS", "Invite_Yes", 4.0, false, false, false, false, 0);
CMD:no(playerid, params[]) return LoopingAnim(playerid, "GANGS", "Invite_No", 4.0, false, false, false, false, 0);
CMD:myjrece(playerid, params[]) return LoopingAnim(playerid, "INT_HOUSE", "wash_up", 4.0, false, false, false, false, 0);
CMD:medyk(playerid, params[]) return LoopingAnim(playerid, "MEDIC", "CPR", 4.0, false, false, false, false, 0);
CMD:oh(playerid, params[]) return LoopingAnim(playerid, "MISC", "plyr_shkhead", 4.0, false, false, false, false, -1);
CMD:dowody(playerid, params[]) return LoopingAnim(playerid, "MISC", "Run_Dive", 4.0, false, true, false, true, 0);
CMD:wniebo(playerid, params[]) return LoopingAnim(playerid, "ON_LOOKERS", "lkup_loop", 4.0, true, true, true, true, -1);
CMD:start(playerid, params[]) return LoopingAnim(playerid, "CAR", "flag_drop", 4.0, false, false, false, false, 0);
CMD:naprawia(playerid, params[]) return LoopingAnim(playerid, "CAR", "Fixn_Car_Loop", 4.0, true, false, false, false, 0);
CMD:naprawil(playerid, params[]) return LoopingAnim(playerid, "CAR", "Fixn_Car_Out", 4.0, false, false, false, false, 0);
CMD:chatauto(playerid, params[]) return LoopingAnim(playerid, "CAR_CHAT", "car_talkm_loop", 4.0, true, false, false, true, 0);
CMD:chatauto2(playerid, params[]) return LoopingAnim(playerid, "CAR_CHAT", "car_talkm_out", 4.0, false, false, false, false, 0);
CMD:bagaznik(playerid, params[]) return LoopingAnim(playerid, "POOL", "POOL_Place_White", 4.0, false, false, false, false, 0);
CMD:krzeslouderz(playerid, params[]) return LoopingAnim(playerid, "INT_OFFICE", "OFF_Sit_Crash", 4.0, false, false, false, true, 0);
CMD:krzeslowypij(playerid, params[]) return LoopingAnim(playerid, "INT_OFFICE", "OFF_Sit_Drink", 4.0, false, false, false, true, 0);
CMD:krzeslokomp(playerid, params[]) return LoopingAnim(playerid, "INT_OFFICE", "OFF_Sit_Type_Loop", 4.0, true, false, false, true, 0);
CMD:krzeslozegarek(playerid, params[]) return LoopingAnim(playerid, "INT_OFFICE", "OFF_Sit_Watch", 4.0, false, false, false, true, 0);
CMD:krzeslorozm1(playerid, params[]) return LoopingAnim(playerid, "MISC", "Seat_Talk_01", 4.0, true, false, false, true, 0);
CMD:krzeslorozm2(playerid, params[]) return LoopingAnim(playerid, "MISC", "Seat_Talk_02", 4.0, true, false, false, true, 0);
CMD:wypij(playerid, params[]) return LoopingAnim(playerid, "BAR", "dnk_stndM_loop", 4.0, false, false, false, false, 0);
CMD:barman(playerid, params[]) return LoopingAnim(playerid, "BAR", "BARman_idle", 4.0, false, false, false, false, 0);
CMD:barman2(playerid, params[]) return LoopingAnim(playerid, "BAR", "Barserve_loop", 4.0, true, false, false, false, 0);
CMD:wyjmijbutelke(playerid, params[]) return LoopingAnim(playerid, "BAR", "Barserve_bottle", 4.0, false, false, false, false, 0);
CMD:nalej(playerid, params[]) return LoopingAnim(playerid, "BAR", "Barserve_glass", 4.0, false, false, false, false, 0);
CMD:polchodz(playerid, params[]) return LoopingAnim(playerid, "POLICE", "CopTraf_Come", 4.0, false, false, false, false, 0);
CMD:polodejdz(playerid, params[]) return LoopingAnim(playerid, "POLICE", "CopTraf_Left", 4.0, false, false, false, false, 0);
CMD:polstop(playerid, params[]) return LoopingAnim(playerid, "POLICE", "CopTraf_Stop", 4.0, false, false, false, false, 0);
CMD:ratunku(playerid, params[]) return LoopingAnim(playerid, "BD_FIRE", "BD_Panic_03", 4.0, true, false, false, false, 0);
CMD:krzeslo(playerid, params[]) return LoopingAnim(playerid, "INT_OFFICE", "OFF_Sit_Idle_Loop", 4.0, true, false, false, false, 0);
alias:calus("caluj")
CMD:calus(playerid, params[]) return LoopingAnim(playerid, "KISSING", "Playa_Kiss_02", 4.0, false, false, false, false, 0);
CMD:sikaj(playerid, params[]) return LoopingAnim(playerid, "PAULNMAC", "Piss_in", 4.1, false, true, true, true, 1);
CMD:drunk(playerid, params[]) return LoopingAnim(playerid, "PED", "WALK_DRUNK", 4.0, true, true, true, true, 0);
CMD:lookout(playerid, params[]) return LoopingAnim(playerid, "SHOP", "ROB_Shifty", 4.0, false, false, false, false, 0);
CMD:napad(playerid, params[]) return LoopingAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, true, false, false, false, 0);
CMD:hide(playerid, params[]) return LoopingAnim(playerid, "ped", "cower", 3.0, true, false, false, false, 0);
CMD:rzygaj(playerid, params[]) return LoopingAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, false, false, false, false, 0);
CMD:jedz(playerid, params[]) return LoopingAnim(playerid, "FOOD", "EAT_Burger", 3.0, false, false, false, false, 0);
CMD:machaj(playerid, params[]) return LoopingAnim(playerid, "ON_LOOKERS", "wave_loop", 4.0, true, false, false, false, 0);
CMD:klepnij(playerid, params[]) return LoopingAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, false, false, false, false, 0);
CMD:deal(playerid, params[]) return LoopingAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, false, false, false, false, 0);
CMD:cpun(playerid, params[]) return LoopingAnim(playerid, "CRACK", "crckdeth2", 4.0, true, false, false, false, 0);
CMD:siadaj(playerid, params[]) return LoopingAnim(playerid, "BEACH", "ParkSit_M_loop", 4.0, true, false, false, false, 0);
CMD:fucku(playerid, params[]) return LoopingAnim(playerid, "PED", "fucku", 4.0, false, false, false, false, 0);
CMD:taichi(playerid, params[]) return LoopingAnim(playerid, "PARK", "Tai_Chi_Loop", 4.0, true, false, false, false, 0);
CMD:pij(playerid, params[]) return LoopingAnim(playerid, "BAR", "dnk_stndF_loop", 4.0, true, false, false, false, 0);
CMD:inbedright(playerid, params[]) return LoopingAnim(playerid, "INT_HOUSE", "BED_In_R", 4.1, false, true, true, true, 1);
CMD:inbedleft(playerid, params[]) return LoopingAnim(playerid, "INT_HOUSE", "BED_In_L", 4.1, false, true, true, true, 1);
CMD:fotel(playerid, params[]) return LoopingAnim(playerid, "INT_HOUSE", "LOU_In", 4.1, false, true, true, true, 1);
CMD:patrz(playerid, params[]) return LoopingAnim(playerid, "SHOP", "SHP_Serve_Start", 4.1, false, true, true, true, 1);
CMD:opieraj(playerid, params[]) return LoopingAnim(playerid, "GANGS", "leanIDLE", 4.1, false, true, true, true, 1);
CMD:zolwik(playerid, params[]) return LoopingAnim(playerid, "GANGS", "hndshkda", 4.1, false, true, true, true, 1);
CMD:carchat(playerid, params[]) return LoopingAnim(playerid, "CAR_CHAT", "car_talkm_out", 4.1, false, true, true, true, 1);
CMD:krzycz(playerid, params[]) return LoopingAnim(playerid, "RIOT", "RIOT_shout", 4.1, false, true, true, true, 1);
CMD:wtf(playerid, params[]) return LoopingAnim(playerid, "RIOT", "RIOT_ANGRY", 4.1, false, true, true, true, 1);
CMD:adhd(playerid, params[]) return LoopingAnim(playerid, "PED", "BIKE_fallR", 4.1, false, true, true, true, 0);
CMD:raczkuj(playerid, params[]) return LoopingAnim(playerid, "PED", "CAR_crawloutRHS", 4.1, false, true, true, true, 0);
CMD:down(playerid, params[]) return LoopingAnim(playerid, "BOX", "bxhwlki", 3.1, false, true, true, true, 0);
CMD:scratch(playerid, params[]) return LoopingAnim(playerid, "MISC", "Scratchballs_01", 3.0, true, false, false, false, 0, 1);
CMD:beach(playerid, params[]) return LoopingAnim(playerid, "BEACH", "SitnWait_loop_W", 4.0999, false, true, true, true, 1, 1);
CMD:circle(playerid, params[]) return LoopingAnim(playerid, "CHAINSAW", "CSAW_Hit_2", 4.0999, true, true, true, true, 1, 1);
CMD:rofl(playerid, params[]) return LoopingAnim(playerid, "PED", "Crouch_Roll_L", 4.0999, false, true, true, true, 1, 1);
CMD:getin(playerid, params[]) return LoopingAnim(playerid, "NEVADA", "NEVADA_getin", 4.0999, false, true, true, true, 1, 1);
CMD:sprayuj(playerid, params[]) return LoopingAnim(playerid, "SPRAYCAN", "spraycan_full", 4.0, false, false, false, false, 0, 1);
CMD:headbutt(playerid, params[]) return LoopingAnim(playerid, "WAYFARER", "WF_Fwd", 4.0, false, false, false, false, 0, 1);
CMD:lightcig(playerid, params[]) return LoopingAnim(playerid, "SMOKING", "M_smk_in", 3.0, false, false, false, false, 0, 1);
CMD:tapcig(playerid, params[]) return LoopingAnim(playerid, "SMOKING", "M_smk_tap", 3.0, false, false, false, false, 0, 1);
CMD:bat(playerid, params[]) return LoopingAnim(playerid, "BASEBALL", "Bat_IDLE", 4.0, true, true, true, true, 0, 1);
CMD:chant(playerid, params[]) return LoopingAnim(playerid, "RIOT", "RIOT_CHANT", 4.0, true, true, true, true, 0, 1);
CMD:finger(playerid, params[]) return LoopingAnim(playerid, "RIOT", "RIOT_FUKU", 2.0, false, false, false, false, 0, 1);
CMD:shouting(playerid, params[]) return LoopingAnim(playerid, "RIOT", "RIOT_shout", 4.0, true, false, false, false, 0, 1);
CMD:cop(playerid, params[]) return LoopingAnim(playerid, "SWORD", "sword_block", 50.0, false, true, true, true, 1, 1);
CMD:lowbodypush(playerid, params[]) return LoopingAnim(playerid, "GANGS", "shake_carSH", 4.0, false, false, false, false, 0, 1);
CMD:push(playerid, params[]) return LoopingAnim(playerid, "GANGS", "shake_cara", 4.0, false, false, false, false, 0, 1);
CMD:rollfall(playerid, params[]) return LoopingAnim(playerid, "PED", "BIKE_fallR", 4.0, false, true, true, true, 0, 1);
CMD:boxanim(playerid, params[]) return LoopingAnim(playerid, "GYMNASIUM", "GYMshadowbox", 4.0, true, true, true, true, 0, 1);
CMD:fall(playerid, params[]) return LoopingAnim(playerid, "PED", "KO_skid_front", 4.0999, false, true, true, true, 0, 1);
CMD:fallback(playerid, params[]) return LoopingAnim(playerid, "PED", "FLOOR_hit_f", 4.0, true, false, false, false, 0, 1);
CMD:robman(playerid, params[]) return LoopingAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, true, false, false, false, 0, 1);
CMD:bacznosc(playerid, params[]) return LoopingAnim(playerid, "RAPPING", "RAP_A_OUT", 4.1, false, true, true, true, 1);

CMD:rap(playerid, params[])
{
	new rap;
	if(sscanf(params, "d", rap) || !(1 <= 6)) return syntaxError(playerid, "/rap [1-6]");

	switch(rap) {
		case 1: LoopingAnim(playerid, "RAPPING", "RAP_B_Loop", 4.0, true, false, false, false, 0);
		case 2: LoopingAnim(playerid, "RAPPING", "RAP_B_Loop", 4.1, false, true, true, true, 1);
		case 3: LoopingAnim(playerid, "RAPPING", "RAP_C_Loop", 4.1, false, true, true, true, 1);
		case 4: LoopingAnim(playerid, "RAPPING", "Laugh_0", 4.1, false, true, true, true, 1);
		case 5: LoopingAnim(playerid, "RAPPING", "RAP_B_IN", 4.1, false, true, true, true, 1);
		case 6: LoopingAnim(playerid, "RAPPING", "RAP_A_IN", 4.1, false, true, true, true, 1);
	}
	return 1;
}

CMD:smoke(playerid, params[])
{
	new sk;
	if(sscanf(params, "d", sk) || !(1 <= sk <= 4)) return syntaxError(playerid, "/smoke [1-4]");

	switch(sk) {
		case 1: LoopingAnim(playerid, "SMOKING", "M_smklean_loop", 4.0, true, false, false, false, 0, 1);
		case 2: LoopingAnim(playerid, "SMOKING", "F_smklean_loop", 4.0, true, false, false, false, 0, 1);
		case 3: LoopingAnim(playerid, "SMOKING", "M_smkstnd_loop", 4.0, true, false, false, false, 0, 1);
		case 4: LoopingAnim(playerid, "SMOKING", "M_smk_out", 4.0, true, false, false, false, 0, 1);
	}
	return 1;
}

CMD:aim(playerid, params[])
{
	new am;
	if(sscanf(params, "d", am) || !(1 <= am <= 3)) return syntaxError(playerid, "/aim [1-3]");

	switch(am) {
		case 1: LoopingAnim(playerid, "PED", "gang_gunstand", 4.0, true, true, true, true, 1, 1);
		case 2: LoopingAnim(playerid, "PED", "Driveby_L", 4.0, false, true, true, true, 1, 1);
		case 3: LoopingAnim(playerid, "PED", "Driveby_R", 4.0, false, true, true, true, 1, 1);
	}
	return 1;
}

CMD:run(playerid, params[])
{
	new run;
	if(sscanf(params, "d", run) || !(1 <= run <= 9)) return syntaxError(playerid, "/run [1-9]");

	switch(run) {
		case 1: LoopingAnim(playerid, "FAT", "FatRun", 4.0, true, true, true, true, 1, 1);
		case 2: LoopingAnim(playerid, "PED", "run_player", 4.0, true, true, true, true, 1, 1);
		case 3: LoopingAnim(playerid, "PED", "jog_femaleA", 4.0, true, true, true, true, 1, 1);
		case 4: LoopingAnim(playerid, "PED", "jog_maleA", 4.0, true, true, true, true, 1, 1);
		case 5: LoopingAnim(playerid, "PED", "run_old", 4.0, true, true, true, true, 1, 1);
		case 6: LoopingAnim(playerid, "PED", "run_left", 4.0, true, true, true, true, 1, 1);
		case 7: LoopingAnim(playerid, "PED", "run_fatold", 4.0, true, true, true, true, 1, 1);
		case 8: LoopingAnim(playerid, "PED", "run_gang1", 4.0, true, true, true, true, 1, 1);
		case 9: LoopingAnim(playerid, "PED", "run_fat", 4.0, true, true, true, true, 1, 1);
	}
	return 1;
}

CMD:lay(playerid, params[])
{
	new lay;
	if(sscanf(params, "d", lay)  || !(1 <= lay <= 9)) return syntaxError(playerid, "/lay [1-9]");

	switch(lay) {
		case 1: LoopingAnim(playerid, "BEACH", "bather", 4.0, true, false, false, false, 0, 1);
		case 2: LoopingAnim(playerid, "BEACH", "parksit_w_loop", 4.0, true, false, false, false, 0, 1);
		case 3: LoopingAnim(playerid, "BEACH", "parksit_m_loop", 4.0, true, false, false, false, 0, 1);
		case 4: LoopingAnim(playerid, "BEACH", "lay_bac_loop", 4.0, true, false, false, false, 0, 1);
		case 5: LoopingAnim(playerid, "BEACH", "sitnwait_loop_w", 4.0, true, false, false, false, 0, 1);
		case 6: LoopingAnim(playerid, "SUNBATHE", "Lay_Bac_in", 3.0, false, true, true, true, 0, 5);
		case 7: LoopingAnim(playerid, "SUNBATHE", "batherdown", 3.0, false, true, true, true, 0, 1);
		case 8: LoopingAnim(playerid, "SUNBATHE", "parksit_m_in", 3.0, false, true, true, true, 0, 1);
		case 9: LoopingAnim(playerid, "CAR", "Fixn_Car_Loop", 4.0, true, false, false, false, 0, 1);
	}
	return 1;
}

CMD:wave(playerid, params[])
{
	new wv;
	if(sscanf(params, "d", wv) || !(1 <= wv <= 5)) return syntaxError(playerid, "/wave [1-5]");

	switch(wv) {
		case 1: LoopingAnim(playerid, "ON_LOOKERS", "wave_loop", 4.0, true, false, false, false, 0, 1);
		case 2: LoopingAnim(playerid, "BD_Fire", "BD_GF_Wave", 4.0, false, false, false, false, 0, 1);
		case 3: LoopingAnim(playerid, "RIOT", "RIOT_CHANT", 4.0, true, true, true, true, 0, 1);
		case 4: LoopingAnim(playerid, "WUZI", "Wuzi_Follow", 5.0, false, false, false, false, 0, 1);
		case 5: LoopingAnim(playerid, "KISSING", "gfwave2", 4.0, false, false, false, false, 0, 1);
	}
	return 1;
}

CMD:chat(playerid, params[])
{
	new ch;
	if(sscanf(params, "d", ch) || !(1 <= ch <= 10)) return syntaxError(playerid, "/chat [1-10]");

	switch(ch) {
		case 1: LoopingAnim(playerid, "PED", "IDLE_CHAT", 2.0, true, false, false, true, 1, 1);
		case 2: LoopingAnim(playerid, "MISC", "IDLE_CHAT_02", 2.0, true, false, false, true, 1, 1);
		case 3: LoopingAnim(playerid, "BAR", "Barcustom_order", 3.0, false, false, false, false, 0, 1);
        case 4: LoopingAnim(playerid, "GANGS", "prtial_gngtlkA", 4.1, true, false, false, false, 0, 1);
        case 5: LoopingAnim(playerid, "GANGS", "prtial_gngtlkB", 4.1, true, false, false, false, 0, 1);
        case 6: LoopingAnim(playerid, "GANGS", "prtial_gngtlkD", 4.1, true, false, false, false, 0, 1);
        case 7: LoopingAnim(playerid, "GANGS", "prtial_gngtlkE", 4.1, true, false, false, false, 0, 1);
        case 8: LoopingAnim(playerid, "GANGS", "prtial_gngtlkF", 4.1, true, false, false, false, 0, 1);
        case 9: LoopingAnim(playerid, "GANGS", "prtial_gngtlkG", 4.1, true, false, false, false, 0, 1);
        case 10: LoopingAnim(playerid, "GANGS", "prtial_gngtlkH", 4.1, true, false, false, false, 0, 1);
	}
	return 1;
}

CMD:walk(playerid, params[])
{
	new wk;
	if(sscanf(params, "d", wk) || !(1 <= wk <= 9)) return syntaxError(playerid, "/walk [1-9]");

	switch(wk) {
		case 1: LoopingAnim(playerid, "PED", "WALK_gang1", 4.0999, true, true, true, true, 1, 1);
		case 2: LoopingAnim(playerid, "PED", "WALK_gang2", 4.0999, true, true, true, true, 1, 1);
		case 3: LoopingAnim(playerid, "FAT", "FatWalk", 4.0999, true, true, true, true, 1, 1);
		case 4: LoopingAnim(playerid, "WUZI", "CS_Wuzi_pt1", 4.0999, true, true, true, true, 1, 1);
		case 5: LoopingAnim(playerid, "WUZI", "Wuzi_walk", 3.0, true, true, true, true, 1);
		case 6: LoopingAnim(playerid, "POOL", "Pool_walk", 3.0, true, true, true, true, 1);
		case 7: LoopingAnim(playerid, "PED", "Walk_player", 3.0, true, true, true, true, 1);
		case 8: LoopingAnim(playerid, "PED", "Walk_old", 3.0, true, true, true, true, 1);
		case 9: LoopingAnim(playerid, "PED", "Walk_fatold", 3.0, true, true, true, true, 1);
	}
	return 1;
}

CMD:look(playerid, params[])
{
	new lk;
	if(sscanf(params, "d", lk)  || !(1 <= lk <= 4)) return syntaxError(playerid, "/look [1-4]");

	switch(lk) {
		case 1: LoopingAnim(playerid, "ON_LOOKERS", "lkup_loop", 3.0, true, false, false, false, 0, 1);
		case 2: LoopingAnim(playerid, "ON_LOOKERS", "lkaround_loop", 3.0, true, false, false, false, 0, 1);
		case 3: LoopingAnim(playerid, "PED", "flee_lkaround_01", 3.0, true, true, true, true, 1);
		case 4: LoopingAnim(playerid, "BAR", "Barserve_bottle", 3.0, false, true, true, true, 0, 1);
	}
	return 1;
}

CMD:shout(playerid, params[])
{
	new st;
	if(sscanf(params, "d", st) || !(1 <= st <= 3)) return syntaxError(playerid, "/shout [1-3]");

	switch(st) {
		case 1: LoopingAnim(playerid, "ON_LOOKERS", "shout_loop", 3.0, true, false, false, false, 0, 6);
		case 2: LoopingAnim(playerid, "ON_LOOKERS", "shout_01", 3.0, true, false, false, false, 0, 1);
		case 3: LoopingAnim(playerid, "ON_LOOKERS", "shout_02", 3.0, true, false, false, false, 0, 1);
	}
	return 1;
}

alias:bj("blowjob", "lod", "lodzik")
CMD:bj(playerid, params[])
{
	new bj;
	if(sscanf(params, "d", bj) || !(1 <= bj <= 3)) return syntaxError(playerid, "/bj [1-3]");

	switch(bj) {
        case 1: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_END_W", 4.0, false, true, true, true, 0, 1);
		case 2: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_END_P", 4.0, false, true, true, true, 0, 1);
		case 3: LoopingAnim(playerid, "BLOWJOBZ", "BJ_STAND_LOOP_W", 4.0, false, true, true, true, 0, 1);
	}
	return 1;
}

CMD:swat(playerid, params[])
{
	new as;
	if(sscanf(params, "d", as) || !(1 <= as <= 6)) return syntaxError(playerid, "/swat [1-6]");

	switch(as) {
		case 1: LoopingAnim(playerid, "SWAT", "gnstwall_injurd", 4.0, false, true, true, true, 0, 1);
		case 2: LoopingAnim(playerid, "SWAT", "JMP_Wall1m_180", 4.0, false, true, true, true, 0, 1);
		case 3: LoopingAnim(playerid, "SWAT", "Rail_fall", 4.0, false, true, true, true, 0, 1);
		case 4: LoopingAnim(playerid, "SWAT", "Rail_fall_crawl", 4.0, false, true, true, true, 0, 1);
		case 5: LoopingAnim(playerid, "SWAT", "swt_breach_01", 4.0, false, true, true, true, 0, 1);
		case 6: LoopingAnim(playerid, "SWAT", "swt_breach_02", 4.0, false, true, true, true, 0, 1);
	}
	return 1;
}

CMD:lean(playerid, params[])
{
	new ln;
	if(sscanf(params, "d", ln)  || !(1 <= ln <= 2)) return syntaxError(playerid, "/lean [1-2]");

	switch(ln) {
		case 1: LoopingAnim(playerid, "GANGS", "leanIDLE", 4.0, false, true, true, true, 0, 1);
		case 2: LoopingAnim(playerid, "MISC", "Plyrlean_loop", 4.0, false, true, true, true, 0, 1);
	}
	return 1;
}

CMD:stance(playerid, params[])
{
	new stance;
	if(sscanf(params, "d", stance)  || !(1 <= stance <= 9)) return syntaxError(playerid, "/stance [1-9]");

	switch(stance) {
		case 1: LoopingAnim(playerid, "DEALER", "DEALER_IDLE", 4.0, true, false, false, false, 0, 1);
		case 2: LoopingAnim(playerid, "PED", "WOMAN_IDLESTANCE", 4.0, true, false, false, false, 0, 1);
		case 3: LoopingAnim(playerid, "PED", "CAR_HOOKERTALK", 4.0, true, false, false, false, 0, 1);
		case 4: LoopingAnim(playerid, "FAT", "FatIdle", 4.0, true, false, false, false, 0, 1);
		case 5: LoopingAnim(playerid, "WUZI", "Wuzi_Stand_Loop", 4.0, true, false, false, false, 0, 1);
		case 6: LoopingAnim(playerid, "GRAVEYARD", "mrnf_loop", 4.0, true, false, false, false, 0, 1);
		case 7: LoopingAnim(playerid, "GRAVEYARD", "mrnm_loop", 4.0, true, false, false, false, 0, 1);
		case 8: LoopingAnim(playerid, "GRAVEYARD", "prst_loopa", 4.0, true, false, false, false, 0, 1);
		case 9: LoopingAnim(playerid, "PED", "idlestance_fat", 4.0, true, false, false, false, 0, 1);
	}
	return 1;
}

CMD:hike(playerid, params[])
{
	new hike;
	if(sscanf(params, "d", hike) || !(1 <= hike <= 3)) return syntaxError(playerid, "/hike [1-3]");

	switch(hike) {
		case 1: LoopingAnim(playerid, "MISC", "hiker_pose", 4.0, true, false, false, false, 0, 1);
		case 2: LoopingAnim(playerid, "MISC", "hiker_pose_l", 4.0, true, false, false, false, 0, 1);
		case 3: LoopingAnim(playerid, "PED", "idle_taxi", 3.0, false, false, false, false, 0, 1);
	}
	return 1;
}

CMD:face(playerid, params[])
{
	new face;
	if(sscanf(params, "d", face) || !(1 <= face <= 6)) return syntaxError(playerid, "/face [1-6]");

	switch(face) {
		case 1: LoopingAnim(playerid, "PED", "facanger", 3.0, true, true, true, true, 1);
		case 2: LoopingAnim(playerid, "PED", "facgum", 3.0, true, true, true, true, 1);
		case 3: LoopingAnim(playerid, "PED", "facsurp", 3.0, true, true, true, true, 1);
		case 4: LoopingAnim(playerid, "PED", "facsurpm", 3.0, true, true, true, true, 1);
		case 5: LoopingAnim(playerid, "PED", "factalk", 3.0, true, true, true, true, 1);
		case 6: LoopingAnim(playerid, "PED", "facurios", 3.0, true, true, true, true, 1);
	}

	return 1;
}

alias:tancz("dance")
CMD:tancz(playerid, params[])
{
    new dancestyle;
	if(sscanf(params, "d", dancestyle) || !(1 <= dancestyle <= 4)) return syntaxError(playerid, "/tancz [1-4]");

	switch(dancestyle) {
		case 1: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE1);
		case 2: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE2);
		case 3: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE3);
        case 4: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DANCE4);
	}
    return 1;
}

CMD:basket(playerid, params[])
{
    new basket;
    if(sscanf(params, "d", basket) || !(1 <= basket <= 6)) return syntaxError(playerid, "/basket [1-6]");

    switch(basket) {
        case 1: LoopingAnim(playerid, "BSKTBALL", "BBALL_def_loop", 4.0, true, false, false, false, 0, 1);
        case 2: LoopingAnim(playerid, "GHANDS", "gsign2LH", 4.0, false, true, true, true, 1, 1);
        case 3: LoopingAnim(playerid, "BSKTBALL", "BBALL_pickup", 4.0, false, false, false, false, 0, 1);
        case 4: LoopingAnim(playerid, "BSKTBALL", "BBALL_Jump_Shot", 4.0, false, false, false, false, 0, 1);
        case 5: LoopingAnim(playerid, "BSKTBALL", "BBALL_Dnk", 4.1, false, true, true, true, 1, 1);
        case 6: LoopingAnim(playerid, "BSKTBALL", "BBALL_run", 4.1, true, true, true, true, 1, 1);
    }
    return 1;
}

CMD:gryps(playerid, params[])
{
    new gryps;
    if(sscanf(params, "d", gryps) || !(1 <= gryps <= 10)) return syntaxError(playerid, "/gryps [1-10]");

    switch(gryps) {
        case 1: LoopingAnim(playerid, "GHANDS", "gsign1", 4.1, false, false, false, false, 0, 1);
        case 2: LoopingAnim(playerid, "GHANDS", "gsign1LH", 4.1, false, false, false, false, 0, 1);
        case 3: LoopingAnim(playerid, "GHANDS", "gsign2", 4.1, false, false, false, false, 0, 1);
        case 4: LoopingAnim(playerid, "GHANDS", "gsign2LH", 4.1, false, false, false, false, 0, 1);
        case 5: LoopingAnim(playerid, "GHANDS", "gsign3", 4.1, false, false, false, false, 0, 1);
        case 6: LoopingAnim(playerid, "GHANDS", "gsign3LH", 4.1, false, false, false, false, 0, 1);
        case 7: LoopingAnim(playerid, "GHANDS", "gsign4", 4.1, false, false, false, false, 0, 1);
        case 8: LoopingAnim(playerid, "GHANDS", "gsign4LH", 4.1, false, false, false, false, 0, 1);
        case 9: LoopingAnim(playerid, "GHANDS", "gsign5", 4.1, false, false, false, false, 0, 1);
        case 10: LoopingAnim(playerid, "GHANDS", "gsign5LH", 4.1, false, false, false, false, 0, 1);
    }
    return 1;
}

CMD:yo(playerid, params[])
{
    new yo;
    if(sscanf(params, "d", yo) || !(1 <= yo <= 9)) return syntaxError(playerid, "/yo [1-9]");

    switch(yo) {
        case 1: LoopingAnim(playerid, "GANGS", "DRUGS_BUY", 4.1, false, false, false, false, 0, 1);
        case 2: LoopingAnim(playerid, "GANGS", "hndshkaa", 4.1, false, false, false, false, 0, 1);
        case 3: LoopingAnim(playerid, "GANGS", "hndshkba", 4.1, false, false, false, false, 0, 1);
        case 4: LoopingAnim(playerid, "GANGS", "hndshkca", 4.1, false, false, false, false, 0, 1);
        case 5: LoopingAnim(playerid, "GANGS", "hndshkcb", 4.1, false, false, false, false, 0, 1);
        case 6: LoopingAnim(playerid, "GANGS", "hndshkda", 4.1, false, false, false, false, 0, 1);
        case 7: LoopingAnim(playerid, "GANGS", "hndshkea", 4.1, false, false, false, false, 0, 1);
        case 8: LoopingAnim(playerid, "GANGS", "hndshkfa", 4.1, false, false, false, false, 0, 1);
        case 9: LoopingAnim(playerid, "GANGS", "hndshkfa_swt", 4.1, false, false, false, false, 0, 1);
    }
    return 1;
}

CMD:siad(playerid, params[])
{
    new siad;
    if(sscanf(params, "d", siad) || !(1 <= siad <= 5)) return syntaxError(playerid, "/siad [1-5]");

    switch(siad) {
        case 1: LoopingAnim(playerid, "Attractors", "Stepsit_in", 4.1, false, true, true, true, 1, 1);
        case 2: LoopingAnim(playerid, "JST_BUISNESS", "girl_02", 4.1, false, true, true, true, 1, 1);
        case 3: LoopingAnim(playerid, "TATTOOS", "TAT_Sit_Out_T", 4.1, false, true, true, true, 1, 1);
        case 4: LoopingAnim(playerid, "TATTOOS", "TAT_Sit_Loop_P", 4.1, false, true, true, true, 1, 1);
        case 5: LoopingAnim(playerid, "BLOWJOBZ", "BJ_COUCH_LOOP_P", 4.1, false, true, true, true, 1, 1);
    }
    return 1;
}

CMD:jem(playerid, params[])
{
	new zarcie;
	if(sscanf(params, "d", zarcie) || !(1 <= zarcie <= 4)) return syntaxError(playerid, "/jem [1-4]");

	switch(zarcie) {
		case 1: LoopingAnim(playerid, "FOOD", "EAT_Burger", 3.0, false, false, false, false, 0);
		case 2: LoopingAnim(playerid, "FOOD", "FF_Sit_Eat1", 4.0, true, false, false, false, 0);
		case 3: LoopingAnim(playerid, "FOOD", "FF_Sit_Eat2", 4.0, true, false, false, false, 0);
		case 4: LoopingAnim(playerid, "FOOD", "FF_Sit_Eat3", 4.0, true, false, false, false, 0);
	}
	return 1;
}

CMD:trup(playerid, params[])
{
	new trup;
	if(sscanf(params, "d", trup) || !(1 <= trup <= 3)) return syntaxError(playerid, "/trup [1-3]");

	switch(trup) {
		case 1: LoopingAnim(playerid, "WUZI", "CS_Dead_Guy", 4.1, false, true, true, true, 1);
		case 2: LoopingAnim(playerid, "SWAT", "gnstwall_injurd", 4.0, true, false, false, false, 0);
		case 3: LoopingAnim(playerid, "WUZI", "CS_Dead_Guy", 4.0, false, true, true, true, -1);
	}
	return 1;
}

alias:pijak("pijany")
CMD:pijak(playerid, params[])
{
	new pijak;
	if(sscanf(params, "d", pijak) || !(1 <= pijak <= 3)) return syntaxError(playerid, "/pijak [1-3]");

	switch(pijak) {
		case 1: LoopingAnim(playerid, "WUZI", "CS_Dead_Guy", 4.1, false, true, true, true, 1);
		case 2: LoopingAnim(playerid, "SWAT", "gnstwall_injurd", 4.0, true, false, false, false, 0);
		case 3: LoopingAnim(playerid, "WUZI", "CS_Dead_Guy", 4.0, false, true, true, true, -1);
	}
	return 1;
}

CMD:rece(playerid, params[])
{
	new rece;
	if(sscanf(params, "d", rece) || !(1 <= rece <= 3)) return syntaxError(playerid, "/rece [1-3]");

	switch(rece) {
		case 1: LoopingAnim(playerid, "PED", "handsup", 4.0, false, true, true, true, 0);
		case 2: LoopingAnim(playerid, "DAM_JUMP", "DAM_Dive_Loop", 4.0, true, false, false, false, 0);
		case 3: LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, false, true, true, true, -1);
	}
	return 1;
}

alias:spadam("fallen", "falling")
CMD:spadam(playerid, params[])
{
	new fall;
	if(sscanf(params, "d", fall) || !(1 <= fall <= 3)) return syntaxError(playerid, "/spadam [1-3]");

	switch(fall) {
		case 1: LoopingAnim(playerid, "TRAIN", "tran_stmb", 4.0, false, false, false, true, 0);
		case 2: LoopingAnim(playerid, "TRAIN", "tran_hng", 4.0, true, false, false, false, 0);
		case 3: LoopingAnim(playerid, "TRAIN", "tran_gtup", 4.0, false, false, false, false, 0);
	} 
	return 1;
}

CMD:call(playerid, params[])
{
	new call;
	if(sscanf(params, "d", call) || !(1 <= call <= 2)) return syntaxError(playerid, "/call [1-2]");

	switch(call) {
		case 1: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
		case 2: SetPlayerSpecialAction(playerid, SPECIAL_ACTION_STOPUSECELLPHONE);
	}
	return 1;
}
