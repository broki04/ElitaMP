/*
    Rozplanowanie napad�w na bazy gangowe:

    - przynajmniej 3 osoby online po obu stronach;
    - czas trwania napadu: [do ustalenia pomi�dzy 3-5 minuty]
    - podczas trwania napadu, jest mo�liwa kradzie� euro z sejfu gangu.
    - za zab�jstwo gracza przeciwnego gangu s� zwi�kszone profity dla gangu.
    - opcja odpalenia nalotu (raz na ca�y napad, wyznaczasz miejsce kombinacj� klawiszy CTRL + N, po 10 sekundach nast�puje pot�na eksplozja).
    - gang, kt�ry osi�gnie najwi�ksz� ilo�� punkt�w (zab�jstwa + damage), zgarnia tyle, ile zajeba� enemy gangowi z sejfu ez.
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