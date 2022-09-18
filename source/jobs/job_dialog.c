stock OnDialogJob(playerid, dialogid, response, listitem)
{
    switch(dialogid) {
        case DIALOG_PRACA_LIST: {
            if(!response) return task_await(RunJobPanel(playerid));

            s[0] = EOS;
            switch(listitem) {
                case JOB_MAGAZYNIER: {
                    strcat(s, ""WHITE"Twoim zadaniem jako "BLUE"magazynier"WHITE" jest dostarczy� towary do pu�ek w magazynie.\n");
                    strcat(s, ""WHITE"S� trzy rodzaje towar�w: "PINK"ma�a, �rednia oraz du�a"WHITE". Im wi�kszy towar, tym wi�ksza nagroda za od�o�enie.\n");
                    strcat(s, ""WHITE"Pami�taj r�wnie�, �e "GREEN"poziom pracy "WHITE"oraz "LY"zwi�kszanie wynagrodzenia"WHITE" odpowiada wi�kszym zarobkom.");
                }

                case JOB_GRABARZ: {
                    strcat(s, ""WHITE"Twoim zadaniem jako "PINK"grabarz"WHITE" jest odebranie zw�ok oraz "LY"pochowanie ich "WHITE"na cmentarzu w Los Santos.\n");
                    strcat(s, ""WHITE"Stawka za jeden poch�wek jest losowa, fajnym urozmaiceniem s� "LY"�mieszne nicki"WHITE" niekt�rych graczy, np. mo�esz pochowa� "RED"Lou"WHITE".\n");
                    strcat(s, ""WHITE"Pami�taj r�wnie�, �e "GREEN"poziom pracy "WHITE"oraz "LY"zwi�kszanie wynagrodzenia"WHITE" odpowiada wi�kszym zarobkom.");
                }

                case JOB_ZRZUT: {
                    strcat(s, ""WHITE"Twoim zadaniem jako "CORAL"pilot "WHITE"jest zrzucenie paczek w "RED"wyznaczone"WHITE" miejsce na mapie.\n");
                    strcat(s, ""WHITE"Stawka za zrzut jednej paczki jest "BLUE"losowana "WHITE"przez serwer!\n");
                    strcat(s, ""WHITE"Pami�taj r�wnie�, �e "GREEN"poziom pracy "WHITE"oraz "LY"zwi�kszanie wynagrodzenia"WHITE" odpowiada wi�kszym zarobkom.");
                }

                case JOB_TAXI: {
                    strcat(s, ""WHITE"Twoim zadaniem jako "BLUE"taks�wkarza"WHITE" jest dostarczenie klient�w w "GREEN"okre�lone "WHITE"przez nich miejsce na mapie.\n");
                    strcat(s, ""WHITE"Prace t� wykonujesz "RED"tylko i wy��cznie "WHITE"na terenie miasta "LY"San Fierro"WHITE".\n");
                    strcat(s, ""WHITE"Stawka za jednego klienta jest "MINT"losowa "WHITE"przez serwer!\n");
                    strcat(s, ""WHITE"Pami�taj r�wnie�, �e "GREEN"poziom pracy "WHITE"oraz "LY"zwi�kszanie wynagrodzenia"WHITE" odpowiada wi�kszym zarobkom.");
                }

                case JOB_MYSLIWY: {
                    strcat(s, ""WHITE"Twoim zadaniem jako "PINK"my�liwego"WHITE" jest zabicie saren w okolicy.\n");
                    strcat(s, ""WHITE"Stawka za jedn� martw� sarn� jest "RED"losowa "WHITE"przez serwer!\n");
                    strcat(s, ""WHITE"Pami�taj r�wnie�, �e "GREEN"poziom pracy "WHITE"oraz "LY"zwi�kszanie wynagrodzenia"WHITE" odpowiada wi�kszym zarobkom.");
                }

                case JOB_DRWAL: {
                    strcat(s, ""WHITE"Twoim zadaniem jako "RED"drwal"WHITE" jest �cinanie drzew w okolicy.\n");
                    strcat(s, ""WHITE"Stawka za jedno drzewo jest "RED"losowa "WHITE"przez serwer!\n");
                    strcat(s, ""WHITE"Pami�taj r�wnie�, �e "GREEN"poziom pracy "WHITE"oraz "LY"zwi�kszanie wynagrodzenia"WHITE" odpowiada wi�kszym zarobkom.");
                }

                case JOB_GRZYBIARZ: {
                    strcat(s, ""WHITE"Twoim zadaniem jako "RED"grzybiarz"WHITE" jest zbieranie grzyb�w w okolicy.\n");
                    strcat(s, ""WHITE"Stawka za zebranego grzyba jest "RED"losowa "WHITE"przez serwer!\n");
                    strcat(s, ""WHITE"Pami�taj r�wnie�, �e "GREEN"poziom pracy "WHITE"oraz "LY"zwi�kszanie wynagrodzenia"WHITE" odpowiada wi�kszym zarobkom.");
                }
            }

            new title[128]; format(title, sizeof(title), ""ORANGE"%s", getJobName(listitem));
            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, rr, ll, ii
                return OnDialogResponse(playerid, DIALOG_PRACA_PANEL, 1, 0, ""); }
            return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, title, s, "OK", #), 1;
        }

        case DIALOG_PRACA_REWARD: {
            if(!response) return task_await(RunJobPanel(playerid));
            if(playerData[playerid][player_job_points] < 1) return sendError(playerid, ""RED"Nie mo�esz zwi�kszy� zarobk�w, bo nie masz punkt�w pracownika!");
            if(pJobReward[playerid][listitem] >= 5) return sendError(playerid, ""RED"Posiadasz ju� maksymalnie zwi�kszone zarobki w pracy: {c}%s{/c}.", getJobName(listitem));

            pJobReward[playerid][listitem] ++;
            playerData[playerid][player_job_points] --;

            new str[128]; for(new i = 0; i < MAX_JOBS; i++) format(str, sizeof(str), "%s%s%i", str, i == 0 ? ("") : (","), pJobReward[playerid][i]);
            m_pquery_format("UPDATE es_accounts SET job_reward='%s', job_points=%d WHERE id=%d LIMIT 1;", str, playerData[playerid][player_job_points], playerData[playerid][player_accountID]);
            return OnDialogResponse(playerid, DIALOG_PRACA_PANEL, 1, 3, ""), 1;
        }

        case DIALOG_PRACA_ULEPSZENIA: {
            if(!response) return task_await(RunJobPanel(playerid));

            if(playerData[playerid][player_job_points] < jobUlepszeniaData[listitem][job_ulep_points]) return sendError(playerid, ""RED"Musisz posiada� {c}%d pkt. pracy{/c}, by zakupi� to ulepszenie!", jobUlepszeniaData[listitem][job_ulep_points]);
            if(pJobUlepszenia[playerid][listitem]) return sendError(playerid, ""RED"Posiadasz ju� zakupione ulepszenie: {c}%s{/c}.", jobUlepszeniaData[listitem][job_ulep_name]);

            inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                #pragma unused pp, dd, ll, ii
                if(!rr) return OnDialogResponse(playerid, DIALOG_PRACA_PANEL, 1, 4, "");

                if(playerData[playerid][player_job_points] < jobUlepszeniaData[listitem][job_ulep_points]) return sendError(playerid, ""RED"Musisz posiada� {c}%d pkt. pracy{/c}, by zakupi� to ulepszenie!", jobUlepszeniaData[listitem][job_ulep_points]);
                if(pJobUlepszenia[playerid][listitem]) return sendError(playerid, ""RED"Posiadasz ju� zakupione ulepszenie: {c}%s{/c}.", jobUlepszeniaData[listitem][job_ulep_name]);

                playerData[playerid][player_job_points] -= jobUlepszeniaData[listitem][job_ulep_points];
                msg(playerid, C_ORANGE, "Zakupi�e�(a�) ulepszenie pracy {c}%s{/c} za: {c}%d pkt. pracy{/c}.", jobUlepszeniaData[listitem][job_ulep_name], jobUlepszeniaData[listitem][job_ulep_points]);
                m_pquery_format("UPDATE es_accounts SET job_ulep='%s' WHERE id=%d LIMIT 1;", GetJobUlepszenia(playerid), playerData[playerid][player_accountID]);
                return task_await(RunJobPanel(playerid)), 1;
            }

            s[0] = EOS;                
            catstr(s, C_GOLD, -1, "Czy napewno chcesz zakupi� ulepszenie: {c}%s{/c}?\n", jobUlepszeniaData[listitem][job_ulep_name]);
            catstr(s, C_GOLD, -1, "Koszt tego zakupu wynosi: {c}%d pkt. pracy{/c}, a ich zwrot nie b�dzie mo�liwy!\n", jobUlepszeniaData[listitem][job_ulep_points]);
            catstr(s, C_GOLD, -1, "Po zakupie tego {c}ulepszenia{/c}, b�dziesz m�g� korzysta� z jego {c}profit�w{/c} w ka�dej pracy!");
            
            new title[128]; format(title, sizeof(title), ""ORANGE"%s", jobUlepszeniaData[listitem][job_ulep_name]);
            return Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, title, s, "Zakup", "Cofnij");
        }

        case DIALOG_PRACA_TOP: {
            if(!response) return task_await(RunJobPanel(playerid));

            inline onDialogData() {
                s[0] = EOS;
                s = ""BLACK"ID\t"WHITE"Nick\t"CORAL"Wynik\n";

                new id, nick[25], result;
                for(new i = 0; i < cache_num_rows(); i++) {
                    cache_get_value_int(i, 0, id);
                    cache_get_value(i, 1, nick, 25);
                    cache_get_value_int(i, 2, result);

                    format(s, sizeof(s), "%s"BLACK"%d.\t"WHITE"%s\t"CORAL"%s\n", s, i + 1, nick, strdot(result));
                }

                inline onTopDialog(pp, dd, rr, ll, string:ii[]) {
                    #pragma unused pp, dd, rr, ll, ii
                    return OnDialogResponse(playerid, DIALOG_PRACA_PANEL, 1, 1, ""); }
                new title[128]; format(title, sizeof(title), ""BLUE"Podium pracy: "WHITE"%s", getJobName(playerData[playerid][player_saveData][0]));
                return Dialog_ShowCallback(playerid, using inline onTopDialog, DIALOG_STYLE_TABLIST_HEADERS, title, s, "OK", #), 1;
            }

            dbstr[0] = EOS;
            switch(listitem) {
                case JOB_MAGAZYNIER: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_MAGAZYNIER + 1);
                case JOB_GRABARZ: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_GRABARZ + 1);
                case JOB_ZRZUT: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_ZRZUT + 1);
                case JOB_TAXI: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_TAXI + 1);
                case JOB_MYSLIWY: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_MYSLIWY + 1);
                case JOB_DRWAL: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_DRWAL + 1);
                case JOB_GRZYBIARZ: format(dbstr, sizeof(dbstr), "SELECT id, nick, CAST(SUBSTRING_INDEX(SUBSTRING_INDEX(ach_count, ',', %d), ',', -1) AS UNSIGNED) `liczba` FROM es_accounts ORDER BY `liczba` DESC LIMIT 10;", ACH_GRZYBIARZ + 1);
            }
            m_pquery_inline(dbconn, using inline onDialogData, dbstr);
            playerData[playerid][player_saveData][0] = listitem;
            return 1;
        }

        case DIALOG_PRACA_PANEL: {
            if(!response) return 1;

            s[0] = EOS; 
            switch(listitem) {
                case 0: {
                    s[0] = EOS;
                    for(new i = 0; i < MAX_JOBS; i++) format(s, sizeof(s), "%s"WHITE"�  "BLUE"%s\n", s, getJobName(i));
                    showDialog(playerid, DIALOG_PRACA_LIST, DIALOG_STYLE_LIST, ""YELLOW"Lista dost�pnych prac", s, "Wybierz", "Cofnij");
                }

                case 1: {
                    s[0] = EOS;
                    for(new i = 0; i < MAX_JOBS; i++) format(s, sizeof(s), "%s"WHITE"�  "BLUE"%s\n", s, getJobName(i));
                    showDialog(playerid, DIALOG_PRACA_TOP, DIALOG_STYLE_LIST, ""YELLOW"Podium pracownik�w - wybierz prac�", s, "Wybierz", "Cofnij");
                }

                case 2, 5: task_await(RunJobPanel(playerid));
                case 3: {
                    s[0] = EOS;
                    s = ""WHITE"Praca\t"BLUE"Wynagrodzenie\n";

                    new temp[50];
                    for(new i = 0; i < MAX_JOBS; i++) {
                        switch(pJobReward[playerid][i]) {
                            case 1: temp = ""GREEN"I"RED"IIII";
                            case 2: temp = ""GREEN"II"RED"III";
                            case 3: temp = ""GREEN"III"RED"II";
                            case 4: temp = ""GREEN"IIII"RED"I";
                            case 5: temp = ""GREEN"IIIII";
                            default: temp = ""RED"IIIII";
                        }
                        format(s, sizeof(s), "%s"WHITE"%s\t%s\n", s, getJobName(i), temp);
                    }
                    showDialog(playerid, DIALOG_PRACA_REWARD, DIALOG_STYLE_TABLIST_HEADERS, ""BLUE"Zarobki w pracach", s, "Zwi�ksz", "Cofnij");
                }

                case 4: {
                    s[0] = EOS;
                    s = ""WHITE"Ulepszenie\t"BLUE"Koszt\t"GREEN"Sta"RED"tus\n";
                    for(new i = 0; i < sizeof(jobUlepszeniaData); i++) format(s, sizeof(s), "%s"WHITE"%s\t"BLUE"%d pkt.\t%s\n", s, jobUlepszeniaData[i][job_ulep_name], jobUlepszeniaData[i][job_ulep_points], (pJobUlepszenia[playerid][i]) ? (""GREEN"ODBLOKOWANE") : (""RED"ZABLOKOWANE"));
                    showDialog(playerid, DIALOG_PRACA_ULEPSZENIA, DIALOG_STYLE_TABLIST_HEADERS, ""YELLOW"Ulepszenia pracy", s, "Zakup", "Cofnij"); 
                }

                default: {
                    if(pJob[playerid] != -1) ExitPraca(playerid);
                    else {
                        pJob[playerid] = listitem - 6;
                        SavePlayerPosition(playerid);
                        ResetPlayerWeapons(playerid);
                        SetHealth(playerid, 100.0, 0.0);

                        s[0] = EOS; format(s, sizeof(s), ""WHITE"Rozpocz��e�(a�) prac� jako: "RED"%s.\n", getJobName(pJob[playerid]));
                        switch(pJob[playerid]) {
                            case JOB_MAGAZYNIER: {
                                strcat(s, ""WHITE"Twoim zadaniem b�dzie "BLUE"roznoszenie towar�w "WHITE"po magazynie.");
                                
                                Teleport(playerid, false, 1341.0951, 1043.8346, 1190.5111, 0.0, 1, VW_MAGAZYNIER);
                                Magazynier_Fetch(playerid);
                            }

                            case JOB_GRABARZ: {
                                strcat(s, ""WHITE"Twoim zadaniem b�dzie "BLUE"odebranie zw�ok"WHITE" oraz pochowanie ich "BLUE"na cmentarzu"WHITE".");
                                
                                SetPlayerVirtualWorld(playerid, VW_GRABARZ);
                                Grabarz_RandomPosition(playerid);
                            }

                            case JOB_ZRZUT: {
                                strcat(s, ""WHITE"Twoim zadaniem b�dzie "RED"zrzucenie paczek "WHITE"do miejsc oznaczonych na mapie.");
                                
                                SetPVarInt(playerid, "player_zrzut_world", VW_ZRZUT + playerid + math_random(1000, 3000));
                                Teleport(playerid, false, 409.8317, 2502.6594, 18.0052, 90.2832, 0, GetPVarInt(playerid, "player_zrzut_world"));
                                Zrzut_Start(playerid);
                            }

                            case JOB_TAXI: {
                                strcat(s, ""WHITE"Twoim zadaniem b�dzie "CORAL"dostarczenie klient�w "WHITE"do wyznaczonych przez nich miejsc na mapie.");

                                DisableRemoteVehicleCollisions(playerid, true);
                                SetPlayerVirtualWorld(playerid, VW_TAXI);
                                Taxi_Start(playerid);
                            }

                            case JOB_MYSLIWY: {
                                strcat(s, ""WHITE"Twoim zadaniem b�dzie "RED"upolowanie "WHITE"saren, kt�re hasaj� w okolicy.");

                                SetPVarInt(playerid, "player_mysliwy_world", VW_MYSLIWY + playerid + math_random(10000, 30000));
                                Teleport(playerid, false, -661.9034, -2042.9316, 25.8775, 199.7764, 0, GetPVarInt(playerid, "player_mysliwy_world"));
                                Mysliwy_Start(playerid);
                            }

                            case JOB_DRWAL: {
                                strcat(s, ""WHITE"Twoim zadaniem b�dzie "RED"�ci�cie "WHITE"drzew w okolicy.");
                            
                                SetPVarInt(playerid, "player_drwal_world", VW_DRWAL + playerid + math_random(30000, 50000));
                                Teleport(playerid, false, -1931.2423, -2449.1733, 30.6181, 309.4200, 0, GetPVarInt(playerid, "player_drwal_world"));
                                Drwal_Start(playerid);
                            }

                            case JOB_GRZYBIARZ: {
                                strcat(s, ""WHITE"Twoim zadaniem b�dzie "RED"zbieranie "WHITE"grzyb w okolicy farmy.");

                                SetPVarInt(playerid, "player_grzybiarz_world", VW_GRZYBIARZ + playerid + math_random(50000, 70000));
                                Teleport(playerid, false, 897.2498, -332.1928, 43.1015, 48.6064, 0, GetPVarInt(playerid, "player_grzybiarz_world"));
                                Grzybiarz_Start(playerid);
                            }
                        }

                        for(new x = 0; x < 2; x++) PlayerTextDrawShow(playerid, tdInfo[x]);
                        catstr(s, C_LRED, -1, "\n\n"LRED"Z pracy mo�esz zrezygnowa�, wpisuj�c: {c}/exit{/c}.");
                        showDialog(playerid, DIALOG_UNUSED, DIALOG_STYLE_MSGBOX, ""YELLOW"Rozpocz�cie pracy", s, "OK", #);
                    }
                }
            }           
        }
    }
    return 0;
}
