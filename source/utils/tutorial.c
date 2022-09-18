#include YSI_Coding\y_hooks

stock ShowTutorial(playerid)
{
    s[0] = EOS;
    catstr(s, 0x0080B0FF, -1, "Witaj na: "NAME" "BLUE2"[ES]\n");
    catstr(s, 0x0080B0FF, -1, "Zach�camy do zapoznania si� z {c}samouczkiem{/c} z racji, i� jeste� nowym graczem!\n");
    catstr(s, 0x0080B0FF, -1, "Poni�ej przedstawiona jest {c}lista{/c}, kt�ry po kolei Ci przedstawi serwer.\n");
    strcat(s, " \n");
    catstr(s, C_GOLD, -1, "  "ARROW" "WHITE"Informacje o serwerze\n");
    catstr(s, C_GOLD, -1, "  "ARROW" "WHITE"Gospodarka serwera\n");
    catstr(s, C_GOLD, -1, "  "ARROW" "WHITE"Atrakcje dla graczy\n");
    strcat(s, " \n");
    catstr(s, C_CORAL, -1, "Zach�camy r�wnie� do do��czenia na Discord: {c}"#DISCORD"{/c}");
    return showDialog(playerid, DIALOG_TUTORIAL, DIALOG_STYLE_LIST, SERVER_TAG, s, "Wybierz", "Zamknij"), 1;
}

alias:tutorial("poradnik", "samouczek", "tut")
CMD:tutorial(playerid) return ShowTutorial(playerid);

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_TUTORIAL: {
            if(!response) return 1;
            switch(listitem) {
                case 0..3, 7, 8: ShowTutorial(playerid);

                case 4: {
                    s[0] = EOS;
                    catstr(s, C_CORAL, -1, "Jeste�my serwerem, kt�ry jest tworzony od {c}starych graczy SA-MP{/c} dla Was!\n");
                    catstr(s, C_CORAL, -1, "Razem z ekip� {c}starych wyjadaczy{/c} tej platformy, stworzyli�my odpowiednie �rodowisko dla ka�dego.\n");
                    catstr(s, C_CORAL, -1, "G��wnym w�a�cicielem projektu, jak i autorem skryptu, jest {c}Broki{/c} - m�ody ch�opak z ambicjami.\n");
                    catstr(s, C_CORAL, -1, "Co tu du�o m�wi�, pisze to o {c}21:37{/c}, a tak serio to zach�cam do przeczytania dalszej cz�ci samouczka!");

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return ShowTutorial(playerid), 1; }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BLUE"Informacje o serwerze", s, "OK", #);
                }

                case 5: {
                    s[0] = EOS;
                    catstr(s, C_MINT, -1, "Gospodarka serwera opiera si� g��wnie na dw�ch walutach, czyli {c}pkt. respektu{/c} oraz {c}got�wce{/c}.\n");
                    catstr(s, C_MINT, -1, "Wspomniane wy�ej waluty mo�na zdoby� wykonuj�c {c}r�ne czynno�ci{/c}, przygotowane dla Was!\n");
                    catstr(s, C_MINT, -1, "Postarali�my si�, by mo�na by�o dosy� {c}�atwo zarabia�{/c} oraz, �eby by�o {c}na co wydawa�{/c} ci�ko zarobione pieni�dze.\n");
                    catstr(s, C_MINT, -1, "Mo�na zarabia� w {c}/praca{/c}, {c}/rybki{/c}, {c}/gornictwo{/c}, a tak�e bior�c udzia� w zabawach, czy eventach!");

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return ShowTutorial(playerid), 1; }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""VIOLET"Gospodarka serwera", s, "OK", #);
                }

                case 6: {
                    s[0] = EOS;
                    catstr(s, C_BLUE2, -1, "Przygotowali�my dla Was, graczy, wiele atrakcji - tak, aby ka�dy mia� na czym zawiesi� swoje oczy.\n");
                    catstr(s, C_BLUE2, -1, "Najbardziej skupili�my si� na {c}dw�ch systemach{/c}, czyli: {c}gangach{/c} oraz {c}prywatnych pojazdach{/c}.\n");
                    catstr(s, C_BLUE2, -1, "Posiadamy r�wnie� wiele pomniejszych system�w, przyk�adowo {c}zadania dnia{/c}, {c}dyskietki{/c}, {c}figurki{/c}, {c}sol�wki{/c}.\n");
                    catstr(s, C_BLUE2, -1, "Zach�camy tak�e do zakupu {c}konta premium{/c}, kt�re dodaje kilka przywilei dla graczy - wi�cej pod: {c}/premium{/c}.");

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return ShowTutorial(playerid), 1; }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""RED"Atrakcje dla graczy", s, "OK", #);
                }
            }
        }
    }
    return 0;
}