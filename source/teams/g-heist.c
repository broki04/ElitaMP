/*
    Rozplanowanie napadów na bazy gangowe:

    - przynajmniej 3 osoby online po obu stronach;
    - czas trwania napadu: [do ustalenia pomiêdzy 3-5 minuty]
    - podczas trwania napadu, jest mo¿liwa kradzie¿ euro z sejfu gangu.
    - za zabójstwo gracza przeciwnego gangu s¹ zwiêkszone profity dla gangu.
    - opcja odpalenia nalotu (raz na ca³y napad, wyznaczasz miejsce kombinacj¹ klawiszy CTRL + N, po 10 sekundach nastêpuje potê¿na eksplozja).
    - gang, który osi¹gnie najwiêksz¹ iloœæ punktów (zabójstwa + damage), zgarnia tyle, ile zajeba³ enemy gangowi z sejfu ez.
*/

#include YSI_Coding\y_hooks

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_GANG_HEIST: {
            return 1;
        }
    }

    return 0;
}