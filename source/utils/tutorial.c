#include YSI_Coding\y_hooks

stock ShowTutorial(playerid)
{
    s[0] = EOS;
    catstr(s, 0x0080B0FF, -1, "Witaj na: "NAME" "BLUE2"[ES]\n");
    catstr(s, 0x0080B0FF, -1, "Zachêcamy do zapoznania siê z {c}samouczkiem{/c} z racji, i¿ jesteœ nowym graczem!\n");
    catstr(s, 0x0080B0FF, -1, "Poni¿ej przedstawiona jest {c}lista{/c}, który po kolei Ci przedstawi serwer.\n");
    strcat(s, " \n");
    catstr(s, C_GOLD, -1, "  "ARROW" "WHITE"Informacje o serwerze\n");
    catstr(s, C_GOLD, -1, "  "ARROW" "WHITE"Gospodarka serwera\n");
    catstr(s, C_GOLD, -1, "  "ARROW" "WHITE"Atrakcje dla graczy\n");
    strcat(s, " \n");
    catstr(s, C_CORAL, -1, "Zachêcamy równie¿ do do³¹czenia na Discord: {c}"#DISCORD"{/c}");
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
                    catstr(s, C_CORAL, -1, "Jesteœmy serwerem, który jest tworzony od {c}starych graczy SA-MP{/c} dla Was!\n");
                    catstr(s, C_CORAL, -1, "Razem z ekip¹ {c}starych wyjadaczy{/c} tej platformy, stworzyliœmy odpowiednie œrodowisko dla ka¿dego.\n");
                    catstr(s, C_CORAL, -1, "G³ównym w³aœcicielem projektu, jak i autorem skryptu, jest {c}Broki{/c} - m³ody ch³opak z ambicjami.\n");
                    catstr(s, C_CORAL, -1, "Co tu du¿o mówiæ, pisze to o {c}21:37{/c}, a tak serio to zachêcam do przeczytania dalszej czêœci samouczka!");

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return ShowTutorial(playerid), 1; }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""BLUE"Informacje o serwerze", s, "OK", #);
                }

                case 5: {
                    s[0] = EOS;
                    catstr(s, C_MINT, -1, "Gospodarka serwera opiera siê g³ównie na dwóch walutach, czyli {c}pkt. respektu{/c} oraz {c}gotówce{/c}.\n");
                    catstr(s, C_MINT, -1, "Wspomniane wy¿ej waluty mo¿na zdobyæ wykonuj¹c {c}ró¿ne czynnoœci{/c}, przygotowane dla Was!\n");
                    catstr(s, C_MINT, -1, "Postaraliœmy siê, by mo¿na by³o dosyæ {c}³atwo zarabiaæ{/c} oraz, ¿eby by³o {c}na co wydawaæ{/c} ciê¿ko zarobione pieni¹dze.\n");
                    catstr(s, C_MINT, -1, "Mo¿na zarabiaæ w {c}/praca{/c}, {c}/rybki{/c}, {c}/gornictwo{/c}, a tak¿e bior¹c udzia³ w zabawach, czy eventach!");

                    inline onDialogData(pp, dd, rr, ll, string:ii[]) {
                        #pragma unused pp, dd, rr, ll, ii
                        return ShowTutorial(playerid), 1; }
                    Dialog_ShowCallback(playerid, using inline onDialogData, DIALOG_STYLE_MSGBOX, ""VIOLET"Gospodarka serwera", s, "OK", #);
                }

                case 6: {
                    s[0] = EOS;
                    catstr(s, C_BLUE2, -1, "Przygotowaliœmy dla Was, graczy, wiele atrakcji - tak, aby ka¿dy mia³ na czym zawiesiæ swoje oczy.\n");
                    catstr(s, C_BLUE2, -1, "Najbardziej skupiliœmy siê na {c}dwóch systemach{/c}, czyli: {c}gangach{/c} oraz {c}prywatnych pojazdach{/c}.\n");
                    catstr(s, C_BLUE2, -1, "Posiadamy równie¿ wiele pomniejszych systemów, przyk³adowo {c}zadania dnia{/c}, {c}dyskietki{/c}, {c}figurki{/c}, {c}solówki{/c}.\n");
                    catstr(s, C_BLUE2, -1, "Zachêcamy tak¿e do zakupu {c}konta premium{/c}, które dodaje kilka przywilei dla graczy - wiêcej pod: {c}/premium{/c}.");

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